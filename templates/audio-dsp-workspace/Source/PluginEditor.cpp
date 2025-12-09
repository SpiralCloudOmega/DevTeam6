#include "PluginEditor.h"

AudioPluginEditor::AudioPluginEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setSize(400, 300);
}

AudioPluginEditor::~AudioPluginEditor() {}

void AudioPluginEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Audio Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginEditor::resized() {
    // Layout your GUI components here
}
