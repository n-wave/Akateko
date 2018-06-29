/*
  ==============================================================================

    XYPad.cpp
    Created: 7 Mar 2018 11:02:58am
    Author:  mario

  ==============================================================================
*/


#include "Akateko.h"
#include "XYPad.h"

using akateko::MidiRow;
using akateko::initMidiRow;

//==============================================================================

/*
 *
 *  Colour backGroundColour;
 *   Colour fillColour;
 *   Colour ellipseColour;
 *   Colour highLightColour;
 *   Colour overlayGradientColourOne;
 *   Colour overlayGradientColourTwo;
 *   ColourGradient overlayColour;
 *
 */

XYPad::XYPad(AkatekoAudioProcessor &p,
             Label &label)
    :
        processor(p),
        useImage(false),
        dragEnabled(false),
        labelRef(label),
        maxWidth(0.f),
        maxHeight(0.f),
        minWidth(0.f),
        minHeight(0.f),
        widthRange(0.f),
        heightRange(0.f),
        cMinWidth(0.f),
        cMinHeight(0.f),
        cMaxWidth(0.f),
        cMaxHeight(0.f),
        scalar(0.0),
        ellipseDia(0.f),
        ellipseRadi(0.f),
        updateCommandId(-1),
        backGroundColour(Colour(0xFF000000)),
        outlineColour(Colours::grey),
        fillColour(Colour(0xFF90B090)),
        ellipseColour(Colour(0xFFb0C0B0)),
        highLightColour(Colour(0x7FFFFFD2)),
        overlayGradientColourOne(Colour(0x00000000)),
        overlayGradientColourTwo(Colour(0x5060A070))
{    
    const OwnedArray<AudioProcessorParameter> &params = processor.getParameters();
    paramIndices = processor.getParameterIndices(AkatekoAudioProcessor::xyPadId);


    if(paramIndices.size() == 3 &&
       params.size() >= paramIndices[2])
    {               
        xyPadX = params.getUnchecked(paramIndices[0]);
        xyPadY = params.getUnchecked(paramIndices[1]);
        xyPadT = params.getUnchecked(paramIndices[2]);

    } else {
        std::cerr << "XYPad::XYPad" << std::endl;
        std::cerr << "Parameters are not bound properly" << std::endl;
    }

    if(xyPadX != nullptr && xyPadY != nullptr){
        xAxis = xyPadX->getValue();
        yAxis = xyPadY->getValue();
    }

    //  Use Graphics to make a distinction
    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");

    overlayColour = ColourGradient(backGroundColour,0,0,backGroundColour,0,0, true);
}

XYPad::~XYPad()
{
    xyPadX = nullptr;
    xyPadY = nullptr;
    xyPadT = nullptr;
}

void XYPad::paint (Graphics& g)
{
    int tmpWidth = this->getWidth();
    int tmpHeight = this->getHeight();

    g.setColour(backGroundColour);

    g.fillRoundedRectangle(0,
                           0,
                           tmpWidth,
                           tmpHeight,
                           5.0);

    g.setColour (outlineColour);        // draw an outline around the component

    g.drawRoundedRectangle(0,
                           0,
                           tmpWidth,
                           tmpHeight,
                           10.0,
                           4.0);

    if(useImage){
        g.setTiledImageFill(backgroundImage, minWidth, minHeight, 0.85f);
        g.fillRect(minWidth,minHeight, widthRange, heightRange);

        g.setGradientFill(innerOverlay);
        g.fillRect(minWidth,minHeight, widthRange, heightRange);
    }

    g.setColour(Colour(0x509F9F9F));
    g.drawRect(minWidth,minHeight, widthRange, heightRange, 2.0f);

    drawCursor(g);

    g.setGradientFill(overlayColour);
    g.fillRoundedRectangle(0 ,0 ,tmpWidth, tmpHeight, 5.0);
}

void XYPad::drawCursor(Graphics &g){
    float xPos = cursor.x-ellipseRadi;
    float yPos = cursor.y-ellipseRadi;

    const ColourGradient gradient = ColourGradient(fillColour,
                                                   cursor.x,
                                                   cursor.y,
                                                   Colours::transparentBlack,
                                                   cursor.x+(ellipseRadi*0.65),
                                                   cursor.y+(ellipseRadi*0.65),
                                                   true
                                                   );

    g.setGradientFill(gradient);
    g.fillEllipse(xPos, yPos, ellipseDia, ellipseDia);
}

void XYPad::resized()
{
    this->setOverlayColour(overlayGradientColourOne,
                           overlayGradientColourTwo);

    this->init();
}

String XYPad::toString(){
    String tmpXYPad = String("xyp ");

    tmpXYPad += String(cursor.x) + " ";
    tmpXYPad += String(xAxis) + " ";
    tmpXYPad += String(cursor.y) + " ";
    tmpXYPad += String(yAxis);

    return tmpXYPad;
}

