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
        double range = 1.0;
        double intensity = rows[i].intensity;

        double sourceValue = writeRegisters[source];



        //std::cout << "Range in Modulation Matrix : " << range << std::endl;

        if(destination == DESTSRC::F1FREQ ||
           destination == DESTSRC::F2FREQ)
        {
           range  = rows[i].range*FILTER_FC_MODRANGE;
        }

    //    std::cout << "Scaled Range in Modulation Matrix : " << range << std::endl;
  //     std::cout << "Intensity in Modulation Matrix : " << intensity << std::endl;


        readRegisters[destination] += sourceValue*intensity*range;

//        std::cout << "current Read Register : " << readRegisters[destination] << std::endl;


    }
}

void AkatekoMatrix::setSource(int source, int id){
    if(!rows.empty() && id >= 0 && id <= rows.size()){
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
        }
    }
}

void AkatekoMatrix::setDestination(int destination, int id){
    if(!rows.empty() && id >= 0 && id <= rows.size()){
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
