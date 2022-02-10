/******************************************************************************
 * @file   ThrottleCurveComponent.cpp
 * @author Tim Brewis (tab1g19@soton.ac.uk)
 * @date   2022-02-09
 * @brief  Component for drawing throttle curves
 *****************************************************************************/

#include <JuceHeader.h>
#include <algorithm>
#include <cmath>

#include "ThrottleCurveComponent.h"

//==================================================== Constructor / destructor

/**
 * @brief Default constructor
 */
ThrottleCurveComponent::ThrottleCurveComponent()
{
    // setup component
    setWantsKeyboardFocus(true);
    setMouseCursor(juce::MouseCursor::CrosshairCursor);
    addKeyListener(this);

    // setup appearance
    backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    borderColour = getLookAndFeel().findColour(juce::ComboBox::outlineColourId);
}

/**
 * @brief Destructor
 */
ThrottleCurveComponent::~ThrottleCurveComponent()
{
    // nothing to do
}

//==================================================================== Graphics

/**
 * @brief Painter
 *
 * @param[in]   g   Graphics context
 */
void ThrottleCurveComponent::paint(juce::Graphics& g)
{
    // fill background
    g.fillAll(backgroundColour);
    
    // draw graph ticks
    g.setColour(juce::Colours::darkgrey);
    int numTicksX = 40;
    int numTicksY = 20;
    
    for (int i = 0; i < numTicksX; i++)
    {
        float x = i * getWidth() / numTicksX;
        g.drawLine(x, 0, x, getHeight());
    }
    
    for (int i = 0; i < numTicksY; i++)
    {
        float y = i * getHeight() / numTicksY;
        g.drawLine(0, y, getWidth(), y);
    }
    
    // draw border around the component
    g.setColour(borderColour);
    g.drawRect(0, 0, getWidth(), getHeight(), borderThickness);
    
    // draw interpolated points
    g.setColour(juce::Colours::white);
    juce::Path path;

    for (int input = 0; input < ThrottleCurve::getInputMax(); input += 1)
    {
        const auto interpolatedPoint = throttleCurve.getInterpolatedPoint(input);
        const auto transformedPoint = transformCurvePointToCanvas(interpolatedPoint);
        
        if (input == 0)
        {
            path.startNewSubPath(transformedPoint.toFloat());
        }
        else
        {
            path.lineTo(transformedPoint.toFloat());
        }
    }

    g.strokePath(path, juce::PathStrokeType(1));

    // draw points
    g.setColour(juce::Colours::orange);

    for (const auto& point : throttleCurve.getPoints())
    {
        auto realPoint = transformCurvePointToCanvas(point);
        g.drawEllipse(realPoint.getX(), realPoint.getY(), pointSize, pointSize, pointStroke);
    }
    
    // draw deadzone
    auto firstPoint = transformCurvePointToCanvas(throttleCurve.getPoints().getFirst());
    
    deadzoneLine.setStart(firstPoint.getX(), borderThickness);
    deadzoneLine.setEnd(firstPoint.getX(), getHeight() - borderThickness * 2);
    
    if (deadzoneLine.getStartX() > 0)
    {
        g.setColour(deadzoneLineColour.withLightness(0.9f).withAlpha(0.2f));
        g.fillRect(borderThickness, borderThickness, deadzoneLine.getStartX(), getHeight() - borderThickness * 2);
        g.setColour(deadzoneLineColour);
        g.drawLine(deadzoneLine.toFloat(), 1);
    }
}

/**
 * @brief Resize handler
 */
void ThrottleCurveComponent::resized()
{
    // nothing to do
}

//====================================================================== Events

