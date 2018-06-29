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
    fxOneStartIndex(22),
    fxTwoStartIndex(23),
    textColour(Colour(0xFF4A997A)),
    outlineColour(Colour(0xFF70C099))
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
    sources.add("PAD T");

    transform.add("unipolar");
    transform.add("bipolar");

    destination.add("F1 Freq");             // Index 0
    destination.add("F1 Reso");             // Index 1
    destination.add("F1 P-Band");           // Index 2
    destination.add("F1 Drive");            // Index 3
    destination.add("F1 Volume");           // Index 4

    destination.add("F2 Freq");             // Index 5
    destination.add("F2 Reso");             // Index 6
    destination.add("F2 P-Band");           // Index 7
    destination.add("F2 Drive");            // Index 8
    destination.add("F2 Volume");           // Index 9

    destination.add("WS Drive");            // Index 10
    destination.add("WS Mix");              // Index 11

    destination.add("LFO1 Freq");           // Index 12
    destination.add("LFO1 Phase");          // Index 13
    destination.add("LFO1 PWM");            // Index 14
    destination.add("LFO2 Freq");           // Index 15
    destination.add("LFO2 Phase");          // Index 16
    destination.add("LFO2 PWM");            // Index 17

    destination.add("ENV1 Dura");           // Index 18
    destination.add("ENV2 Dura");           // Index 19

    destination.add("SSEQ Dura");           // Index 20
    destination.add("SSEQ Length");         // Index 21
    destination.add("SSEQ Offset");         // Index 22
    destination.add("SDEL Cross");          // Index 23

    destination.add("SDEL Mix");            // Index 24
    destination.add("SDEL L-Delay");        // Index 25
    destination.add("SDEL L-FB");           // Index 26
    destination.add("SDEL R-Delay");        // Index 27
    destination.add("SDEL R-FB");           // Index 28

    destination.add("PP L-Delay");          // Index 29
    destination.add("PP L-FB");             // Index 30
    destination.add("PP L-Mix");            // Index 31
    destination.add("PP R-Delay");          // Index 32
    destination.add("PP R-FN");             // Index 33
    destination.add("PP R-Mix");            // Index 34

    destination.add("LCR L-Delay");         // Index 35
    destination.add("LCR L-FB");            // Index 36
    destination.add("LCR L-Mix");           // Index 37
    destination.add("LCR C-Delay");         // Index 38
    destination.add("LCR C-FB");            // Index 39
    destination.add("LCR HPF F");           // Index 40
    destination.add("LCR LPF F");           // Index 41
    destination.add("LCR Amp");             // Index 42
    destination.add("LCR Pan");             // Index 43
    destination.add("LCR R-Delay");         // Index 44
    destination.add("LCR R-FB");            // Index 45
    destination.add("LCR R-Mix");           // Index 46
    destination.add("SFL Rate");            // Index 47
    destination.add("SFL Phase");           // Index 48
    destination.add("SFL Depth");           // Index 49
    destination.add("SFL FB");              // Index 50
    destination.add("DCH Rate");            // Index 51
    destination.add("DCH Spread");          // Index 52
    destination.add("DCH Depth");           // Index 53
    destination.add("DCH Freq");            // Index 54
    destination.add("DCH C-Amp");           // Index 55
    destination.add("DCH C-Pan");           // Index 56
    destination.add("DCH Mix");             // Index 57
    destination.add("HDEL Speed");          // Index 58
    destination.add("HDEL Length");         // Index 59
    destination.add("HDEL Fade");           // Index 60
    destination.add("HDEL Pan");            // Index 61
    destination.add("HDEL Mix");            // Index 62
    destination.add("DEC B-Fract");         // Index 63
    destination.add("DEC S-Rate");          // Index 64
    destination.add("DEC Mix");             // Index 65
    destination.add("PREV B-Width");        // Index 66
    destination.add("PREV Damping");        // Index 67
    destination.add("PREV Decay");          // Index 68
    destination.add("PREV Mix");            // Index 69

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

