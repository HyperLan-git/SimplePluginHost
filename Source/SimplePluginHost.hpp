#pragma once

#include <queue>
#include <string>
namespace SPH {
struct MidiMessage {
    void* data;
    int numBytes;
};
}  // namespace SPH

class SimplePluginHost {
   public:
    SimplePluginHost(std::string file);

    const float** update();

    void addMidiMessage(SPH::MidiMessage message);

    void handleMessages();

    bool isVisible();

    ~SimplePluginHost();

   private:
    void* pluginInstance = NULL;
    void* window = NULL;
    void* buffer = NULL;
    std::queue<SPH::MidiMessage> midiMessages;
};