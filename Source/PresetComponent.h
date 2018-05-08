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

#ifndef __JUCE_HEADER_8D91CCEF57955C4__
#define __JUCE_HEADER_8D91CCEF57955C4__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PresetTable.h"
#include "Akateko.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PresetComponent  : public Component,
                         public ButtonListener,
                         public TextEditorListener
{
public:
    //==============================================================================
    PresetComponent (const String &name, AkatekoAudioProcessor &p, Label &prstLabel, Label &prmLabel );
    ~PresetComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setUpdateCommandId(int id);

    void handleCommandMessage(int commandId) override;

    void textEditorEscapeKeyPressed(TextEditor &tEdit) override;
    void textEditorReturnKeyPressed(TextEditor &tEdit) override;
    void textEditorFocusLost(TextEditor &tEdit) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int updateCommandId;

    AkatekoAudioProcessor &processor;
    Label &presetLabel;
    Label &paramLabel;

    int activeSort;
    bool nameSort;
    bool categorySort;
    bool authorSort;

    Colour buttonColour;
    Colour activeColour;

    enum Sort {
       NameSort = 0,
       CategorySort = 1,
       AuthorSort = 2
    };

    void SortPresets(int sorting);
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> loadButton;
    ScopedPointer<TextButton> saveButton;
    ScopedPointer<TextButton> clearButton;
    ScopedPointer<PresetTable> presetTableComponent;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<TextButton> saveAsButton;
    ScopedPointer<TextButton> deleteButton;
    ScopedPointer<TextButton> sortNameButton;
    ScopedPointer<TextButton> sortCategoryButton;
    ScopedPointer<TextButton> sortAuthorButton;
    ScopedPointer<TextButton> folderButton;
    ScopedPointer<TextEditor> textEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8D91CCEF57955C4__