/**
 * @brief Handle a mouse down event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseDown(const juce::MouseEvent& event)
{
    // check for starting deadzone mode
    if (!deleteMode && !currentlyMovingDeadzone)
    {
        if (deadzoneHitTest(event.getPosition()))
        {
            currentlyMovingDeadzone = true;
        }
    }
    
    // check if a point already exists in the clicked location
    // if one does and not deleting it, begin a move
    if (!deleteMode && !currentlyMovingDeadzone)
    {
        for (int i = 0; i < throttleCurve.getPoints().size(); i++)
        {
            // can move any point except the first which is mapped to the deadzone
            if (pointHitTest(event.getPosition(), throttleCurve.getPoints()[i])
                && i != 0)
            {
                pMovingPoint = throttleCurve.getPointForMove(i);
                currentlyMovingPoint = true;
                setMouseCursor(juce::MouseCursor::DraggingHandCursor);
                break;
            }
        }
    }
    
    // add / delete if not moving
    if (!currentlyMovingPoint && !currentlyMovingDeadzone)
    {
        // delete
        if (deleteMode)
        {
            ThrottleCurve::Point point = transformCanvasPointToCurve(event.getPosition());
            throttleCurve.deleteNearbyPoints(point, throttleCurveClickRadius);
        }
        // add
        else
        {
            auto point = transformCanvasPointToCurve(event.getPosition());
            throttleCurve.addPoint(point);
        }
    }
    
    // trigger a re-paint
    repaint();
}



/**
 * @brief Handle a mouse up event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseUp(const juce::MouseEvent& event)
{
    // stop moving points
    if (currentlyMovingPoint)
    {
        currentlyMovingPoint = false;
        pMovingPoint = nullptr;
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
    }
    else if (currentlyMovingDeadzone)
    {
        currentlyMovingDeadzone = false;
        setMouseCursor(juce::MouseCursor::CrosshairCursor);
    }
}

/**
 * @brief Handle a mouse drag event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseDrag(const juce::MouseEvent& event)
{
    // move a point
    if (currentlyMovingPoint && pMovingPoint != nullptr)
    {
        ThrottleCurve::Point point = transformCanvasPointToCurve(event.getPosition());
        pMovingPoint->setXY(point.getX(), point.getY());
        pMovingPoint = throttleCurve.pointMoved(*pMovingPoint);
    }
    // move deadzone
    if (currentlyMovingDeadzone)
    {
        int x = transformCanvasPointToCurve(event.getPosition()).getX();
        int xLim = throttleCurve.getPoints().getReference(1).getX() - 10;
        
        // restrict movement
        if (x < 0)
        {
            x = 0;
        }
        else if (x > xLim)
        {
            x = xLim;
        }
        
        // move point
        ThrottleCurve::Point* deadzoneStart = throttleCurve.getPointForMove(0);
        deadzoneStart->setXY(x, 0);
        throttleCurve.pointMoved(*deadzoneStart);
            
    }
    
    // trigger a re-paint
    repaint();
}

/**
 * @brief Handle a mouse move event
 *
 * @param[in]   event   Mouse event
 */
void ThrottleCurveComponent::mouseMove(const juce::MouseEvent& event)
{
    // change mouse cursor if a point is within the grab radius
    if (!deleteMode && !currentlyMovingPoint)
    {
        bool pointHit = false;
        bool deadzoneHit = false;
        
        // deadzone hit test
        if (deadzoneHitTest(event.getPosition()))
        {
            deadzoneHit = true;
        }
        // point hit test
        else
        {
            for (const auto& point : throttleCurve.getPoints())
            {
                // check for point hits
                if (pointHitTest(event.getPosition(), point))
                {
                    pointHit = true;
                    break;
                }
            }
        }
            
        // change cursor
        if (deadzoneHit)
        {
            setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        }
        else if (pointHit)
        {
            setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        }
        else
        {
            setMouseCursor(juce::MouseCursor::CrosshairCursor);
        }
    }
}

/**
 * @brief Handle a key press event
 *
 * @param[in]   key                     Key pressed
 * @param[in]   originatingComponent    Component from which the key press originated
 */
