/*
  ==============================================================================

    XYPad.h
    Created: 7 Mar 2018 11:02:58am
    Author:  mario

  ==============================================================================
*/

#ifndef XYPAD_H_INCLUDED
#define XYPAD_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <vector>
//==============================================================================
/*
*/
class XYPad : public Component
{
public:
    XYPad(AkatekoAudioProcessor &p,
          Label &label);

    ~XYPad();

    void paint (Graphics&) override;
    void resized() override;
    void updateGui();

    String toString();
    void restoreFromString(String state);

    void setColour(const int colourId, Colour colour);
    void setOverlayColour(const Colour &colourOne, const Colour &colourTwo);

    void setBackgroundImage(const Image &imageToUse, bool use);
    void setUpdateCommandId(int id);

    void setLookAndFeel(LookAndFeel *laf);

    void mouseUp(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override; //click and mouse move
    void mouseDown(const MouseEvent &event) override;


    enum colourIds {
        backgroundColourId,
        outlineColourId,
        fillColourId,
        ellipseColourId,
        highlightColourId,
        overlayGradientOneId,
        overlayGradientTwoId
    };

private:
    void init();

    void drawCursor(Graphics &g);

    bool checkBounds(Point<float> &point);
    float checkHorizontal(float xPosition);
    float checkVertical(float yPosition);
    int checkPosition(Point<float> &point);
    void handlePopupMenu(int choice, int param);

    bool useImage;
    bool dragEnabled;

    // Boundary's
    float maxWidth;
    float maxHeight;
    float minWidth;
    float minHeight;
    float widthRange;
    float heightRange;

    //cursor boundary's
    float cWidthRange;
    float cHeightRange;
    float cMaxWidth;
    float cMaxHeight;
    float cMinWidth;
    float cMinHeight;

    float xAxis;
    float yAxis;

    double scalar;

    float ellipseDia;
    float ellipseRadi;

    int updateCommandId;
    // Bind on creation in Constructor to modulation Matrix
    Colour backGroundColour;
    Colour outlineColour;
    Colour fillColour;
    Colour ellipseColour;
    Colour highLightColour;
    Colour overlayGradientColourOne;
    Colour overlayGradientColourTwo;

    ColourGradient overlayColour;
    ColourGradient innerOverlay;

    Image backgroundImage;
    Point<float> cursor;

    AudioProcessorParameter *xyPadX = nullptr;
    AudioProcessorParameter *xyPadY = nullptr;
    AudioProcessorParameter *xyPadT = nullptr;

    Label &labelRef;

    std::vector<int> paramIndices;
    PopupMenu menu;

    AkatekoAudioProcessor &processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPad)
};


#endif  // XYPAD_H_INCLUDED
