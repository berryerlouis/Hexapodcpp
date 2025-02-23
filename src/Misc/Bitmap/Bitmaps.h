#pragma once

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

            static const uint8_t Battery100[];
            static const uint8_t Battery75[];
            static const uint8_t Battery50[];
            static const uint8_t Battery25[];
            static const uint8_t Battery0[];
            static const uint8_t Usb[];
            static const uint8_t ArrowRight[];
            static const uint8_t ArrowLeft[];
            static const uint8_t ArrowUp[];
            static const uint8_t ArrowDown[];
            static const uint8_t ArrowCenter[];
            static const uint8_t Communication[];
            static const uint8_t ButtonRelease[];
            static const uint8_t ButtonPush[];
            static const uint8_t SoundLeft[];
            static const uint8_t SoundRight[];
        };
    } // namespace Bitmap
} // namespace Misc
