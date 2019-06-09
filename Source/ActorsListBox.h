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
    ActorItem (var item) : isTherePhoto (0)
    {
        name = item.getProperty ("name", "No Name");
        photoAddress = item.getProperty ("profile_path", "");
        popularity = item.getProperty ("popularity", "");

        //std::cout << "ActorItem: " << name << " " << popularity << " " << photoAddress << "\n";
    }

    Image photo;
    bool isTherePhoto;
    String photoAddress, name;
    String popularity;
};

class ActorsListBoxModel : public ListBoxModel,
                           public Thread::Listener
{
public:
    ActorsListBoxModel (std::vector <var>* actors, ListBox *listBox) : listBox (listBox)
    {
        this->actors = actors;
        for (int i = 0; i < 20 && i < actors->size (); ++ i)
        {
            addFresh ();
        }
    }
    // add destructor
    ~ActorsListBoxModel ()
    {
        for (auto it = jobs.begin (); it != jobs.end (); it ++)
        {
            if (!downloadPool.contains (*it))
            {
                delete *it;
                jobs.erase (it);
                it --;
            }
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
        g.setFont (width/16);
        g.drawText (items[rowNumber].name, width/3, 0, width-width/3, height/2, Justification::centred, true);
        g.drawText (items[rowNumber].popularity, width/3, height/2, width-width/3, height/2, Justification::centred, true);
        // check for intermediate time of photo

        if (items[rowNumber].isTherePhoto)
            g.drawImageWithin (items[rowNumber].photo, 1, 1, width/3-1, height-1, RectanglePlacement::onlyReduceInSize | RectanglePlacement::centred);
        else
        {
            g.setFont (height/8);
            if (items[rowNumber].photoAddress.isEmpty ()) g.drawText ("No Photo", 1, 1, width/3-1, height-1, Justification::centred, true);
            else g.drawText ("Loading..", 1, 1, width/3-1, height-1, Justification::centred, true);
        }
    }

    void addFresh ()
    {
        items.push_back (ActorItem (actors->at (getNumRows ())));
        if (!items.rbegin ()->photoAddress.isEmpty ())
        {
            DownloadImageJob *temp = new DownloadImageJob (items.rbegin ()->photoAddress, items.size ()-1);
            temp->addListener (this);
            downloadPool.addJob (temp, false); // check for memory leak later
            jobs.push_back (temp);
        }
    }

    void exitSignalSent () override
    {
        const MessageManagerLock mmLock;
        //std::cout << "exitSingalSent\n";
        //std::cout << "jobs.size (): " << downloadPool.getNumJobs () << "\n";
        for (int i = 0; i < downloadPool.getNumJobs (); ++ i)
        {
            ThreadPoolJob *t = downloadPool.getJob (i);
            if (!t) continue;
            DownloadImageJob *temp = (DownloadImageJob*)t;
            if (temp->isFinished ())
            {
                int rowNumber = temp->getIndex ();
                if (items[rowNumber].isTherePhoto) continue;
                items[rowNumber].photo = temp->getImage ();
                items[rowNumber].isTherePhoto = true;
                listBox->repaintRow (rowNumber);
                break;
            }
        }

        for (auto it = jobs.begin (); it != jobs.end (); it ++)
        {
            if (!downloadPool.contains (*it))
            {
                delete *it;
                jobs.erase (it);
                it --;
            }
        }
    }

    void listWasScrolled () override
    {
        if (listBox->getVerticalPosition () > 0.70)
        {
            addFresh ();
            listBox->updateContent ();
            // change verticalPosition to stay still
        }
    }
private:
    std::vector <ActorItem> items;
    std::vector <var> *actors;
    ListBox *listBox;
    ThreadPool downloadPool;
    std::vector <DownloadImageJob*> jobs;
};

