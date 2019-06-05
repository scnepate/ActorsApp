/*
  ==============================================================================

    MainPage.h
    Created: 5 Jun 2019 9:10:27pm
    Author:  renyxa

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MainPage    : public Component
{
public:
    MainPage();
    ~MainPage();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPage)
    TextButton showList, showSearch, updateDB;
};
