/*
  ==============================================================================

    ThreadTasks.h
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
            downloadData = url.readEntireTextStream ();
            if (downloadData == "")
            {
                -- i;
                continue;
            }
            parsed = JSON::parse (downloadData);

            if (parsed.getProperty ("results", var::undefined) == var::undefined)
            {
                //std::cout << "undefined!\n" << std::flush;
                //std::cout << downloadData <<"\n";
                wait (10000);
                --i;
                continue;
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
//        std::cout << assetsDirectory.getFullPathName () << std::endl;

        if (!assetsDirectory.exists ())
        {
            assetsDirectory.createDirectory ();
        }

        File dataFile = assetsDirectory.getChildFile ("data.json");
        if (dataFile.exists ()) dataFile.deleteFile ();
        dataFile.create ();

        FileOutputStream out (dataFile);
        JSON::writeToStream (out, *resultData, true);
        delete resultData;
    }
};




class LoadActorsTask : public Thread
{
public:
    LoadActorsTask (std::vector <var> *actors, AlertWindow *alertWindow, double *progress) : actors (actors), alertWindow (alertWindow), progress (progress), Thread ("ActorsLoadingThread") {}

    void run ()
    {
        actors->clear ();
        File dataFile = File::getSpecialLocation (File::userApplicationDataDirectory).getChildFile ("data").getChildFile ("assets").getChildFile ("data.json");
        //if (!dataFile.exists ())
        //{
        //    return;
        //}

        FileInputStream fin (dataFile);
        String line = fin.readNextLine ();
        var parsed = JSON::parse (line);
        *progress = 0.5;

        int total_results = parsed.getProperty ("total_results", -1);
        int cnt = 1;

        for (auto i: *parsed.getProperty ("results", var::undefined).getArray ())
        {
            actors->push_back (i);
            *progress = (cnt ++)/(double)total_results;
        }
        wait (50);
        alertWindow->exitModalState (1);
    }

    bool isOk ()
    {
        File dataFile = File::getSpecialLocation (File::userApplicationDataDirectory).getChildFile ("data").getChildFile ("assets").getChildFile ("data.json");
        return dataFile.exists ();
    }
private:
    std::vector <var> *actors;
    AlertWindow *alertWindow;
    double *progress;
};


class DownloadImage : public Thread
{
public:
    static void downloadImage (String address, Image &image)
    {
        std::cout << "download: " << address << "\n";
    }
};