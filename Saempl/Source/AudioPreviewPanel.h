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

class AudioPreviewPanel
:   public PanelBase,
    public FileDragAndDropTarget,
    public DragAndDropTarget,
    public ChangeBroadcaster,
    private ScrollBar::Listener,
    private Timer
{
public:
    // Constructors
    AudioPreviewPanel(SaemplAudioProcessor& inProcessor, Slider& inSlider, SampleEditor& inSampleEditor);
    ~AudioPreviewPanel();
    
    // Methods
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
    void setURL(URL const & url);
    URL getLastDroppedFile() const noexcept;
    void setZoomFactor(double amount);
    void setRange(Range<double> newRange);
    void setFollowsTransport(bool shouldFollow);
    void showAudioResource();
    void showAudioResource(URL inResource);
    void startOrStop();
    bool loadURLIntoTransport(URL const & audioURL);
    void updateFollowTransportState();

private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(AudioPreviewPanel)
    
    // Fields
    int sampleItemTitleHeight = 25;
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
    
    // Methods
    float timeToX(double const time) const;
    double xToTime(float const x) const;
    bool canMoveTransport() const noexcept;
    void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void timerCallback() override;
    void updateCursorPosition();
};
