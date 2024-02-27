/*
 ==============================================================================
 
 AudioPreviewPanel.h
 Author:  Jonas Blome
 
 Some of the code in this class is adapted from the JUCE DemoRunner Demo
 that can be found under DemoRunner->Audio->AudioPlaybackDemo.h
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "BlomeHelpers.h"
#include "BlomeLookAndFeel.h"
#include "AudioPlayer.h"

/**
 Holds the AudioThumbnail and displays the audio's waveform.
 
 Handles drag and drop of audio files, playback of the audio and zooming into the waveform.
 */
class AudioPreviewPanel
:
public PanelBase,
public FileDragAndDropTarget,
public ChangeBroadcaster,
private ScrollBar::Listener,
private Timer
{
public:
    /**
     The constructor for the audio preview panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inSlider the zoom slider for the AudioThumbnail.
     @param inAudioPlayer the sample editor with the audio player.
     */
    AudioPreviewPanel(SaemplAudioProcessor& inProcessor,
                      Slider& inSlider,
                      AudioPlayer& inAudioPlayer);
    ~AudioPreviewPanel();
    /**
     Sets the URL of the audio source.
     
     @param url the URL of the file.
     */
    void setURL(URL const & url);
    /**
     @returns the URL of the last file that was dropped onto the preview panel.
     */
    URL getLastDroppedFile() const noexcept;
    /**
     Sets the zoom factor of the AudioThumbnail content.
     
     @param amount the zoom amount.
     */
    void setZoomFactor(double amount);
    /**
     Sets the visible range of the AudioThumbnail preview.
     
     @param newRange the range to set the preview to.
     */
    void setRange(Range<double> newRange);
    /**
     @returns whether the preview follows the playhead cursor.
     */
    bool getFollowsTransport();
    /**
     Sets if the preview should centre the playhead and follow the playback.
     
     @param shouldFollow true if it should follow, false if it should stay static.
     */
    void setFollowsTransport(bool shouldFollow);
    /**
     Shows the set audio resource in the preview.
     */
    void showAudioResource();
    /**
     Shows the given audio resource in the preview.
     
     @param inResource the URL to preview.
     */
    void showAudioResource(URL inResource);
    /**
     Start or stop the audio playback.
     */
    void startOrStop();
    /**
     Loads the given URL into the audio transport source.
     
     @param audioURL the URL to load into the source.
     
     @returns if the loading was successful.
     */
    bool loadURLIntoTransport(URL const & audioURL);
    /**
     Empties the audio resource and resets it.
     */
    void emptyAudioResource();
    
private:
    AudioPlayer& audioPlayer;
    Slider& zoomSlider;
    std::unique_ptr<ScrollBar> mAudioPreviewScrollbar;
    std::unique_ptr<AudioThumbnailCache> mThumbnailCache;
    std::unique_ptr<AudioThumbnail> mAudioPreview;
    Range<double> visibleRange;
    bool isFollowingTransport;
    URL lastFileDropped;
    std::unique_ptr<DrawableRectangle> mAudioPositionMarker;
    URL mCurrentAudioFile;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    void mouseDown(MouseEvent const & e) override;
    void mouseDrag(MouseEvent const & e) override;
    void mouseUp(MouseEvent const &) override;
    void mouseWheelMove(MouseEvent const &, MouseWheelDetails const & wheel) override;
    /**
     Converts the the playback time to the x value of the cursor.
     
     @param time the playback time in seconds
     
     @returns the x value of the cursor.
     */
    float timeToX(double const time) const;
    /**
     Converts the x value of the cursor to the playback time.
     
     @param x the x value
     
     @returns the time in seconds.
     */
    double xToTime(float const x) const;
    /**
     @returns whether the preview can be moved or if it's set to follow the playback.
     */
    bool canMoveTransport() const noexcept;
    void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void timerCallback() override;
    /**
     Updates the x position of the playback cursor.
     */
    void updateCursorPosition();
    void resizePanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPreviewPanel);
};
