#include "FilterOne.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"


const int kNumPrograms = 1;

enum EParams
{
  kGain = 0,
  kCutoff,
  kResonance,
  kFilterGain,
  kFilterType,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,
  
  kGainX = 100,
  kGainY = 145,

  kCutoffX = 200,
  kCutoffY = 145,
  
  kResonanceX = 300,
  kResonanceY = 145,
  
  kFilterGainX = 400,
  kFilterGainY = 145,
  
  kFilterTypeX = 500,
  kFilterTypeY = 145,
  
  kKnobFrames = 60
};

FilterOne::FilterOne(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
{
  TRACE;

  
  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGain)->InitDouble("Gain", 0.5, 0.0, 1.0, 0.01, "%");
  GetParam(kGain)->SetShape(1.);
  GetParam(kCutoff)->InitDouble("Cutoff", 0.5, 0.0, 0.99, 0.01, "Hz");
  GetParam(kCutoff)->SetShape(1.);
  GetParam(kResonance)->InitDouble("Q", 0.0, 0.1, 1.0, 0.01, "Q");
  GetParam(kResonance)->SetShape(1.);
  GetParam(kFilterGain)->InitDouble("FilterGain", 1.0, 0.0, 1.0, 0.01, "%");
  GetParam(kFilterGain)->SetShape(1.);
  GetParam(kFilterType)->InitInt("Type", 0, 0, 6);
  

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_BLUE);
  pGraphics->AttachControl(new Plot(this, IRECT(0, 0, kWidth, kHeight), &mFilter));
  
  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kCutoffX, kCutoffY, kCutoff, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kResonanceX, kResonanceY, kResonance, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kFilterGainX, kFilterGainY, kFilterGain, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kFilterTypeX, kFilterTypeY, kFilterType, &knob));
  
  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
  
  mFilter.calculateCoefficients();
  
  
  
}

FilterOne::~FilterOne() {}

void FilterOne::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];
  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
    //*out1 = *in1 * mGain;
    *out1 = mFilter.process(*in1);
    *out2 = *out1;
  }
}

void FilterOne::Reset()
{
  TRACE;
  IMutexLock lock(this);
}

void FilterOne::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGain:
      mGain = GetParam(kGain)->Value() * 1.0;
      break;
    case kCutoff:
      mCutoff = GetParam(kCutoff)->Value() * 20000.0;
      mFilter.setCutoff(mCutoff);
      break;
    case kResonance:
      mResonance = GetParam(kResonance)->Value() * 1.0;
      mFilter.setResonance(mResonance);
      break;
    case kFilterGain:
      mFilterGain = GetParam(kFilterGain)->Value() * 1.0;
      mFilter.setGain(mFilterGain);
      break;
    case kFilterType:
      mFilterType = GetParam(kFilterType)->Value() * 1.0;
      switch((int)mFilterType) {
        case 0:
          mFilter.setMode(Filter::lowpass);
          break;
        case 1:
          mFilter.setMode(Filter::highpass);
          break;
        case 2:
          mFilter.setMode(Filter::bandpass);
          break;
        case 3:
          mFilter.setMode(Filter::notch);
          break;
        case 4:
          mFilter.setMode(Filter::peak);
          break;
        case 5:
          mFilter.setMode(Filter::highshelf);
          break;
        case 6:
          mFilter.setMode(Filter::lowshelf);
          break;
        default: break;
      }
    default: break;
  }
  
  //filterPlot.getYValues(mFilter.a0, mFilter.a1, mFilter.a2, mFilter.b1, mFilter.b2);

}
