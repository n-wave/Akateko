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

#ifndef __JUCE_HEADER_95E50ED86F3D9A4C__
#define __JUCE_HEADER_95E50ED86F3D9A4C__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-gui/StepSequencer.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"
#include "CustomLookAndFeel.h"
#include "PluginProcessor.h"

#include <vector>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class StepSequencerComponent  : public Component,
                                public ButtonListener,
                                public SliderListener,
                                public ComboBoxListener
{
public:
    //==============================================================================
    StepSequencerComponent (const String &name,AkatekoAudioProcessor &p, Label &label);
    ~StepSequencerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void sliderDragEnded(Slider* sliderThatWasDragged) override;
    void calculateBeatDivision(double bpm);

    String getUIState();
    void setUIState(String state);

    void updateGui();
    void setLookAndFeel(LookAndFeel *laf);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void handleCommandMessage (int commandId) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseBeatDivision();
    void initDurationSlider(); //Call when Envelope has been set
    int findClosestTimeDivision(double period);
    int getTimeDivisionIndex(std::vector<double> values,double period);
    void initialiseMidiStrings();

    StringArray beatDivision;
    std::vector<double> valueBeatDivision;
    std::vector<int> paramIndices;
    int requestMenuIds[8];
    StringArray midiStrings;

    int stepAmount;
    double curveMidPoint;
    double beatsPerMinute;

    PopupMenu menu;

    AudioProcessorParameter *duration;
    AkatekoAudioProcessor &processor;
    Label &labelRef;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<StepSequencer> stepSequencer;
    ScopedPointer<Slider> stepLengthSlider;
    ScopedPointer<ToggleButton> enableToggle;
    ScopedPointer<ToggleButton> syncToggle;
    ScopedPointer<Slider> durationSlider;
    ScopedPointer<ComboBox> stepComboBox;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<Slider> offsetSlider;
    ScopedPointer<Slider> exponentSlider;
    ScopedPointer<ToggleButton> chopperToggle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepSequencerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_95E50ED86F3D9A4C__
