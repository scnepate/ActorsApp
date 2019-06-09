/*
  ==============================================================================

    ThreadTasks.h
    Created: 6 Jun 2019 5:56:37pm
    Author:  renyxa

  ==============================================================================
*/
#pragma once

class DownloadImageJob : public ThreadPoolJob
{
public:
    DownloadImageJob (String &iA, int ind) : imageAddress (iA), ind (ind), ThreadPoolJob (iA), hasFinished (false) {}

    JobStatus runJob () override
    {
        // here we need to download image and assign it to *image.
        URL url ("https://image.tmdb.org/t/p/w185" + imageAddress);
        InputStream *inp = url.createInputStream (true);
        JPEGImageFormat jpg;
        image = jpg.decodeImage (*inp);
        delete inp;
        hasFinished = true;
        //std::cout << "DownloadImageJob: " << imageAddress << "\n" << std::flush;
        signalJobShouldExit ();
        return jobHasFinished;
    }

    Image getImage () { return image; }
    int getIndex () { return ind; }
    bool isFinished () { return hasFinished; }
private:
    String imageAddress;
    Image image;
    int ind;
    bool hasFinished;
};



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
        int numberOfPages = parsed.getProperty ("total_pages", var::null);

        Array<var>* results = parsed.getProperty ("results", var::null).getArray ();
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

            if (parsed.getProperty ("results", var::null) == var::null)
            {
                //std::cout << "undefined!\n" << std::flush;
                //std::cout << downloadData <<"\n";
                wait (10000);
                --i;
                continue;
            }
            Array<var>* results = parsed.getProperty ("results", var::null).getArray ();
            for (auto j: *results)
            {
                j.getDynamicObject ()->removeProperty ("known_for");
                resultData->getProperty ("results", var::null).append (j);
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


// add load from file here