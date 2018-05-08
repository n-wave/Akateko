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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PresetComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
/*
    : Component::Component(name),
      nameSort(false),
      categorySort(false),
      authorSort(false),
      processor(p),
      presetLabel(prstLabel),
      paramLabel(prmLabel),
      buttonColour(Colour(0x73707070)),
      activeColour(Colour(0x7f007f7f))

 */
//[/MiscUserDefs]

//==============================================================================
PresetComponent::PresetComponent (const String &name, AkatekoAudioProcessor &p, Label &prstLabel, Label &prmLabel )
    : Component::Component(name),
      updateCommandId(-1),
      activeSort(-1),
      nameSort(false),
      categorySort(false),
      authorSort(false),
      processor(p),
      presetLabel(prstLabel),
      paramLabel(prmLabel),
      buttonColour(Colour(0x73707070)),
      activeColour(Colour(0x7f007f7f))
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[Constructor_pre] You can add your own custom stuff here..

    addAndMakeVisible (loadButton = new TextButton ("loadButton"));
    loadButton->setButtonText (TRANS("Load"));
    loadButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    loadButton->addListener (this);
    loadButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    loadButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    loadButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (saveButton = new TextButton ("saveButton"));
    saveButton->setButtonText (TRANS("Save"));
    saveButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    saveButton->addListener (this);
    saveButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    saveButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    saveButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (clearButton = new TextButton ("clearButton"));
    clearButton->setButtonText (TRANS("Clear"));
    clearButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    clearButton->addListener (this);
    clearButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    clearButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    clearButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (presetTableComponent = new PresetTable(processor.getCurrentPresets()));
    presetTableComponent->setName ("presetTable");

    addAndMakeVisible (nameLabel = new Label ("nameLabel",
                                              TRANS("PRESETS")));
    nameLabel->setFont (Font ("Good Times", 15.00f, Font::plain));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (Label::textColourId, Colours::white);
    nameLabel->setColour (TextEditor::textColourId, Colours::black);
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (saveAsButton = new TextButton ("saveAsButton"));
    saveAsButton->setButtonText (TRANS("Save As"));
    saveAsButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    saveAsButton->addListener (this);
    saveAsButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    saveAsButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    saveAsButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (deleteButton = new TextButton ("deleteButton"));
    deleteButton->setButtonText (TRANS("Delete"));
    deleteButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    deleteButton->addListener (this);
    deleteButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    deleteButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    deleteButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (sortNameButton = new TextButton ("sortNameButton"));
    sortNameButton->setButtonText (TRANS("Name"));
    sortNameButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    sortNameButton->addListener (this);
    sortNameButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    sortNameButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    sortNameButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (sortCategoryButton = new TextButton ("sortCategoryButton"));
    sortCategoryButton->setButtonText (TRANS("Category"));
    sortCategoryButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    sortCategoryButton->addListener (this);
    sortCategoryButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    sortCategoryButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    sortCategoryButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (sortAuthorButton = new TextButton ("sortAuthorButton"));
    sortAuthorButton->setButtonText (TRANS("Author"));
    sortAuthorButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    sortAuthorButton->addListener (this);
    sortAuthorButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    sortAuthorButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    sortAuthorButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (folderButton = new TextButton ("folderButton"));
    folderButton->setButtonText (TRANS("Folder"));
    folderButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    folderButton->addListener (this);
    folderButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    folderButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    folderButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setVisible(false);
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->addListener(this);
    //[/Constructor_pre]



    //[UserPreSize]

    //[/UserPreSize]

    setSize (515, 250);


    //[Constructor] You can add your own custom stuff here..

    activeSort = Sort::NameSort;
    sortNameButton->setColour(TextButton::buttonColourId, activeColour);
    //[/Constructor]
}

