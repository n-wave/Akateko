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

    double getSample(double input);

    void enableFilters(bool enable);
    void filterConfiguration(int config);
    void setResonanceScalar(double scalar);

    // Filter One
    void enableFilterOne(bool enable);
    void setFilterOneDrive(double drive);
    void setFilterOnePassBand(double passBand);
    void setFilterOneFrequency(double frequency);
    void setFilterOneResonance(double resonance);
    void setFilterOneVolume(double volume);
    void setFilterOneType(int type);

    // Filter Two
    void enableFilterTwo(bool enable);
    void setFilterTwoDrive(double drive);
    void setFilterTwoPassBand(double passBand);
    void setFilterTwoFrequency(double frequency);
    void setFilterTwoResonance(double resonance);
    void setFilterTwoVolume(double volume);
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

    const double &fOneVol;
    const double &fTwoVol;

    bool filtersEnabled;
    int filterConfig;

    bool filterOneEnabled;
    bool filterTwoEnabled;
    bool waveShaperEnabled;

    double filterOneVolume;
    double filterTwoVolume;
};

#endif  // AKATEKOVOICE_H_INCLUDED
