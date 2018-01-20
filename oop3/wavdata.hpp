//
//  wavdata.hpp
//  oop3
//
//  Created by Тимур Газыев on 16.01.18.
//  Copyright © 2018 oop3. All rights reserved.
//

#ifndef wavdata_hpp
#define wavdata_hpp
#include <stdio.h>
#include <vector>
using namespace std;
class WaveData {
private:
    char chunkId[4];
    
    uint32_t chunkSize;

    char format[4];
   
    char subchunk1Id[4];
   
    uint32_t subchunk1Size;
  
    uint16_t audioFormat;
    
    uint16_t numChannels;
   
    uint32_t sampleRate;
    
    uint32_t byteRate;
    
    uint16_t blockAlign;
    
    uint16_t bitsPerSample;
   
    char subchunk2Id[4];

    uint32_t subchunk2Size;

    
    // Checks header validity.
    void check_header( size_t file_size_bytes );
    
    // Fills header information, using input parameters. This function calls prefill_header() itself.
    void fill_header(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan);
    
    // Fills 'header_ptr' with default values.
    void prefill_header();
    
    // Fills header with zeroes.
    void null_header();
    
    // Reads file 'filename' and puts PCM data (raw sound data) to 'channels_data'.
    void extract_data_int16( const char* );
    
    vector<vector<short>> channels_data;

public:
    // Reads file 'filename' and puts header's data to 'header_ptr' address.
    void read_header(const char*);
    
    // Creates a new WAV file 'filename', using 'sample_rate' and PCM data from 'channels_data'.
    void make_wav_file(const char* , int);
    
    // Prints header's data from 'header_ptr' address.
    void print_info();
    
    // Makes mono PCM data from stereo 'source'.
    void make_mono();
    
    // Adds a reverberation (it's like an echo).
    void make_reverb();
    
};
#endif /* wavdata_hpp */