PresetComponent::~PresetComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    loadButton = nullptr;
    saveButton = nullptr;
    clearButton = nullptr;
    presetTableComponent = nullptr;
    nameLabel = nullptr;
    saveAsButton = nullptr;
    deleteButton = nullptr;
    sortNameButton = nullptr;
    sortCategoryButton = nullptr;
    sortAuthorButton = nullptr;
    folderButton = nullptr;
    textEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PresetComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PresetComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    loadButton->setBounds (436, 24, 70, 20);
    saveButton->setBounds (436, 48, 70, 20);
    clearButton->setBounds (436, 96, 70, 20);
    presetTableComponent->setBounds (5, 5, 420, 240);
    nameLabel->setBounds (427, 5, 88, 16);
    saveAsButton->setBounds (436, 72, 70, 20);
    deleteButton->setBounds (436, 120, 70, 20);
    sortNameButton->setBounds (436, 176, 70, 20);
    sortCategoryButton->setBounds (436, 200, 70, 20);
    sortAuthorButton->setBounds (436, 224, 70, 20);
    folderButton->setBounds (436, 144, 70, 20);
    textEditor->setBounds (310, 224, 112, 16);
    //[UserResized] Add your own custom resize handling here..
    presetTableComponent->initialiseHeader(420.0, 240.0);
    //[/UserResized]

}

void PresetComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadButton)
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..

       int tmpRowNumber = presetTableComponent->getActiveRow();

        if(tmpRowNumber != -1){
            File tmpFile = presetTableComponent->getFile(tmpRowNumber);

            if(tmpFile.isOnHardDisk()){
                processor.load(tmpFile);
                getParentComponent()->postCommandMessage(updateCommandId);
            } else {
                paramLabel.setText("Could not load file", dontSendNotification);
            }
        }


        //processor.load()
        //[/UserButtonCode_loadButton]
    }
    else if (buttonThatWasClicked == saveButton)
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
        int tmpRowNumber = presetTableComponent->getActiveRow();

        if(tmpRowNumber != -1){
            akateko::PresetRow tmpRow;

            if(presetTableComponent->getPresetRow(tmpRowNumber, tmpRow)){
                if(tmpRow.file.isOnHardDisk()){
                    if(processor.save(tmpRow)){
                        presetTableComponent->setCurrentPresets(processor.getCurrentPresets());
                        presetTableComponent->repaint();

                        const String tmpMessage = String("Saved : ") + String(tmpRow.name) + ".fxp" ;
                        paramLabel.setText(tmpMessage, sendNotificationAsync);
                    } else {
                        const String tmpMessage =  String("Couldn't save file");
                        paramLabel.setText(tmpMessage, sendNotificationAsync);
                    }
                } else {
                    std::cerr << "PresetComponent::buttonClicked::save" << std::endl;
                    std::cerr << "Error ocurred while saving preset" << std::endl;
                    std::cerr << "File doesn't exist anymore" << std::endl;
                }
            }
        } else {
            std::cerr << "PresetComponent::buttonClicked::save" << std::endl;
            std::cerr << "Error ocurred while saving preset" << std::endl;
            std::cerr << "Row Number out of bound" << std::endl;
        }
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == clearButton)
    {
        //[UserButtonCode_clearButton] -- add your button handler code here..
        int tmpRowNumber = presetTableComponent->getActiveRow();

        if(tmpRowNumber != -1){
            akateko::PresetRow tmpRow;

            if(presetTableComponent->getPresetRow(tmpRowNumber, tmpRow)){
                processor.clear(tmpRow);
                getParentComponent()->postCommandMessage(updateCommandId);

                const String tmpMessage = tmpRow.name + String(" : cleared");

                presetLabel.setText(tmpRow.name, sendNotificationSync);
                paramLabel.setText(tmpMessage, sendNotificationSync);
            }
        }
        //Load default preset should be hidden

        //[/UserButtonCode_clearButton]
    }
    else if (buttonThatWasClicked == saveAsButton)
    {
        //[UserButtonCode_saveAsButton] -- add your button handler code here..
        int tmpRowNumber = presetTableComponent->getActiveRow();

        if(tmpRowNumber != -1){
            akateko::PresetRow tmpRow;

            if(presetTableComponent->getPresetRow(tmpRowNumber, tmpRow)){
                if(processor.saveAs(tmpRow)){
                    const String tmpName = processor.getCurrentPresetName() + String(" : saved");

                    paramLabel.setText(tmpName, sendNotificationAsync);

                    presetTableComponent->setCurrentPresets(processor.getCurrentPresets());
                    presetTableComponent->updateContent();
                } else {
                    std::cerr << "PresetComponent::buttonClicked::saveAs" << std::endl;
                    std::cerr << "Error ocurred while saving preset" << std::endl;
                    std::cerr << "File doesn't exist anymore" << std::endl;
                }
            }
        } else {
            if(processor.save()){
                presetTableComponent->setCurrentPresets(processor.getCurrentPresets());
                presetTableComponent->updateContent();
            }
        }


        //[/UserButtonCode_saveAsButton]
    }
    else if (buttonThatWasClicked == deleteButton)
    {
        //[UserButtonCode_deleteButton] -- add your button handler code here..
        int tmpRowNumber = presetTableComponent->getActiveRow();

        if(tmpRowNumber != -1){
            File tmpFile = presetTableComponent->getFile(tmpRowNumber);
            String tmpMessage = tmpFile.getFileNameWithoutExtension();

            if(tmpFile.deleteFile()){
                presetTableComponent->setCurrentPresets(processor.getCurrentPresets());
                presetTableComponent->updateContent();
                presetTableComponent->resetActiveRow();

                tmpMessage += String(" : deleted");

                paramLabel.setText(tmpMessage, dontSendNotification);
            }
        }


        //[/UserButtonCode_deleteButton]
    }
    else if (buttonThatWasClicked == sortNameButton)
    {
        //[UserButtonCode_sortNameButton] -- add your button handler code here..
        activeSort = Sort::NameSort;

        categorySort = false;
        authorSort = false;

        sortNameButton->setColour(TextButton::buttonColourId, activeColour);
        sortCategoryButton->setColour(TextButton::buttonColourId, buttonColour);
        sortAuthorButton->setColour(TextButton::buttonColourId, buttonColour);


        SortPresets(activeSort);
        //presetTableComponent->repaint();

        //[/UserButtonCode_sortNameButton]
    }
    else if (buttonThatWasClicked == sortCategoryButton)
    {
        //[UserButtonCode_sortCategoryButton] -- add your button handler code here..
        activeSort = Sort::CategorySort;

        nameSort = false;
        authorSort = false;

        sortNameButton->setColour(TextButton::buttonColourId, buttonColour);
        sortCategoryButton->setColour(TextButton::buttonColourId, activeColour);
        sortAuthorButton->setColour(TextButton::buttonColourId, buttonColour);

        SortPresets(activeSort);
        //presetTableComponent->repaint();
        //[/UserButtonCode_sortCategoryButton]
    }
    else if (buttonThatWasClicked == sortAuthorButton)
    {
        //[UserButtonCode_sortAuthorButton] -- add your button handler code here..

        activeSort = Sort::AuthorSort;
        nameSort = false;
        categorySort = false;

        sortNameButton->setColour(TextButton::buttonColourId, buttonColour);
        sortCategoryButton->setColour(TextButton::buttonColourId, buttonColour);
        sortAuthorButton->setColour(TextButton::buttonColourId, activeColour);


        SortPresets(activeSort);

        //presetTableComponent->repaint();
        //[/UserButtonCode_sortAuthorButton]
    }
    else if (buttonThatWasClicked == folderButton)
    {
        //[UserButtonCode_folderButton] -- add your button handler code here..
        if(processor.folder()){
            vector<akateko::PresetRow> rows = processor.getCurrentPresets();

            if(!rows.empty()){
                for(int i=0; i<rows.size(); i++){
                    std::cout << "Printing rows" << std::endl;
                    std::cout << rows[i].file.getFullPathName() << std::endl;
                    std::cout << rows[i].name << std::endl;
                    std::cout << rows[i].category << std::endl;
                    std::cout << rows[i].author << std::endl;
                }
            } else {
                std::cout << "rows are empty" << std::endl;
            }


            presetTableComponent->setCurrentPresets(processor.getCurrentPresets());
            presetTableComponent->updateContent();
            presetTableComponent->resetActiveRow();
        } else {
            std::cerr << "PresetComponent::buttonClicked::folder" << std::endl;
            std::cerr << "Something went wrong scanning the folder" << std::endl;
        }
        //[/UserButtonCode_folderButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PresetComponent::handleCommandMessage(int commandId){
    if(commandId == PresetTable::requestTextEditor){
        Rectangle<int> tmpBounds = presetTableComponent->getClickedCellPosition();

        textEditor->setBounds(tmpBounds.getX()+5,
                              tmpBounds.getY()+5,
                              tmpBounds.getWidth(),
                              tmpBounds.getHeight());


        textEditor->setVisible(true);
        textEditor->grabKeyboardFocus();
    }
}

void PresetComponent::textEditorEscapeKeyPressed(TextEditor &tEdit){
    grabKeyboardFocus();
    tEdit.clear();
    tEdit.setVisible(false);
}

