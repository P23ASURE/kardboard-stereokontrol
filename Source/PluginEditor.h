#include "VectorScope.h"

class StereospeadAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener, private juce::Timer
{
public:
    StereospeadAudioProcessorEditor(StereospeadAudioProcessor&);
    ~StereospeadAudioProcessorEditor() override;

    class CustomLookAndFeel : public juce::LookAndFeel_V4 {
    public:
        CustomLookAndFeel() {
            knobImage = juce::ImageCache::getFromMemory(BinaryData::knob256_png, BinaryData::knob256_pngSize);
        }

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
            float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
            juce::Slider&) override {
            const int numFrames = knobImage.getHeight() / knobImage.getWidth();
            const int frameIndex = (int)std::round(sliderPos * (numFrames - 1));
            const int frameSize = knobImage.getWidth();

            const float scaleFactor = 0.60f;
            const int size = juce::jmin(width, height) * scaleFactor;

            const int centerX = x + (width - size) / 2;
            const int centerY = y + (height - size) / 2;

            g.drawImage(knobImage, centerX, centerY, size, size, 0, frameIndex * frameSize, frameSize, frameSize);
        }

    private:
        juce::Image knobImage;
        
    };

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    
private:
    juce::Image background;
    VectorScope vectorScope;

    void timerCallback() override;
    StereospeadAudioProcessor& audioProcessor;

    juce::Slider stereoWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    CustomLookAndFeel customLookAndFeel; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereospeadAudioProcessorEditor)
};