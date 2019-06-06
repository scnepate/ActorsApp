/*
  ==============================================================================

    UpdateTask.h
    Created: 6 Jun 2019 5:56:37pm
    Author:  renyxa

  ==============================================================================
*/

#pragma once

class UpdateTask : public ThreadWithProgressWindow
{
public:
    UpdateTask () : ThreadWithProgressWindow ("Updating Database", true, false, 1000)
    {

    }

    void run ()
    {
        // first check for internet connection
        String api_endpoint = "https://api.themoviedb.org/3/person/popular?api_key=24fbfcd0f7c48ef7fc8efd5b73b559f3&language=en-US&page=1";
        URL url (api_endpoint);

        String downloadData = url.readEntireTextStream ();

        if (downloadData == "")
        {
            setStatusMessage ("No Connection!");
            AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
                                              "No Connection",
                                              "Please check your connection and try again...");
            return;
        }
        File assetsDirectory = File::getSpecialLocation (File::userApplicationDataDirectory).getChildFile ("data").getChildFile ("assets");
        if (!assetsDirectory.exists ())
        {
            assetsDirectory.createDirectory ();
        }

        File dataFile = assetsDirectory.getChildFile ("data.json");
        dataFile.create ();

    }
};
