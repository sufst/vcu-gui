/******************************************************************************
 * @file   MainWindow.cpp
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main GUI window
 *****************************************************************************/

#include "MainWindow.h"

#include "../components/MainComponent.h"
#include <climits>

namespace gui
{

/**
 * @brief       Constructor
 *
 * @param[in]   name    Window name
 */
MainWindow::MainWindow(const juce::String& name)
    : juce::DocumentWindow(
        name,
        juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    setResizeLimits(minWidth, minHeight, INT_MAX, INT_MAX);
    setResizable(true, false);
    setVisible(true);
    centreWithSize(getWidth(), getHeight());

    setContentOwned(new MainComponent(), true);
}

/**
 * @brief Close button pressed handler
 */
void MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

} // namespace gui