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

#include "/work/programming-projects/msm/msm-utilities/ConfigurationFileManager.h"
#include "/work/programming-projects/msm/msm-dsp/Common/BeatDivisor.h"
#include "/work/programming-projects/msm/msm-gui/FilterDisplay.h"
#include "/work/programming-projects/msm/msm-gui/QTableDrawing.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"
#include "/work/programming-projects/msm/msm-gui/NumberBox.h"


#include "XYPad.h"
#include "CustomLookAndFeel.h"
#include "SliderLookAndFeel.h"
#include "SliderLookAndFeelBottomV1.h"
#include "SliderLookAndFeelBottomV2.h"

#include "AkatekoMatrix.h"
#include "LFOComponent.h"
#include "EnvelopeComponent.h"
#include "StepSequencerComponent.h"
#include "WaveShapeComponent.h"
#include "FilterComponent.h"
#include "MatrixTable.h"
#include "FXContainer.h"
#include "PresetComponent.h"

#include "BackPanel.h"

#include "Akateko.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AkatekoAudioProcessorEditor  : public AudioProcessorEditor,
                                     public ButtonListener,
                                     public SliderListener,
                                     public Timer
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
        MODM,
        PRST
    };

    enum CommandId {
        updateTimeId,
    };

    void setEffectParameterDestinations(StringArray &params, int effect);
    void updateGui();

    void paintOverChildren(Graphics &g) override;
    void sliderValueChanged(Slider *slider) override;

    bool keyPressed(const KeyPress &key) override;

    void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void visibilityChanged() override;
    void handleCommandMessage (int commandId) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseDisplayLabel();
    void initialiseModulationContainer();
    void initialiseEffectContainer();
    void initialiseToolBarSection();
    void setButtonState(ImageButton *button);
    void updateGuiSection(int section);
    /* Call after AudioProcessrParameters() */
    void hideModulationComponent(int modComponentId);

    int midiLearnFlash;
    bool modInitialized;
    double currentBPM;
    int activeModulation;

    int xyPadCommandId;
    int effectOneCommandId;
    int effectTwoCommandId;
    int presetCommandId;

    int requestMenuId[2];

    Colour buttonColour;
    Colour activeColour;
    ColourGradient overlay;

    Image backGroundImage;

    std::vector<int> paramIndices;
    StringArray midiStrings;
    PopupMenu menu;

    Label presetLabel;
    Label paramLabel;
    Label bpmLabel;

    AkatekoAudioProcessor &processor;
    AudioProcessorParameter *inputVolume = nullptr;
    AudioProcessorParameter *outputVolume = nullptr;

    ScopedPointer<CustomLookAndFeel> claf;
    ScopedPointer<SliderLookAndFeelBottomV1> r1laf;
    ScopedPointer<SliderLookAndFeelBottomV2> r2laf;
    ScopedPointer<SliderLookAndFeel> slaf;
    ScopedPointer<SliderLookAndFeel> blaf;

    // Modulation Components
    ScopedPointer<LFOComponent> lfoOne;
    ScopedPointer<LFOComponent> lfoTwo;
    ScopedPointer<EnvelopeComponent> envOne;
    ScopedPointer<EnvelopeComponent> envTwo;
    ScopedPointer<StepSequencerComponent> stepSeq;
    ScopedPointer<PresetComponent> preset;

    // DBMeter


    // Back Panel
    ScopedPointer<BackPanel> backPanel;
    //[/UserVariables]
    //==============================================================================
    ScopedPointer<ImageButton> lfoOneButton;
    ScopedPointer<ImageButton> lfoTwoButton;
    ScopedPointer<ImageButton> envOneButton;
    ScopedPointer<ImageButton> envTwoButton;
    ScopedPointer<ImageButton> stepButton;
    ScopedPointer<ImageButton> modulationButton;
    ScopedPointer<ImageButton> loadPresetButton;
    ScopedPointer<ImageButton> savePresetButton;
    ScopedPointer<Slider> InputVolumeSlider;
    ScopedPointer<WaveShapeComponent> waveShaper;
    ScopedPointer<FilterComponent> filter;
    ScopedPointer<XYPad> xyPad;
    ScopedPointer<MatrixTable> modMatrix;
    ScopedPointer<FXContainer> fxContainer;
    ScopedPointer<Slider> mainVolumeSlider;
    ScopedPointer<ImageButton> presetButton;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AkatekoAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9EC7008240452163__
