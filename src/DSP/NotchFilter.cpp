#include "NotchFilter.h"
#include <cmath>

NotchFilter::NotchFilter()
{
    reset();
}

NotchFilter::~NotchFilter()
{
}

void NotchFilter::prepare(double sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void NotchFilter::setFrequency(float freqHz)
{
    frequency = freqHz;
    updateCoefficients();
}

void NotchFilter::setQFactor(float q)
{
    qFactor = q;
    updateCoefficients();
}

void NotchFilter::setGain(float gainDb)
{
    gainDb = gainDb;
    // Para notch, típicamente no usamos ganancia, pero permitimos
}

float NotchFilter::processSample(float sample)
{
    // Direct Form II (menor sensibilidad a ruido)
    float w = sample - a1 * y1 - a2 * y2;
    float out = b0 * w + b1 * y1 + b2 * y2;
    
    y2 = y1;
    y1 = w;
    
    return out;
}

void NotchFilter::reset()
{
    x1 = x2 = 0.0f;
    y1 = y2 = 0.0f;
}

void NotchFilter::updateCoefficients()
{
    // Notch filter biquad (RBJ cookbook)
    const float PI = 3.14159265359f;
    
    float w0 = 2.0f * PI * frequency / (float)sampleRate;
    float sinW0 = std::sin(w0);
    float cosW0 = std::cos(w0);
    float alpha = sinW0 / (2.0f * qFactor);
    
    // Coeficientes del notch
    float a0 = 1.0f + alpha;
    b0 = 1.0f / a0;
    b1 = -2.0f * cosW0 / a0;
    b2 = 1.0f / a0;
    
    a1 = -2.0f * cosW0 / a0;
    a2 = (1.0f - alpha) / a0;
}
