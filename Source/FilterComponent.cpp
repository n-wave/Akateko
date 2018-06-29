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

#include "FilterComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "/work/programming-projects/msm/msm-dsp/Common/msm.h"
using akateko::initMidiRow;
using akateko::MidiRow;

//[UserDefs]

//==============================================================================
FilterComponent::FilterComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();
    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::FilterId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::FilterId);

    if(paramIndices.size() == 18 &&
       paramIds.size() == 18 &&
       params.size() >= paramIndices[17])
    {

        //Store Command Id's
        requestMenuIds[0] = paramIds[0].hash();
        requestMenuIds[1] = paramIds[1].hash();
        requestMenuIds[2] = paramIds[2].hash();
        requestMenuIds[3] = paramIds[3].hash();
        requestMenuIds[4] = paramIds[4].hash();
        requestMenuIds[5] = paramIds[5].hash();
        requestMenuIds[6] = paramIds[6].hash();
        requestMenuIds[7] = paramIds[7].hash();
        requestMenuIds[8] = paramIds[8].hash();
        requestMenuIds[9] = paramIds[9].hash();
        requestMenuIds[10] = paramIds[10].hash();
        requestMenuIds[11] = paramIds[11].hash();
        requestMenuIds[12] = paramIds[12].hash();
        requestMenuIds[13] = paramIds[13].hash();
        requestMenuIds[14] = paramIds[14].hash();
        requestMenuIds[15] = paramIds[15].hash();
        requestMenuIds[16] = paramIds[16].hash();
        requestMenuIds[17] = paramIds[17].hash();

        initialiseMidiStrings();

        addAndMakeVisible (filterToggleButton = new ParamImageToggle (paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible (filterConfigComboBox = new ParamComboBox (paramIds[1], *params.getUnchecked(paramIndices[1]), label));
        // Filter One
        addAndMakeVisible (filterOneToggleButton = new ParamImageToggle (paramIds[2], *params.getUnchecked(paramIndices[2]), label));
        addAndMakeVisible (filterOneTypeComboBox = new ParamComboBox (paramIds[3], *params.getUnchecked(paramIndices[3]), label));
        addAndMakeVisible (filterOneRollOffComboBox = new ParamComboBox (paramIds[4], *params.getUnchecked(paramIndices[4]), label));
        addAndMakeVisible (filterOneFrequencySlider = new ParamSlider (paramIds[5], *params.getUnchecked(paramIndices[5]), label, 20., 20000.));
        addAndMakeVisible (filterOneResonanceSlider = new ParamSlider (paramIds[6], *params.getUnchecked(paramIndices[6]), label));
        addAndMakeVisible (filterOnePassBandSlider = new ParamSlider (paramIds[7], *params.getUnchecked(paramIndices[7]), label, true));
        addAndMakeVisible (filterOneDriveSlider = new ParamSlider(paramIds[8], *params.getUnchecked(paramIndices[8]),label));
        addAndMakeVisible (filterOneVolumeSlider = new ParamSlider(paramIds[9], *params.getUnchecked(paramIndices[9]), label, true));
        // Filter Two
        addAndMakeVisible (filterTwoToggleButton = new ParamImageToggle (paramIds[10], *params.getUnchecked(paramIndices[10]), label));
        addAndMakeVisible (filterTwoTypeComboBox = new ParamComboBox (paramIds[11], *params.getUnchecked(paramIndices[11]), label));
        addAndMakeVisible (filterTwoRollOffComboBox = new ParamComboBox (paramIds[12], *params.getUnchecked(paramIndices[12]), label));
        addAndMakeVisible (filterTwoFrequencySlider = new ParamSlider (paramIds[13], *params.getUnchecked(paramIndices[13]), label, 20., 20000.));
        addAndMakeVisible (filterTwoResonanceSlider = new ParamSlider (paramIds[14], *params.getUnchecked(paramIndices[14]), label));
        addAndMakeVisible (filterTwoPassBandSlider = new ParamSlider(paramIds[15], *params.getUnchecked(paramIndices[15]), label, true));
        addAndMakeVisible (filterTwoDriveSlider = new ParamSlider (paramIds[16], *params.getUnchecked(paramIndices[16]), label));
        addAndMakeVisible (filterTwoVolumeSlider = new ParamSlider (paramIds[17], *params.getUnchecked(paramIndices[17]), label, true));


        proceed = true;
    } else{
        addAndMakeVisible (filterToggleButton = new ImageButton ("filterToggle"));
        addAndMakeVisible (filterConfigComboBox = new ComboBox ("filterConfig"));    
        addAndMakeVisible (filterOneToggleButton = new ImageButton ("filterOneToggle")); // Filter One
        addAndMakeVisible (filterOneTypeComboBox = new ComboBox ("filterOneType"));
        addAndMakeVisible (filterOneRollOffComboBox = new ComboBox ("filterOneRollOff"));
        addAndMakeVisible (filterOneFrequencySlider = new Slider ("filterOneFrequency"));
        addAndMakeVisible (filterOneResonanceSlider = new Slider ("filterOneResonance"));
        addAndMakeVisible (filterOneDriveSlider = new Slider ("filterOneDrive"));
        addAndMakeVisible (filterOnePassBandSlider = new Slider ("filterOnePassBand"));
        addAndMakeVisible (filterOneVolumeSlider = new Slider ("filterOneVolumeSlider"));
        addAndMakeVisible (filterTwoToggleButton = new ImageButton ("filterTwoToggle")); // Filter Two
        addAndMakeVisible (filterTwoTypeComboBox = new ComboBox ("filterTwoType"));
        addAndMakeVisible (filterTwoRollOffComboBox = new ComboBox ("filterTwoRollOff"));
        addAndMakeVisible (filterTwoFrequencySlider = new Slider ("filterTwoFrequency"));
        addAndMakeVisible (filterTwoResonanceSlider = new Slider ("filterTwoResonance"));
        addAndMakeVisible (filterTwoPassBandSlider = new Slider ("filterTwoPassBand"));
        addAndMakeVisible (filterTwoDriveSlider = new Slider ("filterTwoDrive"));
        addAndMakeVisible (filterTwoVolumeSlider = new Slider ("filterTwoVolumeSlider"));

        std::cerr << "FilterComponent::CilterComponent" << std::endl;
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

        proceed = false;
    }

    addAndMakeVisible (filterDisplay = new FilterDisplay ("fdis"));
    filterDisplay->setName ("filterDisplay");
    //[/Constructor_pre]


    //[UserPreSize]

    /* Bypass/Enable Filter */
    filterToggleButton->setButtonText (String());
    filterToggleButton->setImages(false, true, false,
                                    ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                    ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                    ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    filterToggleButton->addListener(this);

    /* Serial/Paralell Filter */
    filterConfigComboBox->setEditableText (false);
    filterConfigComboBox->setJustificationType (Justification::centred);
    filterConfigComboBox->setTextWhenNothingSelected (String());
    filterConfigComboBox->setTextWhenNoChoicesAvailable (TRANS("(Config)"));
    filterConfigComboBox->addItem (TRANS("Series"), 1);
    filterConfigComboBox->addItem (TRANS("Parallel"), 2);
    filterConfigComboBox->addSeparator();
    filterConfigComboBox->postCommandMessage(ParamComboBox::update);
    filterConfigComboBox->addListener (this);


    this->initialiseFilterOne();
    this->initialiseFilterTwo();
    //[/UserPreSize]

    setSize (345, 255);


    //[Constructor] You can add your own custom stuff here..
    filterDisplay->enableFilter(filterOneToggleButton->getToggleState(), 1);
    filterDisplay->enableFilter(filterTwoToggleButton->getToggleState(), 2);

    // Update Filter Display
    const double filterOneFreq = filterOneFrequencySlider->getValue();
    filterDisplay->setFrequency(filterOneFreq, 1);

    const double filterTwoFreq = filterTwoFrequencySlider->getValue();
    filterDisplay->setFrequency(filterTwoFreq, 2);

    // Set ComboBox Look And Feel

    // Set Colours filter Display
    filterDisplay->setColour(FilterDisplay::backgroundColourId, Colour(Colours::black));
    filterDisplay->setColour(FilterDisplay::outlineColourId, Colour(0xAA407050));
    filterDisplay->setColour(FilterDisplay::pathColourId, Colour(0xBB78C0A0));
    filterDisplay->setColour(FilterDisplay::ellipseColourId, Colour(0xFF60B090));
    filterDisplay->setColour(FilterDisplay::highLightColourId, Colour(0x8F9AC9B0));
    filterDisplay->setColour(FilterDisplay::overlayGradientTwoId, Colour(0x4F00FFDA));
    filterDisplay->setColour(FilterDisplay::filterGradientOneId, Colour(0x7F70C099));
   // ImageCache::getFromMemory(BinaryData::smoothValue100Values_png, BinaryData::smoothValue100Values_pngSize);

    // Build Menu
    menu.addItem(1, "learn", true, false, nullptr);
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
    //[/Constructor]
}

FilterComponent::~FilterComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    filterTwoFrequencySlider = nullptr;
    filterTwoResonanceSlider = nullptr;
    filterDisplay = nullptr;
    filterOneTypeComboBox = nullptr;
    filterOneRollOffComboBox = nullptr;
    filterToggleButton = nullptr;
    filterTwoTypeComboBox = nullptr;
    filterTwoRollOffComboBox = nullptr;
    filterOneToggleButton = nullptr;
    filterTwoToggleButton = nullptr;
    filterConfigComboBox = nullptr;
    filterTwoDriveSlider = nullptr;
    filterTwoPassBandSlider = nullptr;
    filterTwoVolumeSlider = nullptr;
    filterOneFrequencySlider = nullptr;
    filterOneResonanceSlider = nullptr;
    filterOneDriveSlider = nullptr;
    filterOnePassBandSlider = nullptr;
    filterOneVolumeSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("F"),
                217, 192, 14, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("Q"),
                282, 192, 14, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("Q"),
                113, 192, 14, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DRIVE"),
                233, 240, 46, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PBG"),
                187, 240, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("VOL"),
                293, 240, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("F"),
                49, 192, 14, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DRIVE"),
                65, 240, 46, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PBG"),
                19, 240, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("VOL"),
                123, 240, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("FILTER"),
                135, 3, 73, 21,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    filterDisplay->setBounds (22, 24, 297, 102);

    filterConfigComboBox->setBounds (134, 128, 73, 16);
    filterOneTypeComboBox->setBounds (24, 128, 55, 16);
    filterOneRollOffComboBox->setBounds (79, 128, 55, 16);
    filterTwoTypeComboBox->setBounds (207, 128, 55, 16);
    filterTwoRollOffComboBox->setBounds (262, 128, 55, 16);


    //[/UserPreResize]

    filterTwoFrequencySlider->setBounds (200, 146, 50, 50);
    filterTwoResonanceSlider->setBounds (264, 146, 50, 50);


    filterToggleButton->setBounds (322, 3, 18, 18);

    filterOneToggleButton->setBounds (150, 155, 16, 16);
    filterTwoToggleButton->setBounds (176, 155, 16, 16);

    filterTwoDriveSlider->setBounds (232, 194, 50, 50);
    filterTwoPassBandSlider->setBounds (184, 202, 40, 40);
    filterTwoVolumeSlider->setBounds (288, 202, 40, 40);
    filterOneFrequencySlider->setBounds (31, 146, 50, 50);
    filterOneResonanceSlider->setBounds (95, 146, 50, 50);
    filterOneDriveSlider->setBounds (63, 194, 50, 50);
    filterOnePassBandSlider->setBounds (15, 202, 40, 40);
    filterOneVolumeSlider->setBounds (119, 202, 40, 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == filterTwoFrequencySlider)
    {
        //[UserSliderCode_filterTwoFrequencySlider] -- add your slider handling code here..
        float freq = sliderThatWasMoved->getValue();
        filterDisplay->setFrequency(freq, 2);
        //[/UserSliderCode_filterTwoFrequencySlider]
    }
    else if (sliderThatWasMoved == filterTwoResonanceSlider)
    {
        //[UserSliderCode_filterTwoResonanceSlider] -- add your slider handling code here..
        float reso = sliderThatWasMoved->getValue();
        filterDisplay->setResonance(reso, 2);
        //[/UserSliderCode_filterTwoResonanceSlider]
    }
    else if (sliderThatWasMoved == filterOneFrequencySlider)
    {
        //[UserSliderCode_filterOneFrequencySlider] -- add your slider handling code here..
        float freq = sliderThatWasMoved->getValue();
        filterDisplay->setFrequency(freq, 1);
        //[/UserSliderCode_filterOneFrequencySlider]
    }
    else if (sliderThatWasMoved == filterOneResonanceSlider)
    {
        //[UserSliderCode_filterOneResonanceSlider] -- add your slider handling code here..
        float reso = sliderThatWasMoved->getValue();
        filterDisplay->setResonance(reso, 1);
        //[/UserSliderCode_filterOneResonanceSlider]
    }

    //[UsersliderValueChanged_Post]


    //[/UsersliderValueChanged_Post]
}

void FilterComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]

    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == filterOneTypeComboBox)
    {
        //[UserComboBoxCode_filterOneTypeComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterType(tmpSelectedIndex, 1);

        //[/UserComboBoxCode_filterOneTypeComboBox]
    }
    else if (comboBoxThatHasChanged == filterOneRollOffComboBox)
    {
        //[UserComboBoxCode_filterOneRollOffComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterRollOff(tmpSelectedIndex, 1);
        //[/UserComboBoxCode_filterOneRollOffComboBox]
    }
    else if (comboBoxThatHasChanged == filterTwoTypeComboBox)
    {
        //[UserComboBoxCode_filterTwoTypeComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterType(tmpSelectedIndex, 2);
        //[/UserComboBoxCode_filterTwoTypeComboBox]
    }
    else if (comboBoxThatHasChanged == filterTwoRollOffComboBox)
    {
        //[UserComboBoxCode_filterTwoRollOffComboBox] -- add your combo box handling code here..
        int tmpSelectedIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        this->setFilterRollOff(tmpSelectedIndex, 2);
        //[/UserComboBoxCode_filterTwoRollOffComboBox]
    }
    else if (comboBoxThatHasChanged == filterConfigComboBox)
    {
        //[UserComboBoxCode_filterConfigComboBox] -- add your combo box handling code here..
        processor.updateFilterConfiguration();
        //[/UserComboBoxCode_filterConfigComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void FilterComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == filterOneToggleButton)
    {
        //[UserButtonCode_filterOneToggleButton] -- add your button handler code here..
        filterDisplay->enableFilter(filterOneToggleButton->getToggleState(), 1);
        processor.updateFilterOneEnabled();
        //[/UserButtonCode_filterOneToggleButton]
    }
    else if (buttonThatWasClicked == filterTwoToggleButton)
    {
        //[UserButtonCode_filterTwoToggleButton] -- add your button handler code here..
        filterDisplay->enableFilter(filterTwoToggleButton->getToggleState(), 2);
        processor.updateFilterTwoEnabled();
        //[/UserButtonCode_filterTwoToggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

/* After constructing and initialising the GUI components
 * Finish setting the rest of the settings
 */
void FilterComponent::initialiseFilterOne(){
    // Filter one Enable
    filterOneToggleButton->setButtonText (String());
    filterOneToggleButton->setImages(false, true, false,
                                     ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                     ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                     ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    filterOneToggleButton->addListener (this);

    // Frequency filter one
    filterOneFrequencySlider->setRange (0, 1, 0.001);
    filterOneFrequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOneFrequencySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterOneFrequencySlider->addListener (this);

    // resonance filter one
    filterOneResonanceSlider->setRange (0, 1, 0.001);
    filterOneResonanceSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOneResonanceSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterOneResonanceSlider->addListener (this);

    // Filter one type
    filterOneTypeComboBox->setEditableText (false);
    filterOneTypeComboBox->setJustificationType (Justification::centred);
    filterOneTypeComboBox->setTextWhenNothingSelected (String());
    filterOneTypeComboBox->setTextWhenNoChoicesAvailable (TRANS("(Type)"));
    filterOneTypeComboBox->addItem (TRANS("LPF"), 1);
    filterOneTypeComboBox->addItem (TRANS("HPF"), 2);
    filterOneTypeComboBox->addItem (TRANS("BPF"), 3);
    filterOneTypeComboBox->addSeparator();
    filterOneTypeComboBox->postCommandMessage(ParamComboBox::update);
    filterOneTypeComboBox->addListener (this);

    // Filter one roll Off
    filterOneRollOffComboBox->setEditableText (false);
    filterOneRollOffComboBox->setJustificationType (Justification::centred);
    filterOneRollOffComboBox->setTextWhenNothingSelected (String());
    filterOneRollOffComboBox->setTextWhenNoChoicesAvailable (TRANS("(R-Off)"));
    filterOneRollOffComboBox->addItem (TRANS("12DB"), 1);
    filterOneRollOffComboBox->addItem (TRANS("24DB"), 2);
    filterOneRollOffComboBox->addSeparator();
    filterOneRollOffComboBox->postCommandMessage(ParamComboBox::update);
    filterOneRollOffComboBox->addListener (this);

    //  Filter Drive
    filterOneDriveSlider->setRange (0, 1, 0);
    filterOneDriveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOneDriveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    // Filter Pass Band
    filterOnePassBandSlider->setRange (0, 1, 0);
    filterOnePassBandSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOnePassBandSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    // Filter Volume
    filterOneVolumeSlider->setRange (0, 1, 0);
    filterOneVolumeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterOneVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterOneVolumeSlider->setSkewFactor(0.5f);
}

void FilterComponent::initialiseFilterTwo(){
    // filter two enable
    filterTwoToggleButton->setButtonText (String());
    filterTwoToggleButton->setImages(false, true, false,
                                     ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                     ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                     ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    filterTwoToggleButton->addListener (this);

    // filter two frequency slider
    filterTwoFrequencySlider->setRange (0, 1, 0);
    filterTwoFrequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoFrequencySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterTwoFrequencySlider->addListener (this);

    // filter two resonance slider
    filterTwoResonanceSlider->setRange (0, 1, 0);
    filterTwoResonanceSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoResonanceSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterTwoResonanceSlider->addListener (this);

    // filter two type box
    filterTwoTypeComboBox->setEditableText (false);
    filterTwoTypeComboBox->setJustificationType (Justification::centred);
    filterTwoTypeComboBox->setTextWhenNothingSelected (String());
    filterTwoTypeComboBox->setTextWhenNoChoicesAvailable (TRANS("(Type)"));
    filterTwoTypeComboBox->addItem (TRANS("LPF"), 1);
    filterTwoTypeComboBox->addItem (TRANS("HPF"), 2);
    filterTwoTypeComboBox->addItem (TRANS("BPF"), 3);
    filterTwoTypeComboBox->addSeparator();
    filterTwoTypeComboBox->postCommandMessage(ParamComboBox::update);
    filterTwoTypeComboBox->addListener (this);

    // Filter Two Roll Off
    filterTwoRollOffComboBox->setEditableText (false);
    filterTwoRollOffComboBox->setJustificationType (Justification::centred);
    filterTwoRollOffComboBox->setTextWhenNothingSelected (String());
    filterTwoRollOffComboBox->setTextWhenNoChoicesAvailable (TRANS("(R-Off)"));
    filterTwoRollOffComboBox->addItem (TRANS("12DB"), 1);
    filterTwoRollOffComboBox->addItem (TRANS("24DB"), 2);
    filterTwoRollOffComboBox->addSeparator();
    filterTwoRollOffComboBox->postCommandMessage(ParamComboBox::update);
    filterTwoRollOffComboBox->addListener (this);

    // Filter Two Drive
    filterTwoDriveSlider->setRange (0, 1, 0);
    filterTwoDriveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoDriveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    // Filter Two Pass Band
    filterTwoPassBandSlider->setRange (0, 1, 0);
    filterTwoPassBandSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoPassBandSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    // Filter Two Volume
    filterTwoVolumeSlider->setRange (0, 1, 0);
    filterTwoVolumeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    filterTwoVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filterTwoVolumeSlider->setSkewFactor(0.5f);
}

void FilterComponent::setFilterType(int &selectedIndex, int filter){
    switch(selectedIndex){
        case 0:
            filterDisplay->setType(FilterDisplay::LPF,filter);
            break;
        case 1:
            filterDisplay->setType(FilterDisplay::HPF,filter);
            break;
        case 2:
            filterDisplay->setType(FilterDisplay::BPF, filter);
    }

    processor.updateFilterType(filter);
}

void FilterComponent::setFilterRollOff(int &selectedIndex, int filter){
    switch(selectedIndex){
        case 0:
            filterDisplay->setRollOff(FilterCurve::RollOff12dB,filter);
            break;
        case 1:
            filterDisplay->setRollOff(FilterCurve::RollOff24dB,filter);
            break;
    }
    processor.updateFilterType(filter);
}

void FilterComponent::updateGui(){
    filterToggleButton->postCommandMessage(ParamToggle::update);
    filterConfigComboBox->postCommandMessage(ParamComboBox::update);
    // Filter One
    filterOneToggleButton->postCommandMessage(ParamToggle::update);
    filterOneTypeComboBox->postCommandMessage(ParamComboBox::update);
    filterOneRollOffComboBox->postCommandMessage(ParamComboBox::update);

    filterOneFrequencySlider->postCommandMessage(ParamSlider::update);
    filterOneResonanceSlider->postCommandMessage(ParamSlider::update);
    filterOneDriveSlider->postCommandMessage(ParamSlider::update);
    filterOnePassBandSlider->postCommandMessage(ParamSlider::update);
    filterOneVolumeSlider->postCommandMessage(ParamSlider::update);

    // Filter Two
    filterTwoToggleButton->postCommandMessage(ParamToggle::update);
    filterTwoTypeComboBox->postCommandMessage(ParamComboBox::update);
    filterTwoRollOffComboBox->postCommandMessage(ParamComboBox::update);

    filterTwoFrequencySlider->postCommandMessage(ParamSlider::update);
    filterTwoResonanceSlider->postCommandMessage(ParamSlider::update);
    filterTwoPassBandSlider->postCommandMessage(ParamSlider::update);
    filterTwoDriveSlider->postCommandMessage(ParamSlider::update);
    filterTwoVolumeSlider->postCommandMessage(ParamSlider::update);
}

void FilterComponent::setLookAndFeel(LookAndFeel *cLaf, LookAndFeel *bLaf, LookAndFeel *sLaf){
    filterConfigComboBox->setLookAndFeel(cLaf);
    filterOneTypeComboBox->setLookAndFeel(cLaf);
    filterOneRollOffComboBox->setLookAndFeel(cLaf);
    filterTwoRollOffComboBox->setLookAndFeel(cLaf);
    filterTwoTypeComboBox->setLookAndFeel(cLaf);


    filterOneFrequencySlider->setLookAndFeel(bLaf);
    filterOneResonanceSlider->setLookAndFeel(bLaf);
    filterOneDriveSlider->setLookAndFeel(bLaf);

    filterOnePassBandSlider->setLookAndFeel(sLaf);
    filterOneVolumeSlider->setLookAndFeel(sLaf);

    filterTwoFrequencySlider->setLookAndFeel(bLaf);
    filterTwoResonanceSlider->setLookAndFeel(bLaf);
    filterTwoDriveSlider->setLookAndFeel(bLaf);

    filterTwoPassBandSlider->setLookAndFeel(sLaf);
    filterTwoVolumeSlider->setLookAndFeel(sLaf);

    menu.setLookAndFeel(cLaf);
}

//Not using but leave in for more easy acces
void FilterComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" Filters: Enable");
    midiStrings.add(" Filters: config");
    midiStrings.add(" Filters 1: Enable");
    midiStrings.add(" Filters 1: Type");
    midiStrings.add(" Filters 1: Roll Off");
    midiStrings.add(" Filters 1 Freq");
    midiStrings.add(" Filter 1: Reso");
    midiStrings.add(" Filter 1: PBG");
    midiStrings.add(" Filter 1: Drive");
    midiStrings.add(" Filter 1: Vol");
    midiStrings.add(" Filter 2: Enable");
    midiStrings.add(" Filter 2: Type");
    midiStrings.add(" Filter 2: Roll Off");
    midiStrings.add(" Filter 2: Freq");
    midiStrings.add(" Filter 2: Reso");
    midiStrings.add(" Filter 2: PGB");
    midiStrings.add(" Filter 2: Drive");
    midiStrings.add(" Filter 2: Vol");
}


void FilterComponent::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    switch(commandId){
        case FilterDisplay::F1CH:
            /* Send async so that the parameter is updated */

            filterOneFrequencySlider->setValue(filterDisplay->getFrequency(1),
                                               NotificationType::sendNotificationAsync);
            filterOneResonanceSlider->setValue(filterDisplay->getResonance(1),
                                               NotificationType::sendNotificationAsync);
            break;
        case FilterDisplay::F2CH:
            filterTwoFrequencySlider->setValue(filterDisplay->getFrequency(2),
                                               NotificationType::sendNotificationAsync);
            filterTwoResonanceSlider->setValue(filterDisplay->getResonance(2),
                                               NotificationType::sendNotificationAsync);
            break;
    }

    if(paramIndices.size() == 18 &&
       midiStrings.size() == 18)
    {
        int index = -1;
        int param = -1;
        int handling = - 1;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuIds[0]){ // Filters Enables
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[2]){ // Filter One Enable
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[3]){ // Filter One Type
            index = menu.show();
            param = 3;
            maxValue = 2;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[4]){ // Filter One RollOff
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[5]){ // Filter One Frequency
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[6]){ //Filter One Reso
            index = menu.show();
            param = 6;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[7]){ // Filter One PBG
            index = menu.show();
            param = 7;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[8]){ // Filter One Drive
            index = menu.show();
            param = 8;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[9]){ // Filter One Volume
            index = menu.show();
            param = 9;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[10]){ //Filter Two Enable
            index = menu.show();
            param = 10;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[11]){ //Filter Two type
            index = menu.show();
            param = 11;
            maxValue = 2;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[12]){ //Filter Two Rolloff
            index = menu.show();
            param = 12;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[13]){ //Filter Two Frequency
            index = menu.show();
            param = 13;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[14]){ //Filter Two Resonance
            index = menu.show();
            param = 14;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[15]){ //Filter Two PBG
            index = menu.show();
            param = 15;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[16]){ //Filter Two Drive
            index = menu.show();
            param = 16;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[17]){ //Filter Two Volume
            index = menu.show();
            param = 17;
            handling = akateko::MIDI_TO_DOUBLE;
        }
        if(index == 1){
            MidiRow tmpRow;
            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling, midiStrings[param],1);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);

            String message = midiStrings[param] + String(" cleared");
            labelRef.setText(message, sendNotificationAsync);
        }
    }

    //[/UserCode_handleCommandMessage]
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="345" initialHeight="255">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
    <METHOD name="focusOfChildComponentChanged (FocusChangeType cause)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="217 192 14 10" fill="solid: ffaaaaaa" hasStroke="0" text="F"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="282 192 14 10" fill="solid: ffaaaaaa" hasStroke="0" text="Q"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="113 192 14 10" fill="solid: ffaaaaaa" hasStroke="0" text="Q"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="233 240 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="DRIVE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="187 240 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="PBG"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="293 240 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="VOL"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="49 192 14 10" fill="solid: ffaaaaaa" hasStroke="0" text="F"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="65 240 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="DRIVE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="19 240 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="PBG"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="123 240 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="VOL"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="135 3 73 21" fill="solid: ffaaaaaa" hasStroke="0" text="FILTER"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="filterTwoFrequency" id="6c33bb30cb3d5303" memberName="filterTwoFrequencySlider"
          virtualName="Slider" explicitFocusOrder="0" pos="200 146 50 50"
          min="0" max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="filterTwoResonance" id="d08caa925953a4fc" memberName="filterTwoResonanceSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="264 146 50 50"
          min="0" max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <GENERICCOMPONENT name="filterDisplay" id="edc4cb916977ee50" memberName="filterDisplay"
                    virtualName="FilterDisplay" explicitFocusOrder="0" pos="25 24 295 104"
                    class="Component" params="&quot;fdis&quot;"/>
  <COMBOBOX name="filterOneType" id="3c016a507f2ddb0b" memberName="filterOneTypeComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="25 128 55 16"
            editable="0" layout="36" items="LPF&#10;HPF&#10;BPF&#10;" textWhenNonSelected="Type"
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="filterOneRollOff" id="48862f03fb86cd01" memberName="filterOneRollOffComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="80 128 55 16"
            editable="0" layout="36" items="12DB&#10;24DB&#10;" textWhenNonSelected="R-Off"
            textWhenNoItems="(no choices)"/>
  <IMAGEBUTTON name="filterToggle" id="9b43f7cd0d304fd2" memberName="filterToggleButton"
               virtualName="ToggleButton" explicitFocusOrder="0" pos="323 0 24 24"
               buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
               state="1"/>
  <COMBOBOX name="filterTwoType" id="a5843f8fa9a567a" memberName="filterTwoTypeComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="208 128 55 16"
            editable="0" layout="36" items="LPF&#10;HPF&#10;BPF&#10;" textWhenNonSelected="Type"
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="filterTwoRollOff" id="57800b9c5ece8ddc" memberName="filterTwoRollOffComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="264 128 55 16"
            editable="0" layout="36" items="12DB&#10;24DB&#10;" textWhenNonSelected="R-Off"
            textWhenNoItems="(no choices)"/>
  <IMAGEBUTTON name="filterOneToggle" id="aa716bba60e2a8a" memberName="filterOneToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="150 152 24 24"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <IMAGEBUTTON name="filterTwoToggle" id="6d0d6cec75cecab1" memberName="filterTwoToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="176 152 24 24"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="filterConfig" id="b81e8bf731f64871" memberName="filterConfigComboBox"
            virtualName="ComboBox" explicitFocusOrder="0" pos="136 131 72 16"
            editable="0" layout="36" items="Series&#10;Parallel&#10;" textWhenNonSelected="Config "
            textWhenNoItems="(no choices)"/>
  <SLIDER name="filterTwoDrive" id="466e20ecdbf0fdfb" memberName="filterTwoDriveSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="232 194 50 50"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="filterTwoPassBand" id="392a07706d86e2cc" memberName="filterTwoPassBandSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="184 202 40 40"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="filterTwoVolumeSlider" id="2eae2fccee403e79" memberName="filterTwoVolumeSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="288 202 40 40"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="filterOneFrequency" id="eb6dc73ad070a836" memberName="filterOneFrequencySlider"
          virtualName="Slider" explicitFocusOrder="0" pos="31 146 50 50"
          min="0" max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="filterOneResonance" id="d0eb13db4b00689" memberName="filterOneResonanceSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="95 146 50 50"
          min="0" max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="filterOneDrive" id="e4bc13dcbce0b883" memberName="filterOneDriveSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="63 194 50 50"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="filterOnePassBand" id="18f95541135f076d" memberName="filterOnePassBandSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="15 202 40 40"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="filterOneVolumeSlider" id="1ee15ffeba899d53" memberName="filterOneVolumeSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="119 202 40 40"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
