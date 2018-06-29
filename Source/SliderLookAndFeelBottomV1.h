/*
  ==============================================================================

    SliderLookAndFeelBottomV1.h
    Created: 12 May 2018 6:30:41pm
    Author:  mario

  ==============================================================================
*/

#ifndef SLIDERLOOKANDFEELBOTTOMV1_H_INCLUDED
#define SLIDERLOOKANDFEELBOTTOMV1_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SliderLookAndFeelBottomV1 : public LookAndFeel_V1
{
public:
    SliderLookAndFeelBottomV1();


    void drawRotarySlider(Graphics &g,
                          int x,
                          int y,
                          int w,
                          int h,
                          float sliderPos,
                          float rotaryStartAngle,
                          float rotaryEndAngle,
                          Slider &s) override;


};


#endif  // SLIDERLOOKANDFEELBOTTOMV1_H_INCLUDED
