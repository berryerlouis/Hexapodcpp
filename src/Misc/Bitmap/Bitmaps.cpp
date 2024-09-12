#include "Bitmaps.h"


namespace Misc
{
    namespace Bitmap
    {
        const uint8_t PROGMEM Bitmaps::Battery100[] =
        {
            XXXXXXXX, XXXXXXX_,
            X_______, ______X_,
            X_XXX_XX, X_XXX_XX,
            X_XX_XXX, _XXX__XX,
            X_X_XXX_, XXX_X_XX,
            X_______, ______X_,
            XXXXXXXX, XXXXXXX_
        };

        const uint8_t PROGMEM Bitmaps::Battery75[] =
        {
            XXXXXXXX, XXXXXXX_,
            X_______, ______X_,
            X_XXX_XX, X_XXX_XX,
            X_XX_XXX, _XXX__XX,
            X_X_XXX_, XXX___XX,
            X_______, ______X_,
            XXXXXXXX, XXXXXXX_
        };

        const uint8_t PROGMEM Bitmaps::Battery50[] =
        {
            XXXXXXXX, XXXXXXX_,
            X_______, ______X_,
            X_XXX_XX, X_____XX,
            X_XX_XXX, ______XX,
            X_X_XXX_, ______XX,
            X_______, ______X_,
            XXXXXXXX, XXXXXXX_
        };

        const uint8_t PROGMEM Bitmaps::Battery25[] =
        {
            XXXXXXXX, XXXXXXX_,
            X_______, ______X_,
            X_XXX___, ______XX,
            X_XX____, ______XX,
            X_X_____, ______XX,
            X_______, ______X_,
            XXXXXXXX, XXXXXXX_
        };

        const uint8_t PROGMEM Bitmaps::Battery0[] =
        {
            XXXXXXXX, XXXXXXX_,
            X_______, ______X_,
            X_______, ______XX,
            X_______, ______XX,
            X_______, ______XX,
            X_______, ______X_,
            XXXXXXXX, XXXXXXX_
        };

        const uint8_t PROGMEM Bitmaps::ArrowRight[] =
        {
            ________, _XXX____,
            ________, __XXX___,
            __XXXXXX, XXXXXX__,
            __XXXXXX, XXXXXX__,
            ________, __XXX___,
            ________, _XXX____
        };

        const uint8_t PROGMEM Bitmaps::ArrowLeft[] =
        {
            ____XXX_, ________,
            ___XXX__, ________,
            __XXXXXX, XXXXXX__,
            __XXXXXX, XXXXXX__,
            ___XXX__, ________,
            ____XXX_, ________
        };

        const uint8_t PROGMEM Bitmaps::ArrowUp[] =
        {
            _______X, X_______,
            ______XX, XX______,
            _____XXX, XXX_____,
            _______X, X_______,
            _______X, X_______,
            _______X, X_______
        };

        const uint8_t PROGMEM Bitmaps::ArrowDown[] =
        {
            _______X, X_______,
            _______X, X_______,
            _______X, X_______,
            _____XXX, XXX_____,
            ______XX, XX______,
            _______X, X_______
        };

        const uint8_t PROGMEM Bitmaps::ArrowCenter[] =
        {
            _______X, X_______,
            _____XXX, XXX_____,
            ____XXXX, XXXX____,
            ____XXXX, XXXX____,
            _____XXX, XXX_____,
            _______X, X_______
        };

        const uint8_t PROGMEM Bitmaps::Communication[] =
        {
            ________, ________,
            ____XXX_, ____XX__,
            ___XX___, ___XXXX_,
            __XX____, __XXXXXX,
            XXXXXX__, ____XX__,
            _XXXX___, ___XX___,
            __XX____, _XXX____,
            ________, ________
        };
    }
}
