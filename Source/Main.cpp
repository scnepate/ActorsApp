/*
  =============================================================================

    This file was auto-generated

    It contains the basic startup code for a JUCE application

  =============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainContentComponent.h"

//=============================================================================
class ActorsAppApplication  : public JUCEApplication
{
public:
    //=============================================================================
    ActorsAppApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true;  }

    //=============================================================================
    void initialise (const String& commandLine) override
    { 
        // Add your application's initialisation code here.
        mainWindow.reset (new MainWindow (getApplicationName ()));
    }

    void shutdown() override
    {
        // Add your application's shutdown code here.
        mainWindow = nullptr;
    }

    //=============================================================================
    void systemRequestedQuit () override
    {
        // This is called when the app is being asked to quit: you can ignore thi
        // request and let the app carry on running, or call quit() to allow the app to close
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running
        // this method is invoked, and the commandLine parameter tells you wha
        // the other instance's command-line arguments were
    }

    class MainWindow : public DocumentWindow
    {
    public:
        MainWindow (String name) : DocumentWindow (name, Colours::lightgrey, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainContentComponent (), true);

            #if JUCE_IOS || JUCE_ANDROID
                setFullScreen (true);
            #els
                setResizable (true, true);
                centreWithSize (getWidth (), getHeight ());
            #endif

            setVisible (true);
            //AlertWindow::showMessageBoxAsync (AlertWindow::AlertIconType::WarningIcon, "No Connection", "Please check your connection and try again...")
        }

        void closeButtonPressed () override
        {
            JUCEApplication::getInstance ()->systemRequestedQuit ();
        }
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr <MainWindow> mainWindow;
};

//=============================================================================
// This macro generates the main() routine that launches the app
START_JUCE_APPLICATION (ActorsAppApplication)


