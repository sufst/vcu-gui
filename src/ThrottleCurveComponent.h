/******************************************************************************
 * @file   ThrottleCurveComponent.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @author Max O'Brien (mao1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Component for drawing throttle curves
 *****************************************************************************/

#pragma once

#include "ThrottleCurve.h"
#include "ThrottleCurveComponent.h"
#include <JuceHeader.h>
#include <cmath>
#include <functional>

/**
 * @brief Throttle curve drawing component
 */
class ThrottleCurveComponent : public juce::Component,
                               public juce::KeyListener,
                               public juce::FileDragAndDropTarget
{
public:

    // constructor / destructor
    ThrottleCurveComponent();
    ~ThrottleCurveComponent() override;

    // painting
    void paint(juce::Graphics&) override;
    void resized() override;

    // event handlers
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;
    bool keyPressed(const juce::KeyPress& key,
                    juce::Component* originatingComponent) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void fileDragEnter(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;

    // interface to parent component
    void setInterpolationMethod(ThrottleCurve::InterpolationMethod method);
    void importProfile();
    void exportProfile();
    void exportCode();

    // callbacks
    std::function<void(ThrottleCurve::InterpolationMethod)> onProfileLoad
        = nullptr;

private:

    ThrottleCurve throttleCurve;
    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::TooltipWindow> toolTip;

    // GUI constants
    static const int lowerBarHeight = 20;
    static const int pointSize = 4;
    static const int pointStroke = 4;
    static const int clickRadius = 20;
    static const int minDeadzoneToPointDistance = 5;

    // appearance
    juce::Colour backgroundColour;
    juce::Colour borderColour;
    juce::Colour deadzoneLineColour = juce::Colours::skyblue;
    int borderThickness = 1;
    static constexpr float fileDragBrightnessFactor = 0.05f;

    // state
    bool currentlyMovingPoint = false;
    bool currentlyMovingDeadzone = false;
    bool deleteMode = false;
    ThrottleCurve::Point* pMovingPoint = nullptr;

    juce::Line<int> deadzoneLine;

    // internal utility
    juce::Point<int>
    transformCurvePointToCanvas(const ThrottleCurve::Point& point) const;
    ThrottleCurve::Point
    transformCanvasPointToCurve(const juce::Point<int>& point) const;
    bool pointHitTest(const juce::Point<int>& canvasPoint,
                      const ThrottleCurve::Point& curvePoint) const;
    bool deadzoneHitTest(const juce::Point<int>& canvasPoint) const;
    void loadProfile(juce::File mapFile);
    void showToolTip()
    {
        if (toolTip == nullptr)
        {
            toolTip.reset(new juce::TooltipWindow(this, 0));
        }

        int x = this->getScreenX() + deadzoneLine.getStartX() - 10;
        int y = juce::Desktop::getMousePosition().getY();

        juce::String tipText
            = juce::String::toDecimalStringWithSignificantFigures(
                100
                    * static_cast<float>(
                        throttleCurve.getPoints().getFirst().getX())
                    / ThrottleCurve::getInputMax(),
                2);
        tipText += "%";
        toolTip->displayTip(juce::Point<int>(x, y), tipText);
        toolTip->setVisible(true);
    };
    void hideToolTip()
    {
        if (toolTip != nullptr)
        {
            toolTip->hideTip();
            toolTip.reset();
        }
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurveComponent)
};
