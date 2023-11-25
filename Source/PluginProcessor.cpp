/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("stereoWidth","Stereo Width",0.0f, 1.0f, 0.5f));
    
    return layout;
}

//==============================================================================
StereospeadAudioProcessor::StereospeadAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    midSideBuffer(bufferSize), // circular buffer
    parameters(*this, nullptr, "Parameters", createParameterLayout())
{
    
}

void StereospeadAudioProcessor::setStereoWidth(float newWidth) {
     stereoWidth = newWidth;
}
StereospeadAudioProcessor::~StereospeadAudioProcessor()
{
}

//==============================================================================
const juce::String StereospeadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereospeadAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereospeadAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereospeadAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereospeadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereospeadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereospeadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereospeadAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereospeadAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereospeadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereospeadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StereospeadAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereospeadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StereospeadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

     for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float mid = 0.0f;
            float side = 0.0f;

            if (totalNumInputChannels > 1)
            {
                // Calculate Mid e Side
               mid = (buffer.getSample(0, sample) + buffer.getSample(1, sample)) * 0.5f;
               side = (buffer.getSample(0, sample) - buffer.getSample(1, sample)) * 0.5f;
               
                // Add Spread on Side
                side *= stereoWidth;
                
                // Add Mid & Side to circular buffer
                midSideBuffer[bufferWritePosition] = { mid, side };
                bufferWritePosition = (bufferWritePosition + 1) % bufferSize;

                 //Reconvert Mid e Side to stereo
                buffer.setSample(0, sample, mid + side);
                buffer.setSample(1, sample, mid - side);
            }
        }
    }
}

//==============================================================================
bool StereospeadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereospeadAudioProcessor::createEditor()
{
    return new StereospeadAudioProcessorEditor (*this);
}

//==============================================================================
void StereospeadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereospeadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereospeadAudioProcessor();
}
