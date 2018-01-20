//
//  main.cpp
//  oop3
//
//  Created by Тимур Газыев on 16.01.18.
//  Copyright © 2018 oop3. All rights reserved.
//

#include <iostream>
#include "wavdata.hpp"
#include "Except.hpp"
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    cout << "************** | WavCore | **************" << endl;
    
    
    // ################  Tests for WavCore  ################
    
    const char* input_fname  = "0.wav";
    const char* output_fname = "out.wav";
    try {
        WaveData exmpl;
        
        // #### Opening WAV file, checking header.
        exmpl.read_header( input_fname);
        
        // #### Printing header.
        exmpl.print_info();
        
        // # Making signal mono from stereo.
        exmpl.make_mono();
        
        
        // #### Making new WAV file using edited PCM data.
        exmpl.make_wav_file( output_fname, 44100 );
        
        // #### Reading the file just created to check its header corectness.
        exmpl.read_header( output_fname );
        exmpl.print_info();
        
    }
    catch (wav_errors_e e) { cout<<e.what()<<endl;}
    
    getchar();
    return 0;
}
