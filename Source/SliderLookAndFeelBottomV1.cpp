/*
  ==============================================================================

    SliderLookAndFeelBottomV1.cpp
    Created: 12 May 2018 6:30:41pm
    Author:  mario

  ==============================================================================
*/

#include "SliderLookAndFeelBottomV1.h"

SliderLookAndFeelBottomV1::SliderLookAndFeelBottomV1(){
    setColour(Slider::rotarySliderFillColourId, Colour(0x6F20DFBF));
    setColour(Slider::rotarySliderOutlineColourId, Colour(0x9F20DBBF));
    setColour(Slider::thumbColourId, Colour(0xFF4A997A));
    setColour(Slider::textBoxHighlightColourId, Colour(0xFF70D0B0));
}



void SliderLookAndFeelBottomV1::drawRotarySlider(Graphics &g,
                                                 int x,
                                                 int y,
                                                 int w,
                                                 int h,
                                                 float sliderPos,
                                                 float rotaryStartAngle,
                                                 float rotaryEndAngle,
                                                 Slider &s)
{
    /* Coordinates for background ellipse */
    const float centreX = x + (w*0.5);
    const float centreY =	y + (h*0.5);

    const float offsetEllipse = w*0.02;
    const float widthEllipse = w*0.98;
    const float EllipseX = x + offsetEllipse;
    const float EllipseY = y + offsetEllipse;
    bool isDownOrDragging = s.isEnabled() && (s.isMouseOverOrDragging() || s.isMouseButtonDown());

    /* drawing Area */
    Rectangle<float>boundingBox(x,y, w, h);

    /* Angle for drawing meter by using PieSegments */
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    /* Calculate angle and convert for the thumbnail position
     * Magnitude of the vector is 1/4 of width, remember that
     * half of the thumb width should be subtracted
     * from the final X and Y position in FillEllipse.
     *
     * change offset : (2.35) for the start position
     * change range  : (4.75) for the final end position
     */

    const float thumbAngle = 2.35 +(sliderPos*4.75);
    const float thumbX = centreX+(w*0.25*cos(thumbAngle));
    const float thumbY = centreY+(h*0.25*sin(thumbAngle));
    const float thumbWidth = w*0.105;
    const float halfThumb = thumbWidth*0.5;
    const float cwe = w*0.22;
    const float cw2 = cwe*0.5;

    ColourGradient gradient = ColourGradient(s.findColour(Slider::rotarySliderFillColourId),
                                             x + w * 0.25,
                                             y,
                                             s.findColour(Slider::rotarySliderOutlineColourId),
                                             x+w,
                                             y,
                                             false);


    /* Drawable object for svg files slider */
    ScopedPointer<Drawable>drawable;
    drawable = Drawable::createFromImageData(BinaryData::rotaryNormal_svg, BinaryData::rotaryNormal_svgSize);
    drawable->replaceColour(Colours::red, s.findColour(Slider::thumbColourId));

    /* set 'BackGround' colour */
    g.setColour(Colours::transparentBlack);

    g.fillEllipse(EllipseX, EllipseY, widthEllipse, widthEllipse);

    Path filledArc;
    filledArc.addPieSegment(EllipseX, EllipseY, widthEllipse, widthEllipse, rotaryStartAngle, angle, 0);

    g.setGradientFill(gradient);
    g.fillPath(filledArc);

    /* draw svg Variable over the pie segment */
    drawable->setTransformToFit(boundingBox,RectanglePlacement::stretchToFit);
    drawable->draw(g, 1.0f);


    /* Change color of the thumbnail as soon as the
     * mouse enter the area
     */

    if(isDownOrDragging){
       g.setColour(s.findColour(Slider::textBoxHighlightColourId));
    } else {
       g.setColour(s.findColour(Slider::thumbColourId));
    }
    /* Draw Ellipse*/

    g.fillEllipse(thumbX-halfThumb, thumbY-halfThumb, thumbWidth, thumbWidth);

    g.fillEllipse(centreX-cw2, centreY-cw2, cwe, cwe);

    g.setColour(Colours::black);
    g.fillEllipse(centreX-halfThumb, centreY-halfThumb, thumbWidth, thumbWidth);

}
