// ===========================================================================
// User_Setup.h pour ESP32-C6-LCD-1.47 Waveshare
// ===========================================================================
// Copier ce fichier dans: Arduino/libraries/TFT_eSPI/User_Setup.h
// (remplacer le fichier existant)
//
// IMPORTANT: Necessite TFT_eSPI modifie pour ESP32-C6
// Voir: https://github.com/Bodmer/TFT_eSPI/issues/3438
// ===========================================================================

#define USER_SETUP_INFO "ESP32-C6-LCD-1.47 Waveshare"

// Driver
#define ST7789_DRIVER

// Resolution
#define TFT_WIDTH  172
#define TFT_HEIGHT 320

// Offsets (pour ST7789 172x320)
#define TFT_OFFSET_X 34
#define TFT_OFFSET_Y 0

// Pinout ESP32-C6-LCD-1.47
#define TFT_MOSI    6
#define TFT_SCLK    7
#define TFT_CS     14
#define TFT_DC     15
#define TFT_RST    21
#define TFT_BL     22

// SPI
#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// ===========================================================================
// FIN CONFIG ESP32-C6-LCD-1.47
// ===========================================================================
