#include "../firmware/reader/src/MotronicDecoder.h"
#include <cassert>
#include <iostream>

void test_successful_code_parse() {
    MotronicDecoder decoder(2500, 150);
    unsigned long timeline = 1000;

    // Digit 1: 1 pulse
    decoder.registerPulse(timeline);
    timeline += 2600;
    decoder.checkTimeout(timeline);

    // Digit 2: 2 pulses 
    timeline += 200; decoder.registerPulse(timeline);
    timeline += 200; decoder.registerPulse(timeline);
    timeline += 2600;
    decoder.checkTimeout(timeline);

    // Digit 3: 1 pulse
    timeline += 200; decoder.registerPulse(timeline);
    timeline += 2600;
    decoder.checkTimeout(timeline);

    // Digit 4: 5 pulses
    for (int p = 0; p < 5; p++) {
        timeline += 200;
        decoder.registerPulse(timeline);
    }
    timeline += 2600;
    decoder.checkTimeout(timeline);

    assert(decoder.isCodeReady() == true);
    assert(decoder.getLatestCode() == 1215);
    std::cout << "[PASS] Test 1215: Decoded Successfully.\n";
}

void test_4444_no_faults_code() {
    MotronicDecoder decoder(2500, 150);
    unsigned long timeline = 1000;

    // Simulate Code 4444 (The maximum number of valid pulses)
    for (int digit = 0; digit < 4; digit++) {
        for (int p = 0; p < 4; p++) {
            timeline += 200;
            decoder.registerPulse(timeline);
        }
        timeline += 2600;
        decoder.checkTimeout(timeline);
    }

    assert(decoder.isCodeReady() == true);
    assert(decoder.getLatestCode() == 4444);
    std::cout << "[PASS] Test 4444: No Faults code parsed cleanly.\n";
}

void test_reset_behavior() {
    MotronicDecoder decoder(2500, 150);
    unsigned long timeline = 1000;

    // Feed a partial, garbage sequence (e.g., user interrupts a reading)
    decoder.registerPulse(timeline);
    timeline += 200; decoder.registerPulse(timeline);
    timeline += 2600; decoder.checkTimeout(timeline);

    // User physically presses the Reset button
    decoder.reset();

    assert(decoder.isCodeReady() == false);
    assert(decoder.getLatestCode() == 0);
    std::cout << "[PASS] Test Reset: System clears state cleanly.\n";
}

int main() {
    std::cout << "Running MotronicDecoder Native TDD Assertions...\n";
    test_successful_code_parse();
    test_4444_no_faults_code();
    test_reset_behavior();
    std::cout << "All local tests compiled and executing successfully!\n";
    return 0;
}

