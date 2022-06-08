#include "SimplePluginHost.hpp"
#define JUCE_LINUX 1
//#define JUCE_DEBUG 1

#include <JuceHeader.h>

#include <utility>

struct Plugin {
    std::unique_ptr<juce::AudioPluginInstance> plugin;
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

    float midiDataGoesInHere[2];
    midiDataGoesInHere[0] = (float)0.01556;
    midiDataGoesInHere[1] = (float)0.01556;
    float* start = midiDataGoesInHere;

    juce::AudioSampleBuffer auBuff(&start, 2, 1);

    juce::MidiBuffer midiBuff;
    midiBuff.addEvent(juce::MidiMessage::noteOn(0, 32, (float)127), 0);

    instance->prepareToPlay(441000, 512);
    instance->processBlock(auBuff, midiBuff);

    pluginInstance = new Plugin{std::move(instance)};

    /*auto editor = instance->createEditor();
     auto bc = editor->getConstrainer();
     editor->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());*/
}

SimplePluginHost::~SimplePluginHost() {
    if (pluginInstance != NULL) delete (Plugin*)pluginInstance;
}