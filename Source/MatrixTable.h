/*
  ==============================================================================

    MatrixTable.h
    Created: 22 Feb 2018 3:26:17pm
    Author:  mario

  ==============================================================================
*/

#ifndef MATRIXTABLE_H_INCLUDED
#define MATRIXTABLE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "/work/programming-projects/msm/msm-dsp/Common/msm.h"
#include "AkatekoMatrix.h"
#include <vector>

//==============================================================================

class MatrixTable    : public TableListBox,
                       public TableListBoxModel
{
public:
    MatrixTable(ModulationMatrix &modm);
    ~MatrixTable();

    void setLookAndFeel(LookAndFeel *laf);

    msm::MatrixRow getMatrixRow(int id);
    void updateGui();

    int getNumRows() override;
    void listWasScrolled() override;
    void paintOverChildren(Graphics &g) override;

    void cellClicked(int rowNumber,
                     int columnId,
                     const MouseEvent &event) override;


    void paintRowBackground (Graphics &g,
                             int rowNumber,
                             int width, int height,
                             bool rowIsSelected) override;

    void paintCell(Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width, int height,
                   bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate) override;

    void initialiseHeader(float width, float height);
    void setSources(StringArray &src);
    void setDestinations(StringArray &dst);
    void setData(int rowNumber, int columnId, float value);

    void getRowAndColumn(int &activeRow, int &column);

    /* id concides with the row,
     * double check to be sure
     */
    int getSource(int id);
    int getTransform(int id);
    double getRange(int id);
    double getIntensity(int id);
    int getDestination(int id);
    bool getEnabled(int id);

    void setEffectsDestination(int fxProcessor, int effect);
private:        
    void sourceMenu(int rowNumber);
    void transformMenu(int rowNumber);
    void rangeMenu(int rowNumber);
    void destinationMenu(int rowNumber);
    void initialiseSubMenus();

    void initialiseStereoDelayMenu(PopupMenu &menu);
    void initialisePingPongDelayMenu(PopupMenu &menu);
    void initialiseLCRDelayMenu(PopupMenu &menu);
    void initialiseStereoFlangerMenu(PopupMenu &menu);
    void initialiseDimensionChorusMenu(PopupMenu &menu);
    void initialiseHoldDelayMenu(PopupMenu &menu);
    void initialiseDecimatorMenu(PopupMenu &menu);
    void initialisePlateReverb(PopupMenu &menu);

    ModulationMatrix &modMatrix;
    std::vector<msm::MatrixRow> rows;
    std::vector<bool> rowIsActive;

    int numRows;
    int numColumn;

    /* active cell */
    int activeRow;
    int activeColumn;

    float cellWidth;
    float cellHeight;

    /* Hard Code for now */
    StringArray sources;
    StringArray transform;
    StringArray destination;

    int fxOneStartIndex;
    int fxTwoStartIndex;

    /* Colours */
    Colour textColour;
    Colour outlineColour;

    ScopedPointer<PopupMenu> menu;

    // DSP Signal Destination Menus
    PopupMenu filterOneMenu;
    PopupMenu filterTwoMenu;
    PopupMenu waveshaperMenu;
    // Modulation PopupMenu
    PopupMenu lfoOne;
    PopupMenu lfoTwo;
    PopupMenu envOne;
    PopupMenu envTwo;
    PopupMenu sseq;

    //Fx Menu
    PopupMenu effectOneMenu;
    PopupMenu effectTwoMenu;

    StringArray fxOneDestinations;
    StringArray fxTwoDestinations;


    CustomLookAndFeel laf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixTable)
};


#endif  // MATRIXTABLE_H_INCLUDED

