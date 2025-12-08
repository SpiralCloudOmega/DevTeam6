#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

/**
 * @brief Audio plugin editor (GUI)
 */
class AudioPluginEditor : public juce::AudioProcessorEditor {
public:
    AudioPluginEditor(AudioPluginProcessor&);
    ~AudioPluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioPluginProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginEditor)
};
