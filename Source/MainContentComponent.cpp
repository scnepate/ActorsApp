/*
  ==============================================================================

    MainContentComponent.cpp
    Created: 5 Jun 2019 11:01:40pm
    Author:  renyxa

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainContentComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize (600, 400);
    addAndMakeVisible (mainPage);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
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
    // g.drawText ("MainContentComponent", getLocalBounds(),
    //             Justification::centred, true);   // draw some placeholder text
}

void MainContentComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    mainPage.setBounds (0, 0, getWidth (), getHeight ());
}
