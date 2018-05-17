//
//  PlotF.h
//  FilterOne
//
//  Created by Chris Lynden on 15/05/2018.
//
//

#ifndef __FilterOne__PlotF__
#define __FilterOne__PlotF__

#include <stdio.h>
#include <math.h>
#include "IControl.h"
#include "FilterF.h"
#define numPoints 1000

class GPoint {
public:
    int id;
    double x;
    double y;
};

class Plot : public IControl {
public:

    GPoint points[numPoints];
    double yValues[numPoints];
    int xPos, yPos, xSize, ySize;
    IColor plotColor;
    Filter *mFilter;
    
    Plot(IPlugBase *pPlug, IRECT pR, Filter *_mFilter) : IControl(pPlug, pR) {
        for(int i = 0; i < numPoints; i++) {
            yValues[i] = 0.0;
            points[i].x = 0.0;
            points[i].y = 0.0;
        }
        xPos = 0;
        yPos = 0;
        xSize = pR.W();
        ySize = pR.H();
        mFilter = _mFilter;
    };
    ~Plot() {};
    
    bool IsDirty() { return true; };
    
    bool Draw(IGraphics *pGraphics) {
        GetYValues(mFilter->a0, mFilter->a1, mFilter->a2, mFilter->b1, mFilter->b2);
        GPoint lastPoint;
        GPoint currentPoint;
        for(int i = 0; i < numPoints; i++) {
            if(i == 0) {
                lastPoint.x = xPos;
                lastPoint.y = yPos + (ySize/2);
            }
            currentPoint.x = xPos + ((i/(double)numPoints) * xSize);
            currentPoint.y = yValues[i];
            pGraphics->DrawLine(&plotColor, lastPoint.x, lastPoint.y, currentPoint.x, currentPoint.y);
            lastPoint = currentPoint;
        }
        return true;
    };
    
    void GetYValues(double a0, double a1, double a2, double b1, double b2) {
        
        for(int i = 0; i < numPoints; i++) {
            double w = (M_PI) * ((i/(double)numPoints));
            
            double y = 20*log((
                               sqrt(
                                    square(a0 * square(cos(w))  - a0 *  square(sin(w))  + a1 * cos(w)+a2) +  square(2 * a0 * cos(w) * sin(w) + a1 * (sin(w)))
                                    )
                               /
                               sqrt(
                                    square(square(cos(w))  -  square(sin(w))  + b1 * cos(w) + b2) + square(2 * cos(w) * sin(w) + b1 * (sin(w)))
                                    )
                               ));
            
//            double y = 20*log10(
//                                sqrt(
//                                     (a0*a0+a1*a1+a2*a2+2*(a0*a1+a1*a2)*cos(w)+2*(a0*a2)*cos(2*w))/
//                                     (1+b1*b1+b2*b2+2*(b1+b1*b2)*cos(w)+2*b2*cos(2*w))
//                                     )
//                                );

            yValues[i] = ySize - (y + (ySize/2));
            
        }
    };
    
    double square(double x) {
        return x * x;
    };
};

#endif /* defined(__FilterOne__PlotF__) */