bool ThrottleCurveComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    // toggle delete mode
    if (key.isKeyCode(juce::KeyPress::backspaceKey))
    {
        deleteMode = !deleteMode;
        
        if (deleteMode)
        {
            setMouseCursor(juce::MouseCursor(juce::ImageCache::getFromMemory(BinaryData::Delete_png, BinaryData::Delete_pngSize), 1, 7, 5));
        }
        
        else
        {
            setMouseCursor(juce::MouseCursor::CrosshairCursor);
        }
    }
    else if (key.isKeyCode(juce::KeyPress::escapeKey))
    {
        throttleCurve.reset();
        repaint();
    }
    return true;
}

/**
 * @brief       Determines whether component will receive file dragging related callbacks
 *
 * @param[in]   files   List of files being dragged
 */
bool ThrottleCurveComponent::isInterestedInFileDrag(const juce::StringArray &files)
{
    return true;
};

/**
 * @brief       Receive dropped files
 *
 * @param[in]   files   List of dropped files
 * @param[in]   x       Mouse x position relative to component
 * @param[in]   y       Mouse y position relative to component
 */
void ThrottleCurveComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    // reset background and border
    backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    borderColour = getLookAndFeel().findColour(juce::ComboBox::outlineColourId);
    borderThickness = 1;
    
    // get result and load profile
    juce::File mapFile(files.getReference(0));
    loadProfile(mapFile);
    
    // need to repaint GUI
    repaint();
}

/**
 * @brief       Callback when file drag enters component
 *
 * @param[in]   files   List of files being dragged
 * @param[in]   x       Mouse x position relative to component
 * @param[in]   y       Mouse y position relative to component
 */
void ThrottleCurveComponent::fileDragEnter(const juce::StringArray& files, int x, int y)
{
    // brighten the background and border when file drag enters component
    backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(fileDragBrightnessFactor);
    borderColour = juce::Colours::skyblue;
    borderThickness = 2;
    
    // need to repaint GUI
    repaint();
}

/**
 * @brief       Callback when file drag exits component
 *
 * @param[in]   files   List of files being dragged
 */
void ThrottleCurveComponent::fileDragExit(const juce::StringArray &files)
{
    // reset background and border
    backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    borderColour = getLookAndFeel().findColour(juce::ComboBox::outlineColourId);
    borderThickness = 1;
    
    // need to repaint GUI
    repaint();
};

//========================================================= Interface to parent

/**
 * @brief       Sets the interpolation method used by the throttle curve
 *
 * @param[in]   method      Interpolation method
 */
void ThrottleCurveComponent::setInterpolationMethod(ThrottleCurve::InterpolationMethod method)
{
    throttleCurve.setInterpolationMethod(method);
    repaint();
}

/**
 * @brief Called by parent component to import a driver profile on button click
 */
void ThrottleCurveComponent::importProfile() {
    
    // create and setup a new file chooser
    fileChooser = std::make_unique<juce::FileChooser> ("Open throttle profile map",
                                                       juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                                                       "*.xml",
                                                       true);

    auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles
                            | juce::FileBrowserComponent::openMode;

    // launch file chooser asynchronously
    fileChooser->launchAsync(fileChooserFlags, [this] (const juce::FileChooser& chooser)
    {
        juce::File mapFile = chooser.getResult();
        
        if (mapFile.existsAsFile())
        {
            loadProfile(mapFile);
        }
    });
    
};

/**
 * @brief Called by parent component to export a driver profile on button click
 */
