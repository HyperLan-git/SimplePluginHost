#include "test.hpp"

#include <JuceHeader.h>

int yolo() { return 420; }

std::unique_ptr<juce::AudioPluginInstance> openVst() {
    OwnedArray<PluginDescription> pluginDescriptions;
    KnownPluginList plist;
    AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats();
    for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i) {
        plist.scanAndAddFile("plug/in", true, pluginDescriptions,
                             *pluginFormatManager.getFormat(i));
    }
    jassert(pluginDescriptions.size() > 0);
    String msg("Oh no!");
    auto instance = pluginFormatManager.createPluginInstance(
        *pluginDescriptions[0], 48000, 1024, msg);

    auto editor = instance->createEditor();
    auto bc = editor->getConstrainer();
    editor->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());
    // addAndMakeVisible(editor);
}