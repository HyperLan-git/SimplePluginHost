#pragma once

#define JUCE_LINUX 1

#include <JuceHeader.h>

#include <utility>
class HostWindow : public juce::ResizableWindow {
   public:
    HostWindow(juce::String name, juce::AudioProcessorEditor *editor);

   private:
};