/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 8 May 2018 12:19:46pm
    Author:  mario

  ==============================================================================
CustomLookAndFeel::CustomLookAndFeel() :
    comboBoxOutline(Colour(0xFF309c8b)),
    comboBoxBackGround(Colour(0x9F0c2823)),
    comboBoxButton(Colour(0xFF33a694))
{
    setColour(PopupMenu::backgroundColourId, comboBoxBackGround);
    setColour(PopupMenu::textColourId, comboBoxOutline);
}


*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel(){

    //ComboBox Colours
    setColour(ComboBox::textColourId, Colour(0xFF4A997A));
    setColour(ComboBox::backgroundColourId,  Colour(0xBF0c2823));
    setColour(ComboBox::outlineColourId, Colour(0xCC60B089));
    setColour(ComboBox::buttonColourId, Colour(0xFF33a694));

    // PopupMenu
    setColour(PopupMenu::backgroundColourId, Colour(0xFF001402));
    setColour(PopupMenu::textColourId, Colour(0xFF4A997A));

    // Linear Bar Slider Colour
    setColour(Slider::backgroundColourId, Colour(0x1F10A0A0));
    setColour(Slider::thumbColourId, Colour(0xCC20BF8F));
    setColour(Slider::textBoxOutlineColourId, Colour(0xCC60B089));

    // Toggle Button
    setColour(ToggleButton::textColourId, Colour(0xFF4A997A));
    setColour(ToggleButton::tickColourId, Colour(0x9F20BFaF));
    setColour(ToggleButton::tickDisabledColourId, Colour(0x2FFFFFFF));

    // Text Button
    setColour(TextButton::textColourOnId, Colour(0xFF4A997A));
    setColour(TextButton::textColourOffId,  Colour(0xCB70C099));

    setColour(TextButton::buttonOnColourId, Colour(0xAF20AA9A));
    setColour(TextButton::buttonColourId, Colour(0x3F10A0A0));

    //Text Editor
    setColour(TextEditor::textColourId, Colour(0xFF4A997A));
    setColour(TextEditor::backgroundColourId, Colour(0xFF000000));
    setColour(TextEditor::outlineColourId, Colour(0xFF70C099));
    setColour(CaretComponent::caretColourId, Colour(0xCC60B089));

    // TableHeader Component Colours
    tableOutLineColour = Colour(0xFF70C099);
    tableBackGroundColour = Colour(0x1F10A0A0);
    tableTextColour = Colour(0xFF4A997A);
}

CustomLookAndFeel::~CustomLookAndFeel(){

}


void CustomLookAndFeel::drawComboBox(Graphics &g,
                                int width,
                                int height,
                                bool isButtonDown,
                                int buttonX,
                                int buttonY,
                                int buttonW,
                                int buttonH,
                                ComboBox &b)
{
    g.fillAll (b.findColour (ComboBox::backgroundColourId));
    g.setColour (b.findColour (ComboBox::outlineColourId));

    g.drawRoundedRectangle(0,0, width, height, 2.0f, 2.0f);
    // Draw Small Ellipse in right Top Corner

    if (b.isEnabled())
    {
        Path p;

        const float offsetX  = buttonW*0.05;
        const float ellipseWidth = buttonW*0.50;
        const float offsetY = (buttonH*0.5)-(ellipseWidth*0.5);


        p.addEllipse(buttonX+offsetX, buttonY+offsetY, ellipseWidth, ellipseWidth);


        g.setColour (b.findColour ((isButtonDown) ? ComboBox::backgroundColourId
                                                    : ComboBox::buttonColourId));
        g.fillPath (p);
    }
}

void CustomLookAndFeel::positionComboBoxText(ComboBox &b,
                                             Label &l)
{
    const float widthOffset = b.getHeight()*0.33;

    const float width = b.getWidth()-widthOffset*2.0;
    const float height = b.getHeight();


    l.setBounds (1+widthOffset, 1,
                     width - 2,
                     b.getHeight() - 2);

    l.setFont (getComboBoxFont (b));
}

// Also Effects ComboBox

void CustomLookAndFeel::getIdealPopupMenuItemSize(const String &text,
                                                  bool isSeparator,
                                                  int standardMenuItemHeight,
                                                  int &idealWidth,
                                                  int &idealHeight)
{
    if (isSeparator)
    {
        idealWidth = 50;
        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight / 2 : 10;
    }
    else
    {
        Font font (getPopupMenuFont());

        if (standardMenuItemHeight > 0 && font.getHeight() > standardMenuItemHeight / 1.3f)
            font.setHeight (standardMenuItemHeight / 1.3f);

        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight : roundToInt (font.getHeight() * 1.3f);
        idealWidth = font.getStringWidth (text) + idealHeight * 2;
        idealWidth -= 2;
    }
}


void CustomLookAndFeel::drawPopupMenuBackground(Graphics &g,
                                                int width,
                                                int height)
{
    const Colour background (findColour (PopupMenu::backgroundColourId));

    g.fillAll (background);
    g.setColour (background.overlaidWith (Colour (0x2badd8e6)));

    for (int i = 0; i < height; i += 3)
        g.fillRect (0, i, width, 1);

   #if ! JUCE_MAC
    g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.85f));
    g.drawRect (0, 0, width, height,1.2);
   #endif
}

