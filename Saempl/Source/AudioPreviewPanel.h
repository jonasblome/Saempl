/*
  ==============================================================================

    AudioPreviewPanel.h
    Created: 27 May 2023 4:20:03pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeHelpers.h"
#include "SampleEditor.h"

/**
 Holds the \ref AudioThumbnail and displays the audio's waveform.
 
 Implements:
 \ref PanelBase
 \ref juce::FileDragAndDropTarget
 \ref juce::DragAndDropTarget
 \ref juce::ChangeBroadcaster
 \ref juce::ScrollBar::Listener
 \ref juce::Timer
 Handles drag and drop of audio files, playback of the audio and zooming into the waveform.
 */
class AudioPreviewPanel
:   public PanelBase,
    public FileDragAndDropTarget,
    public DragAndDropTarget,
    public ChangeBroadcaster,
    private ScrollBar::Listener,
    private Timer
{
public:
    /**
     The constructor for the audio preview panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inSlider the zoom slider for the \ref AudioThumbnail.
     @param inSampleEditor the sample editor with the audio player.
     */
    AudioPreviewPanel(SaemplAudioProcessor& inProcessor, Slider& inSlider, SampleEditor& inSampleEditor);
    ~AudioPreviewPanel();
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    bool isInterestedInDragSource(SourceDetails const& dragSourceDetails) override;
    void itemDropped(SourceDetails const & dragSourceDetails) override;
    void mouseDown(MouseEvent const & e) override;
    void mouseDrag(MouseEvent const & e) override;
    void mouseUp(MouseEvent const &) override;
    void mouseWheelMove(MouseEvent const &, MouseWheelDetails const & wheel) override;
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
     Sets the zoom factor of the \ref AudioThumbnail content.
     
     @param amount the zoom amount.
     */
    void setZoomFactor(double amount);
    /**
     Sets the visible range of the \ref AudioThumbnail preview.
     
     @param newRange the range to set the preview to.
     */
    void setRange(Range<double> newRange);
    /**
     Sets if the preview should center the playhead and follow the playback.
     
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
    // JUCE_HEAVYWEIGHT_LEAK_DETECTOR(AudioPreviewPanel)
    
    SaemplAudioProcessor& currentProcessor;
    SampleEditor& sampleEditor;
    Slider* mZoomSlider;
    std::unique_ptr<ScrollBar> mAudioPreviewScrollbar;
    AudioThumbnailCache mThumbnailCache;
    AudioThumbnail mAudioPreview;
    Range<double> visibleRange;
    bool isFollowingTransport;
    URL lastFileDropped;
    DrawableRectangle mAudioPositionMarker;
    URL mCurrentAudioFile;
    
    float timeToX(double const time) const;
    double xToTime(float const x) const;
    bool canMoveTransport() const noexcept;
    void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void timerCallback() override;
    void updateCursorPosition();
};
