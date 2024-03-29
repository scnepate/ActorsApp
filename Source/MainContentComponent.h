/*
  =============================================================================

    MainContentComponent.h
    Created: 5 Jun 2019 11:01:40
    Author:  renyxa

  =============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThreadTasks.h"
#include "ActorsListBox.h"
//=============================================================================
class MainContentComponent    : public Component,
                                public Button::Listener
{
public:
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked (Button* button) override;
    bool loadActors ();
    bool isThereData ();
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    TextButton showList, showSearch, updateDB, back;
    TextButton triggerSearchButton;
    Label searchText, searchResult;
    Label loadText;
    UpdateTask updateTask;
    std::vector <var> actors;
    ActorsListBoxModel *listBoxModel {nullptr};
    ListBox *list {nullptr};
};

