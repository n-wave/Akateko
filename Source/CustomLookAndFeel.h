/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 8 May 2018 12:19:46pm
    Author:  mario

  ==============================================================================
*/

#ifndef CUSTOMLOOKANDFEEL_H_INCLUDED
#define CUSTOMLOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V1
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();

    void drawComboBox(Graphics &g,
                      int width,
                      int height,
                      bool isButtonDown,
                      int buttonX,
                      int buttonY,
                      int buttonW,
                      int buttonH,
                      ComboBox &b) override;

    void positionComboBoxText(ComboBox &b,
                              Label &l) override;

    // Popup Menu Also affects the combo boxes

    void getIdealPopupMenuItemSize(const String &text,
                                   bool isSeparator,
                                   int standardMenuItemHeight,
                                   int &idealWidth,
                                   int &idealHeight) override;


    void drawPopupMenuBackground(Graphics &g,
                                 int width,
                                 int height) override;


    void drawPopupMenuItem(Graphics &g,
                           const Rectangle<int> &area,
                           bool isSeparator,
                           bool isActive,
                           bool isHighlighted,
                           bool isTicked,
                           bool hasSubMenu,
                           const String &text,
                           const String &shortcutKeyText,
                           const Drawable *icon,
                           const Colour *textColourToUse) override;

    // Draw Toggle Box

    void drawTickBox(Graphics &g,
                     Component &c,
                     float x,
                     float y,
                     float w,
                     float h,
                     bool ticked,
                     bool isEnabled,
                     bool isMouseOverButton,
                     bool isButtonDown) override;

    void drawToggleButton(Graphics &g,
                          ToggleButton &b,
                          bool isMouseOverButton,
                          bool isButtonDown) override;

    // Only use for linear slider Bar

    void drawLinearSlider(Graphics &g,
                          int x,
                          int y,
                          int width,
                          int height,
                          float sliderPos,
                          float minSliderPos,
                          float maxSliderPos,
                          const Slider::SliderStyle style,
                          Slider &slider) override;

    void drawButtonText(Graphics &g,
                        TextButton &b,
                        bool isMouseOverButton,
                        bool isButtonDown) override;

    void drawButtonBackground(Graphics &g,
                              Button &b,
                              const Colour &backgroundColour,
                              bool isMouseOverButton,
                              bool isButtonDown) override;

    // Table Header


    void drawTableHeaderBackground(Graphics &g,
                                   TableHeaderComponent &t) override;


    void drawTableHeaderColumn(Graphics &g,
                               const String &columnName,
                               int columnId,
                               int width,
                               int height,
                               bool isMouseOver,
                               bool isMouseDown,
                               int columnFlags) override;

private:
    Colour tableOutLineColour;
    Colour tableBackGroundColour;
    Colour tableTextColour;
};



#endif  // CUSTOMLOOKANDFEEL_H_INCLUDED
