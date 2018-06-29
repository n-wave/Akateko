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

#ifndef __JUCE_HEADER_E2A166DCAA82AB0E__
#define __JUCE_HEADER_E2A166DCAA82AB0E__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MidiTable.h"
#include "ResonanceOptions.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class BackPanel  : public Component,
                   public ButtonListener,
                   public ComboBoxListener
{
public:
    //==============================================================================
    BackPanel (AkatekoAudioProcessor &p);
    ~BackPanel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void setLookAndFeel(LookAndFeel *cLaf, LookAndFeel *sLaf);
    void visibilityChanged() override;

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void selectEffectTable(int fx);
    void setPresetDirectoryLabel(String directory);

    int activeSection;

    String currentDirectory;
    int currentOverSampling;

    AkatekoAudioProcessor &processor;
    Label label;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> globalTextButton;
    ScopedPointer<TextButton> filtersTextButton;
    ScopedPointer<TextButton> waveShaperTextButton;
    ScopedPointer<TextButton> lfoOneTextButton;
    ScopedPointer<TextButton> lfoTwoTextButton;
    ScopedPointer<TextButton> envOneTextButton;
    ScopedPointer<TextButton> envTwoTextButton;
    ScopedPointer<TextButton> stepSeqTextButton;
    ScopedPointer<TextButton> xyPadButton;
    ScopedPointer<TextButton> fxOneTextButton;
    ScopedPointer<TextButton> fxTwoTextButton;
    ScopedPointer<MidiTable> midiTable;
    ScopedPointer<TextButton> activeTextButton;
    ScopedPointer<TextButton> clearAllTextButton;
    ScopedPointer<TextButton> clearTextButton;
    ScopedPointer<Slider> thresholdSlider;
    ScopedPointer<Slider> preDelaySlider;
    ScopedPointer<Slider> holdDelaySlider;
    ScopedPointer<ComboBox> inputComboBox;
    ScopedPointer<ResonanceOptions> resonanceOptions;
    ScopedPointer<Label> presetFolderLabel;
    ScopedPointer<TextButton> selectFolderButton;
    ScopedPointer<ComboBox> overSamplingComboBox;
    ScopedPointer<Label> sectionLabel;
    ScopedPointer<Slider> preGainSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackPanel)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_E2A166DCAA82AB0E__