void CustomLookAndFeel::drawPopupMenuItem(Graphics &g,
                                          const Rectangle<int> &area,
                                          bool isSeparator,
                                          bool isActive,
                                          bool isHighlighted,
                                          bool isTicked,
                                          bool hasSubMenu,
                                          const String &text,
                                          const String &shortcutKeyText,
                                          const Drawable *icon,
                                          const Colour *textColourToUse)
{
    if (isSeparator)
     {
         Rectangle<int> r (area.reduced (5, 0));
         r.removeFromTop (r.getHeight() / 2 - 1);

         g.setColour (Colour (0x33000000));
         g.fillRect (r.removeFromTop (1));

         g.setColour (findColour(PopupMenu::textColourId));
         g.fillRect (r.removeFromTop (1));
     }
     else
     {
         Colour textColour (findColour (PopupMenu::textColourId));

         if (textColourToUse != nullptr)
             textColour = *textColourToUse;

         Rectangle<int> r (area.reduced (1));

         if (isHighlighted)
         {
             g.setColour (findColour (PopupMenu::highlightedBackgroundColourId));
             g.fillRect (r);

             g.setColour (findColour (PopupMenu::highlightedTextColourId));
         }
         else
         {
             g.setColour (textColour);
         }

         if (! isActive)
             g.setOpacity (0.3f);

         Font font (getPopupMenuFont());

         const float maxFontHeight = area.getHeight() / 1.3f;

         if (font.getHeight() > maxFontHeight)
             font.setHeight (maxFontHeight);

         g.setFont (font);

         Rectangle<float> iconArea (r.removeFromLeft ((r.getHeight() * 5) / 4).reduced (3).toFloat());

         if (icon != nullptr)
         {
             icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
         }
         else if (isTicked)
         {
             const Path tick (getTickShape (1.0f));
             g.fillPath (tick, tick.getTransformToScaleToFit (iconArea, true));
         }

         if (hasSubMenu)
         {
             const float arrowH = 0.6f * getPopupMenuFont().getAscent();

             const float x = (float) r.removeFromRight ((int) arrowH).getX();
             const float halfH = (float) r.getCentreY();

             Path p;
             p.addTriangle (x, halfH - arrowH * 0.5f,
                            x, halfH + arrowH * 0.5f,
                            x + arrowH * 0.6f, halfH);

             g.fillPath (p);
         }

         r.removeFromRight (3);
         g.drawFittedText (text, area, Justification::centred, 1);

         if (shortcutKeyText.isNotEmpty())
         {
             Font f2 (font);
             f2.setHeight (f2.getHeight() * 0.75f);
             f2.setHorizontalScale (0.95f);
             g.setFont (f2);

             g.drawText (shortcutKeyText, r, Justification::centredRight, true);
         }
     }
}

// Toggle Button methods

void CustomLookAndFeel::drawTickBox(Graphics &g,
                                    Component &c,
                                    float x,
                                    float y,
                                    float w,
                                    float h,
                                    bool ticked,
                                    bool isEnabled,
                                    bool isMouseOverButton,
                                    bool isButtonDown)
{
    Path box;
    box.addRoundedRectangle (-0.25f, 1.5f, 6.0f, 6.0f, 1.5f);


    g.setColour (isEnabled ? findColour(ToggleButton::tickColourId).withAlpha(isButtonDown ? 0.3f : 0.1f)
                              : findColour(ToggleButton::tickColourId).withAlpha (0.1f));

    AffineTransform trans (AffineTransform::scale (w *0.1111, h *0.1111).translated (x, y));
    const float border = w*0.85*0.1111;
    /* Change Color When Activated */

    if(ticked){
      g.setColour(findColour(ToggleButton::tickColourId));
    }

    //Use tickDisabled as over colour
    g.fillPath (box, trans);

    if(isMouseOverButton){
      g.setColour(findColour(ToggleButton::tickDisabledColourId));
      g.fillPath (box, trans);
    }

    g.setColour (findColour(ToggleButton::textColourId));
    g.strokePath (box, PathStrokeType (border), trans);
}

void CustomLookAndFeel::drawToggleButton(Graphics &g,
                                         ToggleButton &b,
                                         bool isMouseOverButton,
                                         bool isButtonDown)
{
    const int tickWidth = jmin (20, b.getHeight() - 4);

    drawTickBox (g, b, 4.0f, (b.getHeight() - tickWidth) * 0.5f,
                 (float) tickWidth, (float) tickWidth,
                 b.getToggleState(),
                 b.isEnabled(),
                 isMouseOverButton,
                 isButtonDown);

    g.setColour(Colours::white);

    g.setColour (b.findColour (ToggleButton::textColourId));
    g.setFont (jmin (15.0f, b.getHeight() * 0.6f));

    if (! b.isEnabled())
        g.setOpacity (0.5f);
}

