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
#include "PluginProcessor.h"

#include <vector>

using std::vector;
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
                                public ComboBoxListener,
                                public SliderListener
{
public:
    //==============================================================================
    StepSequencerComponent (const String &name,AkatekoAudioProcessor &p, Label &label);
    ~StepSequencerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void sliderDragEnded(Slider* sliderThatWasDragged) override;

    void setValuesCommandId(int hashCode);
    void setStatesCommandId(int hashCode);
    void setSizeCommandId(int hashCode);
    void setOffsetCommandId(int hashCode);
    void setSyncCommandId(int hashCode);
    void setChopCommandId(int hashCode);
    void setDurationCommandId(int hashCode);
    void setExponentCommandId(int hashCode);

    vector<double> getStepValues();
    vector<int> getButtonStates();
    int getStepAmount();

    String getUIState();
    void setUIState(String state);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void handleCommandMessage (int commandId) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int commandValues;
    int commandStates;
    int commandSize;
    int commandOffset;
    int commandSync;
    int commandChop;
    int commandDuration;
    int commandExponent;

    int stepAmount;
    double curveMidPoint;

    AkatekoAudioProcessor &processor;
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
    ScopedPointer<TextButton> textButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepSequencerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_95E50ED86F3D9A4C__
