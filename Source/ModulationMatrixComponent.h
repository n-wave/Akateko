/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_7932748FC001F0E8__
#define __JUCE_HEADER_7932748FC001F0E8__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-dsp/Common/ModulationMatrix.h"
#include "MatrixTable.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!

    Probably remove when finishing the matrix table.
    use the Matrix table directly.

                                                                    //[/Comments]
*/
class ModulationMatrixComponent  : public Component
{
public:
    //==============================================================================
    ModulationMatrixComponent (ModulationMatrix &modm);
    ~ModulationMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void handleCommandMessage(int commandId) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ModulationMatrix &matrix;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<MatrixTable> matrixTable;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationMatrixComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_7932748FC001F0E8__
