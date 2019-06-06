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
    UpdateTask () : ThreadWithProgressWindow ("Updating Database", true, true, 250) {}

    void run ()
    {
        // first check for internet connection
        String api_endpoint = "https://api.themoviedb.org/3/person/popular?api_key=24fbfcd0f7c48ef7fc8efd5b73b559f3&language=en-US&page=1";
        URL url (api_endpoint);

        String downloadData = url.readEntireTextStream ();

        if (downloadData == "")
        {
            //setStatusMessage ("No Connection!");
            AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
                                              "No Connection",
                                              "Please check your connection.");
            return;
        }

        var parsed = JSON::parse (downloadData);
        var* resultData = new var (parsed);
        resultData->getDynamicObject ()->removeProperty ("page");
        int numberOfPages = parsed.getProperty ("total_pages", var::undefined);

        Array<var>* results = parsed.getProperty ("results", var::undefined).getArray ();
        for (auto j: *results)
            j.getDynamicObject ()->removeProperty ("known_for");


        for (int i = 2; i <= numberOfPages && !threadShouldExit (); ++ i)
        {
            api_endpoint = "https://api.themoviedb.org/3/person/popular?api_key=24fbfcd0f7c48ef7fc8efd5b73b559f3&language=en-US&page=" + var (i).toString ();
            url = URL (api_endpoint);
            parsed = JSON::parse (url.readEntireTextStream ());

            if (downloadData == "")
            {
                setStatusMessage ("No Connection!");
                AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon,
                                                  "No Connection",
                                                  "Please check your connection");
                return;
            }

            Array<var>* results = parsed.getProperty ("results", var::undefined).getArray ();

            for (auto j: *results)
            {
                j.getDynamicObject ()->removeProperty ("known_for");
                resultData->getProperty ("results", var::undefined).append (j);
            }

            setProgress (i/(float)numberOfPages);
        }

        if (threadShouldExit ())
        {
            return;
        }

        File assetsDirectory = File::getSpecialLocation (File::userApplicationDataDirectory).getChildFile ("data").getChildFile ("assets");
        //std::cout << assetsDirectory.getFullPathName () << std::endl;

        if (!assetsDirectory.exists ())
        {
            assetsDirectory.createDirectory ();
        }

        File dataFile = assetsDirectory.getChildFile ("data.json");

        if (!dataFile.exists ())
            dataFile.create ();

        FileOutputStream out (dataFile);
        JSON::writeToStream (out, *resultData, false);
    }
};
