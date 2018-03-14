/*
  ==============================================================================

    MatrixTable.cpp
    Created: 22 Feb 2018 3:26:17pm
    Author:  mario

  ==============================================================================
*/

#include "MatrixTable.h"
#include "/work/programming-projects/msm/msm-gui/NumberBox.h"
#include "Akateko.h"

using std::vector;
using msm::MatrixRow;

using namespace akateko;

//==============================================================================
MatrixTable::MatrixTable(ModulationMatrix &modm) :
    modMatrix(modm),
    numRows(16),
    activeRow(-1),
    activeColumn(-1),
    cellWidth(0.f),
    cellHeight(0.f),
    textColour(Colours::lightgrey)
{   
    rowIsActive.resize(numRows, false);

    for(int i=0; i<numRows; i++){
        MatrixRow tmpRow = MatrixRow();

        tmpRow.id = i; //Order added
        tmpRow.source = -1; // -1 Not set ignore
        tmpRow.transform = -1;
        tmpRow.intensity = 1.0;
        tmpRow.range = 1.0;
        tmpRow.destination = -1; // -1 Not set ignore
        tmpRow.on = 0; //On off

        rows.push_back(tmpRow);
    }

    //Gather variables supplied modMatrix;
    vector<MatrixRow> tmpRows = modMatrix.getRows();

    if(!tmpRows.empty()){
        for(int i=0; i<tmpRows.size(); i++){
            rows[tmpRows[i].id] = tmpRows[i];
            rowIsActive[tmpRows[i].id] = true;
        }
    }


    sources.add("LFO One");
    sources.add("LFO Two");
    sources.add("ENV One");
    sources.add("ENV Two");
    sources.add("SSEQ");
    sources.add("PAD X");
    sources.add("PAD Y");

    transform.add("unipolar");
    transform.add("bipolar");

    destination.add("PRE Drive");           // Index 0
    destination.add("PRE Amount");          // Index 1
    destination.add("F1 Frequency");        // Index 2
    destination.add("F1 Resonance");        // Index 3
    destination.add("F2 Frequency");        // Index 4
    destination.add("F2 Resonance");        // Index 5
    destination.add("WS Drive");            // Index 6
    destination.add("WS Mix");              // Index 7
    destination.add("WS Amplitude");        // Index 8
    destination.add("WS Routing");          // Index 9
    destination.add("LFO1 Frequency");      // Index 10
    destination.add("LFO1 Phase");          // Index 11
    destination.add("LFO1 PWM");            // Index 12
    destination.add("LFO2 Frequency");      // Index 13
    destination.add("LFO2 Phase");          // Index 14
    destination.add("LFO2 PWM");            // Index 15
    destination.add("ENV1 Duration");       // Index 16
    destination.add("ENV2 Duration");       // Index 17
    destination.add("SSEQ duration");       // Index 18
    destination.add("SSEQ Length");         // Index 19
    destination.add("SSEQ Offset");         // Index 20
    destination.add("SSEQ Curve");          // Index 21

    this->initialiseSubMenus();
}

MatrixTable::~MatrixTable()
{
    for(int i= 0; i<numRows; i++){
        NumberBox *tmpIntensity = static_cast<NumberBox*>(getCellComponent(3, i));
        NumberBox *tmpRange = static_cast<NumberBox*>(getCellComponent(4, i));

        if(tmpIntensity != nullptr){
            tmpIntensity = nullptr;
        }

        if(tmpRange != nullptr){
            tmpRange = nullptr;
        }

    }
}

int MatrixTable::getNumRows(){
    return numRows;
}

MatrixRow MatrixTable::getMatrixRow(int id){
    if(id >= 0 && id < numRows){
       return rows[id];
    }

    MatrixRow m;
    m.id = -1; //check for minus 1 in caller

    return m;
}