void CustomLookAndFeel::drawLinearSlider(Graphics &g,
                                         int x,
                                         int y,
                                         int width,
                                         int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         const Slider::SliderStyle style,
                                         Slider &slider)
{
    g.fillAll (slider.findColour (Slider::backgroundColourId));

    const float fy =  y;
    const float fh = height;

    float tw = width*0.075;

    if(tw < 5){
        tw = 5;
    }

    const float tw2 = tw*0.5;

    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    if(isMouseOver){
        g.setColour(slider.findColour (Slider::thumbColourId).brighter(0.1f));
    } else {
        g.setColour (slider.findColour (Slider::thumbColourId).darker(0.1f));
    }

    g.fillRect (sliderPos-tw2, fy, tw, fh);
    g.setColour(Colours::black);
    g.drawRect(sliderPos-tw2-1, fy, tw+1, fh);
}

void CustomLookAndFeel::drawButtonText(Graphics &g,
                                       TextButton &b,
                                       bool isMouseOverButton,
                                       bool isButtonDown)
{
    Font font (getTextButtonFont (b, b.getHeight()));
    g.setFont (font);
    g.setColour (b.findColour (b.getToggleState() ? TextButton::textColourOnId
                                                            : TextButton::textColourOffId)
                       .withMultipliedAlpha (b.isEnabled() ? 1.0f : 0.5f));

    const int yIndent = jmin (4, b.proportionOfHeight (0.3f));
    const int cornerSize = jmin (b.getHeight(), b.getWidth()) / 2.f;

    const int fontHeight = roundToInt (font.getHeight() * 0.6f);
    const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (b.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (b.isConnectedOnRight() ? 4 : 2));
    const int textWidth = b.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText (b.getButtonText(),
                          leftIndent, yIndent, textWidth, b.getHeight() - yIndent * 2,
                          Justification::centred, 2);
}

void CustomLookAndFeel::drawButtonBackground(Graphics &g,
                                             Button &b,
                                             const Colour &backgroundColour,
                                             bool isMouseOverButton,
                                             bool isButtonDown)
{
    Colour baseColour (backgroundColour.withMultipliedSaturation (b.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                          .withMultipliedAlpha (b.isEnabled() ? 0.9f : 0.5f));

    if (isButtonDown || isMouseOverButton)
        baseColour = findColour(TextButton::buttonOnColourId).brighter(isButtonDown ? 0.3f : 0.2f);

    const bool flatOnLeft   = b.isConnectedOnLeft();
    const bool flatOnRight  = b.isConnectedOnRight();
    const bool flatOnTop    = b.isConnectedOnTop();
    const bool flatOnBottom = b.isConnectedOnBottom();

    const float width  = b.getWidth() - 1.0f;
    const float height = b.getHeight() - 1.0f;

    if (width > 0 && height > 0)
    {
        const float cornerSize = jmin (15.0f, jmin (width, height) * 0.45f);
        const float lineThickness = cornerSize * 0.15f;
        const float halfThickness = lineThickness * 0.5f;

        Path outline;
        outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                     cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));

        const Colour outlineColour (b.findColour (b.getToggleState() ? TextButton::textColourOnId
                                                                      : TextButton::textColourOffId));

        g.setColour (baseColour);
        g.fillPath (outline);

        if (! b.getToggleState())
        {
            g.setColour (outlineColour);
            g.strokePath (outline, PathStrokeType (lineThickness));
        }
    }
}

/*
 *  tableBackGroundColour = Colour(0x1F10A0A0);
 *  tableTextColour = Colour(0xCC70C099);
 *  tableHighLightColour = Colour(0xDDA0C9B0);
 *
 */

void CustomLookAndFeel::drawTableHeaderColumn(Graphics &g,
                                              const String &columnName,
                                              int columnId,
                                              int width,
                                              int height,
                                              bool isMouseOver,
                                              bool isMouseDown,
                                              int columnFlags)
{

    g.fillAll (tableBackGroundColour);


    Rectangle<int> area (width, height);
    area.reduce (4, 0);

    if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
    {
        Path sortArrow;
        sortArrow.addTriangle (0.0f, 0.0f,
                               0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
                               1.0f, 0.0f);

        g.setColour (Colour (0x99000000));
        g.fillPath (sortArrow, sortArrow.getTransformToScaleToFit (area.removeFromRight (height / 2).reduced (2).toFloat(), true));
    }

    g.setColour (tableTextColour);
    g.setFont (Font (height * 0.5f, Font::bold));
    g.drawFittedText (columnName, area, Justification::centred, 1);
}

void CustomLookAndFeel::drawTableHeaderBackground(Graphics &g,
                                                  TableHeaderComponent &t)
{
    g.fillAll (Colours::black);

    Rectangle<int> area (t.getLocalBounds());

    g.setGradientFill (ColourGradient (tableBackGroundColour, 0.0f, (float) area.getY(),
                                       tableBackGroundColour.darker(0.5f), 0.0f, (float) area.getBottom(),
                                       false));
    g.fillRect (area);

    g.setColour(tableOutLineColour);
    g.drawRect(area, 1.f);

    for (int i = t.getNumColumns (true); --i >= 0;)
        g.fillRect (t.getColumnPosition (i).removeFromRight (1));
}
