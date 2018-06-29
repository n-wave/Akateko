/*
  ==============================================================================

    PresetTable.h
    Created: 27 Apr 2018 10:06:13pm
    Author:  mario

  ==============================================================================
*/

#ifndef PRESETTABLE_H_INCLUDED
#define PRESETTABLE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "Akateko.h"
#include  <vector>

class PresetTable : public TableListBox,
                    public TableListBoxModel
{
public:
    PresetTable(std::vector<akateko::PresetRow> presets);
    ~PresetTable();

    void setLookAndFeel(LookAndFeel *laf);
    void initialiseHeader(float width, float height);

    int getNumRows() override;

    void paintRowBackground(Graphics &,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(Graphics &,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &) override;
    void cellClicked(int rowNumber, int columnId, const MouseEvent &) override;

    void setCurrentPresets(std::vector<akateko::PresetRow> presets);

    /* Call when the preset has been succesfully changes
     * Instead of loading up all the presets from memory
     */

    void changeName(int rowNumber, const String name);
    void changeAuthor(int rowNumber, const String author);
    void changeCategory(int rowNumber, const String category);

    Rectangle<int>getClickedCellPosition();
    int getActiveRow();
    void resetActiveRow();

    int getClickedRow();
    int getClickedColumn();

    // Return File to be saved
    File getFile(int rowNum);
    String getName(int rowNum);

    bool getPresetRow(int rowNum, akateko::PresetRow &result);

    enum CommandIds {
        requestTextEditor
    };

    // Sort preset Rows

    enum Sort {
        nameSortAscending,
        nameSortDescending,
        categorySortAscending,
        categorySortDescending,
        authorSortAscending,
        autherSortDescending
    };

    void sortPresets(Sort order);

private:
    std::vector<akateko::PresetRow> currentPresets;

    Rectangle<int> cellPosition;

    int numRows;
    int numColums;

    int activeRow;
    int dClickRow;
    int dClickCol;

    float cellWidth;
    float cellHeight;

    Colour outlineColour;
    Colour highLightColour;

    CustomLookAndFeel laf;
}; // PresetTable

#endif  // PRESETTABLE_H_INCLUDED
