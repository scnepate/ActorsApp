/*
  ==============================================================================

    ThreadTasks.h
    Created: 6 Jun 2019 5:56:37pm
    Author:  renyxa

  ==============================================================================
*/
#pragma once

class DownloadImage : public Thread
{
public:
    DownloadImage () : Thread ("downlaodImage") {}
    void run () override
    {
        //File f ("~/Desktop/hello.jpg");
        //url.downloadToFile (f);
        InputStream *inp = url.createInputStream (true);
        JPEGImageFormat jpg;
        newImage = jpg.decodeImage (*inp);
        delete inp;
        //std::cout << "width: " << newImage.getWidth () << " height: " << newImage.getHeight () << "\n";
    }
    void downloadImage (String address, Image &image)
    {
        //std::cout << "download from: " << "https://image.tmdb.org/t/p/w185" + address << "\n";
        url = URL ("https://image.tmdb.org/t/p/w185" + address); // w45
        startThread ();
        while (isThreadRunning ());
        image = newImage;
        //std::cout << "width: " << image.getWidth () << " height: " << image.getHeight () << "\n";
    }
private:
    URL url;
    Image newImage;
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




