#include "SimplePluginHost.hpp"
#define JUCE_LINUX 1
//#define JUCE_DEBUG 1

#include <JuceHeader.h>

#include <utility>

#include "HostComponent.hpp"
#include "HostWindow.hpp"

struct Plugin {
    std::unique_ptr<juce::AudioPluginInstance> plugin;
};

class MessageThread : public Thread {
   public:
    MessageThread() : Thread("MessageThread") {}
    void run() {
        JUCE_TRY { MessageManager::getInstance()->runDispatchLoop(); }
        JUCE_CATCH_EXCEPTION
    }
};

SimplePluginHost::SimplePluginHost(std::string file) {
    OwnedArray<PluginDescription> pluginDescriptions;
    KnownPluginList plist;
    AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();
    for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i) {
        printf("%s\n", pluginFormatManager.getFormat(i)
                           ->getName()
                           .getCharPointer()
                           .getAddress());
        plist.scanAndAddFile(file, true, pluginDescriptions,
                             *pluginFormatManager.getFormat(i));
    }
    printf("heya!%d\n", pluginDescriptions.size());
    if (pluginDescriptions.size() == 0) return;
    String msg = juce::String("Could not create instance");

    std::unique_ptr<juce::AudioPluginInstance> instance =
        pluginFormatManager.createPluginInstance(*pluginDescriptions[0], 48000,
                                                 1024, msg);
    printf("%s : %s\n", instance->getName().getCharPointer().getAddress(),
           instance->getPluginDescription()
               .descriptiveName.getCharPointer()
               .getAddress());

    auto editor = instance->createEditor();
    auto bc = editor->getConstrainer();
    editor->setBounds(0, 0, 300, 300);
    HostWindow* win = new HostWindow("PluginHost", editor);
    this->window = win;

    // float midiDataGoesInHere[2];
    // midiDataGoesInHere[0] = (float)0.01556;
    // midiDataGoesInHere[1] = (float)0.01556;
    // float* start = midiDataGoesInHere;

    // juce::AudioSampleBuffer auBuff(&start, 2, 1);

    // juce::MidiBuffer midiBuff;
    // midiBuff.addEvent(juce::MidiMessage::noteOn(0, 32, (float)127), 0);

    // instance->prepareToPlay(441000, 512);
    // instance->processBlock(auBuff, midiBuff);

    instance->getPlayHead();
    instance->enableAllBuses();
    instance->prepareToPlay(44100, 512);
    pluginInstance = new Plugin{std::move(instance)};

    win->setVisible(true);
    Thread* t = new MessageThread();
    t->startThread(9);
}

const float** SimplePluginHost::update() {
    juce::AudioSampleBuffer auBuff(2, 512);

    juce::MidiBuffer midiBuff;

    ((Plugin*)pluginInstance)->plugin->processBlock(auBuff, midiBuff);
    return auBuff.getArrayOfReadPointers();
}

SimplePluginHost::~SimplePluginHost() {
    if (pluginInstance != NULL) delete (Plugin*)pluginInstance;
    if (window != NULL) delete (HostWindow*)window;
}