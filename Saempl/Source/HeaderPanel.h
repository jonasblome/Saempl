/*
 ==============================================================================
 
 HeaderPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "CentrePanel.h"
#include "FileFilterPanel.h"
#include "BlomeImageButton.h"
#include "AboutPanel.h"
#include "BlomeTransparentButton.h"

/**
 Holds the logo, and components for global functionality.
 
 Encompasses the refresh button, the file chooser button,
 the navigation panel toggles and the filter settings.
 */
class HeaderPanel
:
public PanelBase
{
public:
    /**
     The constructor the plugins header panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inCentrePanel the panel for the central plugin UI content.
     */
    HeaderPanel(SaemplAudioProcessor& inProcessor, CentrePanel& inCentrePanel);
    ~HeaderPanel();
    bool keyPressed(KeyPress const & key) override;
    
private:
    SampleLibrary & sampleLibrary;
    CentrePanel & centrePanel;
    std::unique_ptr<BlomeImageButton> mRefreshLibraryButton;
    std::unique_ptr<BlomeImageButton> mChooseLibraryDirectoryButton;
    std::unique_ptr<FileChooser> mFileChooser;
    std::unique_ptr<BlomeImageButton> mToggleSampleFolderPanelButton;
    std::unique_ptr<BlomeImageButton> mToggleSampleTablePanelButton;
    std::unique_ptr<BlomeImageButton> mToggleSampleGridPanelButton;
    std::unique_ptr<BlomeImageButton> mChangeFilterButton;
    std::unique_ptr<BlomeImageButton> mToggleFilterButton;
    std::unique_ptr<BlomeImageButton> mRandomSampleButton;
    std::unique_ptr<BlomeImageButton> mNormaliseVolumeButton;
    std::unique_ptr<BlomeImageButton> mChooseOutputDeviceButton;
    std::unique_ptr<Slider> mGainSlider;
    std::unique_ptr<BlomeTransparentButton> mShowAboutPanelButton;
    std::unique_ptr<AudioDeviceManager> mAudioDeviceManager;
    static int const logoWidth = 110;
    static int const logoMarginRight = 20;
    static int const sliderWidth = 100;
    static int const connectionLineWidth = 10;
    static int const connectionLineHeight = 2;
    constexpr static float const connectionLineRadius = 1.0;
    
    void paint(Graphics& g) override;
    void setRefreshLibraryButton(int buttonWidth, int x);
    void setChooseLibraryDirectoryButton(int buttonWidth, int x);
    void setToggleSampleFolderPanelButton(int buttonWidth, int x);
    void setToggleSampleTablePanelButton(int buttonWidth, int x);
    void setToggleSampleGridPanelButton(int buttonWidth, int x);
    void setChangeFilterButton(int buttonWidth, int x);
    void setToggleFilterButton(int buttonWidth, int x);
    void setRandomSampleButton(int buttonWidth, int x);
    void setNormaliseVolumeButton(int buttonWidth);
    void setChooseOutputDeviceButton(int buttonWidth);
    void setGainSlider();
    void setShowAboutPanelButton();
    void setPanelComponents() override;
    /**
     Shows the dialogue for choosing the sample library directory.
     */
    void showLibraryChooser();
    void resizePanelComponents() override;
    /**
     Selects an output device for the audio players.
     */
    void selectOutputDevice(String inDeviceName);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel);
};
