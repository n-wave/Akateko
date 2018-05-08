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

#include "FXContainer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using std::vector;
//[/MiscUserDefs]

//==============================================================================
FXContainer::FXContainer (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      activeEffect(1),
      effectFXOne(1),
      effectFXTwo(1),
      effectOneUpdateId(-1),
      effectTwoUpdateId(-1),
      processor(p),
      labelRef(label), buttonColour(Colour(0x73707070)), activeColour(Colour(0x7f007f7f))
{
    //[Constructor_pre] You can add your own custom stuff here.
    vector<int> paramIndices = p.getParameterIndices(AkatekoAudioProcessor::FxContainerId);
    StringArray paramNames = p.getParameterIds(AkatekoAudioProcessor::FxContainerId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() == 4 &&
       paramNames.size() == 4 &&
       params.size() >= paramIndices[3])
    {
        addAndMakeVisible(enableFXToggleButton = new ParamToggle(paramNames[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(fxConfigComboBox = new ParamComboBox(paramNames[1], *params.getUnchecked(paramIndices[1]), label));

        fxInput = params.getUnchecked(paramIndices[2]);
        addAndMakeVisible(fxInputAmp = new Slider(paramNames[2]));

        fxRouting = params.getUnchecked(paramIndices[3]);
        addAndMakeVisible(fxRoutingAmp = new Slider(paramNames[3]));
    } else {
        addAndMakeVisible (enableFXToggleButton = new ToggleButton ("enableFX"));
        addAndMakeVisible (fxConfigComboBox = new ComboBox ("fxConfig"));
        addAndMakeVisible (fxInputAmp = new Slider ("fxInputAmp"));
        addAndMakeVisible (fxRoutingAmp = new Slider ("fxRoutingAmp"));
    }
    //[/Constructor_pre]

    //[UserPreSize]

    enableFXToggleButton->setButtonText (String());

    fxConfigComboBox->setEditableText (false);
    fxConfigComboBox->setJustificationType (Justification::centred);
    fxConfigComboBox->setTextWhenNothingSelected (TRANS("config"));
    fxConfigComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    fxConfigComboBox->addItem (TRANS("Series"), 1);
    fxConfigComboBox->addItem (TRANS("Parallel"), 2);
    fxConfigComboBox->addSeparator();
    fxConfigComboBox->addListener (this);

    fxInputAmp->setTooltip (TRANS("FX Input Volume\n"));
    fxInputAmp->setRange (0, 1, 0);
    fxInputAmp->setSliderStyle (Slider::LinearBar);
    fxInputAmp->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fxInputAmp->setColour (Slider::backgroundColourId, Colour (0xff003000));
    fxInputAmp->setColour (Slider::thumbColourId, Colour (0xbb70bbbb));
    fxInputAmp->setColour (Slider::trackColourId, Colours::white);
    fxInputAmp->setColour (Slider::rotarySliderFillColourId, Colour (0xaa70abab));
    fxInputAmp->setColour (Slider::rotarySliderOutlineColourId, Colour (0x66000000));
    fxInputAmp->addListener (this);

    fxRoutingAmp->setTooltip (TRANS("FX Routing"));
    fxRoutingAmp->setRange (0, 1, 0);
    fxRoutingAmp->setSliderStyle (Slider::LinearBar);
    fxRoutingAmp->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fxRoutingAmp->setColour (Slider::backgroundColourId, Colour (0xff003000));
    fxRoutingAmp->setColour (Slider::thumbColourId, Colour (0xbb70bbbb));
    fxRoutingAmp->setColour (Slider::trackColourId, Colours::white);
    fxRoutingAmp->setColour (Slider::rotarySliderFillColourId, Colour (0xaa70abab));
    fxRoutingAmp->setColour (Slider::rotarySliderOutlineColourId, Colour (0x66000000));
    fxRoutingAmp->addListener (this);

    addAndMakeVisible (effectOne = new Component());
    effectOne->setName ("effectOne");

    addAndMakeVisible (effectTwo = new Component());
    effectTwo->setName ("effectTwo");

    addAndMakeVisible (fxButtonTwo = new TextButton ("fxButtonTwo"));
    fxButtonTwo->setButtonText (TRANS("FX 2"));
    fxButtonTwo->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    fxButtonTwo->addListener (this);
    fxButtonTwo->setColour (TextButton::buttonColourId, Colour (0x73707070));
    fxButtonTwo->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    fxButtonTwo->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (fxButtonOne = new TextButton ("fxButtonOne"));
    fxButtonOne->setButtonText (TRANS("FX 1"));
    fxButtonOne->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    fxButtonOne->addListener (this);
    fxButtonOne->setColour (TextButton::buttonColourId, Colour (0x73707070));
    fxButtonOne->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    fxButtonOne->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (fxEffectBox = new ComboBox ("fxEffectBox"));
    fxEffectBox->setEditableText (false);
    fxEffectBox->setJustificationType (Justification::centred);
    fxEffectBox->setTextWhenNothingSelected (TRANS("effect"));
    fxEffectBox->setTextWhenNoChoicesAvailable (TRANS("effect"));
    fxEffectBox->addItem (TRANS("None"), 1);
    fxEffectBox->addItem (TRANS("S-Delay"), 2);
    fxEffectBox->addItem (TRANS("PP-Delay"), 3);
    fxEffectBox->addItem (TRANS("LCR-Delay"), 4);
    fxEffectBox->addItem (TRANS("S-Flanger"), 5);
    fxEffectBox->addItem (TRANS("D-Chorus"), 6);
    fxEffectBox->addItem (TRANS("H-Delay"), 7);
    fxEffectBox->addItem (TRANS("Decimator"), 8);
    fxEffectBox->addItem (TRANS("P-Reverb"), 9);
    fxEffectBox->addSeparator();
    fxEffectBox->addListener (this);

    //[/UserPreSize]
    setSize (255, 255);


    //[Constructor] You can add your own custom stuff here..
    if(fxInput != nullptr){
        const double amp = fxInput->getValue();
        fxInputAmp->setValue(amp);
    }

    if(fxRouting != nullptr){
        const double routing = fxRouting->getValue();
        fxRoutingAmp->setValue(routing);
    }

    initialiseEffectsStrings();
    String state = processor.getUIState(AkatekoAudioProcessor::FxContainerId);
    setUITState(state);


    if(activeEffect == 1){
        fxButtonOne->setColour(TextButton::buttonColourId, activeColour);
        fxEffectBox->setText(effects[effectFXOne], dontSendNotification);
    } else if(activeEffect == 2){
        fxButtonTwo->setColour(TextButton::buttonColourId, activeColour);
        fxEffectBox->setText(effects[effectFXTwo], dontSendNotification);
    }

    //[/Constructor]
}

FXContainer::~FXContainer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    enableFXToggleButton = nullptr;
    fxConfigComboBox = nullptr;
    fxInputAmp = nullptr;
    fxRoutingAmp = nullptr;
    effectOne = nullptr;
    effectTwo = nullptr;
    fxButtonTwo = nullptr;
    fxButtonOne = nullptr;
    fxEffectBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    fxInput = nullptr;
    fxRouting = nullptr;
    //[/Destructor]
}

//==============================================================================
void FXContainer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FXContainer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    enableFXToggleButton->setBounds (235, -1, 24, 24);
    fxConfigComboBox->setBounds (95, 3, 66, 16);
    fxInputAmp->setBounds (3, 3, 40, 16);
    fxRoutingAmp->setBounds (48, 3, 40, 16);
    effectOne->setBounds (0, 24, 256, 232);
    effectTwo->setBounds (0, 24, 256, 232);
    fxButtonTwo->setBounds (200, 3, 32, 16);
    fxButtonOne->setBounds (169, 3, 32, 16);
    fxEffectBox->setBounds (88, 25, 81, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FXContainer::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == fxConfigComboBox)
    {
        //[UserComboBoxCode_fxConfigComboBox] -- add your combo box handling code here..



        //[/UserComboBoxCode_fxConfigComboBox]
    }
    else if (comboBoxThatHasChanged == fxEffectBox)
    {
        //[UserComboBoxCode_fxEffectBox] -- add your combo box handling code here..
        const int id = fxEffectBox->getSelectedId();

        setEffect(id, activeEffect);

        if(activeEffect == 1){
            effectFXOne = id;
            effectTwo->setVisible(false);
            effectOne->setVisible(true);
            processor.setEffectOne(id);
        } else if(activeEffect == 2){
            effectFXTwo = id;
            effectOne->setVisible(false);
            effectTwo->setVisible(true);
            processor.setEffectTwo(id);
        }

        processor.setUIState(getUIState(), AkatekoAudioProcessor::FxContainerId);

        //[/UserComboBoxCode_fxEffectBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void FXContainer::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == fxInputAmp)
    {
        //[UserSliderCode_fxInputAmp] -- add your slider handling code here..
        const double amp = fxInputAmp->getValue();
        String stringValue = fxInputAmp->getName() + String(amp*2.0, 2);

        fxInput->setValue(amp);
        labelRef.setText(stringValue, dontSendNotification);
        //[/UserSliderCode_fxInputAmp]
    }
    else if (sliderThatWasMoved == fxRoutingAmp)
    {
        //[UserSliderCode_fxRoutingAmp] -- add your slider handling code here..
        const double fxRouting = fxRoutingAmp->getValue();
        double fxOneValue = 1-fxRouting;
        double fxTwoValue = fxRouting;

        String fxOne = String(fxOneValue, 2);
        String fxTwo = String(fxTwoValue, 2);

        String stringValue = fxRoutingAmp->getName() +
                            "[" + fxOne + "," + fxTwo + "]";

        fxRoutingAmp->setValue(fxRouting);
        labelRef.setText(stringValue, dontSendNotification);
        //[/UserSliderCode_fxRoutingAmp]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void FXContainer::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == fxButtonTwo)
    {
        //[UserButtonCode_fxButtonTwo] -- add your button handler code here..
        fxButtonOne->setColour(TextButton::buttonColourId, buttonColour);
        fxButtonTwo->setColour(TextButton::buttonColourId, activeColour);

        effectOne->setVisible(false);
        effectTwo->setVisible(true);
        activeEffect = 2;

        fxEffectBox->setSelectedId(effectFXTwo, dontSendNotification);
        processor.setUIState(getUIState(), AkatekoAudioProcessor::FxContainerId);

        if(effectFXOne != 1){
            fxEffectBox->setItemEnabled(effectFXOne, false);
        }

        if(!fxEffectBox->isItemEnabled(effectFXTwo)){
            fxEffectBox->setItemEnabled(effectFXTwo, true);
        }


        //[/UserButtonCode_fxButtonTwo]
    }
    else if (buttonThatWasClicked == fxButtonOne)
    {
        //[UserButtonCode_fxButtonOne] -- add your button handler code here..
        fxButtonTwo->setColour(TextButton::buttonColourId, buttonColour);
        fxButtonOne->setColour(TextButton::buttonColourId, activeColour);

        effectTwo->setVisible(false);
        effectOne->setVisible(true);

        activeEffect = 1;

        fxEffectBox->setSelectedId(effectFXOne, dontSendNotification);
        processor.setUIState(getUIState(), AkatekoAudioProcessor::FxContainerId);


        if(effectFXTwo != 1){
            fxEffectBox->setItemEnabled(effectFXTwo, false);
        }

        if(!fxEffectBox->isItemEnabled(effectFXOne)){
            fxEffectBox->setItemEnabled(effectFXOne, true);
        }


        //[/UserButtonCode_fxButtonOne]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void FXContainer::initialiseEffectsStrings(){
    effects.add("Effect");
    effects.add("None");
    effects.add("S-Delay");
    effects.add("PP-Delay");
    effects.add("LCR-Delay");
    effects.add("S-Flanger");
    effects.add("D-Chorus");
    effects.add("H-Delay");
    effects.add("Decimator");
}

void FXContainer::setEffect(int fxIndex, int effect){
    if(effect == 1){
        effectOne = nullptr;

        switch(fxIndex){
            case 1:
                addChildComponent(effectOne = new Component("Undefined"), 0);
                break;
            case 2:
                addChildComponent(effectOne = new StereoDelayComponent("Delay : ", processor, labelRef), 0);
                break;
            case 3:
                addChildComponent(effectOne = new PingPongComponent("Ping Pong : ", processor, labelRef), 0);
                break;
            case 4:
                addChildComponent(effectOne = new LCRDelayComponent("LCR Delay : ", processor, labelRef), 0);
                break;                
            case 5:
                addChildComponent(effectOne = new StereoFlangerComponent("S-Flanger : ", processor, labelRef), 0);
                break;
            case 6:
                addChildComponent(effectOne = new DimensionChorusComponent("D-Chorus : ", processor, labelRef), 0);
                break;
            case 7:
                addChildComponent(effectOne = new HoldDelayComponent("H-Delay : ", processor, labelRef), 0);
                break;
            case 8:
                addChildComponent(effectOne = new DecimatorComponent("Decimator : ", processor, labelRef), 0);
                break;
            case 9:
                addChildComponent(effectOne = new PlateReverbComponent("Plate Reverb : ", processor, labelRef), 0);
                break;
            default :
                addChildComponent(effectOne = new Component("Undefined"), 0);
                break;
        }
        getParentComponent()->postCommandMessage(effectOneUpdateId);
        effectOne->setBounds (0, 24, 256, 232);
    } else if(effect == 2){
        effectTwo = nullptr;

        switch(fxIndex){
            case 1:
                addChildComponent(effectTwo = new Component("Undefined"), 0);
                break;
            case 2:
                addChildComponent(effectTwo = new StereoDelayComponent("Delay : ", processor, labelRef), 0);
                break;
            case 3:
                addChildComponent(effectTwo = new PingPongComponent("Ping Pong : ", processor, labelRef), 0);
                break;
            case 4:
                addChildComponent(effectTwo = new LCRDelayComponent("LCR Delay : ", processor, labelRef), 0);
                break;
            case 5:
                addChildComponent(effectTwo = new StereoFlangerComponent("S-Flanger : ", processor, labelRef), 0);
                break;
            case 6:
                addChildComponent(effectTwo = new DimensionChorusComponent("D-Chorus : ", processor, labelRef), 0);
                break;
            case 7:
                addChildComponent(effectTwo = new HoldDelayComponent("H-Delay : ", processor, labelRef), 0);
                break;
            case 8:
                addChildComponent(effectTwo = new DecimatorComponent("Decimator : ", processor, labelRef), 0);
                break;
            case 9:
                addChildComponent(effectTwo = new PlateReverbComponent("Plate Reverb : ", processor, labelRef), 0);
                break;
            default :
                addChildComponent(effectTwo = new Component("Undefined"), 0);
                break;
        }
        getParentComponent()->postCommandMessage(effectTwoUpdateId);
        effectTwo->setBounds (0, 24, 256, 232);
    }
}

String FXContainer::getUIState(){
    String result = String("fx");
    result += " ";
    result += String(activeEffect);
    result += " ";
    result += String(effectFXOne);
    result += " ";
    result += String(effectFXTwo);

    return result;
}

void FXContainer::setUITState(String state){
    StringArray tokens;

    tokens.addTokens(state, " ", "\"");
    int tokenSize = tokens.size();

    if(tokenSize == 4){
        activeEffect = tokens[1].getIntValue();
        effectFXOne = tokens[2].getIntValue();
        effectFXTwo = tokens[3].getIntValue();

        setEffect(effectFXOne, 1);
        setEffect(effectFXTwo, 2);


        if(activeEffect == 1){
            if(effectFXTwo != 1){
                fxEffectBox->setItemEnabled(effectFXTwo, false);
            }
            if(!fxEffectBox->isItemEnabled(effectFXOne)){
                fxEffectBox->setItemEnabled(effectFXOne, true);
            }

            fxEffectBox->setSelectedItemIndex(effectFXOne, dontSendNotification);
            fxButtonOne->setColour(TextButton::buttonColourId, activeColour);
            fxButtonTwo->setColour(TextButton::buttonColourId, buttonColour);
            effectOne->setVisible(true);
        } else if(activeEffect == 2){
            if(effectFXOne != 1){
                fxEffectBox->setItemEnabled(effectFXOne, false);
            }

            if(!fxEffectBox->isItemEnabled(effectFXTwo)){
                fxEffectBox->setItemEnabled(effectFXTwo, true);
            }

            fxEffectBox->setSelectedItemIndex(effectFXTwo, dontSendNotification);
            fxButtonTwo->setColour(TextButton::buttonColourId, activeColour);
            fxButtonOne->setColour(TextButton::buttonColourId, buttonColour);
            effectTwo->setVisible(true);
        }
    }
}

void FXContainer::setEffectOneUpdateId(int commandId){
    effectOneUpdateId = commandId;
}

void FXContainer::setEffectTwoUpdateId(int commandId){
    effectTwoUpdateId = commandId;
}

int FXContainer::getActiveEffectProcessor(int effect){
    int result = 0;

    if(effect == 1){
        result = effectFXOne;
    } else if(effect == 2){
        result = effectFXTwo;
    }
    return result;
}

void FXContainer::updateGui(){
  enableFXToggleButton->postCommandMessage(ParamToggle::update);
  fxConfigComboBox->postCommandMessage(ParamComboBox::update);

  if(fxInput != nullptr){
      const double amp = fxInput->getValue();
      fxInputAmp->setValue(amp);
  }

  if(fxRouting != nullptr){
      const double routing = fxRouting->getValue();
      fxRoutingAmp->setValue(routing);
  }

  effectOne->postCommandMessage(0);
  effectTwo->postCommandMessage(0);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FXContainer" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label), buttonColour(Colour(0x73707070)), activeColour(Colour(0x7f007f7f)),"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="255">
  <BACKGROUND backgroundColour="0"/>
  <TOGGLEBUTTON name="enableFX" id="6708e88cc92f899b" memberName="enableFXToggleButton"
                virtualName="" explicitFocusOrder="0" pos="235 -1 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <COMBOBOX name="fxConfig" id="b81e8bf731f64871" memberName="fxConfigComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="95 3 66 16"
            editable="0" layout="36" items="Series&#10;Parallel&#10;" textWhenNonSelected="config"
            textWhenNoItems="(no choices)"/>
  <SLIDER name="fxInputAmp" id="5e1556d0f65f782e" memberName="fxInputAmp"
          virtualName="Slider" explicitFocusOrder="0" pos="3 3 40 16" tooltip="FX Input Volume&#10;"
          bkgcol="ff003000" thumbcol="bb70bbbb" trackcol="ffffffff" rotarysliderfill="aa70abab"
          rotaryslideroutline="66000000" min="0" max="1" int="0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="fxRoutingAmp" id="c64e2d6f1c96d62f" memberName="fxRoutingAmp"
          virtualName="Slider" explicitFocusOrder="0" pos="48 3 40 16"
          tooltip="FX Routing" bkgcol="ff003000" thumbcol="bb70bbbb" trackcol="ffffffff"
          rotarysliderfill="aa70abab" rotaryslideroutline="66000000" min="0"
          max="1" int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <GENERICCOMPONENT name="effectOne" id="63a231e61fbda1b9" memberName="effectOne"
                    virtualName="" explicitFocusOrder="0" pos="0 24 256 232" class="Component"
                    params=""/>
  <GENERICCOMPONENT name="effectTwo" id="a3753b0dc1068694" memberName="effectTwo"
                    virtualName="" explicitFocusOrder="0" pos="0 24 256 232" class="Component"
                    params=""/>
  <TEXTBUTTON name="fxButtonTwo" id="92fb4021231abae" memberName="fxButtonTwo"
              virtualName="" explicitFocusOrder="0" pos="200 3 32 16" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="FX 2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="fxButtonOne" id="600011c1e1674e40" memberName="fxButtonOne"
              virtualName="" explicitFocusOrder="0" pos="169 3 32 16" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="FX 1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="fxEffectBox" id="5e47a4af8eaa68ce" memberName="fxEffectBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="88 25 81 16"
            editable="0" layout="36" items="Series&#10;Parallel&#10;" textWhenNonSelected="effect"
            textWhenNoItems="effect"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
