/*
  ==============================================================================

    MainContentComponent.h
    Created: 5 Jun 2019 11:01:40pm
    Author:  renyxa

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainPage.h"

//==============================================================================
/*
*/
class MainContentComponent    : public Component
{
public:
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    MainPage mainPage;
};
