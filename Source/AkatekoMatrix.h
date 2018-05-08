#ifndef AKATEKOMATRIX_H
#define AKATEKOMATRIX_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Akateko.h"
#include "/work/programming-projects/msm/msm-dsp/Common/ModulationMatrix.h"

/*
 * Modulation Matrix specifically for the akateko Audio plugin
 * The modulation Matrix should be created first and after
 * that the AudioGenerator or ControlGenerators.
 *
 * Then the modulation sources and destinations should be binded
 * to be used by the control generators.
 *
 * These binding function should be unique for the derived
 * modulation matrix and are shared by the synth voice/Generators
 *
 * a base Reference is binded to the gui Component that represents
 * this mod matrix.
 *
 **/

class AkatekoMatrix : public ModulationMatrix
{
public:
    AkatekoMatrix();
    ~AkatekoMatrix();

    void update() override;

    void setSource(int source, int id) override;
    void setDestination(int destination, int id) override;
    void setTransform(int transform, int id) override;

    String toString() override;
    void restoreFromString(const String &matrix) override;

private:

};

#endif // AKATEKOMATRIX_H
