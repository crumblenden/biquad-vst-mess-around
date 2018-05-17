//
//  FilterF.h
//  FilterOne
//
//  Created by Chris Lynden on 14/05/2018.
//
//

#ifndef __FilterOne__FilterF__
#define __FilterOne__FilterF__

#include <stdio.h>
#include <limits>
#include <math.h>



class Filter {
public:
//    Filter() :
//    cutoff(0.99),
//    resonance(0.6),
//    filterGain(1.0),
//    mode(MODE_LP),
//    a0(0.0)
//    {
//        calculateCoefficients(mode, cutoff, resonance, filterGain);
//    };
    enum filter_mode {
        lowpass = 0,
        highpass,
        bandpass,
        notch,
        peak,
        lowshelf,
        highshelf,
        numFilterModes
    };
    Filter() {
        mode = lowpass;
        a0 = 1.0;
        a1 = a2 = b1 = b2 = 0.0;
        cutoff = 1500;
        resonance = 0.1;
        filterGain = 1.0;
        for(int i = 0; i < 3; i++) {
            inBuf[i] = 0.0;
            outBuf[i] = 0.0;
        }
        sampleRate = 44100;
        calculateCoefficients();
    };
    ~Filter(){};
    void setMode(filter_mode _newMode);
    void setResonance(double _res);
    void setCutoff(double _cutoff);
    void setGain(double _gain);
    double process(double input);
    void calculateCoefficients();
    double lowest_double = std::numeric_limits<double>::lowest();
    double highest_double = fabs(lowest_double);
    

    double sampleRate;
    
    double cutoff;
    double resonance;
    double filterGain;
    filter_mode mode;
    double inBuf[3];
    double outBuf[3];
    double a0, a1, a2, b1, b2;
    
};



#endif /* defined(__FilterOne__FilterF__) */
