/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereospeadAudioProcessorEditor::StereospeadAudioProcessorEditor (StereospeadAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), vectorScope(p)
{
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    stereoWidthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    stereoWidthSlider.setRange(0.0, 1.5, 0.01); // Range da 0.0 a 1.5 con intervalli di 0.01
    stereoWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    stereoWidthSlider.addListener(this);
    addAndMakeVisible(stereoWidthSlider);
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "stereoWidth", stereoWidthSlider);
    stereoWidthSlider.setLookAndFeel(&customLookAndFeel);


    addAndMakeVisible(&vectorScope);
    setSize(400, 600);
    startTimerHz(15);
}

void StereospeadAudioProcessorEditor::timerCallback() {
    vectorScope.repaint(); 
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
    auto* stereospeadProcessor = dynamic_cast<StereospeadAudioProcessor*>(&processor);
    if (!stereospeadProcessor) {
        // Handle the error. For now, let's just return from the function.
        return;
    }
    auto& buffer = stereospeadProcessor->getStereoScopeBuffer();

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

    vectorScope.setBounds(area);
}

void StereospeadAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &stereoWidthSlider) {

        audioProcessor.setStereoWidth(stereoWidthSlider.getValue());
    }
}