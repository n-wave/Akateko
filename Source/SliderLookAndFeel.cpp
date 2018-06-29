/*
  ==============================================================================

    SliderLookAndFeel.cpp
    Created: 8 May 2018 7:53:10pm
    Author:  mario

  ==============================================================================
*/

#include "SliderLookAndFeel.h"

SliderLookAndFeel::SliderLookAndFeel(const Image filmStrip) :
    rotaryStrip(filmStrip)
{

}

SliderLookAndFeel::~SliderLookAndFeel(){}

void SliderLookAndFeel::drawRotarySlider(Graphics &g,
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        float sliderPosProportional,
                                        float rotaryStartAngle,
                                        float rotaryEndAngle,
                                        Slider &s)
{
    g.setImageResamplingQuality(Graphics::highResamplingQuality);

    //const double fractRotation = (s.getValue() - s.getMinimum()) / (s.getMaximum() - s.getMinimum()); //value between 0 and 1 for current amount of rotation
    const int nFrames = rotaryStrip.getHeight()/rotaryStrip.getWidth(); // number of frames for vertical film strip
    const int frameIdx = (int)ceil(sliderPosProportional * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
    const float radius = jmin (width / 2.0f, height / 2.0f) ;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius - 1.0f;
    const float ry = centreY - radius - 1.0f;

    g.drawImage(rotaryStrip,
                         (int)rx,
                         (int)ry,
                         2*(int)radius,
                         2*(int)radius, //Dest
                         0,
                         frameIdx*rotaryStrip.getWidth(),
                         rotaryStrip.getWidth(),
                         rotaryStrip.getWidth()); //Source
}
