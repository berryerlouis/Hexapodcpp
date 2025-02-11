#pragma once

#include "Ssd1306Interface.h"
#include "../../Driver/Twi/TwiInterface.h"

namespace Component
{
    namespace Display
    {
        using namespace Driver;

        class Ssd1306 : public Ssd1306Interface {
        public:
#define SSD1306_ADDRESS                                 0x3CU

#define INVERSE                                         2U

#define SSD1306_SEND_COMMAND                            0x00U
#define SSD1306_MEMORYMODE                              0x20U
#define SSD1306_COLUMNADDR                              0x21U
#define SSD1306_PAGEADDR                                0x22U
#define SSD1306_SETCONTRAST                             0x81U
#define SSD1306_CHARGEPUMP                              0x8DU
#define SSD1306_SEGREMAP                                0xA0U
#define SSD1306_DISPLAYALLON_RESUME                     0xA4U
#define SSD1306_DISPLAYALLON                            0xA5U
#define SSD1306_NORMALDISPLAY                           0xA6U
#define SSD1306_INVERTDISPLAY                           0xA7U
#define SSD1306_SETMULTIPLEX                            0xA8U
#define SSD1306_DISPLAYOFF                              0xAEU
#define SSD1306_DISPLAYON                               0xAFU
#define SSD1306_COMSCANINC                              0xC0U
#define SSD1306_COMSCANDEC                              0xC8U
#define SSD1306_SETDISPLAYOFFSET                        0xD3U
#define SSD1306_SETDISPLAYCLOCKDIV                      0xD5U
#define SSD1306_SETPRECHARGE                            0xD9U
#define SSD1306_SETCOMPINS                              0xDAU
#define SSD1306_SETVCOMDETECT                           0xDBU

#define SSD1306_SETLOWCOLUMN                            0x00U
#define SSD1306_SETHIGHCOLUMN                           0x10U
#define SSD1306_SETSTARTLINE                            0x40U

#define SSD1306_EXTERNALVCC                             0x01U
#define SSD1306_SWITCHCAPVCC                            0x02U

#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26U
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27U
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29U
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2AU
#define SSD1306_DEACTIVATE_SCROLL                       0x2EU
#define SSD1306_ACTIVATE_SCROLL                         0x2FU
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3U


#define BUFFER_DISPLAY_LENGTH    SCREEN_WIDTH *( ( SCREEN_HEIGHT + 7U ) / 8U )


            Ssd1306(Twi::TwiInterface &twi, const uint8_t address = SSD1306_ADDRESS);

            ~Ssd1306() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DrawPixel(const uint16_t x, const uint16_t y, const uint16_t color) final override;

            virtual void DrawLine(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2,
                                  const uint16_t color) final override;

            virtual void
            DrawRectangle(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2,
                          const uint16_t color) final override;

            virtual void
            DrawCircle(const uint16_t xc, const uint16_t yc, const uint16_t r, const uint16_t color) final override;

            virtual void
            DrawChar(char c, uint16_t x, const uint16_t y, const Bitmap::Bitmaps::Color color) final override;

            virtual void
            DrawString(const char *str, uint16_t x, const uint16_t y,
                       const Bitmap::Bitmaps::Color color) final override;

            virtual void DrawBitmap(const Bitmap::Bitmaps::SBitmap *bmp, uint16_t x, uint16_t y,
                                    Bitmap::Bitmaps::Color color) final override;

            virtual void ClearBuffer(void) final override;

            virtual void EraseArea(const uint16_t x, const uint16_t y, const uint16_t width,
                                   const uint16_t height) final override;

        private:
            Twi::TwiInterface &mTwi;
            uint8_t mAddress;
            uint8_t mBufferScreen[2U][BUFFER_DISPLAY_LENGTH];
            bool mNeedToUpdate;
            uint16_t mUpdateIndex;
        };
    }
}
