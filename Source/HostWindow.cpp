#include "HostWindow.hpp"

HostWindow::HostWindow(juce::String name, juce::AudioProcessorEditor *editor)
    : juce::DialogWindow(name, juce::Colour::fromRGB(100, 100, 100), false) {
    centreWithSize(300, 200);
    setContentOwned(editor, true);
}

void HostWindow::closeButtonPressed() { this->setVisible(false); }