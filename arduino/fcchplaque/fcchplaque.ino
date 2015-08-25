#include <Adafruit_NeoPixel.h>

#define ARRAY_SIZE(_ar_) (sizeof(_ar_) / sizeof((_ar_)[0]))

#define NUM_STRIPS 3
#define NUM_PLAQUES_PER_STRIP 7
#define NUM_PIXELS_PER_PLAQUE 8
#define NUM_PIXELS_PER_STRIP (NUM_PLAQUES_PER_STRIP * NUM_PIXELS_PER_PLAQUE)

Adafruit_NeoPixel strip_0 = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, 13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_1 = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, 12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2 = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, 14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel * const strips[NUM_STRIPS] = {&strip_0, &strip_1, &strip_2};

void setup() {
    for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
        Adafruit_NeoPixel &strip = *strips[strip_id];
        strip.begin();
        strip.setBrightness(128);
        strip.show(); // Initialize all pixels to 'off'
    }
    for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
        Adafruit_NeoPixel &strip = *strips[strip_id];
        for (int pix = 0; pix < NUM_PIXELS_PER_STRIP; pix++) {
            strip.setPixelColor(pix, Adafruit_NeoPixel::Color(0xff, 0xff, 0xff));
        }
        strip.show();
    }
    delay(1000);
    for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
        Adafruit_NeoPixel &strip = *strips[strip_id];
        for (int pix = 0; pix < NUM_PIXELS_PER_STRIP; pix++) {
            strip.setPixelColor(pix, Adafruit_NeoPixel::Color(0x00, 0x00, 0x00));
        }
        strip.show();
    }
}

typedef void action_func(uint32_t color, unsigned long pause);

/*
 * We should really separate out iterators and algorithms into separate
 * functions, but just duplicating the functions with loops in different
 * directions is simpler for a first pass.
 */

action_func wipe_r2l_pixel;
void wipe_r2l_pixel(uint32_t color, unsigned long pause) {
    for (int pix = 0; pix < NUM_PIXELS_PER_STRIP; pix++) {
        for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
            Adafruit_NeoPixel &strip = *strips[strip_id];
            strip.setPixelColor(pix, color);
        }
        for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
            Adafruit_NeoPixel &strip = *strips[strip_id];
            strip.show();
        }
        delay(pause);
    }
}

action_func wipe_l2r_pixel;
void wipe_l2r_pixel(uint32_t color, unsigned long pause) {
    for (int pix = NUM_PIXELS_PER_STRIP - 1; pix >= 0; pix--) {
        for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
            Adafruit_NeoPixel &strip = *strips[strip_id];
            strip.setPixelColor(pix, color);
        }
        for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
            Adafruit_NeoPixel &strip = *strips[strip_id];
            strip.show();
        }
        delay(pause);
    }
}

action_func wipe_r2l_plaque;
void wipe_r2l_plaque(uint32_t color, unsigned long pause) {
    for (int plaque = 0; plaque < NUM_PLAQUES_PER_STRIP; plaque++) {
        for (int plaque_pix = 0; plaque_pix < NUM_PIXELS_PER_PLAQUE; plaque_pix++) {
            int pix = (plaque * NUM_PIXELS_PER_PLAQUE) + plaque_pix;
            for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
                Adafruit_NeoPixel &strip = *strips[strip_id];
                strip.setPixelColor(pix, color);
            }
        }
        for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
            Adafruit_NeoPixel &strip = *strips[strip_id];
            strip.show();
        }
        delay(pause);
    }
}

action_func wipe_l2r_plaque;
void wipe_l2r_plaque(uint32_t color, unsigned long pause) {
    for (int plaque = NUM_PLAQUES_PER_STRIP; plaque >= 0; plaque--) {
        for (int plaque_pix = 0; plaque_pix < NUM_PIXELS_PER_PLAQUE; plaque_pix++) {
            int pix = (plaque * NUM_PIXELS_PER_PLAQUE) + plaque_pix;
            for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
                Adafruit_NeoPixel &strip = *strips[strip_id];
                strip.setPixelColor(pix, color);
            }
        }
        for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
            Adafruit_NeoPixel &strip = *strips[strip_id];
            strip.show();
        }
        delay(pause);
    }
}

action_func wipe_up;
void wipe_up(uint32_t color, unsigned long pause) {
    for (int strip_id = 0; strip_id < NUM_STRIPS; strip_id++) {
        Adafruit_NeoPixel &strip = *strips[strip_id];
        for (int pix = 0; pix < NUM_PIXELS_PER_STRIP; pix++) {
            strip.setPixelColor(pix, color);
        }
        strip.show();
        delay(pause);
    }
}

action_func wipe_down;
void wipe_down(uint32_t color, unsigned long pause) {
    for (int strip_id = NUM_STRIPS - 1; strip_id >= 0; strip_id--) {
        Adafruit_NeoPixel &strip = *strips[strip_id];
        for (int pix = 0; pix < NUM_PIXELS_PER_STRIP; pix++) {
            strip.setPixelColor(pix, color);
        }
        strip.show();
        delay(pause);
    }
}

struct {
    action_func *func;
    unsigned long pause;
} actions[] = {
    { wipe_r2l_pixel, 50 },
    { wipe_l2r_pixel, 50 },
    { wipe_r2l_plaque, 50 * NUM_PIXELS_PER_PLAQUE},
    { wipe_l2r_plaque, 50 * NUM_PIXELS_PER_PLAQUE},
    { wipe_up, 1000 },
    { wipe_down, 1000 },
};

void loop() {
    int action_idx = random(0, ARRAY_SIZE(actions));
    action_func *func = actions[action_idx].func;
    unsigned long pause = actions[action_idx].pause;
    int r = min(random(0, 5) * 0x40, 0xff);
    int g = min(random(0, 5) * 0x40, 0xff);
    int b = min(random(0, 5) * 0x40, 0xff);
    uint32_t color = Adafruit_NeoPixel::Color(r, g, b);

    func(color, pause);
}