void ThrottleCurveComponent::exportProfile()
{
    // create top level element
    juce::XmlElement throttleMap("throttle_map");

    // second level elements
    juce::XmlElement* config = new juce::XmlElement("config");
    juce::XmlElement* pointsList = new juce::XmlElement("points");

    // fill config element
    // create option element
    juce::XmlElement* option = new juce::XmlElement("option");

    // interpolation method element
    juce::String interpolationMethod = throttleCurve.getInterpolationMethodName(throttleCurve.getInterpolationMethod());
    option->setAttribute("interpolation_method", interpolationMethod);
    config->addChildElement(option);

    // fill points element
    for (const auto& point : throttleCurve.getPoints()) {
        
        // create inner point element
        juce::XmlElement* pointElement = new juce::XmlElement("point");

        pointElement->setAttribute("x", point.getX());
        pointElement->setAttribute("y", point.getY());

        // add point element to parent
        pointsList->addChildElement(pointElement);
    }

    // add child elements to top level parent
    throttleMap.addChildElement(config);
    throttleMap.addChildElement(pointsList);

    // create and setup file chooser
    fileChooser = std::make_unique<juce::FileChooser> ("Save throttle profile map",
                                                       juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                                                       "*.xml",
                                                       true);

    auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles
                            | juce::FileBrowserComponent::warnAboutOverwriting
                            | juce::FileBrowserComponent::saveMode;

    // launch file chooser asynchronously
    fileChooser->launchAsync(fileChooserFlags, [this, throttleMap] (const juce::FileChooser& chooser)
    {
        // get result
        juce::File mapFile = chooser.getResult();
        
        // validate file
        if (!mapFile.existsAsFile())
        {
            return;
        }

        // write XML file to disk
        if (throttleMap.writeTo(mapFile, {}))
        {
            // validate the curve and get warnings
            juce::StringArray warnings = throttleCurve.validateCurve();
            juce::String alert = "Exported throttle curve to file.";
            
            if (warnings.size() > 0)
            {
                alert += "\n\n";
                for (const auto& warning : warnings)
                {
                    alert += warning + ".\n";
                }
                juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Info", alert);
            }
            else
            {
                juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Info", alert);
            }
        }
        else
        {
            // show a failure dialog
            juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Error", "Error exporting map profile");
        }
    });
    
};

/**
 * @brief Export the throttle curve to C code
 */
void ThrottleCurveComponent::exportCode()
{
    // generate interpolated points for every input
    juce::Array<int> outputs;
    
    for (int input = 0; input < ThrottleCurve::getInputMax() + 1; input++)
    {
        int toAdd = throttleCurve.getInterpolatedPoint(input).getY();
        
        // apply clipping
        if (toAdd <= ThrottleCurve::getOutputMax())
        {
            outputs.add(throttleCurve.getInterpolatedPoint(input).getY());
        }
        else
        {
            outputs.add(ThrottleCurve::getOutputMax());
        }
    }
    
    // generate code text
    int newLineEvery = 16;
    
    juce::String code = "static const uint16_t driver_profile [";
    code += juce::String(ThrottleCurve::getInputMax() + 1);
    code += "] = {\n\t";
    
    for (int i = 0; i < ThrottleCurve::getInputMax() + 1; i++)
    {
        // fixed length hex string
        juce::String hex = juce::String::toHexString(outputs.getReference(i));

        int leadingZeros = 4 - hex.length();
        hex = juce::String::repeatedString("0", leadingZeros) + hex;
        
        // add hex to code
        code += "0x" + hex + ", ";
        
        // new lines every so often for readability
        if ((i % newLineEvery == newLineEvery - 1) && i != ThrottleCurve::getInputMax())
        {
            code += "\n\t";
        }
    }
    
    code.dropLastCharacters(1);
    code += "\n};";
    
    // copy the code to the clipboard
    juce::SystemClipboard::copyTextToClipboard(code);
    
    // validate the curve and get warnings
    juce::StringArray warnings = throttleCurve.validateCurve();
    
    // generate an alert window
    juce::String alert = "Lookup table code copied to clipboard.";
    
    if (warnings.size() > 0)
    {
        alert += "\n\n";
        for (const auto& warning : warnings)
        {
            alert += warning + ".\n";
        }
        juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Info", alert);
    }
    else
    {
        juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Info", alert);
    }
    
}

