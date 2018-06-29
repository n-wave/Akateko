/*
  ==============================================================================

    SliderLookAndFeelBottomV2.h
    Created: 12 May 2018 6:31:49pm
    Author:  mario

  ==============================================================================
*/

#ifndef SLIDERLOOKANDFEELBOTTOMV2_H_INCLUDED
#define SLIDERLOOKANDFEELBOTTOMV2_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SliderLookAndFeelBottomV2 : public LookAndFeel_V1
{
public:
    SliderLookAndFeelBottomV2();

    void drawRotarySlider(Graphics &g,
                          int x,
                          int y,
                          int w,
                          int h,
                          float sliderPos,
                          float rotaryStartAngle,
                          float rotaryEndAngle,
                          Slider &s) override;



}; // SliderLookAndFeelBottomV2



#endif  // SLIDERLOOKANDFEELBOTTOMV2_H_INCLUDED