void XYPad::restoreFromString(String state){
    StringArray tokens;

    tokens.addTokens(state, " ", "\"");
    int tokenSize = tokens.size();

    if(tokenSize == 5 && tokens[0] == "xyp"){
        cursor.setX(tokens[1].getFloatValue());
        xAxis = tokens[2].getDoubleValue();
        cursor.setY(tokens[3].getFloatValue());
        yAxis = tokens[4].getDoubleValue();

        if(xyPadX != nullptr &&
           xyPadY != nullptr)
        {
            xyPadX->setValue(xAxis);
            xyPadY->setValue(yAxis);
        }

    }

    this->repaint();
}

void XYPad::init(){
    float tmpWidth = this->getWidth();
    float tmpHeight = this->getHeight();

    //Inner Rectangle Boundary's
    minWidth = tmpWidth*0.05;
    minHeight = tmpHeight*0.05;
    maxWidth = tmpWidth-minWidth;
    maxHeight = tmpHeight-minHeight;
    widthRange = tmpWidth*0.9;
    heightRange = tmpHeight*0.9;

    ellipseDia = tmpWidth*0.15;
    ellipseRadi = ellipseDia*0.5;

    //Cursor's Boundary
    cMaxWidth = maxWidth-ellipseRadi;
    cMaxHeight = maxHeight-ellipseRadi;
    cMinWidth = minWidth+ellipseRadi;
    cMinHeight = minHeight+ellipseRadi;

    cWidthRange = cMaxWidth-cMinWidth;
    cHeightRange = cMaxHeight-cMinHeight;

    scalar = 1/(widthRange - ellipseDia);

    cursor = Point<float>(cMinWidth+(cWidthRange*xAxis),
                          cMinHeight+(cHeightRange*(1-yAxis)));

  //  std::cout << scalar << std::endl;
  //  std::cout << widthRange << std::endl;
  //  std::cout << heightRange << std::endl;
}

void XYPad::setBackgroundImage(const Image &imageToUse, bool use){
    backgroundImage = Image(imageToUse);

    innerOverlay = ColourGradient(Colour(0x7F000000),
                                  getWidth()*0.5,
                                  getHeight()*0.5,
                                  Colour(0xFF000000),
                                  maxWidth*0.95,
                                  maxHeight*0.95,
                                  true);

    useImage = true;
}

void XYPad::setUpdateCommandId(int id){
    updateCommandId = id;
}

void XYPad::setColour(const int colourId, Colour colour){
    switch(colourId){
        case backgroundColourId:
            backGroundColour = colour;
            break;
        case outlineColourId:
            outlineColour = colour;
            break;
        case fillColourId:
            fillColour = colour;
            break;
        case ellipseColourId:
            ellipseColour = colour;
            break;
        case highlightColourId:
            highLightColour = colour;
            break;
        case overlayGradientOneId:
            overlayGradientColourOne = colour;
            setOverlayColour(overlayGradientColourOne,
                             overlayGradientColourTwo);
            break;
        case overlayGradientTwoId:
            overlayGradientColourTwo = colour;
            setOverlayColour(overlayGradientColourOne,
                             overlayGradientColourTwo);
            break;
    }
}

void XYPad::setOverlayColour(const Colour &colourOne, const Colour &colourTwo){
    int tmpHeight = this->getHeight();
    int tmpWidth = this->getWidth();

    overlayColour = ColourGradient(colourOne,   //Colour One
                                   tmpWidth*0.5,  //X pos Colour one
                                   tmpHeight*0.5, //Y pos Colour one
                                   colourTwo,   //Colour Two
                                   tmpWidth*1.3,  //X pos Colour Two
                                   tmpHeight*1.3, //Y pos Colour Two
                                   true);         //Set radial

}

void XYPad::mouseUp(const MouseEvent &event){
    if(xyPadT != nullptr){
    xyPadT->setValue(0);
    }

    dragEnabled = false;
    getParentComponent()->postCommandMessage(updateCommandId);
}

/* Mouse callBacks */
void XYPad::mouseDown(const MouseEvent &event){
    Point<float> mousePosition = event.position;
    ModifierKeys modifier = event.mods;

    if(checkBounds(mousePosition)){

        float tmpXAxis = xAxis;
        float tmpYaxis = yAxis;

        if(modifier.isLeftButtonDown() && !modifier.isCtrlDown()){
            float xPos = mousePosition.x;
            float yPos = mousePosition.y;

            xPos = checkHorizontal(xPos);
            yPos = checkVertical(yPos);

            cursor.setX(xPos);
            cursor.setY(yPos);


            xAxis = (cursor.x-cMinWidth)*scalar;
            yAxis = 1-(cursor.y-cMinHeight)*scalar;

            dragEnabled = true;

            if(yAxis < 0){
                yAxis = 0;
            }

            if(xyPadX != nullptr &&
               xyPadY != nullptr &&
               xyPadT != nullptr)
            {
                xyPadX->setValue(xAxis);
                xyPadY->setValue(yAxis);
                xyPadT->setValue(1);
            }
        }

        else if(modifier.isCtrlDown())
        {
            cursor.setX(getWidth()*0.5);
            cursor.setY(getHeight()*0.5);

            if(xyPadX != nullptr &&
               xyPadY != nullptr &&
               xyPadT != nullptr)
            {
                xyPadX->setValue(0.5);
                xyPadY->setValue(0.5);
                xyPadT->setValue(0);
            }

            dragEnabled = false;
        }

        if(xAxis != tmpXAxis || yAxis != tmpYaxis){
            String tmpString = getName() + "[" + String(xAxis,2) + "," +  String(yAxis, 2) + "]";
            labelRef.setText(tmpString, dontSendNotification);
        }

        this->repaint();

    }

    if(modifier.isRightButtonDown()){
        int param = checkPosition(mousePosition);
        int index = menu.show();

        if(param != -1){
            handlePopupMenu(index, param);
        }
    }
}

