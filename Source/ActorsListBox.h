/*
  ==============================================================================

    ActorsListBox.h
    Created: 7 Jun 2019 3:36:18am
    Author:  renyxa

  ==============================================================================
*/

#pragma once
#include <vector>

class ActorsListBoxModel : public ListBoxModel
{
public:
    ActorsListBoxModel ()
    {
    }
    ~ActorsListBoxModel ()
    {
    }

    int getNumRows () override
    {
        return items.size ();
    }

    void paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) override
    {

    }

    void addFresh ()
    {

    }
private:
    std::vector <ActorItem> items;

    struct ActorItem : public Component
    {
        ActorItem () {}
        ~ActorItem () {}

        Image photo;
        String name;
        float popularity;
    };
};

class ActorsListBox : public ListBox, public ScrollBar::Listener
{
public:
    ActorsListBox () {}
    ~ActorsListBox () {}

    void scrollBarMoved (ScrollBar *scrollBar, double newRangeStart) override
    {

    }
private:
    ActorsListBoxModel actorsListBoxModel;
};