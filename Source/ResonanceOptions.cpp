/*
  ==============================================================================

    ResonanceOptions.cpp
    Created: 22 Jun 2018 11:49:18am
    Author:  mario

  ==============================================================================
*/

#include "ResonanceOptions.h"

ResonanceOptions::ResonanceOptions(AkatekoAudioProcessor &p) :
    textColour(Colour(0xFF70C099)),
    outlineColour(Colour(0xFF409069)),
    highLightColour(Colour(0x6F20AA9A)),
    selectedRow(2),
    processor(p)
{
    options.clear();
    options.add("I'm too young to die");
    options.add("Hey, not too rough ");
    options.add("Hurt me plenty");
    options.add("Ultra violence");
    options.add("Nightmare");

    setColour(ListBox::backgroundColourId, Colours::black);

    selectedRow = processor.getFilterResonance();

    setModel(this);
}

ResonanceOptions::~ResonanceOptions(){
    header = nullptr;
}

int ResonanceOptions::getNumRows(){
    return options.size();
}

void ResonanceOptions::paintListBoxItem(int rowNumber,
                                        Graphics &g,
                                        int width,
                                        int height,
                                        bool rowIsSelected)
{
    g.setColour(Colours::black);
    g.fillAll();

    if(rowNumber == selectedRow){
        g.setColour(highLightColour);
        g.fillRect(0 , 0, width, height);
    }

    g.setColour(outlineColour);

    // Outline drawing
    g.fillRect(0, 0, width, 1);
    g.fillRect(0, 0, 1, height);
    g.fillRect(width-1, 0, 1, height);

    if(rowNumber == options.size()-1){
        g.fillRect(0, height-1, width, 1);
    }

    g.setColour(textColour);
    if(rowNumber < options.size()){
        g.drawText(options[rowNumber], 0, 0, width, height, Justification::centred);
    }
}

void ResonanceOptions::listBoxItemClicked(int row,
                                          const MouseEvent &e)
{
    if(e.mods.isAnyMouseButtonDown()){
        selectedRow = row;

        double scalar = 0;

        switch(row){
            case 0: scalar = 2.0; break;
            case 1: scalar = 2.5; break;
            case 2: scalar = 3.0; break;
            case 3: scalar = 3.5; break;
            case 4: scalar = 4.0; break;
        }

        processor.storeConfigurationOption(String(selectedRow), AkatekoAudioProcessor::FilterResonanceId);

        if(scalar != 0){
            processor.updateResonanceScalar(scalar);
        }
    }
    repaint();
}

void ResonanceOptions::setSelectedRow(int row){
    selectedRow = row;

    repaint();
}
