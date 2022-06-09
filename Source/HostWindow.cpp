#include "HostWindow.hpp"

HostWindow::HostWindow(juce::String name, juce::AudioProcessorEditor *editor)
    : juce::ResizableWindow(name, true) {
    centreWithSize(300, 200);
    setContentOwned(editor, true);
}