#include "AkatekoMatrix.h"
#include <iostream>

using namespace akateko;

AkatekoMatrix::AkatekoMatrix()
{
    writeRegisters.resize(MODSRC::SRCSIZE, 0.0);
    readRegisters.resize(DESTSRC::DSTSIZE, 0.0);
}

AkatekoMatrix::~AkatekoMatrix(){}

void AkatekoMatrix::update(){
    /*clear active destination registers */

    std::fill(readRegisters.begin(), readRegisters.end(), 0.0);

    for(int i=0; i<rows.size(); i++){
        int source = rows[i].source;
        int transform = rows[i].transform ;
        int destination = rows[i].destination;
        double range = rows[i].range;
        double intensity = rows[i].intensity;

        double sourceValue = writeRegisters[source];

        if(source == MODSRC::LFO1 || source == MODSRC::LFO2){
            if(transform == TRANSFORM::UNIPOLAR){
                sourceValue = msm::bipolarToUnitpolar(sourceValue);
            }
        }

        else if(source == MODSRC::ENV1 ||
                source == MODSRC::ENV2 ||
                source == MODSRC::PADX ||
                source == MODSRC::PADY)
        {
            if(transform == TRANSFORM::BIPOLAR){
                sourceValue = msm::unipolarToBipolar(sourceValue);
            }
        }

        //std::cout << "Range in Modulation Matrix : " << range << std::endl;

        //std::cout << "Scaled Range in Modulation Matrix : " << range << std::endl;
        //std::cout << "Intensity in Modulation Matrix : " << intensity << std::endl;
        if(rows[i].on == 1){
            readRegisters[destination] += sourceValue*intensity*range;
        }

        //std::cout << "current Read Register : " << readRegisters[destination] << std::endl;


    }
}

void AkatekoMatrix::setSource(int source, int id){
    if(!rows.empty()){
        if(source >= 0 && source <= MODSRC::SRCSIZE){
            bool foundId = false;
            int index = 0;


            while(!foundId && index < rows.size()){
                if(id == rows[index].id){
                    rows[index].source = source;
                    rows[index].transform = TRANSFORM::NONE; //source changed reset
                    foundId = true;
                }
                index++;
            }

            std::cout << toString() << std::endl;
        }
    }
}

void AkatekoMatrix::setDestination(int destination, int id){
    if(!rows.empty()){
        if(destination >= 0 && destination <= DESTSRC::DSTSIZE){
            bool foundId = false;
            int index = 0;

            while(!foundId && index < rows.size()){
                if(id == rows[index].id){
                    rows[index].destination = destination;
                    foundId = true;
                }
                index++;
            }
        }
    }
}
/* Should be checked against the possible transform
 * available, this depends on the plugin.
 *
 * The amount of transformations possible and the range
 * of the transform variable is defined by the input
 * source and is defined in the akateko namepspace
 */
void AkatekoMatrix::setTransform(int transform, int id){
    if(!rows.empty()){
        bool foundId = false;
        int index = 0;

        while(!foundId && index < rows.size()){
            if(rows[index].id == id){
               /* check for possible transforms although it is checked in the
                * MatrixTable GUI representation double check here.
                */
                if(transform >= -1 && transform < TRANSFORM::TSIZE){
                    rows[index].transform = transform;
                }

               foundId = true;
            }
            index++;
        }
    }
}

/* Store String in preset */

String AkatekoMatrix::toString(){
    String tmpString = String("mm") + " ";

    for(int i=0; i<rows.size(); i++){
        tmpString += String(rows[i].id) + " ";
        tmpString += String(rows[i].source) + " ";
        tmpString += String(rows[i].transform) + " ";
        tmpString += String(rows[i].range) + " ";
        tmpString += String(rows[i].intensity) + " ";
        tmpString += String(rows[i].destination) + " ";
        tmpString += String(rows[i].on) + " ";
    }

    return tmpString;
}

void AkatekoMatrix::restoreFromString(const String &matrix){
    StringArray tokens;

    tokens.addTokens(matrix, " ", "\"");
    int tokenSize = tokens.size();

    if(tokenSize >= 8 && tokens[0] == "mm"){
        rows.clear(); //Clear rows and start adding.

        int nrOfSteps = (tokenSize-1)/7;
        int stepIndex = 0;
        int index = 1;

        std::cout << "AkatekoMatrix::restoreFromString" << std::endl;
        std::cout << "Printing number of steps : "<< nrOfSteps << std::endl;

        while(index < tokenSize && stepIndex < nrOfSteps){
            MatrixRow tmpRow = MatrixRow();

            tmpRow.id = tokens[index].getIntValue();
            tmpRow.source = tokens[index+1].getIntValue();
            tmpRow.transform = tokens[index+2].getIntValue();
            tmpRow.range = tokens[index+3].getDoubleValue();
            tmpRow.intensity = tokens[index+4].getDoubleValue();
            tmpRow.destination = tokens[index+5].getIntValue();
            tmpRow.on = tokens[index+6].getIntValue();

            addRow(tmpRow);

            index += 7;
            stepIndex++;
        }
    }
}



