/*
  ==============================================================================

    HostComponent.h
    Created: 9 Jun 2022 10:56:55am
    Author:  youness

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class HostComponent  : public juce::Component
{
public:
    HostComponent();
    ~HostComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HostComponent)
};
