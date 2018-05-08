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

#ifndef __JUCE_HEADER_B539B8D0F4E3ECFE__
#define __JUCE_HEADER_B539B8D0F4E3ECFE__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"

#include "StereoDelayComponent.h"
#include "PingPongComponent.h"
#include "LCRDelayComponent.h"
#include "StereoFlangerComponent.h"
#include "DimensionChorusComponent.h"
#include "HoldDelayComponent.h"
#include "DecimatorComponent.h"
#include "PlateReverbComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FXContainer  : public Component,
                     public ComboBoxListener,
                     public SliderListener,
                     public ButtonListener
{
public:
    //==============================================================================
    FXContainer (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~FXContainer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setEffect(int fxIndex, int effect);
    String getUIState();
    void setUITState(String state);

    void setEffectOneUpdateId(int commandId);
    void setEffectTwoUpdateId(int commandId);

    int getActiveEffectProcessor(int effect);

    void updateGui();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseEffectsStrings();

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *fxInput;
    AudioProcessorParameter *fxRouting;

    int activeEffect;
    int effectFXOne;
    int effectFXTwo;

    int effectOneUpdateId;
    int effectTwoUpdateId;

    // Parameters Destinations for the
    // Modulation Matrix Menus

    Colour buttonColour;
    Colour activeColour;

    StringArray effects;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> enableFXToggleButton;
    ScopedPointer<ComboBox> fxConfigComboBox;
    ScopedPointer<Slider> fxInputAmp;
    ScopedPointer<Slider> fxRoutingAmp;
    ScopedPointer<Component> effectOne;
    ScopedPointer<Component> effectTwo;
    ScopedPointer<TextButton> fxButtonTwo;
    ScopedPointer<TextButton> fxButtonOne;
    ScopedPointer<ComboBox> fxEffectBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXContainer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B539B8D0F4E3ECFE__
