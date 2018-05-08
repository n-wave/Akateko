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

#ifndef __JUCE_HEADER_62CD23C3F29F59D6__
#define __JUCE_HEADER_62CD23C3F29F59D6__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-gui/QTableDrawing.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"
#include "/work/programming-projects/msm/msm-gui/FilterDisplay.h"

#include "Akateko.h"
#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FilterComponent  : public Component,
                         public SliderListener,
                         public ComboBoxListener,
                         public ButtonListener
{
public:
    //==============================================================================
    FilterComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~FilterComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void updateGui();

    enum ParameterId {
        FilterEnable,
        FilterConfig,
        FilterOneEnable,
        FilterOneType,
        FilterOneRollOff,
        FilterOneFrequency,
        FilterOneResonance,
        FilterTwoType,
        FilterTwoRollOff,
        FilterTwoEnable,
        FilterTwoFrequency,
        FilterTwoResonance
    };
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void handleCommandMessage (int commandId) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void setFilterType(int &selectedIndex, int filter);
    void setFilterRollOff(int &selectedIndex, int filter);

    void initialiseFilterOne();
    void initialiseFilterTwo();

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *filterOnePassBand;
    AudioProcessorParameter *filterOneVolume;

    AudioProcessorParameter *filterTwoPassBand;
    AudioProcessorParameter *filterTwoVolume;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> filterTwoFrequencySlider;
    ScopedPointer<Slider> filterTwoResonanceSlider;
    ScopedPointer<FilterDisplay> filterDisplay;
    ScopedPointer<ComboBox> filterOneTypeComboBox;
    ScopedPointer<ComboBox> filterOneRollOffComboBox;
    ScopedPointer<ToggleButton> filterToggleButton;
    ScopedPointer<ComboBox> filterTwoTypeComboBox;
    ScopedPointer<ComboBox> filterTwoRollOffComboBox;
    ScopedPointer<ToggleButton> filterOneToggleButton;
    ScopedPointer<ToggleButton> filterTwoToggleButton;
    ScopedPointer<ComboBox> filterConfigComboBox;
    ScopedPointer<Slider> filterTwoDriveSlider;
    ScopedPointer<Slider> filterTwoPassBandSlider;
    ScopedPointer<Slider> filterTwoVolumeSlider;
    ScopedPointer<Slider> filterOneFrequencySlider;
    ScopedPointer<Slider> filterOneResonanceSlider;
    ScopedPointer<Slider> filterOneDriveSlider;
    ScopedPointer<Slider> filterOnePassBandSlider;
    ScopedPointer<Slider> filterOneVolumeSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_62CD23C3F29F59D6__
