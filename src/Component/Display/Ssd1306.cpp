#include "Ssd1306.h"
#include "../../Misc/Bitmap/Font.h"
#include <math.h>
#include <string.h>

namespace Component
{
    namespace Display
    {
        Ssd1306::Ssd1306(Twi::TwiInterface &twi, const uint8_t address)
            : mTwi(twi)
              , mAddress(address)
              , mBufferScreen{0x00U}
              , mNeedToUpdate(false)
              , mUpdateIndex(BUFFER_DISPLAY_LENGTH) {
        }

        Core::CoreStatus Ssd1306::Initialize(void) {
            uint8_t vccState = SSD1306_SWITCHCAPVCC;
            uint8_t init1[] =
            {
                SSD1306_DISPLAYOFF,
                SSD1306_SETDISPLAYCLOCKDIV,
                0x80U,
                SSD1306_SETMULTIPLEX
            };
            this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETLOWCOLUMN, init1, sizeof(init1));
            this->mTwi.WriteRegister(this->mAddress, SSD1306_SETLOWCOLUMN, SCREEN_HEIGHT - 1);

            uint8_t init2[] =
            {
                SSD1306_SETDISPLAYOFFSET,
                0x0,
                SSD1306_SETSTARTLINE | 0x0,
                SSD1306_CHARGEPUMP
            };
            this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETLOWCOLUMN, init2, sizeof(init2));

            this->mTwi.WriteRegister(this->mAddress, SSD1306_SETLOWCOLUMN,
                                     (vccState == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

            uint8_t init3[] =
            {
                SSD1306_MEMORYMODE,
                0x00,
                SSD1306_SEGREMAP | 0x1,
                SSD1306_COMSCANDEC
            };
            this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETLOWCOLUMN, init3, sizeof(init3));

            uint8_t init4a[] =
            {
                SSD1306_SETCOMPINS,
                0x02,
                SSD1306_SETCONTRAST,
                0x8F
            };
            this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETLOWCOLUMN, init4a, sizeof(init4a));

            this->mTwi.WriteRegister(this->mAddress, SSD1306_SETLOWCOLUMN, SSD1306_SETPRECHARGE);
            this->mTwi.WriteRegister(this->mAddress, SSD1306_SETLOWCOLUMN,
                                     (vccState == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
            uint8_t init5[] = {
                SSD1306_SETVCOMDETECT,
                0x40,
                SSD1306_DISPLAYALLON_RESUME,
                SSD1306_NORMALDISPLAY,
                SSD1306_DEACTIVATE_SCROLL,
                SSD1306_DISPLAYON
            };
            this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETLOWCOLUMN, init5, sizeof(init5));

            return (Core::CoreStatus::CORE_OK);
        }

