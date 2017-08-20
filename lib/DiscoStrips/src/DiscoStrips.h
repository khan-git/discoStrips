#ifndef DISCOSTRIPS_H
#include <Arduino.h>
#include "SimpleVector.h"
#include "FastLED.h"
#include "Answer.h"

using namespace std;

class DiscoStrips {

  public:
    DiscoStrips(SimpleVector<CRGB>* ledArray);
    ~DiscoStrips();

    void tick();
    void shift_pattern();

    int getBrightness() {return brightness;}
    void set_brightness(int lvl) { brightness = lvl;}

    void rest_parsing(String line, Answer* answer);
    void disable();
    void enable();

    static const int NUM_COLORS = 5;
    static const int NUM_PATTERNS = 6;

  private:
    int current_pattern;
    unsigned long last_tick;
    unsigned long tick_time;
    SimpleVector<char>* discoPatterns;
    SimpleVector<CRGB>* ledArray;
    int brightness;
    String rest_prefix;

    void handleBrightness(String line, String work_line, Answer* answer);
    void handleTempo(String line, String work_line, Answer* answer);
    bool enabled;

    CRGB::HTMLColorCode discoPallets[NUM_COLORS] {
      CRGB::Blue,
      CRGB::DarkGreen,
      CRGB::Yellow,
      CRGB::Red,
      CRGB::White
    };

    void create_pattern();
};

#endif
