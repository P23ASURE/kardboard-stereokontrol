/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereospeadAudioProcessorEditor::StereospeadAudioProcessorEditor (StereospeadAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    stereoWidthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    stereoWidthSlider.setRange(0.0, 1.0, 0.01); // Range da 0.0 a 1.0 con intervalli di 0.01
    stereoWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    stereoWidthSlider.addListener(this);
    addAndMakeVisible(stereoWidthSlider);
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "stereoWidth", stereoWidthSlider);
    stereoWidthSlider.setLookAndFeel(&customLookAndFeel);

    setSize(400, 600);
    startTimerHz(30);
}

void StereospeadAudioProcessorEditor::timerCallback() {
    repaint(); // Richiama repaint per ridisegnare l'interfaccia
}

StereospeadAudioProcessorEditor::~StereospeadAudioProcessorEditor()
{
    stereoWidthSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void StereospeadAudioProcessorEditor::paint(juce::Graphics& g)
{

    g.drawImageAt(background, 0, 0);

    // Buffer from processor
    auto& stereospeadProcessor = static_cast<StereospeadAudioProcessor&>(processor);
    auto& buffer = stereospeadProcessor.getStereoScopeBuffer();

    //vectorscope
    int scopeWidth = getWidth();
    int scopeHeight = getHeight() / 2; 
    int scopeYStart = getHeight() / 2; 

    for (const auto& sample : buffer) {
        float x = (scopeWidth / 2) + (sample.side * scopeWidth / 2);
        float y = scopeYStart + (scopeHeight / 2) - (sample.mid * scopeHeight / 2);
        // draw on vectorscope
        g.setColour(juce::Colours::white);
        g.fillEllipse(x, y, 2, 2);  
    }
}

void StereospeadAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto topHalf = area.removeFromTop(area.getHeight() / 2);
    int knobSize = juce::jmin(topHalf.getWidth(), topHalf.getHeight()) / 1;
    int knobX = (topHalf.getWidth() - knobSize) / 2;
    int knobY = (topHalf.getHeight() - knobSize) / 2;
    // knob position
    stereoWidthSlider.setBounds(topHalf.withSizeKeepingCentre(knobSize, knobSize));
}

void StereospeadAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &stereoWidthSlider) {

        audioProcessor.setStereoWidth(stereoWidthSlider.getValue());
    }
}