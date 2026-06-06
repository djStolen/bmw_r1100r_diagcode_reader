#ifndef MOTRONIC_DECODER_H
#define MOTRONIC_DECODER_H

class MotronicDecoder {
private:
    int pulseCount;
    unsigned long lastPulseTime;
    int digits[4];
    int currentDigit;
    const unsigned long gapThreshold;
    const unsigned long debounceDelay;
    bool codeReady;

public:
    MotronicDecoder(unsigned long gap = 2500, unsigned long debounce = 150);
    
    bool registerPulse(unsigned long currentTime);
    bool checkTimeout(unsigned long currentTime);
    int getLatestCode() const;
    bool isCodeReady() const;
    void reset();
};

#endif

