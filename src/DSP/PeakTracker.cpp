#include "PeakTracker.h"
#include <cmath>
#include <algorithm>

PeakTracker::PeakTracker()
{
}

PeakTracker::~PeakTracker()
{
}

void PeakTracker::prepare(double sr)
{
    sampleRate = sr;
}

bool PeakTracker::track(float frequency, float magnitude)
{
    int nearestIdx = findNearestPeak(frequency);
    
    if (nearestIdx >= 0) {
        // Peak existente encontrado
        trackedPeaks[nearestIdx].frequency = frequency;
        trackedPeaks[nearestIdx].magnitude = magnitude;
        trackedPeaks[nearestIdx].persistenceCount++;
        return trackedPeaks[nearestIdx].persistenceCount >= PERSISTENCE_THRESHOLD;
    } else {
        // Nuevo peak
        TrackedPeak newPeak;
        newPeak.frequency = frequency;
        newPeak.magnitude = magnitude;
        newPeak.persistenceCount = 1;
        newPeak.id = nextPeakId++;
        trackedPeaks.push_back(newPeak);
        return false;
    }
}

void PeakTracker::update()
{
    // Remover peaks que no persisten
    trackedPeaks.erase(
        std::remove_if(trackedPeaks.begin(), trackedPeaks.end(),
            [](const TrackedPeak& p) { return p.persistenceCount < 1; }),
        trackedPeaks.end()
    );
}

void PeakTracker::reset()
{
    trackedPeaks.clear();
    nextPeakId = 0;
}

int PeakTracker::findNearestPeak(float frequency)
{
    int nearestIdx = -1;
    float minDistance = FREQUENCY_TOLERANCE;
    
    for (int i = 0; i < (int)trackedPeaks.size(); ++i) {
        float distance = std::abs(trackedPeaks[i].frequency - frequency);
        if (distance < minDistance) {
            minDistance = distance;
            nearestIdx = i;
        }
    }
    
    return nearestIdx;
}
