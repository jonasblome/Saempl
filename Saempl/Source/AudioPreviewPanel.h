/*
  ==============================================================================

    AudioPreviewPanel.h
    Created: 27 May 2023 4:20:03pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class AudioPreviewPanel
:   public PanelBase,
    public ChangeListener,
    public FileDragAndDropTarget,
    public ChangeBroadcaster,
    private ScrollBar::Listener,
    private Timer
{
public:
    // Constructors
    AudioPreviewPanel(AudioFormatManager& formatManager, AudioTransportSource& source, Slider& inSlider);
    ~AudioPreviewPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelStyle();
    void setURL (const URL& url);
    URL getLastDroppedFile() const noexcept;
    void setZoomFactor (double amount);
    void setRange (Range<double> newRange);
    void setFollowsTransport (bool shouldFollow);
    void changeListenerCallback (ChangeBroadcaster* source) override;
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel) override;

private:
    AudioTransportSource& transportSource;
    Slider* mZoomSlider;
    std::unique_ptr<ScrollBar> mAudioPreviewScrollbar;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail mAudioPreview;
    Range<double> visibleRange;
    bool isFollowingTransport;
    URL lastFileDropped;
    DrawableRectangle mPositionMarker;
    
    // Methods
    float timeToX(const double time) const;
    double xToTime(const float x) const;
    bool canMoveTransport() const noexcept;
    void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
    void timerCallback() override;
    void updateCursorPosition();
};