void MatrixTable::initialiseHeader(float width, float height){
    cellWidth = width*0.1925;
    float widthTwo = width*0.05;
    cellHeight = height*0.085;

    getHeader().addColumn("Source", 1, cellWidth , cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("Transform", 2, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("Intensity", 3, cellWidth , cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("Range", 4, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1 );
    getHeader().addColumn("Destination", 5, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("On", 6, widthTwo, widthTwo, -1, TableHeaderComponent::visible, -1);

    numColumn = getHeader().getNumColumns(false);
    getHeader().setStretchToFitActive(false);
    getHeader().setPopupMenuActive(false);

    /* set View property's before it is added */

    getViewport()->setScrollBarsShown(false,false,true,false); //enable mouse scrolling for now

    setHeaderHeight(cellHeight);
    setRowHeight(cellHeight);


    setModel(this);

}
/* Paint Overlay, takes care of the divising of the cell blocks */

void MatrixTable::paintOverChildren(Graphics &g){
    int tmpWidth = getWidth();
    int tmpHeight = getHeight();


    g.setColour(Colours::white.withAlpha(0.5f));

    g.drawRect(0 , 0, tmpWidth, tmpHeight, 1.0);

    for(int i=1; i<=5; i++){
        tmpWidth = cellWidth*i-1;
        g.fillRect(tmpWidth, 0, 1, tmpHeight);
    }
}


void MatrixTable::paintRowBackground(Graphics &g,
                                     int rowNumber,
                                     int width,
                                     int height,
                                     bool rowIsSelected)
{
   if(rowNumber < numRows){
        Colour colour = findColour(TableListBox::backgroundColourId);

        if(rowIsSelected){
            g.setColour(Colours::darkgrey);
        }

        g.fillAll( colour );
        // draw the cell bottom divider beween rows
        g.setColour( Colours::white);
        g.drawLine( 0, height, width, height );
   }
}


void MatrixTable::paintCell(Graphics &g,
                            int rowNumber,
                            int columnId,
                            int width,
                            int height,
                            bool rowIsSelected)
{
    Colour colour = findColour(TableListBox::backgroundColourId);


    if(activeRow == rowNumber &&
         activeColumn == columnId)
    {
        g.setColour(Colours::lightgrey.withAlpha(0.25f));
        g.fillRect(0,0,width, height);
    }

    if(rowNumber < numRows){
        g.setColour(textColour);
        g.setFont(15.f);

        if(rows[rowNumber].source != -1){
            int si = rows[rowNumber].source;
            int ti = rows[rowNumber].transform; //transform = 0

            if(columnId == 1 && si < sources.size()){
                g.drawText(sources[si], 0, 0, width, height, Justification::centred);
            } else if(columnId == 2 && ti < transform.size()){

                if(ti == TRANSFORM::NONE){
                    if(si >= MODSRC::ENV1 && si <= MODSRC::SSEQ){
                        ti =0;
                    } else if(si >= MODSRC::LFO1 && si <= MODSRC::LFO2){
                        ti = 1;
                    }
                }
                     g.drawText(transform[ti], 0, 0, width, height, Justification::centred);
            }
        }

        if(rows[rowNumber].destination != -1 && columnId == 5){
            int di = rows[rowNumber].destination;
            g.drawText(destination[di], 0, 0, width, height, Justification::centred);
        }


        if(rows[rowNumber].source != -1 && rows[rowNumber].destination != -1 && columnId == 6){
            if(rows[rowNumber].on){
                float tmpWidth = height*0.65;
                float tmpHeight = height*0.60;
                float xOffset = width*0.14;
                float yOffset = height*0.2;

                g.setColour(textColour);
                g.fillRoundedRectangle(xOffset, yOffset, tmpWidth, tmpHeight, 2.0);
            }
        }

     //   g.setColour( Colours::white);
     //   g.drawLine( width, 0, width, height );
    }
}

void MatrixTable::cellClicked(int rowNumber,
                              int columnId,
                              const MouseEvent &event)
{
    ModifierKeys modifier = event.mods;

    activeRow = rowNumber;
    activeColumn = columnId;


    if(modifier.isRightButtonDown()){
        if(activeColumn == 1){
            sourceMenu(rowNumber);
        } else if(activeColumn == 2){
            transformMenu(rowNumber);
        } else if(activeColumn == 5){
            destinationMenu(rowNumber);
        } else if(activeColumn == 6){
            if(rows[rowNumber].source != -1 &&
                rows[rowNumber].destination != -1)
            {
                rows[rowNumber].on = !(rows[rowNumber].on);
                modMatrix.setEnabled(rows[rowNumber].on, activeRow);
            }
        }
    }

    updateContent();
    this->repaint();

}

Component* MatrixTable::refreshComponentForCell(int rowNumber,
                                                int columnId,
                                                bool isRowSelected,
                                                Component *existingComponentToUpdate)
{

    if((columnId == 3 || columnId == 4)){
            NumberBox *numberBox = static_cast<NumberBox*>(existingComponentToUpdate);

            if(numberBox == nullptr){
                numberBox = new NumberBox(*this, String(rowNumber), string(0.f, 2));

                numberBox->setRowAndColumn(rows[rowNumber].id, columnId);
                numberBox->setColour(Label::textColourId, Colours::transparentBlack); // change at later point
                numberBox->setSize(2, 10);

                if(columnId == 3){
                    const float tmpIntensity = rows[rowNumber].intensity;
                    numberBox->setText(String(tmpIntensity, 2), NotificationType::dontSendNotification);

                    if(rows[rowNumber].source != -1){
                         numberBox->setColour(Label::textColourId, textColour);
                    }
                    numberBox->setMaxValue(1.0);
                    numberBox->setMinValue(-1.0);
                } else if(columnId == 4){
                    const float tmpRange = rows[rowNumber].range;
                    numberBox->setText(String(tmpRange, 2), NotificationType::dontSendNotification);

                    if(rows[rowNumber].destination != -1){
                        numberBox->setColour(Label::textColourId, textColour); // change at later point
                    }
                    numberBox->setMaxValue(1.0);
                    numberBox->setMinValue(0.0);
                }


                numberBox->setColour(Label::backgroundColourId, Colour(0x00000000));
                numberBox->setColour(Label::outlineColourId, Colour(0x00000000));
                numberBox->setJustificationType(Justification::centred);

        } else {
            /* if exist update the value in the
             * box based on the matrix row data
             */

                int rowId = numberBox->getRowId();

                /* Have to create a new set of numberBoxes for the last rows
                 * The tableListBox only creates and supplies components
                 * currently visible, it will resupply the previous numberboxes
                 * binded to the lower range matrix row although the rownumber
                 * is in the upper range, which will have as effect that the
                 * upper row are simultaneously the lower row.
                 *
                 * That is why in this part of the function the NumberBox *numberBox
                 * has to be set to zero and the existing component
                 * deleted and newly created based on the current row number.
                 *
                 * This way the values are stored correctly in the matrix row and
                 * correctly supplied to the new cell component.
                 */

                if(rowId != rowNumber){
                    numberBox = nullptr;
                    delete existingComponentToUpdate;

                    numberBox = new NumberBox(*this, String(rowNumber), string(0.f, 2));
                    numberBox->setRowAndColumn(rowNumber, columnId);
                    numberBox->setColour(Label::outlineColourId, Colour(0x00000000));


                    if(columnId == 3){

                        const float tmpIntensity = rows[rowNumber].intensity;
                        numberBox->setText(String(tmpIntensity, 2), NotificationType::dontSendNotification);

                        if(rows[rowNumber].source != -1){
                           numberBox->setColour(Label::textColourId, textColour);
                        }
                        numberBox->setMaxValue(1.0);
                        numberBox->setMinValue(-1.0);
                    } else if(columnId == 4){
                        const float tmpRange = rows[rowNumber].range;
                        numberBox->setText(String(tmpRange, 2), NotificationType::dontSendNotification);

                        if(rows[rowNumber].destination != -1){
                            numberBox->setColour(Label::textColourId, textColour); // change at later point
                        }
                        numberBox->setMaxValue(1.0);
                        numberBox->setMinValue(0.0);
                    }

                    numberBox->setColour(Label::backgroundColourId, Colour(0x00000000));
                    numberBox->setColour(Label::textColourId, Colours::whitesmoke); // change at later point
                    numberBox->setJustificationType(Justification::centred);
                } else {
                    /* Row Number and identity's are correct proceed on setting the values */
                     numberBox->setColour(Label::textColourId, Colours::transparentBlack);

                    if(columnId == 3 && rows[rowNumber].id == rowNumber){
                        const float tmpIntensity = rows[rowNumber].intensity;
                        numberBox->setText(String(tmpIntensity, 2), NotificationType::dontSendNotification);

                        if(rows[rowNumber].source != -1){
                            numberBox->setColour(Label::textColourId, textColour);
                        }
                     }  else if(columnId == 4){
                        const float tmpRange = rows[rowNumber].range;
                        numberBox->setText(String(tmpRange, 2), NotificationType::dontSendNotification);

                        if(rows[rowNumber].destination != -1){
                            numberBox->setColour(Label::textColourId, textColour); // change at later point
                        }
                    }
                }
        }

        return numberBox;

    } else {
        return nullptr;
     }
}

void MatrixTable::listWasScrolled() {
    updateContent();
}

void MatrixTable::setData(int rowNumber, int columnId, float value){
    if(rowNumber >= 0 && rowNumber < rows.size())
    {
        activeRow = rowNumber;
        activeColumn = columnId;

        if(columnId == 3){
            rows[rowNumber].intensity = value;
        } else if(columnId == 4){
            rows[rowNumber].range = value;
        }

        if(rowIsActive[rowNumber]){
            if(columnId == 3){
                modMatrix.setIntensity(rows[rowNumber].intensity,  rows[rowNumber].id);
            } else if(columnId == 4){
                modMatrix.setRange(rows[rowNumber].range,  rows[rowNumber].id);
            }
        }
    }
}

void MatrixTable::getRowAndColumn(int &row, int &column){
    row = activeRow;
    column = activeColumn;
}

/* row id should concide with the rowNumber
 * check nonetheless
 */

int MatrixTable::getSource(int id){
    int result = -1;

    if(id >= 0 && id < numRows){
        if(rows[id].id == id){
            result = rows[id].source;
        }
    }
    return result;
}

int MatrixTable::getTransform(int id){
    int result = -1;

    if(id >= 0 && id < numRows){
        if(rows[id].id == id){
            result = rows[id].transform;
        }
    }
    return result;
}

double MatrixTable::getRange(int id){
    double result;

    if(id >= 0 && id < numRows){
        if(rows[id].id == id){
            result = rows[id].range;
        }
    }
    return result;
}


double MatrixTable::getIntensity(int id){
    double result;

    if(id >= 0 && id < numRows){
        if(rows[id].id == id){
            result = rows[id].intensity;
        }
    }
    return result;
}

int MatrixTable::getDestination(int id){
    int result = -1;

    if(id >= 0 && id < numRows){
        if(rows[id].id == id){
            result = rows[id].destination;
        }
    }
    return result;
}

bool MatrixTable::getEnabled(int id){
    bool result = false;

    if(id >= 0 && id < numRows){
        result = (rows[id].on == 1) ? true : false;
    }
    return result;
}

void MatrixTable::sourceMenu(int rowNumber){
    menu = new PopupMenu();

    int i;

    for(i=0; i<sources.size(); i++){
        menu->addItem(i+1, sources[i], true, false, nullptr);
    }

    menu->addSeparator();
    menu->addItem(0xFF, "clear", true, false, nullptr);

    int index = menu->show()-1;

    /* later concoct a method for filling up the
     * row with std setting for transformations
     * etc */
    if(index >= 0 && index < sources.size()){
        rows[rowNumber].source = index;

        /* elaborate and check for multiple targets */

        rows[rowNumber].transform = TRANSFORM::NONE;
        std::cout << "source changed printing transform" << std::endl;

        if(rows[rowNumber].destination != -1){
            if(!rowIsActive[rowNumber]){
                rowIsActive[rowNumber] = true;
                rows[rowNumber].on = 1;
                //getParentComponent()->postCommandMessage(RowAddedId);
                modMatrix.addRow(rows[rowNumber]);
            } else {
               // getParentComponent()->postCommandMessage(SourceChangedId);
                modMatrix.setSource(rows[rowNumber].source, rows[rowNumber].id);
            }
        }
    }

    if(index+1 == 0xFF){
        rows[rowNumber].source = -1;

        if(rowIsActive[rowNumber]){
            rowIsActive[rowNumber] = false;
            //getParentComponent()->postCommandMessage(RowRemovedId);
            modMatrix.removeRow(rows[rowNumber].id);
        }
    }

    this->repaint();
}


void MatrixTable::transformMenu(int rowNumber){
    /* Check if a sources is present */
    if(rows[rowNumber].source != -1){

        /* yes create a popup menu based on
         * the source i.e. midi input has
         * a different set of transformations
         *
         *
         *  transform.add("unipolar"); index 0
         *  transform.add("bipolar");  index 1
         */

        menu = new PopupMenu();

        for(int i=0; i<transform.size(); i++){
            menu->addItem(i+1, transform[i], true, false, nullptr);          
        }

        int index = menu->show() -1;
        std::cout << "transform index : " << index << std::endl;

        if(index >= 0 && index <= 1){
            int source  = rows[rowNumber].source;
            int transform = index;

            std::cout << "printing source : " << source << std::endl;
            /* Better to check everything in the gui implmentation
             * than in the ModulationMatrix update function
             * in the process loop
             */
            if(source >= MODSRC::LFO1 && source <= MODSRC::LFO2 && index == 1){
                transform = TRANSFORM::NONE;
            }

            else if(source >= MODSRC::ENV1 && source <= MODSRC::SSEQ && index == 0){
                transform = TRANSFORM::NONE;
            }

            rows[rowNumber].transform = transform;

            if(rowIsActive[rowNumber]){
                //getParentComponent()->postCommandMessage(TransformChangedId);
                modMatrix.setTransform(rows[rowNumber].transform, rows[rowNumber].id);
            }
        }
    }
    this->repaint();
}

void MatrixTable::destinationMenu(int rowNumber){
    menu = new PopupMenu();

    menu->addSubMenu("Preamp", preAmpMenu);
    menu->addSubMenu("Filter One", filterOneMenu);
    menu->addSubMenu("Filter Two", filterTwoMenu);
    menu->addSubMenu("Waveshaper", waveshaperMenu);
    menu->addSeparator();
    menu->addSubMenu("LFO One", lfoOne);
    menu->addSubMenu("LFO Two", lfoTwo);
    menu->addSubMenu("Env One", envOne);
    menu->addSubMenu("Env Two", envTwo);
    menu->addSubMenu("Step seq", sseq);
    menu->addSeparator();
    menu->addItem(0xFF, "clear", true, false, nullptr);


    const int index = menu->show() -1;

    if(index >= 0 && index < destination.size()){
        rows[rowNumber].destination = index;

        if(rows[rowNumber].source != -1){
            if(!rowIsActive[rowNumber]){
                rows[rowNumber].on = 1;  //set Row to active
                rowIsActive[rowNumber] = true;

                //getParentComponent()->postCommandMessage(RowAddedId);
                modMatrix.addRow(rows[rowNumber]);
            } else {
                //getParentComponent()->postCommandMessage(DestinationChangedId);
                modMatrix.setDestination(rows[rowNumber].destination, rows[rowNumber].id);
            }
        }
    }

    if(index+1 == 0xFF){
        rows[rowNumber].destination = -1;

        if(rowIsActive[rowNumber]){
            rowIsActive[rowNumber] = false;
            //getParentComponent()->postCommandMessage(RowRemovedId);
            modMatrix.removeRow(rows[rowNumber].id);
        }
    }

    this->repaint();
}

/* Test Menu's for now at a later point, Make the effect Menu's configurable
 * by a setter, if a flexible effect section is implmented. Also keep in mind
 * that the modulation destinations have a fixed index.
 *
 */

void MatrixTable::initialiseSubMenus(){
    // Preamp Filter section
    preAmpMenu.addItem(1, "Drive", true, false, nullptr);
    preAmpMenu.addItem(2, "Amount", true, false, nullptr);

    // Filter one Menu
    filterOneMenu.addItem(3, "Frequency", true, false, nullptr);
    filterOneMenu.addItem(4, "Resonance", true, false, nullptr);

    // Filter Two Menu
    filterTwoMenu.addItem(5, "Frequency", true, false, nullptr);
    filterTwoMenu.addItem(6, "Resonance", true, false, nullptr);

    // WavevShaper Menu
    waveshaperMenu.addItem(7, "Drive", true, false, nullptr);
    waveshaperMenu.addItem(8, "Mix", true, false, nullptr);
    waveshaperMenu.addItem(9, "Amplitude", true,false, nullptr);
    waveshaperMenu.addItem(10, "FX Routing", true, false, nullptr);
    //Low Frequency Osc Menu One
    lfoOne.addItem(11, "Frequency", true, false, nullptr);
    lfoOne.addItem(12, "Phase", true, false, nullptr);
    lfoOne.addItem(13, "PWM", true, false, nullptr);
    //Low Frequency Osc Menu Two
    lfoTwo.addItem(14, "Frequency", true, false, nullptr);
    lfoTwo.addItem(15, "Phase", true, false, nullptr);
    lfoTwo.addItem(16, "PWM", true, false, nullptr);
    //Envelopes
    envOne.addItem(17, "Duration", true, false, nullptr);
    envTwo.addItem(18, "Duration", true, false, nullptr);
    //Step Sequencer
    sseq.addItem(19, "Duration", true, false, nullptr);
    sseq.addItem(20, "Length", true, false, nullptr);
    sseq.addItem(21, "Off value", true, false, nullptr);
    sseq.addItem(22, "Curve", true, false, nullptr);

    // FX Menus
    effectOneMenu.addItem(10, "none", true, false, nullptr);
    effectTwoMenu.addItem(11, "none", true, false, nullptr);
}


