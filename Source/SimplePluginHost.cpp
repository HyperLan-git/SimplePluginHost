#include "SimplePluginHost.hpp"

#ifdef _WIN32
#define JUCE_WINDOWS 1
#else
#define JUCE_LINUX 1
#endif
#define JUCE_CATCH_UNHANDLED_EXCEPTIONS 1
#define JUCE_LOG_ASSERTIONS 1
#define JUCE_DEBUG 1

#include <JuceHeader.h>

#include <fstream>
#include <iostream>
#include <utility>

#include "HostWindow.hpp"

struct Plugin {
    std::unique_ptr<juce::AudioPluginInstance> plugin;
};

std::map<std::string, std::string> readPlugins(std::vector<std::string> paths) {
    std::map<std::string, std::string> result;
    juce::AudioPluginFormatManager pluginFormatManager;

    pluginFormatManager.addDefaultFormats();
    for (std::string folder : paths)
        for (auto entry : std::filesystem::directory_iterator(folder)) {
            juce::OwnedArray<juce::PluginDescription> pluginDescriptions;
            juce::KnownPluginList plist;
            for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i)
                if (plist.scanAndAddFile(entry.path().c_str(), true,
                                         pluginDescriptions,
                                         *pluginFormatManager.getFormat(i))) {
                    result.emplace(pluginDescriptions[0]->name.toStdString(),
                                   entry.path());
                }
        }
    return result;
}

SimplePluginHost::SimplePluginHost(std::string file, unsigned int sampleRate,
                                   unsigned int bufferLength, bool visible,
                                   std::string dataFile) {
    juce::initialiseJuce_GUI();
    juce::MessageManager::getInstance();  // Make sure it gets created
    juce::OwnedArray<juce::PluginDescription> pluginDescriptions;
    juce::KnownPluginList plist;
    juce::AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();
    for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i) {
        if (plist.scanAndAddFile(file, true, pluginDescriptions,
                                 *pluginFormatManager.getFormat(i)))
            break;
    }
    if (pluginDescriptions.size() == 0) return;
    juce::String msg = juce::String("Could not create instance");

    // TODO maybe the (a)sync shit that breaks everything comes from there
    std::unique_ptr<juce::AudioPluginInstance> instance =
        pluginFormatManager.createPluginInstance(
            *pluginDescriptions[0], sampleRate, bufferLength * 4, msg);

    auto editor = instance->createEditor();
    // editor->setBounds(0, 0, 300, 300);
    HostWindow* win = new HostWindow("PluginHost", editor);
    this->window = win;

    instance->enableAllBuses();
    instance->prepareToPlay(sampleRate, bufferLength);
    if (strcmp("None", dataFile.c_str()) != 0) {
        std::ifstream s(dataFile, std::ifstream::ate | std::ios::binary);
        size_t sz = s.tellg();
        s.seekg(s.beg);
        char* chars = new char[sz + 1]{0};
        s.read(chars, sz);
        instance->setStateInformation(chars, (int)sz);
        delete[] chars;
    }
    pluginInstance = new Plugin{std::move(instance)};

    win->setVisible(visible);
    buffer = new juce::AudioSampleBuffer(4, bufferLength);
}

const float** SimplePluginHost::update() {
    if (!active) return NULL;
    juce::AudioSampleBuffer* auBuff = (juce::AudioSampleBuffer*)buffer;

    juce::MidiBuffer midiBuff;
    while (!midiMessages.empty()) {
        SPH::MidiMessage evt = midiMessages.front();
        midiBuff.addEvent(juce::MidiMessage(evt.data, (int)evt.numBytes), 0);
        delete[](char*) evt.data;
        midiMessages.pop();
    }
    if (!active || pluginInstance == NULL) return NULL;
    ((Plugin*)pluginInstance)->plugin->processBlock(*auBuff, midiBuff);
    return auBuff->getArrayOfReadPointers();
}

void SimplePluginHost::addMidiMessage(SPH::MidiMessage message) {
    this->midiMessages.emplace(message);
}

void SimplePluginHost::handleMessages() {
    JUCE_TRY {
        juce::MessageManager::getInstance()->setCurrentThreadAsMessageThread();
        juce::MessageManager::getInstance()->runDispatchLoop();
    }
    JUCE_CATCH_EXCEPTION;
}

bool SimplePluginHost::isVisible() {
    if (window == NULL) return false;
    return ((HostWindow*)window)->isVisible();
}

void SimplePluginHost::setVisible(bool visible) {
    if (window == NULL) return;
    ((HostWindow*)window)->setVisible(visible);
}

bool SimplePluginHost::isActive() { return active; }

void* deleteWindow(void* window) {
    if (window != NULL) {
        ((HostWindow*)window)->setVisible(false);
        delete (HostWindow*)window;
    }
    return NULL;
}
void* deletePlugin(void* plugin) {
    if (plugin != NULL) delete (Plugin*)plugin;
    return NULL;
}

void SimplePluginHost::stop() {
    active = false;
    void* a = pluginInstance;
    juce::MessageManager::getInstance()->callFunctionOnMessageThread(
        deleteWindow, window);
    juce::MessageManager::getInstance()->callFunctionOnMessageThread(
        deletePlugin, pluginInstance);
    juce::MessageManager::getInstance()->stopDispatchLoop();
}

void SimplePluginHost::savePluginData(std::ostream& stream) {
    juce::MessageManagerLock lock;
    Plugin* p = (Plugin*)pluginInstance;
    juce::MemoryBlock b;
    p->plugin->getStateInformation(b);
    stream.write((char*)b.getData(), b.getSize());
}

SimplePluginHost::~SimplePluginHost() {
    if (buffer != NULL) delete (juce::AudioSampleBuffer*)buffer;
    juce::shutdownJuce_GUI();
    juce::MessageManager::deleteInstance();
}