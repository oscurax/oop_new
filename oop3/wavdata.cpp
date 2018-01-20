//
//  wavdata.cpp
//  oop3
//
//  Created by Тимур Газыев on 16.01.18.
//  Copyright © 2018 oop3. All rights reserved.
//

#include "wavdata.hpp"
#include "Except.hpp"
#define sizeofheader 44
void WaveData::read_header(const char* filename)
{
    printf( ">>>> read_header( %s )\n", filename );
    null_header(); // Fill header with zeroes.
    
    FILE* f = fopen( filename, "rb" );
    if ( !f ) {
        throw IO_ERROR();
    }
    
    size_t blocks_read = fread(this, sizeofheader, 1, f);
    if ( blocks_read != 1 ) {
        // can't read header, because the file is too small.
        throw BAD_FORMAT();
    }
    
    fseek( f, 0, SEEK_END ); // seek to the end of the file
    size_t file_size = ftell( f ); // current position is a file size!
    fclose( f );
    
    check_header( file_size );
    extract_data_int16( filename );
}

void WaveData::print_info()
{
    printf( "-------------------------\n" );
    printf( " audioFormat   %u\n", audioFormat );
    printf( " numChannels   %u\n", numChannels );
    printf( " sampleRate    %u\n", sampleRate );
    printf( " bitsPerSample %u\n", bitsPerSample );
    printf( " byteRate      %u\n", byteRate );
    printf( " blockAlign    %u\n", blockAlign );
    printf( " chunkSize     %u\n", chunkSize );
    printf( " subchunk1Size %u\n", subchunk1Size );
    printf( " subchunk2Size %u\n", subchunk2Size );
    printf( "-------------------------\n" );
}


void WaveData::check_header( size_t file_size_bytes )
{
    // Go to wav_header.h for details
    
    if ( chunkId[0] != 0x52 ||
        chunkId[1] != 0x49 ||
        chunkId[2] != 0x46 ||
        chunkId[3] != 0x46 )
        throw HEADER_RIFF_ERROR();
    
    if ( chunkSize != file_size_bytes - 8 ) throw HEADER_FILE_SIZE_ERROR();
    
    if ( format[0] != 0x57 ||
        format[1] != 0x41 ||
        format[2] != 0x56 ||
        format[3] != 0x45 )
        throw HEADER_WAVE_ERROR();
    
    if ( subchunk1Id[0] != 0x66 ||
        subchunk1Id[1] != 0x6d ||
        subchunk1Id[2] != 0x74 ||
        subchunk1Id[3] != 0x20 ||
        subchunk2Id[0] != 0x64 ||
        subchunk2Id[1] != 0x61 ||
        subchunk2Id[2] != 0x74 ||
        subchunk2Id[3] != 0x61)
        throw HEADER_FMT_ERROR();
    
    if ( audioFormat != 1 ) throw HEADER_NOT_PCM();
    
    if ( subchunk1Size != 16 ) throw HEADER_SUBCHUNK1_ERROR();
    
    if ( byteRate != sampleRate * numChannels * bitsPerSample/8 ) throw HEADER_BYTES_RATE_ERROR();
    
    if ( blockAlign != numChannels * bitsPerSample/8 ) throw HEADER_BLOCK_ALIGN_ERROR();
    
    if ( subchunk2Size != file_size_bytes - 44 ) throw HEADER_SUBCHUNK2_SIZE_ERROR();
}

void WaveData::prefill_header()
{
    // Go to wav.h for details
    
    chunkId[0] = 0x52;
    chunkId[1] = 0x49;
    chunkId[2] = 0x46;
    chunkId[3] = 0x46;
    
    format[0] = 0x57;
    format[1] = 0x41;
    format[2] = 0x56;
    format[3] = 0x45;
    
    subchunk1Id[0] = 0x66;
    subchunk1Id[1] = 0x6d;
    subchunk1Id[2] = 0x74;
    subchunk1Id[3] = 0x20;
    
    subchunk2Id[0] = 0x64;
    subchunk2Id[1] = 0x61;
    subchunk2Id[2] = 0x74;
    subchunk2Id[3] = 0x61;
    
    audioFormat   = 1;
    subchunk1Size = 16;
    bitsPerSample = 16;
    
}

