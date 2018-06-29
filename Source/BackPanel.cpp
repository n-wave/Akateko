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

#include "BackPanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamComboBox.h"
//[/MiscUserDefs]

//==============================================================================
BackPanel::BackPanel (AkatekoAudioProcessor &p)
    : processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..

    std::vector<int> paramIndices = processor.getParameterIndices(AkatekoAudioProcessor::aTriggerId);
    StringArray paramIds = processor.getParameterIds(AkatekoAudioProcessor::aTriggerId);
    const OwnedArray<AudioProcessorParameter> &params = processor.getParameters();

    if(paramIndices.size() == 5 &&
       paramIds.size() == 5 &&
       params.size() >= paramIndices[4])
    {
        addAndMakeVisible(preGainSlider = new ParamSlider(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(thresholdSlider = new ParamSlider(paramIds[1], *params.getUnchecked(paramIndices[1]), label, 0.1, 0.95));
        addAndMakeVisible(preDelaySlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), label, 0.0, 250.0));
        addAndMakeVisible(holdDelaySlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label, 0.0, 250.0));
        addAndMakeVisible(inputComboBox = new ParamComboBox(paramIds[4], *params.getUnchecked(paramIndices[4]), label));

    } else {
        addAndMakeVisible (thresholdSlider = new Slider ("threshold"));
        addAndMakeVisible (preDelaySlider = new Slider ("Pre Delay"));
        addAndMakeVisible (holdDelaySlider = new Slider ("Hold Delay"));
        addAndMakeVisible (inputComboBox = new ComboBox ("Input Slection"));

        std::cerr << "BackPanel::BackPanel" << std::endl;
        std::cerr << "/-----Parameters not bound-----/" << std::endl;

        int index = 0;

        while(index < paramIndices.size() &&
              index < paramIds.size())
        {
            std::cerr << "ParamIndex ["
                      << paramIndices[index]
                      << "] : "
                      << paramIds[index]
                      << std::endl;

            index++;
        }
    }

    currentDirectory = processor.getDefaultPresetDirectory();
    currentOverSampling = processor.getOverSampling();
    //[/Constructor_pre]


    //[UserPreSize]
    addAndMakeVisible (globalTextButton = new TextButton ("Global"));
    globalTextButton->setButtonText (TRANS("global"));
    globalTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    globalTextButton->addListener (this);

    addAndMakeVisible (filtersTextButton = new TextButton ("Filters"));
    filtersTextButton->setButtonText (TRANS("filters"));
    filtersTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    filtersTextButton->addListener (this);

    addAndMakeVisible (waveShaperTextButton = new TextButton ("Waveshaper"));
    waveShaperTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    waveShaperTextButton->addListener (this);

    addAndMakeVisible (lfoOneTextButton = new TextButton ("lfoOne"));
    lfoOneTextButton->setButtonText (TRANS("LFO One"));
    lfoOneTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    lfoOneTextButton->addListener (this);

    addAndMakeVisible (lfoTwoTextButton = new TextButton ("lfoTwo"));
    lfoTwoTextButton->setButtonText (TRANS("LFO Two"));
    lfoTwoTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    lfoTwoTextButton->addListener (this);

    addAndMakeVisible (envOneTextButton = new TextButton ("envOne"));
    envOneTextButton->setButtonText (TRANS("ENV One "));
    envOneTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    envOneTextButton->addListener (this);

    addAndMakeVisible (envTwoTextButton = new TextButton ("envTwo"));
    envTwoTextButton->setButtonText (TRANS("ENV Two"));
    envTwoTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    envTwoTextButton->addListener (this);

    addAndMakeVisible (stepSeqTextButton = new TextButton ("stepSeq"));
    stepSeqTextButton->setButtonText (TRANS("Step Seq"));
    stepSeqTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    stepSeqTextButton->addListener (this);

    addAndMakeVisible (xyPadButton = new TextButton ("xyPad"));
    xyPadButton->setButtonText (TRANS("XY Pad"));
    xyPadButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    xyPadButton->addListener (this);

    addAndMakeVisible (fxOneTextButton = new TextButton ("fxOne"));
    fxOneTextButton->setButtonText (TRANS("FX One "));
    fxOneTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    fxOneTextButton->addListener (this);

    addAndMakeVisible (fxTwoTextButton = new TextButton ("fxTwo"));
    fxTwoTextButton->setButtonText (TRANS("FX Two "));
    fxTwoTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    fxTwoTextButton->addListener (this);

    addAndMakeVisible (midiTable = new MidiTable(p));
    midiTable->setName ("midiTable");

    addAndMakeVisible (activeTextButton = new TextButton ("active"));
    activeTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    activeTextButton->addListener (this);

    addAndMakeVisible (clearAllTextButton = new TextButton ("clearAll"));
    clearAllTextButton->setButtonText (TRANS("clear all"));
    clearAllTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    clearAllTextButton->addListener (this);

    addAndMakeVisible (clearTextButton = new TextButton ("clear"));
    clearTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    clearTextButton->addListener (this);


    //Sliders
    thresholdSlider->setRange (0, 1, 0);
    thresholdSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    thresholdSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    preDelaySlider->setRange (0, 1, 0);
    preDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    preDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    holdDelaySlider->setRange (0, 1, 0);
    holdDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    holdDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    preGainSlider->setRange (0, 1, 0);
    preGainSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    preGainSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    inputComboBox->setEditableText (false);
    inputComboBox->setJustificationType (Justification::centred);
    inputComboBox->setTextWhenNothingSelected (TRANS("Listening channel"));
    inputComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    inputComboBox->addItem (TRANS("left channel"), 1);
    inputComboBox->addItem (TRANS("right channel "), 2);
    inputComboBox->addItem (TRANS("both channels"), 3);
    inputComboBox->postCommandMessage(ParamComboBox::update);
    inputComboBox->addListener (this);

    addAndMakeVisible (label);
    label.setFont (Font (15.00f, Font::plain));
    label.setJustificationType (Justification::centredLeft);
    label.setEditable (false, false, false);
    label.setColour (Label::backgroundColourId, Colour (0xEF002020));
    label.setColour (Label::textColourId, Colour(0xFF4A997A));
    label.setColour (TextEditor::textColourId, Colours::black);


    // Global Options
    addAndMakeVisible (resonanceOptions = new ResonanceOptions(p));
    resonanceOptions->setName ("Resonance Options");

    addAndMakeVisible (presetFolderLabel = new Label ("presetFolderLabel",
                                                      TRANS("/preset\n")));
    presetFolderLabel->setFont (Font (15.00f, Font::plain));
    presetFolderLabel->setJustificationType (Justification::centredLeft);
    presetFolderLabel->setEditable (false, false, false);
    presetFolderLabel->setColour (Label::backgroundColourId, Colour(0xEF002020));
    presetFolderLabel->setColour (Label::textColourId, Colour(0xFF4A997A));
    presetFolderLabel->setColour (TextEditor::textColourId, Colours::black);

    addAndMakeVisible (selectFolderButton = new TextButton ("selectFolderButton"));
    selectFolderButton->setButtonText (TRANS("f"));
    selectFolderButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    selectFolderButton->addListener (this);

    addAndMakeVisible (overSamplingComboBox = new ComboBox ("overSamplingComboBox"));
    overSamplingComboBox->setEditableText (false);
    overSamplingComboBox->setJustificationType (Justification::centred);
    overSamplingComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    overSamplingComboBox->addItem (TRANS("1x"), 1);
    overSamplingComboBox->addItem (TRANS("2x"), 2);
    overSamplingComboBox->addItem (TRANS("4x"), 3);
    overSamplingComboBox->addItem (TRANS("8x"), 4);
    overSamplingComboBox->addListener (this);

    addAndMakeVisible (sectionLabel = new Label ("Section label",
                                                 TRANS(": Low Frequency Oscillator One\n")));
    sectionLabel->setFont (Font ("Good Times", 14.00f, Font::plain));
    sectionLabel->setJustificationType (Justification::centredLeft);
    sectionLabel->setEditable (false, false, false);
    sectionLabel->setColour (Label::backgroundColourId, Colours::black);
    sectionLabel->setColour (Label::textColourId, Colour(0xFF4A997A));
    sectionLabel->setColour (TextEditor::textColourId, Colours::black);

    //[/UserPreSize]

    setSize (895, 600);


    //[Constructor] You can add your own custom stuff here..
    setPresetDirectoryLabel(currentDirectory);
    overSamplingComboBox->setSelectedItemIndex(currentOverSampling, sendNotificationAsync);
    //[/Constructor]
}

