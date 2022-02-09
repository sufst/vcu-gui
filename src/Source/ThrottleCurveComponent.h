/******************************************************************************
 * @file   ThrottleCurveComponent.h
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Component for drawing throttle curves
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>

#include "ThrottleCurveComponent.h"
#include "ThrottleCurve.h"

/**
 * @brief Throttle curve drawing component
 */
class ThrottleCurveComponent
    :   public juce::Component,
        public juce::KeyListener
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
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

private:
    
    ThrottleCurve throttleCurve;
    
    // GUI constants
    static const int lowerBarHeight = 20;
    static const int pointSize = 4;
    static const int pointStroke = 2;
    static const int clickRadius = 10 * 100;
    
    // state
    bool currentlyMovingPoint = false;
    bool deleteMode = false;
    ThrottleCurve::Point* pMovingPoint = nullptr;
    
    // internal utility
    juce::Point<int> transformCurvePointToCanvas(const ThrottleCurve::Point& point) const;
    ThrottleCurve::Point transformCanvasPointToCurve(const juce::Point<int>& point) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThrottleCurveComponent)
    
};
