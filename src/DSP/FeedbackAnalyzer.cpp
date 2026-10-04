#include "FeedbackAnalyzer.h"
#include <cmath>
#include <algorithm>

FeedbackAnalyzer::FeedbackAnalyzer()
{
}

FeedbackAnalyzer::~FeedbackAnalyzer()
{
}

void FeedbackAnalyzer::prepare(double sr, int maxBlockSize)
{
    sampleRate = sr;
    
    // Crear ventana Hann
    window.resize(FFT_SIZE);
    for (int i = 0; i < FFT_SIZE; ++i) {
        window[i] = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::pi * i / (FFT_SIZE - 1)));
    }
    
    fftBuffer.resize(FFT_SIZE);
    spectrum.resize(FFT_SIZE);
}

std::vector<PeakInfo> FeedbackAnalyzer::analyze(const juce::AudioBuffer<float>& buffer)
{
    std::vector<PeakInfo> peaks;
    
    // Usar canal 0 (mono)
    if (buffer.getNumChannels() == 0)
        return peaks;
    
    const auto* samples = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();
    
    // Copiar a buffer circular
    std::copy(samples, samples + std::min(numSamples, FFT_SIZE), fftBuffer.begin());
    
    // Aplicar ventana Hann
    applyHannWindow(fftBuffer);
    
    // FFT simple (Radix-2 Cooley-Tukey)
    // Nota: en producción usar kissfft o pffft
    juce::dsp::FFT fft(10); // 2^10 = 1024, ajustar según FFT_SIZE
    
    // Calcular magnitud
    std::vector<float> magnitude(FFT_SIZE / 2);
    for (int i = 0; i < FFT_SIZE / 2; ++i) {
        float real = fftBuffer[i * 2];
        float imag = fftBuffer[i * 2 + 1];
        magnitude[i] = std::sqrt(real * real + imag * imag);
        // Convertir a dB
        magnitude[i] = 20.0f * std::log10(magnitude[i] + 1e-10f);
    }
    
    // Detectar picos
    peaks = detectPeaks(magnitude);
    
    return peaks;
}

void FeedbackAnalyzer::applyHannWindow(std::vector<float>& buffer)
{
    for (int i = 0; i < std::min((int)buffer.size(), FFT_SIZE); ++i) {
        buffer[i] *= window[i];
    }
}

std::vector<PeakInfo> FeedbackAnalyzer::detectPeaks(const std::vector<float>& magnitude)
{
    std::vector<PeakInfo> peaks;
    
    // Detectar picos locales
    for (int i = 1; i < (int)magnitude.size() - 1; ++i) {
        if (magnitude[i] > magnitude[i - 1] && magnitude[i] > magnitude[i + 1]) {
            // Pico encontrado
            if (magnitude[i] > -20.0f) { // Threshold mínimo
                PeakInfo peak;
                peak.frequency = binToFrequency(i);
                peak.magnitude = magnitude[i];
                peak.bandwidth = sampleRate / FFT_SIZE; // Resolución FFT
                peaks.push_back(peak);
            }
        }
    }
    
    // Ordenar por magnitud y limitar a 12 picos
    std::sort(peaks.begin(), peaks.end(), 
        [](const PeakInfo& a, const PeakInfo& b) { return a.magnitude > b.magnitude; });
    
    if (peaks.size() > 12)
        peaks.resize(12);
    
    return peaks;
}

float FeedbackAnalyzer::getMagnitude(int binIndex, const std::vector<float>& magnitude)
{
    if (binIndex < 0 || binIndex >= (int)magnitude.size())
        return -100.0f;
    return magnitude[binIndex];
}

float FeedbackAnalyzer::binToFrequency(int bin) const
{
    return (bin * sampleRate) / FFT_SIZE;
}
