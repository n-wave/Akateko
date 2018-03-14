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

#include "FilterComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
/*

 Parameters Are bounded as follows

 filterEnable       : ParamID = FLEN
 filterConfig       : ParamID = FLCF

 filterOneEnable    : ParamID = F1EN
 filterOneType      : ParamID = F1TP
 filterOneRollOff   : ParamID = F1RF
 filterOneFrequency : ParamID = F1FQ
 filterOneResonance : ParamID = F1RE

 filterTwoEnable    : ParamID = F2EN
 filterTwoType      : ParamID = F2TP
 filterTwoRollOff   : ParamID = F2RF
 filterTwoFrequency : ParamID = F2FQ
 filterTwoResonance : ParamID = F2RE
 */
//[/MiscUserDefs]

//==============================================================================
FilterComponent::FilterComponent (const String &name, AkatekoAudioProcessor &p, Label &label) :
    Component::Component(name),
    processor(p),
    labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    //[Constructor_pre] You can add your own custom stuff here..
    vector<int> paramIndices = p.getParameterIndices(AkatekoAudioProcessor::FilterId);
    StringArray paramNames = p.getParameterIds(AkatekoAudioProcessor::FilterId);

    int nrOfParams = paramIndices.size();

    if(nrOfParams == 12){
        addAndMakeVisible (filterToggleButton = new ParamToggle (paramNames[0], *p.getParameters().getUnchecked(paramIndices[0]), label));
        addAndMakeVisible (filterConfigComboBox = new ParamComboBox (paramNames[1], *p.getParameters().getUnchecked(paramIndices[1]), label));
        addAndMakeVisible (filterOneToggleButton = new ParamToggle (paramNames[2], *p.getParameters().getUnchecked(paramIndices[2]), label));
        addAndMakeVisible (filterOneTypeComboBox = new ParamComboBox (paramNames[3], *p.getParameters().getUnchecked(paramIndices[3]), label));
        addAndMakeVisible (filterOneRollOffComboBox = new ParamComboBox (paramNames[4], *p.getParameters().getUnchecked(paramIndices[4]), label));
        addAndMakeVisible (filterOneFrequencySlider = new ParamSlider (paramNames[5], *p.getParameters().getUnchecked(paramIndices[5]), label, 80., 18000.));
        addAndMakeVisible (filterOneResonanceSlider = new ParamSlider (paramNames[6], *p.getParameters().getUnchecked(paramIndices[6]), label));
        addAndMakeVisible (filterTwoToggleButton = new ParamToggle (paramNames[7], *p.getParameters().getUnchecked(paramIndices[7]), label));
        addAndMakeVisible (filterTwoTypeComboBox = new ParamComboBox (paramNames[8], *p.getParameters().getUnchecked(paramIndices[8]), label));
        addAndMakeVisible (filterTwoRollOffComboBox = new ParamComboBox (paramNames[9], *p.getParameters().getUnchecked(paramIndices[9]), label));
        addAndMakeVisible (filterTwoFrequencySlider = new ParamSlider (paramNames[10], *p.getParameters().getUnchecked(paramIndices[10]), label, 80., 18000.));
        addAndMakeVisible (filterTwoResonanceSlider = new ParamSlider (paramNames[11], *p.getParameters().getUnchecked(paramIndices[11]), label));
    } else{
        addAndMakeVisible (filterToggleButton = new ToggleButton ("filterToggle"));
        addAndMakeVisible (filterConfigComboBox = new ComboBox ("filterConfig"));
        addAndMakeVisible (filterOneToggleButton = new ToggleButton ("filterOneToggle"));
        addAndMakeVisible (filterOneTypeComboBox = new ComboBox ("filterOneType"));
        addAndMakeVisible (filterOneRollOffComboBox = new ComboBox ("filterOneRollOff"));
        addAndMakeVisible (filterOneFrequencySlider = new Slider ("filterOneFrequency"));
        addAndMakeVisible (filterOneResonanceSlider = new Slider ("filterOneResonance"));
        addAndMakeVisible (filterTwoToggleButton = new ToggleButton ("filterTwoToggle"));
        addAndMakeVisible (filterTwoTypeComboBox = new ComboBox ("filterTwoType"));
        addAndMakeVisible (filterTwoRollOffComboBox = new ComboBox ("filterTwoRollOff"));
        addAndMakeVisible (filterTwoFrequencySlider = new Slider ("filterTwoFrequency"));
        addAndMakeVisible (filterTwoResonanceSlider = new Slider ("filterTwoResonance"));

        std::cerr << "FilterComponent::CilterComponent" << std::endl;
        std::cerr << "/-----Parameters not bound-----/" << std::endl;
    }

    addAndMakeVisible (filterDisplay = new FilterDisplay ("fdis"));
    filterDisplay->setName ("filterDisplay");
    //[/Constructor_pre]

    //[UserPreSize]

    /* Bypass/Enable Filter */
    filterToggleButton->setButtonText (String());

    /* Serial/Paralell Filter */
    filterConfigComboBox->setEditableText (false);
    filterConfigComboBox->setJustificationType (Justification::centredLeft);
    filterConfigComboBox->setTextWhenNothingSelected (String());
    filterConfigComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterConfigComboBox->addItem (TRANS("Series"), 1);
    filterConfigComboBox->addItem (TRANS("Parallel"), 2);
    filterConfigComboBox->addSeparator();
    filterConfigComboBox->postCommandMessage(ParamComboBox::update);
    filterConfigComboBox->addListener (this);


    this->initialiseFilterOne();
    this->initialiseFilterTwo();
    //[/UserPreSize]

    setSize (255, 255);


    //[Constructor] You can add your own custom stuff here..
    filterDisplay->enableFilter(filterOneToggleButton->getToggleState(), 1);
    filterDisplay->enableFilter(filterTwoToggleButton->getToggleState(), 2);
    //[/Constructor]
}

