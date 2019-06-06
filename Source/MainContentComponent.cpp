/*
  =============================================================================

    MainContentComponent.cpp
    Created: 5 Jun 2019 11:01:40
    Author:  renyxa

  =============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainContentComponent.h"

//=============================================================================
MainContentComponent::MainContentComponent()
{
    // In your constructor, you should add any child components, an
    // initialise any special settings that your component needs
    setSize (600, 400);


    addAndMakeVisible (showList);
    showList.setButtonText ("Show List");
    showList.addListener (this);

    addAndMakeVisible (showSearch);
    showSearch.setButtonText ("Search Actor");
    showSearch.addListener (this);

    addAndMakeVisible (updateDB);
    updateDB.setButtonText ("Update DB");
    updateDB.addListener (this);


    showList.setSize (getWidth ()/2, getHeight ()/8);
    showList.setCentreRelative (0.5f, 0.25f);
    showSearch.setSize (getWidth ()/2, getHeight ()/8);
    showSearch.setCentreRelative (0.5f, 0.5f);
    updateDB.setSize (getWidth ()/2, getHeight ()/8);
    updateDB.setCentreRelative (0.5f, 0.75f);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::buttonClicked (Button *button)
{
    if (button == &showList)
    {
       button->setVisible (false);
       showSearch.setVisible (false);
       updateDB.setVisible (false);
    }
    else if (button == &showSearch)
    {

    }
    else if (button == &updateDB)
    {
        updateTask.runThread ();
        //updateTask.threadComplete ();
    }
}


void MainContentComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background an
       draws some placeholder text to get you started

       You should replace everything in this method with your own
       drawing code.
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    // g.setColour (Colours::grey)
    // g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    // g.setColour (Colours::white)
    // g.setFont (14.0f)
    // g.drawText ("MainContentComponent", getLocalBounds())
    //             Justification::centred, true);   // draw some placeholder text
}

void MainContentComponent::resized ()
{
    // This method is where you should set the bounds of any child
    // components that your component contains.

    showList.setSize (getWidth ()/2, getHeight ()/8);
    showList.setCentreRelative (0.5f, 0.25f);
    showSearch.setSize (getWidth ()/2, getHeight ()/8);
    showSearch.setCentreRelative (0.5f, 0.5f);
    updateDB.setSize (getWidth ()/2, getHeight ()/8);
    updateDB.setCentreRelative (0.5f, 0.75f);
}

