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

#include "StepSequencerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>
#include <vector>

using std::vector;
//[/MiscUserDefs]

//==============================================================================
StepSequencerComponent::StepSequencerComponent (const String &name,AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      commandValues(-1),
      commandStates(-1),
      commandSize(-1),
      commandOffset(-1),
      commandSync(-1),
      commandDuration(-1),
      commandExponent(-1)
{
    //[Constructor_pre] You can add your own custom stuff here..

    /*  check against the following labels
     *
     *    AudioParameter       paramCom        label
     *  -------------------------------------------------
     *  AudioParameterBool  : ParamToggle   : "enable"
     *  AudioParameterBool  : ParamToggle   : "sync"
     *  AudioParameterBool  : ParamToggle   : "chopper"
     *  AudioParameterFloat : ParamSlider   : "duration"
     *  AudioParameterFloat : ParamSlider   : "length"
     *  AudioParameterFloat : ParamSlider   : "offset"
     *  AudioParameterFloat : ParamSlider   : "curve"
     *  AudioParameterInt   : ParamSlider   : "amount"
     *
     */

    vector<int> paramIndices = p.getParameterIndices(AkatekoAudioProcessor::StepSequencerId);
    StringArray paramName = p.getParameterIds(AkatekoAudioProcessor::StepSequencerId);

    int nrOfParams = paramIndices.size();

    if(nrOfParams == 8){
        addAndMakeVisible(enableToggle = new ParamToggle(paramName[0], *p.getParameters().getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(syncToggle = new ParamToggle(paramName[1], *p.getParameters().getUnchecked(paramIndices[1]), label));
        addAndMakeVisible(chopperToggle = new ParamToggle(paramName[2], *p.getParameters().getUnchecked(paramIndices[2]), label));
        addAndMakeVisible(durationSlider = new ParamSlider(paramName[3], *p.getParameters().getUnchecked(paramIndices[3]), label, 50.f, 5000.f));
        addAndMakeVisible(stepLengthSlider = new ParamSlider(paramName[4], *p.getParameters().getUnchecked(paramIndices[4]), label, 0.05f, 1.00));
        addAndMakeVisible(offsetSlider = new ParamSlider(paramName[5], *p.getParameters().getUnchecked(paramIndices[5]), label, 0.f, 1.f));
        addAndMakeVisible(exponentSlider = new ParamSlider(paramName[6], *p.getParameters().getUnchecked(paramIndices[6]), label));
        addAndMakeVisible(stepComboBox = new ParamComboBox(paramName[7], *p.getParameters().getUnchecked(paramIndices[7]), label));
    } else {
        addAndMakeVisible(enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (syncToggle = new ToggleButton ("syncToggle"));
        addAndMakeVisible (chopperToggle = new ToggleButton("chopperTOggle"));
        addAndMakeVisible (durationSlider = new Slider ("durationSlider"));
        addAndMakeVisible (stepLengthSlider = new Slider ("stepLength"));
        addAndMakeVisible (offsetSlider = new Slider ("offsetSlider"));
        addAndMakeVisible (exponentSlider = new Slider ("exponentSlider"));
        addAndMakeVisible (stepComboBox = new ComboBox ("stepComboBox"));

        std::cout << "StepSequencerComponent::StepSequencerComponent" << std::endl;
        std::cout << "/------------Parameters not bound------------/" << std::endl;
    }

    //[/Constructor_pre]


    //[UserPreSize]
    addAndMakeVisible (stepSequencer = new StepSequencer ("sseq"));
    stepSequencer->setName ("stepSequencer");

    stepLengthSlider->setRange (0, 1, 0);
    stepLengthSlider->setSliderStyle (Slider::LinearHorizontal);
    stepLengthSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

    enableToggle->setButtonText (String());
    enableToggle->addListener (this);

    syncToggle->setTooltip (TRANS("sync"));
    syncToggle->setButtonText (String());
    syncToggle->addListener (this);


    durationSlider->setRange (0, 1, 0);
    durationSlider->setSliderStyle (Slider::LinearHorizontal);
    durationSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

    stepComboBox->setEditableText (false);
    stepComboBox->setJustificationType (Justification::centredLeft);
    stepComboBox->setTextWhenNothingSelected (TRANS("Step Amount"));
    stepComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    stepComboBox->addItem (TRANS("8"), 1);
    stepComboBox->addItem (TRANS("12"), 2);
    stepComboBox->addItem (TRANS("16"), 3);
    stepComboBox->addItem (TRANS("24"), 4);
    stepComboBox->addItem (TRANS("32"), 5);
    stepComboBox->addListener (this);

    addAndMakeVisible (nameLabel = new Label ("nameLabel",
                                              TRANS("SSEQ")));
    nameLabel->setFont (Font ("Good Times", 15.00f, Font::plain));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (Label::textColourId, Colours::white);
    nameLabel->setColour (TextEditor::textColourId, Colours::black);
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    offsetSlider->setRange (0, 1, 0);
    offsetSlider->setSliderStyle (Slider::LinearHorizontal);
    offsetSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

    exponentSlider->setRange (0, 1, 0);
    exponentSlider->setSliderStyle (Slider::LinearHorizontal);
    exponentSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    exponentSlider->addListener(this);

    chopperToggle->setTooltip (TRANS("Generated curve is affected by the step Length"));
    chopperToggle->setButtonText (String());
    chopperToggle->addListener (this);

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText (TRANS("t"));
    textButton->addListener (this);
    //[/UserPreSize]

    setSize (515, 250);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

StepSequencerComponent::~StepSequencerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]
    stepSequencer = nullptr;
    stepLengthSlider = nullptr;
    enableToggle = nullptr;
    syncToggle = nullptr;
    durationSlider = nullptr;
    stepComboBox = nullptr;
    nameLabel = nullptr;
    offsetSlider = nullptr;
    exponentSlider = nullptr;
    chopperToggle = nullptr;
    textButton = nullptr;
    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void StepSequencerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323232));

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("Length"),
                436, 82, 52, 12,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("duration"),
                431, 40, 73, 10,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("offvalue"),
                431, 126, 65, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("curve"),
                436, 166, 49, 12,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StepSequencerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    stepSequencer->setBounds (5, 5, 400, 240);
    stepLengthSlider->setBounds (408, 96, 104, 20);
    enableToggle->setBounds (494, 0, 24, 24);
    syncToggle->setBounds (408, 32, 24, 24);
    durationSlider->setBounds (408, 56, 104, 20);
    stepComboBox->setBounds (412, 224, 96, 20);
    nameLabel->setBounds (431, 1, 60, 24);
    offsetSlider->setBounds (408, 136, 104, 20);
    exponentSlider->setBounds (408, 184, 104, 20);
    chopperToggle->setBounds (408, 160, 24, 24);
    textButton->setBounds (408, 8, 23, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StepSequencerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enableToggle)
    {
        //[UserButtonCode_enableToggle] -- add your button handler code here..
        //[/UserButtonCode_enableToggle]
    }
    else if (buttonThatWasClicked == syncToggle)
    {
        //[UserButtonCode_syncToggle] -- add your button handler code here..
        //stepSequencer->printValues();

        //[/UserButtonCode_syncToggle]
    }
    else if (buttonThatWasClicked == chopperToggle)
    {
        //[UserButtonCode_chopperToggle] -- add your button handler code here..
        processor.setChopCurve();
        //[/UserButtonCode_chopperToggle]
    }
    else if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void StepSequencerComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == stepComboBox)
    {
        //[UserComboBoxCode_stepComboBox] -- add your combo box handling code here..
        int tmpIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        stepAmount = 8;

        switch(tmpIndex){
            case 0:
                stepAmount = 8;
                break;
            case 1:
                stepAmount = 12;
                break;
            case 2:
                stepAmount = 16;
                break;
            case 3:
                stepAmount = 24;
                break;
            case 4:
                stepAmount = 32;
                break;
        }

        stepSequencer->setStepSize(stepAmount);
        processor.setStepSeq(stepSequencer->getStepValues(), stepSequencer->getButtonStates());
        processor.setUIState(stepSequencer->toString(), AkatekoAudioProcessor::StepSequencerId);
        //[/UserComboBoxCode_stepComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void StepSequencerComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == exponentSlider)
    {
        //[UserSliderCode_exponentSlider] -- add your slider handling code here..
        //[/UserSliderCode_exponentSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void StepSequencerComponent::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    switch(commandId){
        case StepSequencer::valuesUpdated:
            processor.setStepValues(stepSequencer->getStepValues());
            processor.setUIState(stepSequencer->toString(), AkatekoAudioProcessor::StepSequencerId);
            break;
        case StepSequencer::buttonsUpdated:
            processor.setButtonStates(stepSequencer->getButtonStates());
            processor.setUIState(stepSequencer->toString(), AkatekoAudioProcessor::StepSequencerId);
            break;
    }
    //[/UserCode_handleCommandMessage]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void StepSequencerComponent::sliderDragEnded(Slider *sliderThatWasDragged){
    if (sliderThatWasDragged == exponentSlider)
    {
        //[UserSliderCode_exponentSlider] -- add your slider handling code here..
        processor.setStepMidPoint();
        //[/UserSliderCode_exponentSlider]
    }
}

vector<double> StepSequencerComponent::getStepValues(){
    return stepSequencer->getStepValues();
}

vector<int> StepSequencerComponent::getButtonStates(){
    return stepSequencer->getButtonStates();
}

int StepSequencerComponent::getStepAmount(){
    return stepAmount;
}

void StepSequencerComponent::setValuesCommandId(int hashCode){
    commandValues = hashCode;
}

void StepSequencerComponent::setStatesCommandId(int hashCode){
    commandStates = hashCode;
}

void StepSequencerComponent::setSizeCommandId(int hashCode){
    commandSize = hashCode;
}

void StepSequencerComponent::setOffsetCommandId(int hashCode){
    commandOffset = hashCode;
}

void StepSequencerComponent::setSyncCommandId(int hashCode){
    commandSync = hashCode;
}

void StepSequencerComponent::setChopCommandId(int hashCode){
    commandChop = hashCode;
}

void StepSequencerComponent::setDurationCommandId(int hashCode){
    commandDuration = hashCode;
}

void StepSequencerComponent::setExponentCommandId(int hashCode){
    commandExponent = hashCode;
}

String StepSequencerComponent::getUIState(){
    return stepSequencer->toString();
}

void StepSequencerComponent::setUIState(String state){
   stepSequencer->restoreFromString(state);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StepSequencerComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name,AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;commandValues(-1)&#10;commandStates(-1)&#10;commandSize(-1)&#10;commandOffset(-1)&#10;commandSync(-1)&#10;commandDuration(-1)&#10;commandExponent(-1)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="515" initialHeight="250">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323232">
    <TEXT pos="436 82 52 12" fill="solid: ffaaaaaa" hasStroke="0" text="Length"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="33"/>
    <TEXT pos="431 40 73 10" fill="solid: ffaaaaaa" hasStroke="0" text="duration"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="33"/>
    <TEXT pos="431 126 65 12" fill="solid: ffaaaaaa" hasStroke="0" text="offvalue"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
    <TEXT pos="436 166 49 12" fill="solid: ffaaaaaa" hasStroke="0" text="curve"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="stepSequencer" id="86fad89d553cc7f2" memberName="stepSequencer"
                    virtualName="StepSequencer" explicitFocusOrder="0" pos="5 5 400 240"
                    class="Component" params="&quot;sseq&quot;"/>
  <SLIDER name="stepLength" id="3c6a04987e56e55e" memberName="stepLengthSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 96 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="enableToggle" id="1ba5aa71427704e0" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="494 0 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="syncToggle" id="fc1623111ab0edcb" memberName="syncToggle"
                virtualName="" explicitFocusOrder="0" pos="408 32 24 24" tooltip="sync"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="durationSlider" id="5a0add9013433671" memberName="durationSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 56 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <COMBOBOX name="stepComboBox" id="76c1081265fe0ec3" memberName="stepComboBox"
            virtualName="" explicitFocusOrder="0" pos="412 224 96 20" editable="0"
            layout="33" items="8&#10;12&#10;16&#10;24&#10;32" textWhenNonSelected="Step Amount"
            textWhenNoItems="(no choices)"/>
  <LABEL name="nameLabel" id="a3d123be4d25c5a3" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="431 1 60 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="SSEQ" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Good Times"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="offsetSlider" id="162a5c977a1e137c" memberName="offsetSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 136 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="exponentSlider" id="befbb53b3929a4c8" memberName="exponentSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 184 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="ChopperToggle" id="b40454ae286af469" memberName="chopperToggle"
                virtualName="" explicitFocusOrder="0" pos="408 160 24 24" tooltip="Generated curve is affected by the step Length"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="new button" id="5277f09e753d96aa" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="408 8 23 24" buttonText="t"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