FilterComponent::~FilterComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    filterOneFrequencySlider = nullptr;
    filterOneResonanceSlider = nullptr;
    filterTwoFrequencySlider = nullptr;
    filterTwoResonanceSlider = nullptr;
    filterDisplay = nullptr;
    filterOneTypeComboBox = nullptr;
    filterOneRollOffComboBox = nullptr;
    filterToggleButton = nullptr;
    filterTwoTypeComboBox = nullptr;
    filterTwoRollOffComboBox = nullptr;
    filterOneToggleButton = nullptr;
    filterTwoToggleButton = nullptr;
    filterConfigComboBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    filterOneFrequencySlider->setBounds (3, 176, 65, 65);
    filterOneResonanceSlider->setBounds (75, 184, 50, 50);
    filterTwoFrequencySlider->setBounds (131, 176, 65, 65);
    filterTwoResonanceSlider->setBounds (203, 184, 50, 50);
    filterDisplay->setBounds (1, 24, 253, 128);
    filterOneTypeComboBox->setBounds (23, 152, 50, 20);
    filterOneRollOffComboBox->setBounds (73, 152, 50, 20);
    filterToggleButton->setBounds (1, 0, 24, 24);
    filterTwoTypeComboBox->setBounds (151, 152, 50, 20);
    filterTwoRollOffComboBox->setBounds (201, 152, 50, 20);
    filterOneToggleButton->setBounds (1, 151, 24, 24);
    filterTwoToggleButton->setBounds (129, 151, 24, 24);
    filterConfigComboBox->setBounds (174, 3, 80, 20);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == filterOneFrequencySlider)
    {
        //[UserSliderCode_filterOneFrequencySlider] -- add your slider handling code here..
        float freq = sliderThatWasMoved->getValue();
        filterDisplay->setFrequency(freq, 1);
        //[/UserSliderCode_filterOneFrequencySlider]
    }
    else if (sliderThatWasMoved == filterOneResonanceSlider)
    {
        //[UserSliderCode_filterOneResonanceSlider] -- add your slider handling code here..
        float reso = sliderThatWasMoved->getValue();
        filterDisplay->setResonance(reso, 1);
        //[/UserSliderCode_filterOneResonanceSlider]
    }
    else if (sliderThatWasMoved == filterTwoFrequencySlider)
    {
        //[UserSliderCode_filterTwoFrequencySlider] -- add your slider handling code here..
        float freq = sliderThatWasMoved->getValue();
         filterDisplay->setFrequency(freq, 2);
        //[/UserSliderCode_filterTwoFrequencySlider]
    }
    else if (sliderThatWasMoved == filterTwoResonanceSlider)
    {
        //[UserSliderCode_filterTwoResonanceSlider] -- add your slider handling code here..
        float reso = sliderThatWasMoved->getValue();
        filterDisplay->setResonance(reso, 2);
        //[/UserSliderCode_filterTwoResonanceSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void FilterComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == filterOneTypeComboBox)
    {
        //[UserComboBoxCode_filterOneTypeComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterType(tmpSelectedIndex, 1);

        //[/UserComboBoxCode_filterOneTypeComboBox]
    }
    else if (comboBoxThatHasChanged == filterOneRollOffComboBox)
    {
        //[UserComboBoxCode_filterOneRollOffComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterRollOff(tmpSelectedIndex, 1);
        //[/UserComboBoxCode_filterOneRollOffComboBox]
    }
    else if (comboBoxThatHasChanged == filterTwoTypeComboBox)
    {
        //[UserComboBoxCode_filterTwoTypeComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterType(tmpSelectedIndex, 2);
        //[/UserComboBoxCode_filterTwoTypeComboBox]
    }
    else if (comboBoxThatHasChanged == filterTwoRollOffComboBox)
    {
        //[UserComboBoxCode_filterTwoRollOffComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterRollOff(tmpSelectedIndex, 2);
        //[/UserComboBoxCode_filterTwoRollOffComboBox]
    }
    else if (comboBoxThatHasChanged == filterConfigComboBox)
    {
        //[UserComboBoxCode_filterConfigComboBox] -- add your combo box handling code here..

        //[/UserComboBoxCode_filterConfigComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void FilterComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == filterOneToggleButton)
    {
        //[UserButtonCode_filterOneToggleButton] -- add your button handler code here..
        filterDisplay->enableFilter(filterOneToggleButton->getToggleState(), 1);
        //[/UserButtonCode_filterOneToggleButton]
    }
    else if (buttonThatWasClicked == filterTwoToggleButton)
    {
        //[UserButtonCode_filterTwoToggleButton] -- add your button handler code here..
        filterDisplay->enableFilter(filterTwoToggleButton->getToggleState(), 2);
        //[/UserButtonCode_filterTwoToggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void FilterComponent::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    switch(commandId){
        case FilterDisplay::F1CH:
            /* Send async so that the parameter is updated */

              filterOneFrequencySlider->setValue(filterDisplay->getFrequency(1),
                                                NotificationType::sendNotificationAsync);
              filterOneResonanceSlider->setValue(filterDisplay->getResonance(1),
                                                 NotificationType::sendNotificationAsync);
            break;
        case FilterDisplay::F2CH:
            filterTwoFrequencySlider->setValue(filterDisplay->getFrequency(2),
                                               NotificationType::sendNotificationAsync);
            filterTwoResonanceSlider->setValue(filterDisplay->getResonance(2),
                                               NotificationType::sendNotificationAsync);

            break;
    }

    //getParentComponent()->postCommandMessage();

    //[/UserCode_handleCommandMessage]
}

void FilterComponent::focusOfChildComponentChanged (FocusChangeType cause)
{
    //[UserCode_focusOfChildComponentChanged] -- Add your code here...

    //[/UserCode_focusOfChildComponentChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

/* After constructing and initialising the GUI components
 * Finish setting the rest of the settings
 */
void FilterComponent::initialiseFilterOne(){
    /* filter one Enable */
    filterOneToggleButton->setButtonText (String());
    filterOneToggleButton->addListener (this);

    /* frequency filter one */
    filterOneFrequencySlider->setRange (0, 1, 0.001);
    filterOneFrequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOneFrequencySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterOneFrequencySlider->addListener (this);

    /* resonance filter one */
    filterOneResonanceSlider->setRange (0, 1, 0.001);
    filterOneResonanceSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOneResonanceSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterOneResonanceSlider->addListener (this);

    /* filter one type */
    filterOneTypeComboBox->setEditableText (false);
    filterOneTypeComboBox->setJustificationType (Justification::centredLeft);
    filterOneTypeComboBox->setTextWhenNothingSelected (String());
    filterOneTypeComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterOneTypeComboBox->addItem (TRANS("LPF"), 1);
    filterOneTypeComboBox->addItem (TRANS("HPF"), 2);
    filterOneTypeComboBox->addItem (TRANS("BPF"), 3);
    filterOneTypeComboBox->addSeparator();
    filterOneTypeComboBox->postCommandMessage(ParamComboBox::update);
    filterOneTypeComboBox->addListener (this);

    /* filter one roll Off */
    filterOneRollOffComboBox->setEditableText (false);
    filterOneRollOffComboBox->setJustificationType (Justification::centredLeft);
    filterOneRollOffComboBox->setTextWhenNothingSelected (String());
    filterOneRollOffComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterOneRollOffComboBox->addItem (TRANS("12DB"), 1);
    filterOneRollOffComboBox->addItem (TRANS("24DB"), 2);
    filterOneRollOffComboBox->addSeparator();
    filterOneRollOffComboBox->postCommandMessage(ParamComboBox::update);
    filterOneRollOffComboBox->addListener (this);




}

void FilterComponent::initialiseFilterTwo(){
    /* filter two enable */
    filterTwoToggleButton->setButtonText (String());
    filterTwoToggleButton->addListener (this);

    /* filter two frequency slider */
    filterTwoFrequencySlider->setRange (0, 1, 0.001);
    filterTwoFrequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoFrequencySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterTwoFrequencySlider->addListener (this);

    /* filter two resonance slider */
    filterTwoResonanceSlider->setRange (0, 1, 0.001);
    filterTwoResonanceSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoResonanceSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterTwoResonanceSlider->addListener (this);

    /* filter two type box */
    filterTwoTypeComboBox->setEditableText (false);
    filterTwoTypeComboBox->setJustificationType (Justification::centredLeft);
    filterTwoTypeComboBox->setTextWhenNothingSelected (String());
    filterTwoTypeComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterTwoTypeComboBox->addItem (TRANS("LPF"), 1);
    filterTwoTypeComboBox->addItem (TRANS("HPF"), 2);
    filterTwoTypeComboBox->addItem (TRANS("BPF"), 3);
    filterTwoTypeComboBox->addSeparator();
    filterTwoTypeComboBox->postCommandMessage(ParamComboBox::update);
    filterTwoTypeComboBox->addListener (this);

    /* filter two roll off */
    filterTwoRollOffComboBox->setEditableText (false);
    filterTwoRollOffComboBox->setJustificationType (Justification::centredLeft);
    filterTwoRollOffComboBox->setTextWhenNothingSelected (String());
    filterTwoRollOffComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterTwoRollOffComboBox->addItem (TRANS("12DB"), 1);
    filterTwoRollOffComboBox->addItem (TRANS("24DB"), 2);
    filterTwoRollOffComboBox->addSeparator();
    filterTwoRollOffComboBox->postCommandMessage(ParamComboBox::update);
    filterTwoRollOffComboBox->addListener (this);
}

void FilterComponent::setFilterType(int &selectedIndex, int filter){
    switch(selectedIndex){
        case 0:
            filterDisplay->setType(FilterDisplay::LPF,filter);
            break;
        case 1:
            filterDisplay->setType(FilterDisplay::HPF,filter);
            break;
        case 2:
            filterDisplay->setType(FilterDisplay::BPF, filter);
    }
}

void FilterComponent::setFilterRollOff(int &selectedIndex, int filter){
    switch(selectedIndex){
        case 0:
            filterDisplay->setRollOff(FilterCurve::RollOff12dB,filter);
            break;
        case 1:
            filterDisplay->setRollOff(FilterCurve::RollOff24dB,filter);
            break;
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="255" initialHeight="255">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
    <METHOD name="focusOfChildComponentChanged (FocusChangeType cause)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff"/>
  <SLIDER name="filterOneFrequency" id="7691e009ec0e141b" memberName="filterOneFrequencySlider"
          virtualName="Slider" explicitFocusOrder="0" pos="3 176 65 65"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="filterOneResonance" id="c75615b5049f5dc9" memberName="filterOneResonanceSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="75 184 50 50"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="filterTwoFrequency" id="6c33bb30cb3d5303" memberName="filterTwoFrequencySlider"
          virtualName="Slider" explicitFocusOrder="0" pos="131 176 65 65"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="filterTwoResonance" id="d08caa925953a4fc" memberName="filterTwoResonanceSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="203 184 50 50"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <GENERICCOMPONENT name="filterDisplay" id="edc4cb916977ee50" memberName="filterDisplay"
                    virtualName="FilterDisplay" explicitFocusOrder="0" pos="1 24 253 128"
                    class="Component" params="&quot;fdis&quot;"/>
  <COMBOBOX name="filterOneType" id="3c016a507f2ddb0b" memberName="filterOneTypeComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="23 152 50 20"
            editable="0" layout="33" items="LPF&#10;HPF&#10;BPF&#10;" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="filterOneRollOff" id="48862f03fb86cd01" memberName="filterOneRollOffComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="73 152 50 20"
            editable="0" layout="33" items="12DB&#10;24DB&#10;" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="filterToggle" id="9b43f7cd0d304fd2" memberName="filterToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="1 0 24 24"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="1"/>
  <COMBOBOX name="filterTwoType" id="a5843f8fa9a567a" memberName="filterTwoTypeComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="151 152 50 20"
            editable="0" layout="33" items="LPF&#10;HPF&#10;BPF&#10;" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="filterTwoRollOff" id="57800b9c5ece8ddc" memberName="filterTwoRollOffComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="201 152 50 20"
            editable="0" layout="33" items="12DB&#10;24DB&#10;" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="filterOneToggle" id="aa716bba60e2a8a" memberName="filterOneToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="1 151 24 24"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="filterTwoToggle" id="6d0d6cec75cecab1" memberName="filterTwoToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="129 151 24 24"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="filterConfig" id="b81e8bf731f64871" memberName="filterConfigComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="174 3 80 20"
            editable="0" layout="33" items="Series&#10;Parallel&#10;" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
