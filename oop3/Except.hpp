//
//  Except.hpp
//  oop3
//
//  Created by Тимур Газыев on 18.01.18.
//  Copyright © 2018 oop3. All rights reserved.
//

#ifndef Except_hpp
#define Except_hpp

#include <stdio.h>
#include <stdexcept>
using namespace std;

class wav_errors_e : public runtime_error {
public:
    explicit wav_errors_e(const char * arg) : runtime_error(arg) {};
};

class IO_ERROR : public wav_errors_e {
public:
    explicit IO_ERROR() : wav_errors_e("IO_ERROR") {};
};

class BAD_FORMAT : public wav_errors_e {
public:
    explicit BAD_FORMAT() : wav_errors_e("BAD_FORMAT") {};
};

class UNSUPPORTED_FORMAT : public wav_errors_e {
public:
    explicit UNSUPPORTED_FORMAT() : wav_errors_e("UNSUPPORTED_FORMAT") {};
};

class BAD_PARAMS : public wav_errors_e {
public:
    explicit BAD_PARAMS() : wav_errors_e("BAD_PARAMS") {};
};

class DATA_SIZE_ERROR : public wav_errors_e {
public:
    explicit DATA_SIZE_ERROR() : wav_errors_e("DATA_SIZE_ERROR") {};
};

class wav_headers_errors_e : public runtime_error {
public:
    explicit wav_headers_errors_e(const char * arg) : runtime_error(arg) {};
};

class HEADER_RIFF_ERROR : public wav_headers_errors_e {
public:
    explicit HEADER_RIFF_ERROR() : wav_headers_errors_e("HEADER_RIFF_ERROR") {};
};

class HEADER_FILE_SIZE_ERROR: public wav_headers_errors_e {
public:
    explicit HEADER_FILE_SIZE_ERROR() : wav_headers_errors_e("HEADER_FILE_SIZE_ERROR") {};
};

class HEADER_WAVE_ERROR: public wav_headers_errors_e {
public:
    explicit HEADER_WAVE_ERROR() : wav_headers_errors_e("HEADER_WAVE_ERROR") {};
};

class HEADER_FMT_ERROR : public wav_headers_errors_e {
public:
    explicit HEADER_FMT_ERROR() : wav_headers_errors_e("HEADER_FMT_ERROR") {};
};

class HEADER_NOT_PCM : public wav_headers_errors_e {
public:
    explicit HEADER_NOT_PCM() : wav_headers_errors_e("HEADER_NOT_PCM") {};
};

class HEADER_SUBCHUNK1_ERROR : public wav_headers_errors_e {
public:
    explicit HEADER_SUBCHUNK1_ERROR() : wav_headers_errors_e("HEADER_SUBCHUNK1_ERROR") {};
};

class HEADER_BYTES_RATE_ERROR : public wav_headers_errors_e {
public:
    explicit HEADER_BYTES_RATE_ERROR() : wav_headers_errors_e("HEADER_BYTES_RATE_ERROR") {};
};

class HEADER_BLOCK_ALIGN_ERROR : public wav_headers_errors_e {
public:
    explicit HEADER_BLOCK_ALIGN_ERROR() : wav_headers_errors_e("HEADER_BLOCK_ALIGN_ERROR") {};
};

class HEADER_SUBCHUNK2_SIZE_ERROR : public wav_headers_errors_e {
public:
    explicit HEADER_SUBCHUNK2_SIZE_ERROR() : wav_headers_errors_e("HEADER_SUBCHUNK2_SIZE_ERROR") {};
};


#endif /* Except_hpp */

