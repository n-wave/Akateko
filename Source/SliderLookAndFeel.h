/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 8 May 2018 7:53:10pm
    Author:  mario

  ==============================================================================
*/

#ifndef SLIDERLOOKANDFEEL_H_INCLUDED
#define SLIDERLOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SliderLookAndFeel : public LookAndFeel_V1
{
public:
    SliderLookAndFeel(const Image filmStrip);
    ~SliderLookAndFeel();


    void drawRotarySlider(Graphics &g,
                        int x,
                         int y,
                         int width,
                         int height,
                        float sliderPosProportional,
                         float rotaryStartAngle,
                         float rotaryEndAngle,
                        Slider &s) override;
private:
    Image rotaryStrip;
}; // Slider LookAndFeel


#endif  // SLIDERLOOKANDFEEL_H_INCLUDED
