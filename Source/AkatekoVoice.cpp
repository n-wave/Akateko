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
    filterOne(MoogLadderFilter(matrix.getReadRegister(DESTSRC::F1FREQ))),
    filterTwo(MoogLadderFilter(matrix.getReadRegister(DESTSRC::F2FREQ))),
    filtersEnabled(true),
    filterOneEnabled(true),
    filterTwoEnabled(true),
    filterConfig(1)
{
    filterOne.setFilterType(MoogLadderFilter::BPF4);
    filterTwo.setFilterType(MoogLadderFilter::BPF4);
}

void AkatekoVoice::setSampleRate(double sampleRate){
    filterOne.setSampleRate(sampleRate);
    filterTwo.setSampleRate(sampleRate);
}

void AkatekoVoice::update(){
    filterOne.update();
    filterTwo.update();
}

void AkatekoVoice::getSample(double *sample){
    double tmpSample = *sample;

    if(filtersEnabled){
        if(filterConfig == 0){ //Series

            if(filterOneEnabled){
                filterOne.getSample(&tmpSample);
            }
            if(filterTwoEnabled){
                filterTwo.getSample(&tmpSample);
            }


        } else if(filterConfig == 1){ //parallel
            double tmpParalell = tmpSample;

            if(filterOneEnabled){
                filterOne.getSample(&tmpSample);
            }
            if(filterTwoEnabled){
                filterTwo.getSample(&tmpParalell);
            }

            tmpSample += tmpParalell;
        }
    }

    if(waveShaperEnabled){
        waveShaper.getSample(&tmpSample);
    }

    *sample = tmpSample;
}

void AkatekoVoice::getSample(float *sample){
    float tmpSample = *sample;

    if(filtersEnabled){
        if(filterConfig == 0){ //Series

            if(filterOneEnabled){
                filterOne.getSample(&tmpSample);
            }
            if(filterTwoEnabled){
                filterTwo.getSample(&tmpSample);
            }


        } else if(filterConfig == 1){ //parallel
            float tmpParalell = tmpSample;

            if(filterOneEnabled){
                filterOne.getSample(&tmpSample);
            }
            if(filterTwoEnabled){
                filterTwo.getSample(&tmpParalell);
            }
            tmpSample += tmpParalell;
        }
    }

    if(waveShaperEnabled){
        waveShaper.getSample(&tmpSample);
    }

    *sample = tmpSample;
}

// Filter Config
void AkatekoVoice::enableFilters(bool enable){
    filtersEnabled = enable;
}

void AkatekoVoice::filterConfiguration(int config){
    filterConfig = config;
}

// Filter One
//==============================================================================
void AkatekoVoice::enableFilterOne(bool enable){
    filtersEnabled = enable;
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

void AkatekoVoice::setFilterOneType(int type){
    filterOne.setFilterType(type);
}
//==============================================================================
// Filter Two
//==============================================================================
void AkatekoVoice::enableFilterTwo(bool enable){
    filtersEnabled = enable;
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
}
