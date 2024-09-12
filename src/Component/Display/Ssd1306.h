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
#define SSD1306_ADDRESS                                 0x3CU ///< I2C address of the ssd1306 module

#define INVERSE                                         2U    ///< Invert pixels

#define SSD1306_MEMORYMODE                              0x20U ///< See datasheet
#define SSD1306_COLUMNADDR                              0x21U ///< See datasheet
#define SSD1306_PAGEADDR                                0x22U ///< See datasheet
#define SSD1306_SETCONTRAST                             0x81U ///< See datasheet
#define SSD1306_CHARGEPUMP                              0x8DU ///< See datasheet
#define SSD1306_SEGREMAP                                0xA0U ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME                     0xA4U ///< See datasheet
#define SSD1306_DISPLAYALLON                            0xA5U ///< Not currently used
#define SSD1306_NORMALDISPLAY                           0xA6U ///< See datasheet
#define SSD1306_INVERTDISPLAY                           0xA7U ///< See datasheet
#define SSD1306_SETMULTIPLEX                            0xA8U ///< See datasheet
#define SSD1306_DISPLAYOFF                              0xAEU ///< See datasheet
#define SSD1306_DISPLAYON                               0xAFU ///< See datasheet
#define SSD1306_COMSCANINC                              0xC0U ///< Not currently used
#define SSD1306_COMSCANDEC                              0xC8U ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET                        0xD3U ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV                      0xD5U ///< See datasheet
#define SSD1306_SETPRECHARGE                            0xD9U ///< See datasheet
#define SSD1306_SETCOMPINS                              0xDAU ///< See datasheet
#define SSD1306_SETVCOMDETECT                           0xDBU ///< See datasheet

#define SSD1306_SETLOWCOLUMN                            0x00U ///< Not currently used
#define SSD1306_SETHIGHCOLUMN                           0x10U ///< Not currently used
#define SSD1306_SETSTARTLINE                            0x40U ///< See datasheet

#define SSD1306_EXTERNALVCC                             0x01U ///< External display voltage source
#define SSD1306_SWITCHCAPVCC                            0x02U ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26U ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27U ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29U ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2AU ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                       0x2EU ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                         0x2FU ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3U ///< Set scroll range


#define BUFFER_DISPLAY_LENGTH    SCREEN_WIDTH *( ( SCREEN_HEIGHT + 7U ) / 8U )


            Ssd1306(Twi::TwiInterface &twi, const uint8_t address = SSD1306_ADDRESS);

            ~Ssd1306() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DrawPixel(uint16_t x, uint16_t y, uint16_t color) final override;

            virtual void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) final override;

            virtual void
            DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) final override;

            virtual void DrawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color) final override;

            virtual void DrawChar(char c, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color) final override;

            virtual void
            DrawString(const char *str, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color) final override;

            virtual void DrawBitmap(const Bitmap::Bitmaps::SBitmap *bmp, uint16_t x, uint16_t y,
                                    Bitmap::Bitmaps::Color color) final override;

            virtual void ClearBuffer(void) final override;

            virtual void EraseArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height) final override;

        private:
            Twi::TwiInterface &mTwi;
            const uint8_t mAddress;
            uint8_t mBufferScreen[BUFFER_DISPLAY_LENGTH];
            bool mNeedToUpdate;
            int16_t mUpdateIndex;
        };
    }
}
