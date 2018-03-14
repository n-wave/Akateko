/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ModulationMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ModulationMatrixComponent::ModulationMatrixComponent (ModulationMatrix &modm)
    : matrix(modm)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (matrixTable = new MatrixTable(modm));
    matrixTable->setName ("matrixTable");


    //[UserPreSize]
    matrixTable->setColour(TableListBox::backgroundColourId, Colour(0xFF000000));


    //[/UserPreSize]

    setSize (515, 250);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ModulationMatrixComponent::~ModulationMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    matrixTable = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ModulationMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff7f7f7f));

    //[UserPaint] Add your own custom painting code here..

    g.setColour (Colours::grey);
    g.fillRoundedRectangle(2.5f,
                           2.5f,
                           510,
                           245,
                           5.00);

    //[/UserPaint]
}

void ModulationMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    matrixTable->setBounds (5, 5, 505, 240);
    //[UserResized] Add your own custom resize handling here..
    matrixTable->initialiseHeader(505, 240);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


/*      Command Messages from the matrix Table
 *
 *      RowAddedId,
 *      RowRemovedId,
 *      SourceChangedId,
 *      TransformChangedId,
 *      RangeChangedId,
 *      DestinationChangedId,
 *      RowEnablementChangedId
 *
 */


void ModulationMatrixComponent::handleCommandMessage(int commandId){
    int activeRow = -1;
    int activeColumn = -1;

    matrixTable->getRowAndColumn(activeRow, activeColumn);


    std::cout << "\n ModulationMatrixComponent::handleCommandMessage" << std::endl;
    std::cout << "Active Row : " << activeRow << std::endl;
    std::cout << "Active Column : " << activeColumn << "\n"<< std::endl;

    switch(commandId){
        case MatrixTable::RowAddedId:
            std::cout << "Row needs to be added" << std::endl;
            //matrix.addRow(matrixTable->getMatrixRow(activeRow));
            break;
        case MatrixTable::RowRemovedId:
            //matrix.removeRow(activeRow);
            break;
        case MatrixTable::SourceChangedId:
            //matrix.setSource(matrixTable->getSource(activeRow), activeRow);
            break;
        case MatrixTable::TransformChangedId:
            //matrix.setTransform(matrixTable->getTransform(activeRow), activeRow);
            break;
        case MatrixTable::RangeChangedId:
            //matrix.setRange(matrixTable->getRange(activeRow), activeRow);
            break;
        case MatrixTable::IntensityChangedId:
            //matrix.setIntensity(matrixTable->getIntensity(activeRow), activeRow);
            break;
        case MatrixTable::DestinationChangedId:
            //matrix.setDestination(matrixTable->getDestination(activeRow), activeRow);
            break;
        case MatrixTable::RowEnablementChangedId:
            //matrix.setEnabled(matrixTable->getEnabled(activeRow), activeRow);
            break;
     }
    String result = matrix.toString();

    std::cout <<  result << std::endl;

}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModulationMatrixComponent"
                 componentName="" parentClasses="public Component" constructorParams="ModulationMatrix &amp;modm"
                 variableInitialisers="matrix(modm)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="515"
                 initialHeight="250">
  <BACKGROUND backgroundColour="ff7f7f7f"/>
  <GENERICCOMPONENT name="matrixTable" id="9691f8056a245af3" memberName="matrixTable"
                    virtualName="MatrixTable" explicitFocusOrder="0" pos="5 5 505 240"
                    class="Component" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
