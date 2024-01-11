/*
  ==============================================================================

    Vectorscope.cpp
    Created: 11 Jan 2024 11:16:25am
    Author:  Marco

  ==============================================================================
*/

#include "VectorScope.h"

VectorScope::VectorScope(StereospeadAudioProcessor& processor) : audioProcessor(processor) {
    // Configurazione iniziale se necessario
}

void VectorScope::paint(juce::Graphics& g) {
    // Buffer dal processore
    auto& buffer = audioProcessor.getStereoScopeBuffer();

    // Calcolo delle dimensioni del Vectorscope
    int scopeWidth = getWidth();
    int scopeHeight = getHeight();
    int scopeYStart = 0;

    for (const auto& sample : buffer) {
        float x = (scopeWidth / 2) + (sample.side * scopeWidth / 2);
        float y = scopeYStart + (scopeHeight / 2) - (sample.mid * scopeHeight / 2);

        // Disegno sul Vectorscope
        g.setColour(juce::Colours::white);
        g.fillEllipse(x - 1, y - 1, 2, 2); // Ridotto la dimensione degli ellissi per un look più preciso
    }
}

void VectorScope::resized() {
    // Gestione del ridimensionamento se necessario
}
