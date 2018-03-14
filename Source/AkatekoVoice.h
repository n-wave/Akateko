/*
  ==============================================================================

    AkatekoVoice.h
    Created: 10 Mar 2018 9:06:01am
    Author:  mario

  ==============================================================================
*/

#ifndef AKATEKOVOICE_H_INCLUDED
#define AKATEKOVOICE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Akateko.h"
#include "AkatekoMatrix.h"
#include "/work/programming-projects/msm/msm-dsp/Filters/MoogLadderFilter.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/WaveShaper.h"

class AkatekoVoice {
public:
    AkatekoVoice(AkatekoMatrix &matrix);

    void setSampleRate(double sampleRate);
    void update();
    void getSample(double *sample);
    void getSample(float *sample);

    void enableFilters(bool enable);
    void filterConfiguration(int config);

    // Filter One
    void enableFilterOne(bool enable);
    void setFilterOneDrive(double drive);
    void setFilterOnePassBand(double passBand);
    void setFilterOneFrequency(double frequency);
    void setFilterOneResonance(double resonance);
    void setFilterOneType(int type);

    // Filter Two
    void enableFilterTwo(bool enable);
    void setFilterTwoDrive(double drive);
    void setFilterTwoPassBand(double passBand);
    void setFilterTwoFrequency(double frequency);
    void setFilterTwoResonance(double resonance);
    void setFilterTwoType(int type);

    // WaveShaper
    void enableWaveShaper(bool enabled);
    void setWaveShaperDrive(double drive);
    void setWaveShaperMix(double mix);
    void setWaveShaperBuffer(msmBuffer &buffer);
private:
    MoogLadderFilter filterOne;
    MoogLadderFilter filterTwo;
    WaveShaper waveShaper;

    bool filtersEnabled;
    int filterConfig;

    bool filterOneEnabled;
    bool filterTwoEnabled;
    bool waveShaperEnabled;
};

#endif  // AKATEKOVOICE_H_INCLUDED
