/*
  ==============================================================================

    ResonanceOptions.h
    Created: 22 Jun 2018 11:49:18am
    Author:  mario

  ==============================================================================
*/

#ifndef RESONANCEOPTIONS_H_INCLUDED
#define RESONANCEOPTIONS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class ResonanceOptions : public ListBox,
                         public ListBoxModel
{
public:
    ResonanceOptions(AkatekoAudioProcessor &p);
    ~ResonanceOptions();

    int getNumRows() override;

    void paintListBoxItem(int rowNumber,
                          Graphics &g,
                          int width,
                          int height,
                          bool rowIsSelected) override;

    void listBoxItemClicked(int row,
                            const MouseEvent &e) override;

    void setSelectedRow(int row);
private:
   AkatekoAudioProcessor &processor;

   ScopedPointer<Label> header;
   StringArray options;

   Colour textColour;
   Colour outlineColour;
   Colour highLightColour;

   int selectedRow;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonanceOptions)
}; //ResonanceOptions


#endif  // RESONANCEOPTIONS_H_INCLUDED
