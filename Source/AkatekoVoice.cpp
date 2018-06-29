/*
  ==============================================================================

    AkatekoVoice.cpp
    Created: 10 Mar 2018 9:06:01am
    Author:  mario

  ==============================================================================
*/

#include "AkatekoVoice.h"

using namespace akateko;

AkatekoVoice::AkatekoVoice(AkatekoMatrix &matrix) :
    filterOne(MoogLadderFilter(matrix.getReadRegister(DESTSRC::F1DRIVE),
                               matrix.getReadRegister(DESTSRC::F1PBG),
                               matrix.getReadRegister(DESTSRC::F1FREQ),
                               matrix.getReadRegister(DESTSRC::F1RESO))),

    filterTwo(MoogLadderFilter(matrix.getReadRegister(DESTSRC::F2DRIVE),
                               matrix.getReadRegister(DESTSRC::F2PBG),
                               matrix.getReadRegister(DESTSRC::F2FREQ),
                               matrix.getReadRegister(DESTSRC::F2RESO))),

    waveShaper(WaveShaper(matrix.getReadRegister(DESTSRC::WSDRI),
                          matrix.getReadRegister(DESTSRC::WSMIX))),

    fOneVol(matrix.getReadRegister(DESTSRC::F1VOL)),
    fTwoVol(matrix.getReadRegister(DESTSRC::F2VOL)),

    filtersEnabled(true),
    filterOneEnabled(true),
    filterTwoEnabled(true),
    waveShaperEnabled(true),
    filterOneVolume(0.0),
    filterTwoVolume(0.0),
    filterConfig(1)
{
    filterOne.setFilterType(MoogLadderFilter::BPF4);
    filterTwo.setFilterType(MoogLadderFilter::BPF4);

    /*
     * const double &dModIn,
     * const double &pModIn,
     * const double &fModIn,
     * const double &rModIn
     *
     * const double &dModIn,
     * const double &mModIn,
     * const double &aModIn,
     * const double &rModIn
     *
     */
}

void AkatekoVoice::setSampleRate(double sampleRate){
    filterOne.setSampleRate(sampleRate);
    filterTwo.setSampleRate(sampleRate);
    waveShaper.setSampleRate(sampleRate);
}

void AkatekoVoice::update(){
    filterOne.update();
    filterTwo.update();
    waveShaper.update();
}

double AkatekoVoice::getSample(double input){
    double tmpSample = input;

    if(filtersEnabled){
        if(filterConfig == 0){ //Series

            if(filterOneEnabled){
                tmpSample = filterOne.getSample(tmpSample)*filterOneVolume;
                //std::cout << "Filter One : " << tmpSample << std::endl;

            }
            if(filterTwoEnabled){
                tmpSample = filterTwo.getSample(tmpSample)*filterTwoVolume;
               // std::cout << "Filter Two : " << tmpSample << std::endl;
            }

        } else if(filterConfig == 1){ //parallel

            //TODO fade In
            double tmpParalell = tmpSample;

            if(filterOneEnabled){
                tmpSample = filterOne.getSample(tmpSample)*filterOneVolume;
            }
            if(filterTwoEnabled){
                tmpParalell = filterTwo.getSample(tmpParalell)*filterTwoVolume;
            }

            tmpSample += tmpParalell;
        }
    }

    if(waveShaperEnabled){
       tmpSample =  waveShaper.getSample(tmpSample);
    }

    return tmpSample;
}

// Filter Config
void AkatekoVoice::enableFilters(bool enable){
    filtersEnabled = enable;
}

void AkatekoVoice::filterConfiguration(int config){
    filterConfig = config;
}

void AkatekoVoice::setResonanceScalar(double scalar){
    filterOne.setResonanceScalar(scalar);
    filterTwo.setResonanceScalar(scalar);
}

// Filter One
//==============================================================================
void AkatekoVoice::enableFilterOne(bool enable){
    filterOneEnabled = enable;
}

void AkatekoVoice::setFilterOneDrive(double drive){
    filterOne.setSaturation(drive);
}

void AkatekoVoice::setFilterOnePassBand(double passBand){
    filterOne.setPassBandGain(passBand);
}

void AkatekoVoice::setFilterOneFrequency(double frequency){
    filterOne.setFrequency(frequency);
}

void AkatekoVoice::setFilterOneResonance(double resonance){
    filterOne.setResonance(resonance);
}

void AkatekoVoice::setFilterOneVolume(double volume){
    filterOneVolume = volume;
}

void AkatekoVoice::setFilterOneType(int type){
    filterOne.setFilterType(type);
}
//==============================================================================
// Filter Two
//==============================================================================
void AkatekoVoice::enableFilterTwo(bool enable){
    filterTwoEnabled = enable;
}

void AkatekoVoice::setFilterTwoDrive(double drive){
    filterTwo.setSaturation(drive);
}

void AkatekoVoice::setFilterTwoPassBand(double passBand){
    filterTwo.setPassBandGain(passBand);
}

void AkatekoVoice::setFilterTwoFrequency(double frequency){
    filterTwo.setFrequency(frequency);
}

void AkatekoVoice::setFilterTwoResonance(double resonance){
    filterTwo.setResonance(resonance);
}

void AkatekoVoice::setFilterTwoVolume(double volume){
    filterTwoVolume = volume;
}

void AkatekoVoice::setFilterTwoType(int type){
    filterTwo.setFilterType(type);
}
//==============================================================================
// WaveShaper
void AkatekoVoice::enableWaveShaper(bool enable){
    waveShaperEnabled = enable;
}

void AkatekoVoice::setWaveShaperDrive(double drive){
    waveShaper.setDrive(drive);
}

void AkatekoVoice::setWaveShaperMix(double mix){
    waveShaper.setAmount(mix);
}

void AkatekoVoice::setWaveShaperBuffer(msmBuffer &buffer){
    waveShaper.setBuffer(buffer);
    waveShaper.swapBuffer();
}
