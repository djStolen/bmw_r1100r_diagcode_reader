#include "MotronicDecoder.h"

MotronicDecoder::MotronicDecoder(unsigned long gap, unsigned long debounce) 
    : gapThreshold(gap), debounceDelay(debounce) {
    reset();
}

void MotronicDecoder::reset() {
    pulseCount = 0;
    lastPulseTime = 0;
    currentDigit = 0;
    codeReady = false;
    for (int i = 0; i < 4; i++) {
        digits[i] = 0;
    }
}

bool MotronicDecoder::registerPulse(unsigned long currentTime) {
    if (codeReady || currentDigit >= 4) return false;

    // Direct assignment if this is the absolute first pulse of the session
    if (lastPulseTime == 0) {
        pulseCount++;
        lastPulseTime = currentTime;
        return true;
    }

    // Standard debounce confirmation
    if (currentTime - lastPulseTime > debounceDelay) {
        pulseCount++;
        lastPulseTime = currentTime;
        return true;
    }
    return false;
}

bool MotronicDecoder::checkTimeout(unsigned long currentTime) {
    if (codeReady || currentDigit >= 4) return false;
    if (lastPulseTime == 0) return false;

    // Detect digit boundary transitions
    if (currentTime - lastPulseTime > gapThreshold) {
        digits[currentDigit] = pulseCount;
        currentDigit++;
        pulseCount = 0;
        lastPulseTime = currentTime; // Advance anchor timeline point

        if (currentDigit == 4) {
            codeReady = true;
        }
        return true;
    }

    return false;
}

int MotronicDecoder::getLatestCode() const {
    int d[4];
    for(int i = 0; i < 4; i++) {
        d[i] = digits[i];
    }

    // Apply the active real-time accumulator to the active index position
    if (currentDigit < 4 && pulseCount > 0) {
        d[currentDigit] = pulseCount;
    }

    return (d[0] * 1000) + (d[1] * 100) + (d[2] * 10) + d[3];
}

bool MotronicDecoder::isCodeReady() const {
    return codeReady;
}

