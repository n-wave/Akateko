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

#ifndef __JUCE_HEADER_31DC4CC42E8C0A9E__
#define __JUCE_HEADER_31DC4CC42E8C0A9E__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-gui/QTableDrawing.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"
#include "/work/programming-projects/msm/msm-gui/EnvelopeDrawing.h"

#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EnvelopeComponent  : public Component,
                           public ButtonListener,
                           public ComboBoxListener
{
public:
    //==============================================================================
    EnvelopeComponent (const String &name, AkatekoAudioProcessor &p, int env, Label &label);
    ~EnvelopeComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void handleCommandMessage(int commandId) override;

    bool getLoopPoints(float &startPos, float &endPos);
    bool getSustainPoints(float &startPos, float &endPos);
    void getEndPoints(float &startPos, float &endPos);

    void setBPM(double bpm);

    void setUIState(String state);
    String getUIState();

    msmBuffer getBuffer();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    /* temporary commands, phase out when testing is completed */
    void updateEnvelope(int env);
    void updateLoop(int env);
    void updateSustain(int env);

    int commandTrigger;
    int commandRelease;

    int envNumber;
    /* Envelope Drawing has been updated */

    double BPM;

    AkatekoAudioProcessor &processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<EnvelopeDrawing> envelopeDrawing;
    ScopedPointer<Slider> durationSlider;
    ScopedPointer<Slider> envLoopSlider;
    ScopedPointer<TextButton> triggerButton;
    ScopedPointer<ToggleButton> envEnableToggle;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<TextButton> releaseButton;
    ScopedPointer<ComboBox> loopDirectionComboBox;
    ScopedPointer<ComboBox> susDirectionComboBox;
    ScopedPointer<ToggleButton> envSyncToggle;
    ScopedPointer<ComboBox> triggerComboBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_31DC4CC42E8C0A9E__