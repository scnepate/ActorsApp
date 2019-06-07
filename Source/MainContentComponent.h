/*
  =============================================================================

    MainContentComponent.h
    Created: 5 Jun 2019 11:01:40
    Author:  renyxa

  =============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "UpdateTask.h"
//#include "ActorsListBox.h"
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
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    TextButton showList, showSearch, updateDB, back;
    TextButton triggerSearchButton;
    UpdateTask updateTask;
    Label searchText, searchResult;
    std::vector <var> actors;
};

class LoadActorsTask : public ThreadWithProgressWindow
{
public:
    LoadActorsTask (std::vector <var> *actors, Component *comp) : actors (actors), ThreadWithProgressWindow ("Loading Actors", true, false, 10000, String (), comp), state (true) {}

    void run ()
    {
        actors->clear ();
        File dataFile = File::getSpecialLocation (File::userApplicationDataDirectory).getChildFile ("data").getChildFile ("assets").getChildFile ("data.json");
        if (!dataFile.exists ())
        {
            state = false;
            return;
        }

        FileInputStream fin (dataFile);

        String line = fin.readNextLine ();
        //std::cout << line << "\n";

        var parsed = JSON::parse (line);

        int total_results = parsed.getProperty ("total_results", var(1));
        int cnt = 1;

        for (auto i: *parsed.getProperty ("results", var::undefined).getArray ())
        {
            actors->push_back (i);
            setProgress ((cnt ++)/(float)total_results);
            //std::cout << "cnt: " << cnt << "\n";
        }
    }

    bool getState () { return state; }

private:
    std::vector <var> *actors;
    bool state;
};