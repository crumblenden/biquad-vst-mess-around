#ifndef __FILTERONE__
#define __FILTERONE__

#include "IPlug_include_in_plug_hdr.h"
#include "FilterF.h"
#include "PlotF.h"

class FilterOne : public IPlug {
public:
  FilterOne(IPlugInstanceInfo instanceInfo);
  ~FilterOne();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  

private:
  Filter mFilter;
  double mGain;
  double mCutoff;
  double mResonance;
  double mFilterGain;
  double mFilterType;
};










#endif