BackPanel::~BackPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    //[Destructor]. You can add your own custom destruction code here..
    globalTextButton = nullptr;
    filtersTextButton = nullptr;
    waveShaperTextButton = nullptr;
    lfoOneTextButton = nullptr;
    lfoTwoTextButton = nullptr;
    envOneTextButton = nullptr;
    envTwoTextButton = nullptr;
    stepSeqTextButton = nullptr;
    xyPadButton = nullptr;
    fxOneTextButton = nullptr;
    fxTwoTextButton = nullptr;
    midiTable = nullptr;
    activeTextButton = nullptr;
    clearAllTextButton = nullptr;
    clearTextButton = nullptr;
    thresholdSlider = nullptr;
    preDelaySlider = nullptr;
    holdDelaySlider = nullptr;
    inputComboBox = nullptr;
    resonanceOptions = nullptr;

    presetFolderLabel = nullptr;
    selectFolderButton = nullptr;
    overSamplingComboBox = nullptr;
    sectionLabel = nullptr;
    preGainSlider = nullptr;

    //[/Destructor]
}

//==============================================================================
void BackPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    g.fillAll (Colour (0xff474747));

    g.setColour (Colours::black);
    g.fillRect (8, 10, 880, 406);
    g.fillRect (8, 426, 432, 162);
    g.fillRect (448, 426, 440, 162);


    g.setColour (Colour (0xFF4A997A));
    g.setFont (Font ("Good Times", 14.00f, Font::plain));

    g.drawText (TRANS("Section"),
                24, 18, 77, 16,
                Justification::centred, true);

    g.drawText (TRANS("Audio Trigger"),
                15, 434, 135, 18,
                Justification::centred, true);

    g.drawText (TRANS("Filer Resonance"),
                465, 434, 160, 18,
                Justification::centred, true);

    g.drawText (TRANS("MIDI Bindings"),
                112, 18, 124, 16,
                Justification::centred, true);

    g.drawText (TRANS("Threshold"),
                118, 567, 103, 18,
                Justification::centred, true);

    g.drawText (TRANS("Pre Delay"),
                227, 567, 95, 18,
                Justification::centred, true);

    g.drawText (TRANS("Hold Delay"),
                329, 567, 103, 18,
                Justification::centred, true);

    g.drawText (TRANS("Preset Folder"),
                650, 434, 136, 18,
                Justification::centredLeft, true);

    g.drawText (TRANS("Oversampling"),
                650, 504, 135, 18,
                Justification::centredLeft, true);

    g.setFont (Font ("Good Times", 14.00f, Font::plain));
    g.drawText (TRANS("Pre Gain"),
                23, 567, 87, 18,
                Justification::centred, true);

    g.drawText (TRANS("global options"),
                730, 567, 150, 18,
                Justification::centred, true);

    //[/UserPaint]
}

void BackPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    globalTextButton->setBounds (16, 38, 88, 32);
    filtersTextButton->setBounds (16, 72, 88, 32);
    waveShaperTextButton->setBounds (16, 106, 88, 32);
    lfoOneTextButton->setBounds (16, 140, 88, 32);
    lfoTwoTextButton->setBounds (16, 174, 88, 32);
    envOneTextButton->setBounds (16, 208, 88, 32);
    envTwoTextButton->setBounds (16, 242, 88, 32);
    stepSeqTextButton->setBounds (16, 276, 88, 32);
    xyPadButton->setBounds (16, 310, 88, 32);
    fxOneTextButton->setBounds (16, 344, 88, 32);
    fxTwoTextButton->setBounds (16, 378, 88, 32);
    midiTable->setBounds (112, 40, 768, 369);
    activeTextButton->setBounds (689, 15, 60, 20);
    clearAllTextButton->setBounds (819, 15, 60, 20);
    clearTextButton->setBounds (754, 15, 60, 20);
    midiTable->initialiseHeader(726, 369);

    // Slider
    preGainSlider->setBounds (24, 477, 86, 86);
    thresholdSlider->setBounds (128, 477, 86, 86);
    preDelaySlider->setBounds (232, 477, 86, 86);
    holdDelaySlider->setBounds (336, 477, 86, 86);

    inputComboBox->setBounds (160, 432, 130, 20);
    label.setBounds (302, 432, 130, 20);
    resonanceOptions->setBounds (468, 460, 154, 110);

    presetFolderLabel->setBounds (645, 460, 208, 24);
    selectFolderButton->setBounds (853, 460, 16, 24);
    overSamplingComboBox->setBounds (813, 501, 56, 20);
    sectionLabel->setBounds (233, 19, 328, 14);
    //[/UserResized]
}

