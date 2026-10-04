#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class FeedbackSuppressorAudioProcessorEditor : public juce::AudioProcessorEditor,
                                              public juce::Slider::Listener
{
public:
    FeedbackSuppressorAudioProcessorEditor(FeedbackSuppressorAudioProcessor&);
    ~FeedbackSuppressorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    FeedbackSuppressorAudioProcessor& audioProcessor;

    // Toggle para enabled
    juce::ToggleButton enabledToggle;
    juce::Label enabledLabel;

    // Sliders
    juce::Slider thresholdSlider;
    juce::Label thresholdLabel;

    juce::Slider ratioSlider;
    juce::Label ratioLabel;

    juce::Slider qFactorSlider;
    juce::Label qFactorLabel;

    juce::Slider numNotchesSlider;
    juce::Label numNotchesLabel;

    juce::Slider outputGainSlider;
    juce::Label outputGainLabel;

    // Attachments para binding automático
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qFactorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> numNotchesAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeedbackSuppressorAudioProcessorEditor)
};
