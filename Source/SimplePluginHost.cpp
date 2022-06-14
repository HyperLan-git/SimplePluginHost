#include "SimplePluginHost.hpp"
#define JUCE_LINUX 1
#define JUCE_CATCH_UNHANDLED_EXCEPTIONS 1
#define JUCE_LOG_ASSERTIONS 1
#define JUCE_DEBUG 1

#define BUFFER_LENGTH 10000

#include <JuceHeader.h>

#include <utility>

#include "HostComponent.hpp"
#include "HostWindow.hpp"

struct Plugin {
    std::unique_ptr<juce::AudioPluginInstance> plugin;
};

SimplePluginHost::SimplePluginHost(std::string file) {
    initialiseJuce_GUI();
    juce::MessageManager::getInstance();
    OwnedArray<PluginDescription> pluginDescriptions;
    KnownPluginList plist;
    AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();
    for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i) {
        plist.scanAndAddFile(file, true, pluginDescriptions,
                             *pluginFormatManager.getFormat(i));
    }
    if (pluginDescriptions.size() == 0) return;
    String msg = juce::String("Could not create instance");

    std::unique_ptr<juce::AudioPluginInstance> instance =
        pluginFormatManager.createPluginInstance(*pluginDescriptions[0], 48000,
                                                 BUFFER_LENGTH * 2, msg);
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

    instance->enableAllBuses();
    instance->prepareToPlay(48000, BUFFER_LENGTH);
    pluginInstance = new Plugin{std::move(instance)};

    win->setVisible(true);
    buffer = new juce::AudioSampleBuffer(4, BUFFER_LENGTH);
}

const float** SimplePluginHost::update() {
    juce::AudioSampleBuffer* auBuff = (juce::AudioSampleBuffer*)buffer;

    juce::MidiBuffer midiBuff;
    while (!midiMessages.empty()) {
        SPH::MidiMessage evt = midiMessages.front();
        midiMessages.pop();
        midiBuff.addEvent(juce::MidiMessage(evt.data, evt.numBytes), 0);
    }
    ((Plugin*)pluginInstance)->plugin->processBlock(*auBuff, midiBuff);
    return auBuff->getArrayOfReadPointers();
}

void SimplePluginHost::addMidiMessage(SPH::MidiMessage message) {
    this->midiMessages.emplace(message);
}

void SimplePluginHost::handleMessages() {
    JUCE_TRY { MessageManager::getInstance()->runDispatchLoop(); }
    JUCE_CATCH_EXCEPTION;
}

bool SimplePluginHost::isVisible() {
    if (window == NULL) return false;
    return ((HostWindow*)window)->isVisible();
}

SimplePluginHost::~SimplePluginHost() {
    if (pluginInstance != NULL) delete (Plugin*)pluginInstance;
    if (window != NULL) delete (HostWindow*)window;
    if (buffer != NULL) delete (juce::AudioSampleBuffer*)buffer;
    juce::MessageManager::getInstance()->stopDispatchLoop();
}