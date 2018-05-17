//
//  FilterF.cpp
//  FilterOne
//
//  Created by Chris Lynden on 14/05/2018.
//
//

#include "FilterF.h"
#include <math.h>
;



void Filter::setMode(filter_mode _newMode) {
    mode = _newMode;
    calculateCoefficients();
}
void Filter::setCutoff(double _cutoff) {
    cutoff = _cutoff;
    calculateCoefficients();
}
void Filter::setResonance(double _res) {
    resonance = _res;
    calculateCoefficients();
}
void Filter::setGain(double _gain) {
    filterGain = (_gain * 2.0) -1.0;
    
    calculateCoefficients();
}

double Filter::process(double input) {

    
    inBuf[0] = input;
    outBuf[0] = (inBuf[0] * a0) + (inBuf[1] * a1) + (inBuf[2] * a2) -
                                            (outBuf[0] * b1) - (outBuf[1] * b2);
    if(outBuf[0] < -1.0) {outBuf[0] = -1.0;}
    if(outBuf[0] > 1.0) {outBuf[0] = 1.0;}
    inBuf[2] = inBuf[1];
    inBuf[1] = inBuf[0];
    outBuf[2] = outBuf[1];
    outBuf[1] = outBuf[0];
    return outBuf[0];
    
}


//void Filter::calculateCoefficients() {
//    double c;
//    double pi = M_PI;
//    switch(mode) {
//            
//        case MODE_LP:
//            c = 1.0 / tan(pi * cutoff / sampleRate); //map cutoff to 0.0-1.0
//            a0 = 1.0 / (1.0 + resonance * c + c * c);
//            a1 = 2 * a0;
//            a2 = a0;
//            b1 = 2.0 * (1.0 - c * c) * a0;
//            b2 = (1.0 - resonance * c + c * c) * a0;
// //test coefs
////                a0 = 1.0;
////                a1 = 0.0;
////                a2 = -1.0;
////                b1 = 0.1;
////                b2 = 0.9;
////            //hp12k, 3.0q
////            a0 = 0.3697369761130797;
////            a1 = -0.7394739522261594;
////            a2 = 0.3697369761130797;
////            b1 = 0.23770017226522885;
////            b2 = 0.7166480767175475;
//            break;
//        case MODE_HP:
//            c = tan(pi * cutoff / sampleRate); //map cutoff to 0.0-1.0
//            a0 = 1.0 / (1.0 + resonance * c + c * c);
//            a1 = -2 * a0;
//            a2 = a0;
//            b1 = 2.0 * (c * c - 1.0) * a0;
//            b2 = (1.0 - resonance * c + c * c) * a0;
//
//            break;
//        default:
//            break;
//    }
//    
//}

void Filter::calculateCoefficients() {

    double Q = resonance;
    double Fc = cutoff;
    double norm = 0.0;
    double Fs = 44100.0;
    
    
    //double V = pow(10, fabs(filterGain) / 20);
    double V = filterGain;
    double K = tan(M_PI * Fc / Fs);
    switch (mode) {
        case lowpass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case highpass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = 1 * norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case bandpass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K / Q * norm;
            a1 = 0;
            a2 = -a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case notch:
            norm = 1 / (1 + K / Q + K * K);
            a0 = (1 + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = a0;
            b1 = a1;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case peak:
            if (filterGain >= 0.0) {    // boost
                norm = 1 / (1 + 1/Q * K + K * K);
                a0 = (1 + V/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - V/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - 1/Q * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + V/Q * K + K * K);
                a0 = (1 + 1/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - 1/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - V/Q * K + K * K) * norm;
            }
            break;
        case lowshelf:
            if (filterGain >= 0.0) {    // boost
                norm = 1 / (1 + M_SQRT2 * K + K * K);
                a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
                a1 = 2 * (V * K * K - 1) * norm;
                a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - M_SQRT2 * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
                a0 = (1 + M_SQRT2 * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - M_SQRT2 * K + K * K) * norm;
                b1 = 2 * (V * K * K - 1) * norm;
                b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
            }
            break;
        case highshelf:
            if (filterGain >= 0.0) {    // boost
                norm = 1 / (1 + M_SQRT2 * K + K * K);
                a0 = (V + sqrt(2*V) * K + K * K) * norm;
                a1 = 2 * (K * K - V) * norm;
                a2 = (V - sqrt(2*V) * K + K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - M_SQRT2 * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (V + sqrt(2*V) * K + K * K);
                a0 = (1 + M_SQRT2 * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - M_SQRT2 * K + K * K) * norm;
                b1 = 2 * (K * K - V) * norm;
                b2 = (V - sqrt(2*V) * K + K * K) * norm;
            }
            break;
        default: break;
    }
    }