void MatrixTable::updateGui(){
    for(int i= 0; i<numRows; i++){
        rows[i].source = -1;
        rows[i].transform = -1;
        rows[i].intensity = 1.0;
        rows[i].range = 1.0;
        rows[i].destination = -1;
        rows[i].on = 0;
        rowIsActive[i] = false;
    }

    vector<MatrixRow> tmpRows = modMatrix.getRows();

    if(!tmpRows.empty()){
        for(int i=0; i<tmpRows.size(); i++){
            const int index = tmpRows[i].id;

            rows[index] = tmpRows[i];
            rowIsActive[index] = true;
        }
    }
    updateContent();
    this->repaint();
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
    cellWidth = width*0.1923;
    float widthTwo = width*0.04;
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
    getHeader().setLookAndFeel(&laf);

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


    g.setColour(outlineColour);

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
            g.setColour(Colours::white);
        }

        g.fillAll( colour );
        // draw the cell bottom divider beween rows
        g.setColour(outlineColour);
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
                    if(si >= MODSRC::ENV1 && si <= MODSRC::SSEQ ||
                       si >= MODSRC::PADX && si <= MODSRC::PADT){
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
    repaint();
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

                    // Set range based on destination
                    const int dest = rows[rowNumber].destination;

                   // std::cout << "Printing out Destintion" << dest << std::endl;

                    if(dest == SDLDEL || dest == SDRDEL || dest == PPLDEL || dest == PPRDEL ||
                       dest == LCRLDEL || dest == LCRCDEL || dest == LCRRDEL)
                    {
                        numberBox->setInterval(2.5);
                        numberBox->setMaxValue(500.0);
                        numberBox->setMinValue(0.0);
                    } else {
                        const float tmpValue = numberBox->getValue();

                        if(tmpValue > 1.0){
                            numberBox->setValue(1.0);
                        }

                        numberBox->setInterval(0.05);
                        numberBox->setMaxValue(1.0);
                        numberBox->setMinValue(0.0);
                    }
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


                        const int dest = rows[rowNumber].destination;

                        if(dest == SDLDEL || dest == SDRDEL || dest == PPLDEL || dest == PPRDEL ||
                           dest == LCRLDEL || dest == LCRCDEL || dest == LCRRDEL)
                        {
                            numberBox->setInterval(2.5);
                            numberBox->setMaxValue(500.0);
                            numberBox->setMinValue(0.0);
                        } else {
                            const float tmpValue = numberBox->getValue();

                            if(tmpValue > 1.0){
                                numberBox->setValue(1.0);     
                            }

                            numberBox->setInterval(0.05);
                            numberBox->setMaxValue(1.0);
                            numberBox->setMinValue(0.0);
                        }
                    }

                    numberBox->setColour(Label::backgroundColourId, Colour(0x00000000));
                    numberBox->setColour(Label::textColourId, textColour); // change at later point
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


                        const int dest = rows[rowNumber].destination;

                        if(dest == SDLDEL || dest == SDRDEL || dest == PPLDEL || dest == PPRDEL ||
                           dest == LCRLDEL || dest == LCRCDEL || dest == LCRRDEL)
                        {
                            numberBox->setInterval(2.5);
                            numberBox->setMaxValue(500.0);
                            numberBox->setMinValue(0.0);
                        } else {
                            const float tmpValue = numberBox->getValue();

                            if(tmpValue > 1.0){
                                numberBox->setValue(1.0);

                            }

                            numberBox->setInterval(0.05);
                            numberBox->setMaxValue(1.0);
                            numberBox->setMinValue(0.0);
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

/*
 *  sources.add("LFO One");
 *  sources.add("LFO Two");
 *  sources.add("ENV One");
 *  sources.add("ENV Two");
 *  sources.add("SSEQ");
 *  sources.add("PAD X");
 *  sources.add("PAD Y");
 *
 */

void MatrixTable::sourceMenu(int rowNumber){
    menu = new PopupMenu();
    menu->setLookAndFeel(&laf);

    menu->addItem(1, "LFO One", true, false, nullptr);
    menu->addItem(2, "LFO Two", true, false, nullptr);
    menu->addItem(3, "Env One", true, false, nullptr);
    menu->addItem(4, "Env Two", true, false, nullptr);
    menu->addItem(5, "SSEQ", true, false, nullptr);
    menu->addSeparator();
    menu->addItem(6, "PAD X", true, false, nullptr);
    menu->addItem(7, "PAD Y", true, false, nullptr);
    menu->addItem(8, "Pad T", true, false, nullptr);

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

        if(rows[rowNumber].destination != -1){
            if(!rowIsActive[rowNumber]){
                rowIsActive[rowNumber] = true;
                rows[rowNumber].on = 1;
                //getParentComponent()->postCommandMessage(RowAddedId);
                modMatrix.addRow(rows[rowNumber]);
            } else {
               // getParentComponent()->postCommandMessage(SourceChangedId);
                std::cout << "MatrixTable::sourceMenu" << rows[rowNumber].id << std::endl;

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
        menu->setLookAndFeel(&laf);

        for(int i=0; i<transform.size(); i++){
            menu->addItem(i+1, transform[i], true, false, nullptr);          
        }

        int index = menu->show() -1;
       // std::cout << "transform index : " << index << std::endl;

        if(index >= 0 && index <= 1){
            int source  = rows[rowNumber].source;
            int transform = index;

            //std::cout << "printing source : " << source << std::endl;
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
    menu->setLookAndFeel(&laf);

    menu->addSubMenu("Filter One", filterOneMenu);
    menu->addSubMenu("Filter Two", filterTwoMenu);
    menu->addSubMenu("Waveshaper", waveshaperMenu);
    menu->addSeparator();
    menu->addSubMenu("LFO One", lfoOne);
    menu->addSubMenu("LFO Two", lfoTwo);
    menu->addSubMenu("Env One", envOne);
    menu->addSubMenu("Env Two", envTwo);
    menu->addSubMenu("Step Seq", sseq);
    menu->addSeparator();
    menu->addSubMenu("FX One", effectOneMenu);
    menu->addSubMenu("FX Two", effectTwoMenu);
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

/*
 * Test Menu's for now at a later point, Make the effect Menu's configurable
 * by a setter, if a flexible effect section is implmented. Also keep in mind
 * that the modulation destinations have a fixed index.
 *
 */

void MatrixTable::initialiseSubMenus(){
    // Filter one Menu
    filterOneMenu.addItem(1, "Frequency", true, false, nullptr);
    filterOneMenu.addItem(2, "Resonance", true, false, nullptr);
    filterOneMenu.addItem(3, "Pass Band", true, false, nullptr);
    filterOneMenu.addItem(4, "Drive", true, false, nullptr);
    filterOneMenu.addItem(5, "Volume", true , false, nullptr);

    // Filter Two Menu
    filterTwoMenu.addItem(6, "Frequency", true, false, nullptr);
    filterTwoMenu.addItem(7, "Resonance", true, false, nullptr);
    filterTwoMenu.addItem(8, "Pass Band", true, false, nullptr);
    filterTwoMenu.addItem(9, "Drive", true, false, nullptr);
    filterTwoMenu.addItem(10, "Volume", true, false, nullptr);

    // WavevShaper Menu
    waveshaperMenu.addItem(11, "Drive", true, false, nullptr);
    waveshaperMenu.addItem(12, "Mix", true, false, nullptr);
    //Low Frequency Osc Menu One
    lfoOne.addItem(13, "Frequency", true, false, nullptr);
    lfoOne.addItem(14, "Phase", true, false, nullptr);
    lfoOne.addItem(15, "PWM", true, false, nullptr);
    //Low Frequency Osc Menu Two
    lfoTwo.addItem(16, "Frequency", true, false, nullptr);
    lfoTwo.addItem(17, "Phase", true, false, nullptr);
    lfoTwo.addItem(18, "PWM", true, false, nullptr);
    //Envelopes
    envOne.addItem(19, "Duration", true, false, nullptr);
    envTwo.addItem(20, "Duration", true, false, nullptr);
    //Step Sequencer
    sseq.addItem(21, "Duration", true, false, nullptr);
    sseq.addItem(22, "Length", true, false, nullptr);
    sseq.addItem(23, "Off value", true, false, nullptr);

    // FX Menus
    effectOneMenu.addItem(-1, "None", true, false, nullptr);
    effectTwoMenu.addItem(-1, "None", true, false, nullptr);


    // Set All look and Feel Methods
    filterOneMenu.setLookAndFeel(&laf);
    filterTwoMenu.setLookAndFeel(&laf);
    waveshaperMenu.setLookAndFeel(&laf);
    lfoOne.setLookAndFeel(&laf);
    lfoTwo.setLookAndFeel(&laf);
    envOne.setLookAndFeel(&laf);
    sseq.setLookAndFeel(&laf);
    effectOneMenu.setLookAndFeel(&laf);
    effectTwoMenu.setLookAndFeel(&laf);
}

void MatrixTable::setEffectsDestination(int fxProcessor, int effect){
    if(effect == 1){
        switch(fxProcessor){
            case 2:
                initialiseStereoDelayMenu(effectOneMenu);
                break;
            case 3:
                initialisePingPongDelayMenu(effectOneMenu);
                break;
            case 4:
                initialiseLCRDelayMenu(effectOneMenu);
                break;
            case 5:
                initialiseStereoFlangerMenu(effectOneMenu);
                break;
            case 6:
                initialiseDimensionChorusMenu(effectOneMenu);
                break;
            case 7:
                initialiseHoldDelayMenu(effectOneMenu);
                break;
            case 8:
                initialiseDecimatorMenu(effectOneMenu);
                break;
            case 9:
                initialisePlateReverb(effectOneMenu);
                break ;
            default:
                effectOneMenu.clear();
                effectOneMenu.addItem(-1, "None", true, false, nullptr);
                break;
        }

    } else if(effect == 2){
        switch(fxProcessor){
            case 2:
                initialiseStereoDelayMenu(effectTwoMenu);
                break;
            case 3:
                initialisePingPongDelayMenu(effectTwoMenu);
                break;
            case 4:
                initialiseLCRDelayMenu(effectTwoMenu);
                break;
            case 5:
                initialiseStereoFlangerMenu(effectTwoMenu);
                break;
            case 6:
                initialiseDimensionChorusMenu(effectTwoMenu);
                break;
            case 7:
                initialiseHoldDelayMenu(effectTwoMenu);
                break;
            case 8:
                initialiseDecimatorMenu(effectTwoMenu);
                break;
            case 9:
                initialisePlateReverb(effectTwoMenu);
                break;
            default:
                effectTwoMenu.clear();
                effectTwoMenu.addItem(-1, "None", true, false, nullptr);
                break;
        }
    }
}

void MatrixTable::initialiseStereoDelayMenu(PopupMenu &menu){
    menu.clear();
    menu.addItem(24, "Cross", true, false, nullptr);
    menu.addItem(25, "Mix", true, false, nullptr);
    menu.addItem(26, "L-Delay", true, false, nullptr);
    menu.addItem(27, "L-FB", true, false, nullptr);
    menu.addItem(28, "R-Delay", true, false, nullptr);
    menu.addItem(29, "R-FB", true, false, nullptr);
}

void MatrixTable::initialisePingPongDelayMenu(PopupMenu &menu){
    menu.clear();
    menu.addItem(30, "L-Delay", true, false, nullptr);
    menu.addItem(31, "L-FB", true, false, nullptr);
    menu.addItem(32, "L-Mix", true, false, nullptr);
    menu.addItem(33, "R-Delay", true, false, nullptr);
    menu.addItem(34, "R-FB", true, false, nullptr);
    menu.addItem(35, "R-Mix", true, false, nullptr);
}

void MatrixTable::initialiseLCRDelayMenu(PopupMenu &menu){
   menu.clear();
   menu.addItem(36, "L-Delay", true, false, nullptr);
   menu.addItem(37, "L-FB", true, false, nullptr);
   menu.addItem(38, "L-Mix", true, false, nullptr);
   menu.addItem(39, "C-Delay", true, false, nullptr);
   menu.addItem(40, "C-FB", true, false, nullptr);
   menu.addItem(41, "HPF Freq", true, false, nullptr);
   menu.addItem(42, "LPF Freq", true, false, nullptr);
   menu.addItem(43, "C-Amp", true, false, nullptr);
   menu.addItem(44, "C-Pan", true, false, nullptr);
   menu.addItem(45, "R-Delay", true, false, nullptr);
   menu.addItem(46, "R-FB", true, false, nullptr);
   menu.addItem(47, "R-Mix", true, false, nullptr);
}

void MatrixTable::initialiseStereoFlangerMenu(PopupMenu &menu){
   menu.clear();
   menu.addItem(48, "Rate", true, false, nullptr);
   menu.addItem(49, "Phase", true, false, nullptr);
   menu.addItem(50, "Depth", true, false, nullptr);
   menu.addItem(51, "FB", true, false, nullptr);
}

void MatrixTable::initialiseDimensionChorusMenu(PopupMenu &menu){
   menu.clear();
   menu.addItem(52, "Rate", true, false, nullptr);
   menu.addItem(53, "Spread", true, false, nullptr);
   menu.addItem(54, "Depth", true, false, nullptr);
   menu.addItem(55, "HPF Freq", true, false, nullptr);
   menu.addItem(56, "C-Amp", true, false, nullptr);
   menu.addItem(57, "C-Pan", true, false, nullptr);
   menu.addItem(58, "Mix", true, false, nullptr);
}

void MatrixTable::initialiseHoldDelayMenu(PopupMenu &menu){
   menu.clear();
   menu.addItem(59, "Speed", true, false, nullptr);
   menu.addItem(60, "Length", true, false, nullptr);
   menu.addItem(61, "Fade", true, false, nullptr);
   menu.addItem(62, "Pan", true, false, nullptr);
   menu.addItem(63, "Mix", true , false, nullptr);
}

void MatrixTable::initialiseDecimatorMenu(PopupMenu &menu){
    menu.clear();
    menu.addItem(64, "B-Fraction", true, false, nullptr);
    menu.addItem(65, "S-Rate", true, false, nullptr);
    menu.addItem(66, "Mix", true, false, nullptr);
}

void MatrixTable::initialisePlateReverb(PopupMenu &menu){
    menu.clear();

    menu.addItem(67, "Bandwidth", true, false, nullptr);
    menu.addItem(68, "Damping", true, false, nullptr);
    menu.addItem(69, "Decay", true, false, nullptr);
    menu.addItem(70, "Mix", true, false, nullptr);

}
