/*
  ==============================================================================

    MainPage.cpp
    Created: 5 Jun 2019 9:10:27pm
    Author:  renyxa

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainPage.h"

//==============================================================================
MainPage::MainPage()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize (getParentWidth (), getParentHeight ());

    addAndMakeVisible (showList);
    showList.setButtonText ("Show List");
    showList.setSize (getWidth ()/2, getHeight ()/10);
    showList.setCentreRelative (0.5f, 0.2f);

    addAndMakeVisible (showSearch);
    showSearch.setButtonText ("Search Actor");
    showSearch.setSize (getWidth ()/2, getHeight ()/10);
    showSearch.setCentreRelative (0.5f, 0.4f);

    addAndMakeVisible (updateDB);
    updateDB.setButtonText ("Update DB");
    updateDB.setSize (getWidth ()/2, getHeight ()/10);
    updateDB.setCentreRelative (0.5f, 0.6f);
}

MainPage::~MainPage()
{
}

void MainPage::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    // g.setColour (Colours::grey);
    // g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    // g.setColour (Colours::white);
    // g.setFont (14.0f);
    // g.drawText ("MainPage", getLocalBounds(),
                // Justification::centred, true);   // draw some placeholder text
}

void MainPage::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    showList.setSize (getWidth ()/2, getHeight ()/10);
    showList.setCentreRelative (0.5f, 0.2f);
    showSearch.setSize (getWidth ()/2, getHeight ()/10);
    showSearch.setCentreRelative (0.5f, 0.4f);
    updateDB.setSize (getWidth ()/2, getHeight ()/10);
    updateDB.setCentreRelative (0.5f, 0.6f);
}