void WaveData::fill_header(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan)
{
    if ( bits_per_sample != 16 ) throw UNSUPPORTED_FORMAT();
    
    if ( chan_count < 1 ) throw BAD_PARAMS();
    
    prefill_header();
    
    int file_size_bytes = 44 + chan_count * (bits_per_sample/8) * samples_count_per_chan;
    
    sampleRate    = sample_rate;
    numChannels   = chan_count;
    bitsPerSample = 16;
    
    chunkSize     = file_size_bytes - 8;
    subchunk2Size = file_size_bytes - 44;
    
    byteRate      = sampleRate * numChannels * bitsPerSample/8;
    blockAlign    = numChannels * bitsPerSample/8;
    
}

void WaveData::make_wav_file(const char* filename, int sample_rate)
{
    int chan_count = (int)channels_data.size();
    
    if ( chan_count < 1 ) throw BAD_PARAMS();
    
    int samples_count_per_chan = (int)channels_data[0].size();
    
    // Verify that all channels have the same number of samples.
    for ( size_t ch = 0; ch < chan_count; ch++ ) {
        if ( channels_data[ ch ].size() != (size_t) samples_count_per_chan ) throw BAD_PARAMS();
    }
    
    fill_header( chan_count, 16, sample_rate, samples_count_per_chan );
    
    std::vector<short> all_channels;
    all_channels.resize( chan_count * samples_count_per_chan );
    
    for ( int ch = 0; ch < chan_count; ch++ ) {
        const std::vector<short>& chdata = channels_data[ ch ];
        for ( size_t i = 0; i < samples_count_per_chan; i++ ) {
            all_channels[ chan_count * i + ch ] = chdata[ i ];
        }
    }
    
    FILE* f = fopen( filename, "wb" );
    fwrite(this, sizeofheader, 1, f );
    fwrite( all_channels.data(), sizeof(short), all_channels.size(), f );
    if ( !f ) throw IO_ERROR();
    
    fclose( f );
    
}

void WaveData::null_header()
{
    memset( this, 0, sizeofheader );
}

void WaveData::make_mono()
{
    int chan_count = (int)channels_data.size();
    
    if ( chan_count != 2 ) throw BAD_PARAMS();
    
    int samples_count_per_chan = (int)channels_data[0].size();
    
    // Verify that all channels have the same number of samples.
    for ( size_t ch = 0; ch < chan_count; ch++ ) {
        if ( channels_data[ ch ].size() != (size_t) samples_count_per_chan ) throw BAD_PARAMS();
    }
    
    vector<short>& mono = channels_data[ 0 ];
    mono.resize( samples_count_per_chan );
    // Mono channel is an arithmetic mean of all (two) channels.
    for ( size_t i = 0; i < samples_count_per_chan; i++ ) {
        mono[ i ] = ( channels_data[0][i] + channels_data[1][i] ) / 2;
    }
    
}





void WaveData::extract_data_int16( const char* filename )
{
    
    if ( bitsPerSample != 16 ) throw UNSUPPORTED_FORMAT();
    
    FILE* f = fopen( filename, "rb" );
    if ( !f ) IO_ERROR();
    
    fseek( f, 44, SEEK_SET ); // Seek to the begining of PCM data.
    
    int chan_count = numChannels;
    int samples_per_chan = ( subchunk2Size / sizeof(short) ) / chan_count;
    
    // 1. Reading all PCM data from file to a single vector.
    std::vector<short> all_channels;
    all_channels.resize( chan_count * samples_per_chan );
    size_t read_bytes = fread( all_channels.data(), 1, subchunk2Size, f );
    if ( read_bytes != subchunk2Size ) throw IO_ERROR();
    fclose( f );
    
    
    // 2. Put all channels to its own vector.
    channels_data.resize( chan_count );
    for ( size_t ch = 0; ch < channels_data.size(); ch++ ) {
        channels_data[ ch ].resize( samples_per_chan );
    }
    
    for ( int ch = 0; ch < chan_count; ch++ ) {
        std::vector<short>& chdata = channels_data[ ch ];
        for ( size_t i = 0; i < samples_per_chan; i++ ) {
            chdata[ i ] = all_channels[ chan_count * i + ch ];
        }
    }
}






