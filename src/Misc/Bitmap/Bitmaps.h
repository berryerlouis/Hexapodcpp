#pragma once

#ifdef AVR
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include <stdint.h>
#include "Pixel.h"

namespace Misc
{
    namespace Bitmap
    {
        class Bitmaps {
        public:
            struct SBitmap {
                uint8_t *bmp;
                uint16_t width;
                uint16_t height;
            };

            enum Color {
                COLOR_BLACK,
                COLOR_WHITE,
            };

#ifdef RPI
#undef PROGMEM
#define PROGMEM
#endif

            static const uint8_t PROGMEM Battery100[];
            static const uint8_t PROGMEM Battery75[];
            static const uint8_t PROGMEM Battery50[];
            static const uint8_t PROGMEM Battery25[];
            static const uint8_t PROGMEM Battery0[];
            static const uint8_t PROGMEM Usb[];
            static const uint8_t PROGMEM ArrowRight[];
            static const uint8_t PROGMEM ArrowLeft[];
            static const uint8_t PROGMEM ArrowUp[];
            static const uint8_t PROGMEM ArrowDown[];
            static const uint8_t PROGMEM ArrowCenter[];
            static const uint8_t PROGMEM Communication[];
            static const uint8_t PROGMEM ButtonRelease[];
            static const uint8_t PROGMEM ButtonPush[];
            static const uint8_t PROGMEM SoundLeft[];
            static const uint8_t PROGMEM SoundRight[];
        };
    } // namespace Bitmap
} // namespace Misc
