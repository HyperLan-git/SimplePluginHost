#pragma once

#include <queue>
#include <string>
namespace SPH {
struct MidiMessage {
    void* data;
    size_t numBytes;
};
}  // namespace SPH

class SimplePluginHost {
   public:
    SimplePluginHost(std::string file, unsigned int sampleRate,
                     unsigned int bufferLength, bool visible = false,
                     std::string dataFile = "None");

    const float** update();

    void addMidiMessage(SPH::MidiMessage message);

    void handleMessages();

    void setVisible(bool visible);
    bool isVisible();

    bool isActive();

    void stop();
    void savePluginData(std::ostream& stream);

    ~SimplePluginHost();

   private:
    void* pluginInstance = NULL;
    void* window = NULL;
    void* buffer = NULL;
    bool active = true;
    std::queue<SPH::MidiMessage> midiMessages;
};