void PresetComponent::textEditorReturnKeyPressed(TextEditor &tEdit){
    String tmpText = tEdit.getText();

    if(!tmpText.isEmpty()){
        int tmpRowNumber = presetTableComponent->getClickedRow();
        int tmpColNumber = presetTableComponent->getClickedColumn();

        File tmpFile = presetTableComponent->getFile(tmpRowNumber);

        if(tmpColNumber >= 1 && tmpColNumber <= 3){
            textEditor->clear();
            textEditor->setVisible(false);

            // Change Name
            if(tmpColNumber == 1 && processor.changeName(tmpFile, tmpText)){
                presetTableComponent->changeName(tmpRowNumber, tmpText);
            }
            // Change Category
            else if(tmpColNumber == 2 && processor.changeCategory(tmpFile, tmpText)){
                presetTableComponent->changeCategory(tmpRowNumber, tmpText);
            }
            //Change Author
            else if(tmpColNumber == 3 && processor.changeAuthor(tmpFile, tmpText)){
                presetTableComponent->changeAuthor(tmpRowNumber, tmpText);
            }

            presetTableComponent->repaint();
        }
    }
}

void PresetComponent::textEditorFocusLost(TextEditor &tEdit){
    tEdit.setText("");
    tEdit.setVisible(false);
}

void PresetComponent::setUpdateCommandId(int id){
    updateCommandId = id;
}

void PresetComponent::SortPresets(int sorting){
    if(sorting == Sort::NameSort){
        if(!nameSort){
            presetTableComponent->sortPresets(PresetTable::nameSortAscending);
        } else {
            presetTableComponent->sortPresets(PresetTable::nameSortDescending);
        }

        nameSort = !nameSort;

        presetTableComponent->repaint();
    }

    else if(sorting == Sort::CategorySort){
        if(!categorySort){
            presetTableComponent->sortPresets(PresetTable::categorySortAscending);
        } else {
            presetTableComponent->sortPresets(PresetTable::categorySortDescending);
        }
        categorySort = !categorySort;

        presetTableComponent->repaint();
    }

    else if(sorting == Sort::AuthorSort){
        if(!authorSort){
            presetTableComponent->sortPresets(PresetTable::authorSortAscending);
        } else {
            presetTableComponent->sortPresets(PresetTable::autherSortDescending);
        }
        authorSort = !authorSort;

        presetTableComponent->repaint();
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PresetComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;prstLabel, Label &amp;prmLabel "
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;presetLabel(prstLabel), &#10;paramLabel(prmLabel)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="515" initialHeight="250">
  <BACKGROUND backgroundColour="ffffff"/>
  <TEXTBUTTON name="loadButton" id="76af27469265e466" memberName="loadButton"
              virtualName="" explicitFocusOrder="0" pos="436 24 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Load" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="saveButton" id="c3189cfdbfa9f46b" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="436 48 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Save" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="clearButton" id="94593925ed6149ba" memberName="clearButton"
              virtualName="" explicitFocusOrder="0" pos="436 96 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Clear" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="presetTable" id="cea717f193f43f60" memberName="presetTableComponent"
                    virtualName="PresetTable" explicitFocusOrder="0" pos="5 5 420 240"
                    class="Component" params=""/>
  <LABEL name="nameLabel" id="a3d123be4d25c5a3" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="427 5 88 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="PRESETS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Good Times"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="saveAsButton" id="67aa2b2b9b1bfd84" memberName="saveAsButton"
              virtualName="" explicitFocusOrder="0" pos="436 72 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Save As" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="deleteButton" id="743d3afcd45b5886" memberName="deleteButton"
              virtualName="" explicitFocusOrder="0" pos="436 120 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Delete" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sortNameButton" id="9c4463595ee243db" memberName="sortNameButton"
              virtualName="" explicitFocusOrder="0" pos="436 176 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Name" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sortCategoryButton" id="1ead5370c2b85515" memberName="sortCategoryButton"
              virtualName="" explicitFocusOrder="0" pos="436 200 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Category" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sortAuthorButton" id="5802295ad9557b46" memberName="sortAuthorButton"
              virtualName="" explicitFocusOrder="0" pos="436 224 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Author" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="folderButton" id="18d2a322b8e0faae" memberName="folderButton"
              virtualName="" explicitFocusOrder="0" pos="436 144 70 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="Folder" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="300b149bee65076c" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="310 224 112 16" initialText="change category&#10;"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="0"
              caret="1" popupmenu="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
