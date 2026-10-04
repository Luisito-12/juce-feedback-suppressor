#include "PluginProcessor.h"
#include "PluginEditor.h"

FeedbackSuppressorAudioProcessorEditor::FeedbackSuppressorAudioProcessorEditor(
    FeedbackSuppressorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(600, 500);
    setResizable(false, false);

    // Enabled toggle
    addAndMakeVisible(&enabledToggle);
    enabledToggle.setButtonText("Enable");
    enabledToggle.setColour(juce::ToggleButton::tickColourId, juce::Colours::green);
    enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "enabled", enabledToggle);

    // Threshold slider
    addAndMakeVisible(&thresholdSlider);
    thresholdSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    thresholdSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    thresholdLabel.setText("Threshold (dB):", juce::dontSendNotification);
    thresholdLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(&thresholdLabel);
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "threshold", thresholdSlider);

    // Ratio slider
    addAndMakeVisible(&ratioSlider);
    ratioSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    ratioSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    ratioLabel.setText("Ratio:", juce::dontSendNotification);
    ratioLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(&ratioLabel);
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "ratio", ratioSlider);

    // Q Factor slider
    addAndMakeVisible(&qFactorSlider);
    qFactorSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    qFactorSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    qFactorSlider.setColour(juce::Slider::thumbColourId, juce::Colours::yellow);
    qFactorLabel.setText("Q Factor:", juce::dontSendNotification);
    qFactorLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(&qFactorLabel);
    qFactorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "qFactor", qFactorSlider);

    // Num Notches slider
    addAndMakeVisible(&numNotchesSlider);
    numNotchesSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    numNotchesSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    numNotchesSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    numNotchesLabel.setText("Max Notches:", juce::dontSendNotification);
    numNotchesLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(&numNotchesLabel);
    numNotchesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "numNotches", numNotchesSlider);

    // Output Gain slider
    addAndMakeVisible(&outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    outputGainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::green);
    outputGainLabel.setText("Output Gain (dB):", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(&outputGainLabel);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "outputGain", outputGainSlider);
}

FeedbackSuppressorAudioProcessorEditor::~FeedbackSuppressorAudioProcessorEditor()
{
}

void FeedbackSuppressorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(40, 40, 40));
    
    // Título
    g.setColour(juce::Colours::white);
    g.setFont(24.0f);
    g.drawText("Feedback Suppressor", 0, 10, getWidth(), 40, juce::Justification::centred);

    // Línea separadora
    g.setColour(juce::Colours::grey);
    g.drawHorizontalLine(55, 10.0f, getWidth() - 10.0f);
}

void FeedbackSuppressorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    // Enabled toggle
    enabledToggle.setBounds(area.removeFromTop(40));

    area.removeFromTop(20); // Espaciado

    const int labelWidth = 140;
    const int controlHeight = 40;

    // Threshold
    auto thresholdArea = area.removeFromTop(controlHeight);
    thresholdLabel.setBounds(thresholdArea.removeFromLeft(labelWidth));
    thresholdSlider.setBounds(thresholdArea);

    area.removeFromTop(10);

    // Ratio
    auto ratioArea = area.removeFromTop(controlHeight);
    ratioLabel.setBounds(ratioArea.removeFromLeft(labelWidth));
    ratioSlider.setBounds(ratioArea);

    area.removeFromTop(10);

    // Q Factor
    auto qArea = area.removeFromTop(controlHeight);
    qFactorLabel.setBounds(qArea.removeFromLeft(labelWidth));
    qFactorSlider.setBounds(qArea);

    area.removeFromTop(10);

    // Num Notches
    auto notchesArea = area.removeFromTop(controlHeight);
    numNotchesLabel.setBounds(notchesArea.removeFromLeft(labelWidth));
    numNotchesSlider.setBounds(notchesArea);

    area.removeFromTop(10);

    // Output Gain
    auto gainArea = area.removeFromTop(controlHeight);
    outputGainLabel.setBounds(gainArea.removeFromLeft(labelWidth));
    outputGainSlider.setBounds(gainArea);
}

void FeedbackSuppressorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    juce::ignoreUnused(slider);
    // Los attachments manejan la actualización automáticamente
}
