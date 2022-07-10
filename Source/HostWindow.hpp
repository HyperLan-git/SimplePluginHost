#pragma once

#ifdef _WIN32
#define JUCE_WINDOWS 1
#else
#define JUCE_LINUX 1
#endif

#include <JuceHeader.h>

#include <utility>
class HostWindow : public juce::DialogWindow {
   public:
    HostWindow(juce::String name, juce::AudioProcessorEditor *editor);

    void closeButtonPressed();

   private:
};