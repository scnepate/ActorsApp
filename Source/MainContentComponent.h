/*
  =============================================================================

    MainContentComponent.h
    Created: 5 Jun 2019 11:01:40
    Author:  renyxa

  =============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

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
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    TextButton showList, showSearch, updateDB;
};

