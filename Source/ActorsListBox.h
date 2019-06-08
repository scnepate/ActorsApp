/*
  ==============================================================================

    ActorsListBox.h
    Created: 7 Jun 2019 3:36:18am
    Author:  renyxa

  ==============================================================================
*/

#pragma once
#include <vector>
#include "ThreadTasks.h"


struct ActorItem
{
    ActorItem (var item)
    {
        name = item.getProperty ("name", "No Name");
        photoAddress = item.getProperty ("profile_path", "");
        popularity = item.getProperty ("popularity", 0.0);
        DownloadImage::downloadImage (photoAddress, photo);

        std::cout << "ActorItem: " << name << " " << popularity << "\n";
    }

/*
    void paint (Graphics &g) override
    {
        g.fillAll (Colours::white);
    }

    void resized () override
    {

    }
*/
    Image photo;
    String photoAddress, name;
    float popularity;
};

class ActorsListBoxModel : public ListBoxModel
{
public:
    ActorsListBoxModel (std::vector <var>* actors, ListBox *listBox) : listBox (listBox)
    {
        this->actors = actors;
        for (int i = 0; i < 20 && i < actors->size (); ++ i)
        {
            items.push_back (ActorItem (actors->at(i)));
        }
    }

    int getNumRows () override
    {
        return items.size ();
    }

    void paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) override
    {
        //items[rowNumber].paintEntireComponent (g, false);
        g.setFont (height);
        g.setColour (Colours::white);
        g.drawText (items[rowNumber].name, 0, 0, width, height, Justification::centred, true);
    }

    void addFresh ()
    {
        int n = getNumRows ();
        for (int i = n; i < n+20 && i < actors->size (); ++ i)
        {
            items.push_back (ActorItem (actors->at (i)));
        }
    }

    void listWasScrolled () override
    {
        if (listBox->getVerticalPosition () > 0.95)
        {
            addFresh ();
            listBox->updateContent ();
        }
    }
private:
    std::vector <ActorItem> items;
    std::vector <var> *actors;
    ListBox *listBox;
};

