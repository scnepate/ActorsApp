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


    addChildComponent (back);
    back.setBounds (getWidth ()-105, getHeight ()-45, 100, 40);
    back.setButtonText ("Back");

    addChildComponent (searchText);
    searchText.setText ("First Name", dontSendNotification);
    addChildComponent (triggerSearchButton);
    triggerSearchButton.setButtonText ("Search");
    searchText.setBounds (20, 20, getWidth ()-130, 26);
    triggerSearchButton.setBounds (getWidth ()-100, 20, 80, 26);
    searchText.setFont (22.0f);
    searchText.setEditable (true);
    addChildComponent (searchResult);
    searchResult.setBounds (0, 50, getWidth (), getHeight ()-100);
    searchResult.setFont (getHeight ()/2.0f);
    searchResult.setJustificationType (Justification::centred);
}

MainContentComponent::~MainContentComponent()
{
    if (list)
    {
        delete list;
        delete listBoxModel;
    }
}


bool MainContentComponent::loadActors ()
{
    double progress = 0.05;
    AlertWindow *alert = getLookAndFeel ().createAlertWindow ("Loading", "Please wait...", {}, {}, {}, AlertWindow::NoIcon, 0, this);
    alert->addProgressBarComponent (progress);

    LoadActorsTask *loadActorsTask = new LoadActorsTask (&actors, alert, &progress);

    if (!loadActorsTask->isOk ())
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
                          "Data not found!",
                          "Please update database first.");
        delete alert;
        delete loadActorsTask;
        return false;
    }

    loadActorsTask->startThread ();
    alert->runModalLoop ();

    delete loadActorsTask;
    delete alert;
    return true;
}


void MainContentComponent::buttonClicked (Button *button)
{
    if (button == &showList)
    {
       if (!loadActors ()) return;

       showList.setVisible (false);
       showSearch.setVisible (false);
       updateDB.setVisible (false);
       back.setVisible (true);
       back.addListener (this);

       // allocate new ListBox
       list = new ListBox ("comp");
       listBoxModel = new ActorsListBoxModel (&actors, list);
       list->setModel (listBoxModel);
       addAndMakeVisible (list);
       list->setBounds (getLocalBounds ());
       list->setRowHeight (64);
    }
    else if (button == &showSearch) // ok
    {
       if (!loadActors ()) return;

       showList.setVisible (false);
       showSearch.setVisible (false);
       updateDB.setVisible (false);
       back.setVisible (true);
       back.addListener (this);
       triggerSearchButton.setVisible (true);
       triggerSearchButton.addListener (this);

       searchText.setVisible (true);
       searchText.grabKeyboardFocus ();
    }
    else if (button == &updateDB) // ok
    {
        updateTask.launchThread ();
    }
    else if (button == &back)
    {
        button->setVisible (false);

        if (searchText.isVisible ())
        {
            searchText.setVisible (false);
            searchText.setText ("First Name", dontSendNotification);
            triggerSearchButton.setVisible (false);
            searchResult.setVisible (false);
            triggerSearchButton.removeListener (this);
        }
        else
        {
            removeChildComponent (list);
            delete list;
            delete listBoxModel;
        }

        showList.setVisible (true);
        showSearch.setVisible (true);
        updateDB.setVisible (true);

        button->removeListener (this);
    }
    else if (button == &triggerSearchButton)
    {
        if (searchText.getText ().isEmpty ()) return;
        searchText.setText (searchText.getText ().trimCharactersAtStart (" ").trimCharactersAtEnd (" "), dontSendNotification);
        int result = 0;
        for (auto i: actors)
        {
            String name = i.getProperty ("name", String ());
            if (name.isNotEmpty ())
            {
                String firstName;
                int spaceIndex = name.indexOfChar (' ');
                if (spaceIndex == -1) firstName = name;
                else firstName = name.substring (0, name.indexOfChar (' '));

                if (firstName.toLowerCase () == searchText.getText ().toLowerCase ())
                {
                    ++ result;
                }
            }
        }

        searchResult.setVisible (true);
        searchResult.setText (var (result).toString (), dontSendNotification);
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

    if (searchText.isVisible ())
    {
        g.setColour (Colours::grey);
        g.drawRect (searchText.getBounds(), 1);   // draw an outline around the component
    }
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
    back.setBounds (getWidth ()-105, getHeight ()-45, 100, 40);
    searchText.setBounds (20, 20, getWidth ()-130, 26);
    triggerSearchButton.setBounds (getWidth ()-100, 20, 80, 26);
    searchResult.setBounds (0, 50, getWidth (), getHeight ()-100);
    searchResult.setFont (getHeight ()/2.0f);
    if (list != nullptr)
        list->setBounds (getLocalBounds ());
}

