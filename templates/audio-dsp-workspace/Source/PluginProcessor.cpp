#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginProcessor::AudioPluginProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
    // Add parameters
    addParameter(gainParameter = new juce::AudioParameterFloat(
        "gain", "Gain", 0.0f, 1.0f, 0.5f));
}

AudioPluginProcessor::~AudioPluginProcessor() {}

const juce::String AudioPluginProcessor::getName() const {
    return JucePlugin_Name;
}

bool AudioPluginProcessor::acceptsMidi() const {
    return false;
}

bool AudioPluginProcessor::producesMidi() const {
    return false;
}

bool AudioPluginProcessor::isMidiEffect() const {
    return false;
}

double AudioPluginProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int AudioPluginProcessor::getNumPrograms() {
    return 1;
}

int AudioPluginProcessor::getCurrentProgram() {
    return 0;
}

void AudioPluginProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

const juce::String AudioPluginProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginProcessor::changeProgramName(int index, const juce::String& newName) {
    juce::ignoreUnused(index, newName);
}

void AudioPluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    // Prepare your audio processing here
}

void AudioPluginProcessor::releaseResources() {
    // Release resources when playback stops
}

void AudioPluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midiMessages) {
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Apply gain
    float gain = gainParameter->get();
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            channelData[sample] *= gain;
        }
    }
}

bool AudioPluginProcessor::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* AudioPluginProcessor::createEditor() {
    return new juce::GenericAudioProcessorEditor(*this);
}

void AudioPluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // Save plugin state
    juce::ignoreUnused(destData);
}

void AudioPluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // Restore plugin state
    juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioPluginProcessor();
}