void BackPanel::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == globalTextButton)
    {
        //[UserButtonCode_globalTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::globalSection);
        activeSection = 0;

        sectionLabel->setText(": Global", sendNotificationAsync);
        //[/UserButtonCode_globalTextButton]
    }
    else if (buttonThatWasClicked == filtersTextButton)
    {
        //[UserButtonCode_filtersTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::filterSection);
        activeSection = 1;
        sectionLabel->setText(": Filters",sendNotificationAsync);
        //[/UserButtonCode_filtersTextButton]
    }
    else if (buttonThatWasClicked == waveShaperTextButton)
    {
        //[UserButtonCode_waveShaperTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::waveshapersection);
        activeSection = 2;
        sectionLabel->setText(": Wave Shaper",sendNotificationAsync);
        //[/UserButtonCode_waveShaperTextButton]
    }
    else if (buttonThatWasClicked == lfoOneTextButton)
    {
        //[UserButtonCode_lfoOneTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::lfoOneSection);
        activeSection = 3;
        sectionLabel->setText(": Low Frequency Oscillator One", sendNotificationAsync);
        //[/UserButtonCode_lfoOneTextButton]
    }
    else if (buttonThatWasClicked == lfoTwoTextButton)
    {
        //[UserButtonCode_lfoTwoTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::lfoTwoSection);
        activeSection = 4;
        sectionLabel->setText(": Low Frequency Oscillator Two", sendNotificationAsync);
        //[/UserButtonCode_lfoTwoTextButton]
    }
    else if (buttonThatWasClicked == envOneTextButton)
    {
        //[UserButtonCode_envOneTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::envOneSection);
        activeSection = 5;
        sectionLabel->setText(": Envelope Generator One", sendNotificationAsync);
        //[/UserButtonCode_envOneTextButton]
    }
    else if (buttonThatWasClicked == envTwoTextButton)
    {
        //[UserButtonCode_envTwoTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::envTwoSection);
        activeSection = 6;
        sectionLabel->setText(": Envelope Generator Two", sendNotificationAsync);
        //[/UserButtonCode_envTwoTextButton]
    }
    else if (buttonThatWasClicked == stepSeqTextButton)
    {
        //[UserButtonCode_stepSeqTextButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::sseqSection);
        activeSection = 7;
        sectionLabel->setText(": Step Sequencer", sendNotificationAsync);
        //[/UserButtonCode_stepSeqTextButton]
    }
    else if (buttonThatWasClicked == xyPadButton)
    {
        //[UserButtonCode_xyPadButton] -- add your button handler code here..
        midiTable->displaySection(MidiTable::xyPadSection);
        activeSection = 8;
        sectionLabel->setText(": XY Pad", sendNotificationAsync);
        //[/UserButtonCode_xyPadButton]
    }
    else if (buttonThatWasClicked == fxOneTextButton)
    {
        //[UserButtonCode_fxOneTextButton] -- add your button handler code here..
        selectEffectTable(processor.getActiveEffect(1));
        activeSection = 9;
        sectionLabel->setText(": Effect One", sendNotificationAsync);
        //[/UserButtonCode_fxOneTextButton]
    }
    else if (buttonThatWasClicked == fxTwoTextButton)
    {
        //[UserButtonCode_fxTwoTextButton] -- add your button handler code here..
        selectEffectTable(processor.getActiveEffect(2));
        activeSection = 10;
        sectionLabel->setText(": Effect Two", sendNotificationAsync);
        //[/UserButtonCode_fxTwoTextButton]
    }
    else if (buttonThatWasClicked == activeTextButton)
    {
        //[UserButtonCode_activeTextButton] -- add your button handler code here..
        activeSection = -2;
        midiTable->displayActiveMidi();
        sectionLabel->setText(": Active parameters", sendNotificationAsync);
        //[/UserButtonCode_activeTextButton]
    }
    else if (buttonThatWasClicked == clearAllTextButton)
    {
        //[UserButtonCode_clearAllTextButton] -- add your button handler code here..
        midiTable->clearAllMidiRows();
        //[/UserButtonCode_clearAllTextButton]
    }
    else if (buttonThatWasClicked == clearTextButton)
    {
        //[UserButtonCode_clearTextButton] -- add your button handler code here..
        midiTable->clearMidiRows();
        //[/UserButtonCode_clearTextButton]
    }
    else if (buttonThatWasClicked == selectFolderButton)
    {
        //[UserButtonCode_selectFolderButton] -- add your button handler code here..
        ScopedPointer<FileChooser> fileChooser = new FileChooser("Preset Folder");

        if(fileChooser->browseForDirectory()){
            File path = fileChooser->getResult();
            String dir;

            currentDirectory = path.getFullPathName();

            // If full path name length is bigger truncate String
            // And only display the last two folders recheck
            // For length because you never know if the user
            // has really long directory names just for fun

            if(currentDirectory.length() > 28){
               StringArray tokens;

               tokens.addTokens(currentDirectory, "/", "\"");

               int size = tokens.size();

               if(size >= 2){
                  int last = size-1;
                  int prev = last-1;

                  dir = String(".../") + tokens[prev] +
                        String("/") + tokens[last];

               } else {
                   dir = String(".../") + path.getFileName();
               }
            } else {
                dir = currentDirectory;
            }

            //Should not be necessary but humans are unpredictable
            if(dir.length() > 28){
                int endIndex = dir.length();
                int startIndex = endIndex-24;

                dir = String(".../") + dir.substring(startIndex, endIndex);
            }

            presetFolderLabel->setText(dir, sendNotificationAsync);
        }


        //[/UserButtonCode_selectFolderButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void BackPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == inputComboBox)
    {
        //[UserComboBoxCode_inputComboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_inputComboBox]
    }
    else if (comboBoxThatHasChanged == overSamplingComboBox)
    {
        //[UserComboBoxCode_overSamplingComboBox] -- add your combo box handling code here..

       const int index = overSamplingComboBox->getSelectedItemIndex();

       if(currentOverSampling != index){
           const String value = String(index);
           currentOverSampling = index;

           processor.storeConfigurationOption(value, AkatekoAudioProcessor::OverSamplingId);
       }
        //[/UserComboBoxCode_overSamplingComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void BackPanel::setLookAndFeel(LookAndFeel *cLaf, LookAndFeel *sLaf){
    globalTextButton->setLookAndFeel(cLaf);
    filtersTextButton->setLookAndFeel(cLaf);
    waveShaperTextButton->setLookAndFeel(cLaf);
    lfoOneTextButton->setLookAndFeel(cLaf);
    lfoTwoTextButton->setLookAndFeel(cLaf);
    envOneTextButton->setLookAndFeel(cLaf);
    envTwoTextButton->setLookAndFeel(cLaf);
    stepSeqTextButton->setLookAndFeel(cLaf);
    xyPadButton->setLookAndFeel(cLaf);
    fxOneTextButton->setLookAndFeel(cLaf);
    fxTwoTextButton->setLookAndFeel(cLaf);
    activeTextButton->setLookAndFeel(cLaf);
    clearTextButton->setLookAndFeel(cLaf);
    clearAllTextButton->setLookAndFeel(cLaf);
    midiTable->setLookAndFeel(cLaf);
    inputComboBox->setLookAndFeel(cLaf);

    thresholdSlider->setLookAndFeel(sLaf);
    preDelaySlider->setLookAndFeel(sLaf);
    holdDelaySlider->setLookAndFeel(sLaf);
    preGainSlider->setLookAndFeel(sLaf);

    resonanceOptions->setLookAndFeel(cLaf);

    selectFolderButton->setLookAndFeel(cLaf);
    overSamplingComboBox->setLookAndFeel(cLaf);
}

void BackPanel::selectEffectTable(int fx){
    switch(fx){
        case 1:
            midiTable->displaySection(MidiTable::fxSection);
            break;
        case 2:
            midiTable->displaySection(MidiTable::sDelaySection);
            break;
        case 3:
            midiTable->displaySection(MidiTable::ppDelaySection);
            break;
        case 4:
            midiTable->displaySection(MidiTable::lcrDelaySection);
            break;
        case 5:
            midiTable->displaySection(MidiTable::sFlangerSection);
            break;
        case 6:
            midiTable->displaySection(MidiTable::dChorusSection);
            break;
        case 7:
            midiTable->displaySection(MidiTable::hDelaySection);
            break;
        case 8:
            midiTable->displaySection(MidiTable::decimatorSection);
            break;
        case 9:
            midiTable->displaySection(MidiTable::pReverbSection);
            break;
        default:
            midiTable->displaySection(MidiTable::none);
            break;
    }
}

void BackPanel::visibilityChanged(){
    if(isVisible()){
        midiTable->restoreMidiRows();

        switch(activeSection){
            case -2: midiTable->displayActiveMidi(); break;
            case 0: midiTable->displaySection(MidiTable::globalSection); break;
            case 1: midiTable->displaySection(MidiTable::filterSection); break;
            case 2: midiTable->displaySection(MidiTable::waveshapersection); break;
            case 3: midiTable->displaySection(MidiTable::lfoOneSection); break;
            case 4: midiTable->displaySection(MidiTable::lfoTwoSection); break;
            case 5: midiTable->displaySection(MidiTable::envOneSection); break;
            case 6: midiTable->displaySection(MidiTable::envTwoSection); break;
            case 7: midiTable->displaySection(MidiTable::sseqSection); break;
            case 8: midiTable->displaySection(MidiTable::xyPadSection); break;
            case 9: selectEffectTable(processor.getActiveEffect(1)); break;
            case 10: selectEffectTable(processor.getActiveEffect(2));break;
        }
    }
}

void BackPanel::setPresetDirectoryLabel(String directory){

    std::cout << "Preset Directory : " << directory << std::endl;

    String dir = String("/presets");
    StringArray tokens;
    tokens.addTokens(directory, "/", "\"");

    if(directory.length() > 28){
       int size = tokens.size();

       std::cout << "Token size : " << size << std::endl;

       if(size >= 2){

          int last = size-1;
          int prev = last-1;

          std::cout << "Prev Tokens" << std::endl;

          dir = String(".../") + tokens[prev] +
                String("/") + tokens[last];

       } else {
           std::cout << "Inside last token should be able to add build" << std::endl;
           dir = String(".../") + tokens[tokens.size()-1];
       }
    } else {
        dir = directory;
    }

    //Should not be necessary but humans are unpredictable
    if(dir.length() > 28){
        int endIndex = dir.length();
        int startIndex = endIndex-24;

        dir = String(".../") + dir.substring(startIndex, endIndex);
    }

    presetFolderLabel->setText(dir, sendNotificationAsync);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="BackPanel" componentName=""
                 parentClasses="public Component" constructorParams="AkatekoAudioProcessor &amp;p"
                 variableInitialisers="processor(p)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="895"
                 initialHeight="600">
  <BACKGROUND backgroundColour="ff474747">
    <RECT pos="8 10 880 406" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="24 18 77 16" fill="solid: ffdbdbdb" hasStroke="0" text="Section"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <RECT pos="8 426 432 166" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="448 426 440 166" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="17 434 135 18" fill="solid: ffdbdbdb" hasStroke="0" text="Audio Trigger"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="454 434 80 18" fill="solid: ffdbdbdb" hasStroke="0" text="Options"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="112 18 124 16" fill="solid: ffdbdbdb" hasStroke="0" text="MIDI Bindings"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="121 560 103 18" fill="solid: ffdbdbdb" hasStroke="0" text="Threshold"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="225 560 95 18" fill="solid: ffdbdbdb" hasStroke="0" text="Pre Delay"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="329 560 103 18" fill="solid: ffdbdbdb" hasStroke="0" text="Hold Delay"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="641 432 136 18" fill="solid: ffdbdbdb" hasStroke="0" text="Preset Folder"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="649 504 135 18" fill="solid: ffdbdbdb" hasStroke="0" text="Oversampling"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="25 560 87 18" fill="solid: ffdbdbdb" hasStroke="0" text="Pre Gain"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <TEXTBUTTON name="Global" id="f65ca7208eee8c91" memberName="globalTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 38 88 32" buttonText="global"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Filters" id="8d0acc90244a49c7" memberName="filtersTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 72 88 32" buttonText="filters"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Waveshaper" id="9363735a2d84aad0" memberName="waveShaperTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 106 88 32" buttonText="Waveshaper"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="lfoOne" id="67590c06f772e598" memberName="lfoOneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 140 88 32" buttonText="LFO One"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="lfoTwo" id="487a4a568d9376d1" memberName="lfoTwoTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 174 88 32" buttonText="LFO Two"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="envOne" id="2b1600e980c148e8" memberName="envOneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 208 88 32" buttonText="ENV One "
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="envTwo" id="993bfc388b5259c2" memberName="envTwoTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 242 88 32" buttonText="ENV Two"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="stepSeq" id="5bae5b89a07f1d04" memberName="stepSeqTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 276 88 32" buttonText="Step Seq"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="xyPad" id="e420a6a9f0ae71b4" memberName="xyPadButton" virtualName=""
              explicitFocusOrder="0" pos="16 310 88 32" buttonText="XY Pad"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="fxOne" id="2b660ad9d6d8ce20" memberName="fxOneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 344 88 32" buttonText="FX One "
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="fxTwo" id="8111f305f097c5fe" memberName="fxTwoTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 378 88 32" buttonText="FX Two "
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="midiTable" id="c1fb461b9819dc18" memberName="midiTable"
                    virtualName="MidiTable" explicitFocusOrder="0" pos="112 40 768 369"
                    class="Component" params=""/>
  <TEXTBUTTON name="active" id="ddc4e04e00f47d5" memberName="activeTextButton"
              virtualName="" explicitFocusOrder="0" pos="689 15 60 20" buttonText="active"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="clearAll" id="6995df2180bdc34e" memberName="clearAllTextButton"
              virtualName="" explicitFocusOrder="0" pos="819 15 60 20" buttonText="clear all"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="clear" id="18950f6928d300b7" memberName="clearTextButton"
              virtualName="" explicitFocusOrder="0" pos="754 15 60 20" buttonText="clear"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="threshold" id="ca2dc6c39de035de" memberName="thresholdSlider"
          virtualName="" explicitFocusOrder="0" pos="128 472 86 86" rotaryslideroutline="ffffffff"
          min="0" max="10" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="Pre Delay" id="a02a54c0977ce97b" memberName="preDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="232 472 86 86" rotaryslideroutline="ffffffff"
          min="0" max="10" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="Hold Delay" id="e487035ebcb51b7c" memberName="holdDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="336 472 86 86" rotaryslideroutline="ffffffff"
          min="0" max="10" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <COMBOBOX name="Input Slection" id="64d295484e1a46b9" memberName="inputComboBox"
            virtualName="" explicitFocusOrder="0" pos="164 432 130 20" editable="0"
            layout="36" items="left channel&#10;right channel &#10;both channels"
            textWhenNonSelected="channel" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="2e909809c10a6a85" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="312 430 120 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="hold delay: 250&#10;&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="Resonance Options" id="7d4d9c9be1831a63" memberName="resonanceOptions"
                    virtualName="ResonanceOptions" explicitFocusOrder="0" pos="456 456 150 120"
                    class="Component" params=""/>
  <LABEL name="presetFolderLabel" id="951ab4d0cdd47503" memberName="presetFolderLabel"
         virtualName="" explicitFocusOrder="0" pos="640 456 208 24" bkgCol="3a7c7c7c"
         textCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="/preset&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="selectFolderButton" id="c1f778762604d6f8" memberName="selectFolderButton"
              virtualName="" explicitFocusOrder="0" pos="848 456 16 24" buttonText="f"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="overSamplingComboBox" id="1b36ce23f2144480" memberName="overSamplingComboBox"
            virtualName="" explicitFocusOrder="0" pos="808 502 56 20" editable="0"
            layout="36" items="1x&#10;2x&#10;4x&#10;8x" textWhenNonSelected="1x"
            textWhenNoItems="(no choices)"/>
  <LABEL name="Section label" id="7c6ced8114e13c0f" memberName="sectionLabel"
         virtualName="" explicitFocusOrder="0" pos="234 19 328 14" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText=": Low Frequency Oscillator One&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Good Times" fontsize="14" bold="0" italic="0" justification="33"/>
  <SLIDER name="preGainSlider" id="10d064d420e1548b" memberName="preGainSlider"
          virtualName="" explicitFocusOrder="0" pos="24 472 86 86" rotaryslideroutline="ffffffff"
          min="0" max="10" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
