/*
  ==============================================================================

    PresetTable.cpp
    Created: 27 Apr 2018 10:06:13pm
    Author:  mario

  ==============================================================================
*/

#include "PresetTable.h"
#include <algorithm>


using std::vector;
using std::sort;
using akateko::PresetRow;


PresetTable::PresetTable(vector<PresetRow> presets) :
    numRows(16),
    cellWidth(0.f),
    cellHeight(0.f),
    activeRow(-1),
    currentPresets(presets),
    outlineColour(Colour(0xFF70C099)),
    highLightColour(Colour(0x6F20AA9A))
{
    if(!currentPresets.empty()){
        numRows = currentPresets.size();
    }

    sortPresets(nameSortAscending);
    setColour(backgroundColourId, Colours::black);
}

PresetTable::~PresetTable(){

}

void PresetTable::setLookAndFeel(LookAndFeel *laf){
    getHeader().setLookAndFeel(laf);
}

void PresetTable::initialiseHeader(float width, float height){
    cellWidth = width*0.3333;
    cellHeight = height*0.085;

    getHeader().addColumn("Preset", 1, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("Category", 2, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1 );
    getHeader().addColumn("Author", 3, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);

    numColums = getHeader().getNumColumns(false);
    getHeader().setStretchToFitActive(true);
    getHeader().setPopupMenuActive(false);

    getViewport()->setScrollBarsShown(false,false,true,false); //enable mouse scrolling for now

    setHeaderHeight(cellHeight);
    setRowHeight(cellHeight);
    setModel(this);
}

int PresetTable::getNumRows(){
    return numRows;
}

void PresetTable::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected){
    if(rowNumber < numRows){
         Colour colour = Colours::black;

         if(rowIsSelected){
             colour = (highLightColour);
         }

         g.fillAll( colour );
         // draw the cell bottom divider beween rows
         g.setColour( Colours::white);
         g.drawLine( 0, height, width, height );
    }
}

void PresetTable::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.setColour(outlineColour);
    g.fillRect(0, height-1, width, 1);

    String tmpCell;

    if(rowNumber < currentPresets.size()){
        switch(columnId){
            case 1:
                tmpCell = currentPresets[rowNumber].name;
                g.fillRect(0, 0, 1, height);
                g.fillRect(width-1, 0, 1, height);
                break;
            case 2:
                tmpCell = currentPresets[rowNumber].category;
                g.fillRect(width-1, 0, 1, height);
                break;
            case 3:
                tmpCell = currentPresets[rowNumber].author;
                g.fillRect(width-1, 0, 1, height);
                break;
        }
    }
    g.drawText(tmpCell, 0, 0, width, height, Justification::centred);
}

void PresetTable::cellClicked(int rowNumber, int columnId, const MouseEvent &){
    activeRow = rowNumber;
}

// Todo if the category or Author colum is double clicked
// Prompt the user with a window for changing
// the category or Author
void PresetTable::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &){
    if(rowNumber < currentPresets.size()){
        cellPosition = getCellPosition(columnId,rowNumber, true);
        activeRow = rowNumber;
        dClickRow = rowNumber;
        dClickCol = columnId;

        getParentComponent()->postCommandMessage(requestTextEditor);
    }
}

void PresetTable::setCurrentPresets(std::vector<akateko::PresetRow> presets){
    currentPresets.clear();
    currentPresets = vector<PresetRow>(presets);
    numRows = currentPresets.size();
}

void PresetTable::changeName(int rowNumber, const String name){
    if(rowNumber < currentPresets.size()){
        currentPresets[rowNumber].name = name;
    }
}

void PresetTable::changeAuthor(int rowNumber, const String author){
    if(rowNumber < currentPresets.size()){
        currentPresets[rowNumber].author = author;
    }
}

void PresetTable::changeCategory(int rowNumber, const String category){
    if(rowNumber < currentPresets.size()){
        currentPresets[rowNumber].category = category;
    }
}



Rectangle<int> PresetTable::getClickedCellPosition(){
    return cellPosition;
}

int PresetTable::getActiveRow(){
    return activeRow;
}

void PresetTable::resetActiveRow(){
    activeRow = -1;
}

int PresetTable::getClickedRow(){
    return dClickRow;
}

int PresetTable::getClickedColumn(){
    return dClickCol;
}

File PresetTable::getFile(int rowNum){
    File tmpFile;

    if(rowNum < currentPresets.size()){
        tmpFile = currentPresets[rowNum].file;
    }

    return tmpFile;
}

String PresetTable::getName(int rowNum){
    String tmpName = String();

    if(rowNum < currentPresets.size()){
        tmpName = currentPresets[rowNum].name;
    }

    return tmpName;
}

bool PresetTable::getPresetRow(int rowNum, PresetRow &result){
    if(rowNum < currentPresets.size()){
        result.file = currentPresets[rowNum].file;
        result.name = currentPresets[rowNum].name;
        result.category = currentPresets[rowNum].category;
        result.author = currentPresets[rowNum].author;

        return true;
    }
    return false;
}

void PresetTable::sortPresets(Sort order){
    switch(order){
        case nameSortAscending:
            sort(currentPresets.begin(), currentPresets.end(), &akateko::nameSortAscending);
            break;
        case nameSortDescending:
            sort(currentPresets.begin(), currentPresets.end(), &akateko::nameSortDescending);
            break;
        case categorySortAscending:
            sort(currentPresets.begin(), currentPresets.end(), &akateko::categorySortAscending);
            break;
        case categorySortDescending:
            sort(currentPresets.begin(), currentPresets.end(), &akateko::categorySortDescending);
            break;
        case authorSortAscending:
            sort(currentPresets.begin(), currentPresets.end(), &akateko::authorSortAscending);
            break;
        case autherSortDescending:
            sort(currentPresets.begin(), currentPresets.end(), &akateko::authorSortDescending);
            break;
    }
}



