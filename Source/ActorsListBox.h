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
        popularity = item.getProperty ("popularity", "");

        DownloadImage d;
        d.downloadImage (photoAddress, photo);

        std::cout << "ActorItem: " << name << " " << popularity << " " << photoAddress << "\n";
    }

    Image photo;
    String photoAddress, name;
    String popularity;
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
        g.setColour (Colours::grey);
        g.drawRect (0, 0, width, height, 1);
        g.drawLine (width/3, 0, width/3, height, 1);
        g.drawLine (width/3, height/2, width, height/2, 1);

        g.setColour (Colours::white);
        g.setFont (width/12);
        g.drawText (items[rowNumber].name, width/3, 0, width-width/3, height/2, Justification::centred, true);
        g.drawText (items[rowNumber].popularity, width/3, height/2, width-width/3, height/2, Justification::centred, true);
        g.drawImageWithin (items[rowNumber].photo, 0, 0, width/3, height, RectanglePlacement::onlyReduceInSize | RectanglePlacement::centred);
    }

    void addFresh ()
    {
        int n = getNumRows ();
        items.push_back (ActorItem (actors->at (n)));
    }

    void listWasScrolled () override
    {
        if (listBox->getVerticalPosition () > 0.70)
        {
            for (int i = 0; i < 10; ++ i)
            {
                addFresh ();
                listBox->updateContent ();
            }
        }
    }
private:
    std::vector <ActorItem> items;
    std::vector <var> *actors;
    ListBox *listBox;
    // add some background task
};

