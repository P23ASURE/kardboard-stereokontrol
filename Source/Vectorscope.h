/*
  ==============================================================================

    Vectorscope.h
    Created: 11 Jan 2024 11:16:25am
    Author:  Marco

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "PluginProcessor.h"

class VectorScope : public juce::Component {
public:
    VectorScope(StereospeadAudioProcessor& processor);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    StereospeadAudioProcessor& audioProcessor;
};
