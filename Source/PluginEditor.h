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

#ifndef __JUCE_HEADER_9EC7008240452163__
#define __JUCE_HEADER_9EC7008240452163__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"

#include "/work/programming-projects/msm/msm-gui/FilterDisplay.h"
#include "/work/programming-projects/msm/msm-gui/QTableDrawing.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"
#include "/work/programming-projects/msm/msm-gui/NumberBox.h"
#include "/work/programming-projects/msm/msm-gui/XYPad.h"

#include "LFOComponent.h"
#include "EnvelopeComponent.h"
#include "StepSequencerComponent.h"
#include "WaveShapeComponent.h"
#include "FilterComponent.h"
#include "ModulationMatrixComponent.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AkatekoAudioProcessorEditor  : public AudioProcessorEditor,
                                     public ButtonListener
{
public:
    //==============================================================================
    AkatekoAudioProcessorEditor (AkatekoAudioProcessor& p);
    ~AkatekoAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    enum ModulationId {
        LFO1,
        LFO2,
        ENV1,
        ENV2,
        SSEQ,
        MODM
    };

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void visibilityChanged() override;
    void handleCommandMessage (int commandId) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseDisplayLabel();
    void initialisePreAmpSection();
    void initialiseModulationContainer();
    void initialiseEffectContainer();
    void initialiseToolBarSection();

    /* Call after AudioProcessrParameters() */
    void hideModulationComponent(int modComponentId);

    Label label;
    AkatekoAudioProcessor &processor;

    ScopedPointer<LFOComponent> lfoOne;
    ScopedPointer<LFOComponent> lfoTwo;
    ScopedPointer<EnvelopeComponent> envOne;
    ScopedPointer<EnvelopeComponent> envTwo;
    ScopedPointer<StepSequencerComponent> stepSeq;
    ScopedPointer<ModulationMatrixComponent> modMatrix;

    /* Test Values for binding XYPad remove later */
    double xAxis;
    double yAxis;

    int activeModulation;

    Colour buttonColour;
    Colour activeColour;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> preDriveSlider;
    ScopedPointer<Slider> preAmplitudeSlider;
    ScopedPointer<ToggleButton> preToggleButton;
    ScopedPointer<TextButton> lfoOneButton;
    ScopedPointer<ToggleButton> effectToggleButton;
    ScopedPointer<TextButton> lfoTwoButton;
    ScopedPointer<TextButton> envOneButton;
    ScopedPointer<TextButton> envTwoButton;
    ScopedPointer<TextButton> stepButton;
    ScopedPointer<TextButton> modulationButton;
    ScopedPointer<TextButton> loadPresetButton;
    ScopedPointer<TextButton> savePresetButton;
    ScopedPointer<Slider> mainVolumeSlider;
    ScopedPointer<ToggleButton> bypassToggle;
    ScopedPointer<WaveShapeComponent> waveShaper;
    ScopedPointer<FilterComponent> filter;
    ScopedPointer<XYPad> xyPad;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AkatekoAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9EC7008240452163__