//============================================================ Internal utility

/**
 * @brief Transforms a point on a throttle curve to its position on the component
 *
 * @param[in]   point   Point on the throttle curve
 *
 * @return Position to draw the point on the component
 */
juce::Point<int> ThrottleCurveComponent::transformCurvePointToCanvas(const ThrottleCurve::Point& point) const
{
    int x = getWidth() * (static_cast<float>(point.getX()) / static_cast<float>(ThrottleCurve::getInputMax()));
    int y = getHeight() * (1 - static_cast<float>(point.getY()) / static_cast<float>(ThrottleCurve::getOutputMax()));
    
    return juce::Point<int>(x, y);
}

/**
 * @brief Transforms a point on a throttle curve to its position on the component
 *
 * @param[in]   point   Point on the component
 *
 * @return Position to place the point on the throttle curve
 */
ThrottleCurve::Point ThrottleCurveComponent::transformCanvasPointToCurve(const juce::Point<int>& point) const
{
    int x = ThrottleCurve::getInputMax() * (static_cast<float>(point.getX()) / static_cast<float>(getWidth()));
    int y = ThrottleCurve::getOutputMax() * (1 - static_cast<float>(point.getY()) / static_cast<float>(getHeight()));
    
    return ThrottleCurve::Point(x, y);
}

/**
 * @brief Hit test between a point on the canvas and a point on the curve
 */

bool ThrottleCurveComponent::pointHitTest(const juce::Point<int>& canvasPoint, const ThrottleCurve::Point& curvePoint) const
{
    juce::Point<int> transformedCurvePoint = transformCurvePointToCanvas(curvePoint);
    return (canvasPoint.getDistanceFrom(transformedCurvePoint) < clickRadius);
}

/**
 * @brief Hit test for the deadzone
 */
bool ThrottleCurveComponent::deadzoneHitTest(const juce::Point<int>& canvasPoint) const
{
    return (canvasPoint.getX() <= deadzoneLine.getStartX());
}

/**
 * @brief   Load a throttle profile
 *
 * @param[in]   mapFile     Path to throttle map file
 */
void ThrottleCurveComponent::loadProfile(juce::File mapFile)
{
    // create new XML document
    static std::unique_ptr<juce::XmlElement> mapRoot = juce::XmlDocument::parse(mapFile);

    // validate file
    if  (mapRoot == nullptr)
        return;

    if (!mapRoot->hasTagName("throttle_map"))
        return;

    // search for elements by name
    for (auto* e : mapRoot->getChildIterator())
    {
        if (e->hasTagName("config"))
        {
            for (auto* p : e->getChildIterator())
            {
                if (p->hasAttribute("interpolation_method"))
                {
                    // set interpolation type
                    juce::String interpolationMethodName = p->getStringAttribute("interpolation_method");

                    for (const auto& method : ThrottleCurve::getAllInterpolationMethods())
                    {
                        if (interpolationMethodName == ThrottleCurve::getInterpolationMethodName(method))
                        {
                            setInterpolationMethod(method);
                            break;
                        }
                    }
                    break;
                }
            }

        }
        else if (e->hasTagName("points"))
        {
            // clear all old points
            throttleCurve.reset();

            // loop through all points
            for (auto* p : e->getChildIterator())
            {
                // check element is a point
                if (!p->hasTagName("point"))
                    return;

                // create point
                int pointX = p->getIntAttribute("x");
                int pointY = p->getIntAttribute("y");

                ThrottleCurve::Point point(pointX, pointY);

                // add point
                throttleCurve.addPoint(point);
            }

            // need to repaint GUI after loading new points
            this->repaint();
            
            // need to inform parent component of changes via callback
            if (onProfileLoad != nullptr)
            {
                onProfileLoad(throttleCurve.getInterpolationMethod());
            }
        
        }
    }
};