void XYPad::mouseDrag(const MouseEvent &event){
    if(dragEnabled){
        Point<float> mousePosition = event.position;

        float tmpXAxis = xAxis;
        float tmpYaxis = yAxis;

        float xPos = mousePosition.x;
        float yPos = mousePosition.y;

        xPos = checkHorizontal(xPos);
        yPos = checkVertical(yPos);

        cursor.setX(xPos);
        cursor.setY(yPos);

        /* Flip axis */

        xAxis = (cursor.x-cMinWidth)*scalar;
        yAxis = 1-(cursor.y-cMinHeight)*scalar;

       if(yAxis < 0){
           yAxis = 0;
       }

       if(xyPadX != nullptr &&
          xyPadY != nullptr)
       {
          xyPadX->setValue(xAxis);
          xyPadY->setValue(yAxis);
       }

       if(xAxis != tmpXAxis || yAxis != tmpYaxis){
           String tmpString = getName() + "[" + String(xAxis,2) + "," +  String(yAxis, 2) + "]";
           labelRef.setText(tmpString, dontSendNotification);
       }

       this->repaint();
    }
}

bool XYPad::checkBounds(Point<float> &point){
    bool result = false;

    if(point.x >= minWidth && point.x <= maxWidth &&
       point.y >= minHeight && point.y <= maxHeight)
    {
        result = true;
    }

    return result;
}

float XYPad::checkHorizontal(float xPosition){
    float xPos = xPosition;

    if(xPos <= cMinWidth){
        xPos = cMinWidth;
    }

    if(xPos >= cMaxWidth){
        xPos = cMaxWidth;
    }

    return xPos;
}

float XYPad::checkVertical(float yPosition){
    float yPos = yPosition;

    if(yPos <= cMinHeight){
        yPos = cMinHeight;
    }

    if(yPos >= cMaxHeight){
        yPos = cMaxHeight;
    }

    return yPos;
}

int XYPad::checkPosition(Point<float> &point){
    if(point.x >= minWidth &&
       point.x <= maxWidth &&
       point.y >= 0 &&
       point.y <= minHeight)
    {
        return 0; // Create X-axis Menu
    } else if(point.y >= minHeight &&
              point.y <= maxHeight &&
              point.x >= 0 &&
              point.x <= minWidth)
    {
        return 1;
    } else if(checkBounds(point)){
        return 2;
    }

    return -1;
}

void XYPad::handlePopupMenu(int choice, int param){
    if(choice == 1){
        if(paramIndices.size() == 3){
            MidiRow tmpRow;

            switch(param){
                case 0: initMidiRow(tmpRow, 0, 0, 127, 0.0, 1.0, paramIndices[0], akateko::MIDI_TO_DOUBLE, " XY-Pad: X-Axis", 8); break;
                case 1: initMidiRow(tmpRow, 0, 1, 127, 0.0, 1.0, paramIndices[1], akateko::MIDI_TO_DOUBLE, " XY-Pad: Y-Axis", 8); break;
                case 2: initMidiRow(tmpRow, 2, 0, 127, 0, 1, paramIndices[2], akateko::MIDI_TO_INT_BUTTON, " XY-Pad: Trigger", 8); break;
            }
            processor.initiateMidiLearn(tmpRow);
        }
    } else if(choice == 0xFF){
        if(paramIndices.size() == 3){
            switch(param){
                case 0: processor.removeMidiRow(paramIndices[0]); break;
                case 1: processor.removeMidiRow(paramIndices[1]) ;break;
                case 2: processor.removeMidiRow(paramIndices[2]); break;
            }
        }
    }
}

void XYPad::setLookAndFeel(LookAndFeel *laf){
    menu.setLookAndFeel(laf);
}

void XYPad::updateGui(){
    if(xyPadX != nullptr && xyPadY != nullptr){
        xAxis = xyPadX->getValue();
        yAxis = xyPadY->getValue();

        cursor.setX(cMinWidth+(cWidthRange*xAxis));
        cursor.setY(cMinHeight+(cHeightRange*(1-yAxis)));

        repaint();
    }
}