        void Ssd1306::Update(const uint64_t currentTime) {
            (void) currentTime;
            static constexpr uint8_t NB_BYTES = 64U;
            if (this->mNeedToUpdate == true) {
                if (this->mUpdateIndex == BUFFER_DISPLAY_LENGTH) {
                    uint8_t screenConfig[] = {
                        SSD1306_PAGEADDR,
                        0U,
                        0xFFU,
                        SSD1306_COLUMNADDR,
                        0U
                    };

                    this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETLOWCOLUMN, screenConfig, sizeof(screenConfig));
                    this->mTwi.WriteRegister(this->mAddress, SSD1306_SETLOWCOLUMN, SCREEN_WIDTH - 1);
                }

                if (this->mUpdateIndex >= NB_BYTES) {
                    this->mTwi.WriteRegisters(this->mAddress, SSD1306_SETSTARTLINE,
                                              &this->mBufferScreen[BUFFER_DISPLAY_LENGTH - this->mUpdateIndex],
                                              NB_BYTES);
                    this->mUpdateIndex -= NB_BYTES;
                }

                if (this->mUpdateIndex == 0U) {
                    this->mNeedToUpdate = false;
                    this->mUpdateIndex = BUFFER_DISPLAY_LENGTH;
                }
            }
        }

        void Ssd1306::EraseArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
            for (uint8_t loop_x = 0U; loop_x < width; loop_x++) {
                for (uint8_t loop_y = 0U; loop_y < height; loop_y++) {
                    Ssd1306::DrawPixel(x + loop_x, y + loop_y, Bitmap::Bitmaps::Color::COLOR_BLACK);
                }
            }
            this->mNeedToUpdate = true;
        }

        void Ssd1306::ClearBuffer(void) {
            memset(this->mBufferScreen, 0U, BUFFER_DISPLAY_LENGTH);
            this->mNeedToUpdate = true;
        }

        void Ssd1306::DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
            if ((x < SCREEN_WIDTH) && (y < SCREEN_HEIGHT)) {
                switch (color) {
                    case Bitmap::Bitmaps::Color::COLOR_WHITE:
                        this->mBufferScreen[x + (y / 8U) * SCREEN_WIDTH] |= (1U << (y & 7U));
                        break;

                    case Bitmap::Bitmaps::Color::COLOR_BLACK:
                        this->mBufferScreen[x + (y / 8U) * SCREEN_WIDTH] &= ~(1U << (y & 7U));
                        break;

                    case INVERSE:
                        this->mBufferScreen[x + (y / 8U) * SCREEN_WIDTH] ^= (1U << (y & 7U));
                        break;
                }
            }
        }

        void Ssd1306::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
            if (
                (x1 <= SCREEN_WIDTH) && (x2 <= SCREEN_WIDTH) &&
                (y1 <= SCREEN_HEIGHT) && (y2 <= SCREEN_HEIGHT)
            ) {
                uint16_t dx = abs(x2 - x1);
                uint16_t dy = abs(y2 - y1);
                if (dx != 0) {
                    float m = static_cast<float>(dy) / dx;
                    for (uint16_t x = x1; x <= x2; x++) {
                        uint16_t y = static_cast<uint16_t>(y1 + m * (x - x1));
                        Ssd1306::DrawPixel(x, y, color);
                    }
                } else {
                    for (uint16_t y = y1; y <= y2; y++) {
                        Ssd1306::DrawPixel(x1, y, color);
                    }
                }
                this->mNeedToUpdate = true;
            }
        }

        void Ssd1306::DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
            if (
                (x1 < SCREEN_WIDTH) && (x2 < SCREEN_WIDTH) &&
                (y1 < SCREEN_HEIGHT) && (y2 < SCREEN_HEIGHT)
            ) {
                Ssd1306::DrawLine(x1, y1, x2, y1, color);
                Ssd1306::DrawLine(x1, y2, x2, y2, color);
                Ssd1306::DrawLine(x1, y1, x1, y2, color);
                Ssd1306::DrawLine(x2, y1, x2, y2, color);
                this->mNeedToUpdate = true;
            }
        }

        void Ssd1306::DrawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color) {
            int16_t x, y, p;
            x = 0U;
            y = r;
            Ssd1306::DrawPixel(xc + x, yc - y, color);

            p = 3 - (2 * r);

            for (x = 0U; x <= y; x++) {
                if (p < 0) {
                    p = (p + (4 * x) + 6);
                } else {
                    y -= 1;
                    p += ((4 * (x - y) + 10));
                }

                Ssd1306::DrawPixel(xc + x, yc - y, color);
                Ssd1306::DrawPixel(xc - x, yc - y, color);
                Ssd1306::DrawPixel(xc + x, yc + y, color);
                Ssd1306::DrawPixel(xc - x, yc + y, color);
                Ssd1306::DrawPixel(xc + y, yc - x, color);
                Ssd1306::DrawPixel(xc - y, yc - x, color);
                Ssd1306::DrawPixel(xc + y, yc + x, color);
                Ssd1306::DrawPixel(xc - y, yc + x, color);
            }
            this->mNeedToUpdate = true;
        }

        void Ssd1306::DrawChar(char c, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color) {
            uint8_t i, j;

            // Convert the character to an index
            c = c & 0x7FU;
            if (c < ' ') {
                c = 0U;
            } else {
                c -= ' ';
            }

            // which is really just a 1D array of size 96*char_width.
            const unsigned char *chr = Bitmap::Font::font[static_cast<uint8_t>(c)];

            // Draw pixels
            for (j = 0U; j < CHARACTER_WIDTH; j++) {
                for (i = 0U; i < CHARACTER_HEIGHT; i++) {
                    if (chr[j] & (1 << i)) {
                        Ssd1306::DrawPixel(x + j, y + i, color);
                    } else {
                        Ssd1306::DrawPixel(x + j, y + i,
                                           color == Bitmap::Bitmaps::Color::COLOR_BLACK
                                               ? Bitmap::Bitmaps::Color::COLOR_WHITE
                                               : Bitmap::Bitmaps::Color::COLOR_BLACK);
                    }
                }
            }
            this->mNeedToUpdate = true;
        }

        void Ssd1306::DrawString(const char *str, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color) {
            while (*str) {
                Ssd1306::DrawChar(*str++, x, y, color);
                x += CHARACTER_WIDTH + 1U;
            }
        }

        void Ssd1306::DrawBitmap(const Bitmap::Bitmaps::SBitmap *bmp, uint16_t x, uint16_t y,
                                 Bitmap::Bitmaps::Color color) {
#ifndef GTEST
            uint16_t byteWidth = (bmp->width + 7U) / 8U;
#endif
            uint8_t b = 0U;

            for (uint16_t j = 0U; j < bmp->height; j++, y++) {
                for (uint16_t i = 0U; i < bmp->width; i++) {
                    if (i & 7U) {
                        b <<= 1U;
                    } else {
#ifndef GTEST
                        b = pgm_read_byte(&bmp->bmp[j * byteWidth + i / 8U]);
#endif
                    }
                    if (b & 0x80) {
                        this->DrawPixel(x + i, y, color);
                    }
                }
            }
            this->mNeedToUpdate = true;
        }
    }
}
