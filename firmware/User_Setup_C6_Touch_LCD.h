// =============================================================================
// User_Setup.h pour ESP32-C6 Touch LCD 1.9" Waveshare
// =============================================================================
// Copier ce fichier dans: Arduino/libraries/TFT_eSPI/User_Setup.h
// (remplacer le fichier existant)
//
// Source: https://www.waveshare.com/wiki/ESP32-C6-Touch-LCD-1.9
// =============================================================================

#define USER_SETUP_INFO "ESP32-C6-Touch-LCD-1.9 Waveshare"

// =============================================================================
// DRIVER LCD
// =============================================================================
#define ST7789_DRIVER

// Resolution 170x320
#define TFT_WIDTH  170
#define TFT_HEIGHT 320

// Offsets ST7789 170x320
#define TFT_OFFSET_X 35
#define TFT_OFFSET_Y 0

// Inversion couleurs (decommenter si couleurs inversees)
// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

// =============================================================================
// PINOUT LCD - ESP32-C6 Touch LCD 1.9" (OFFICIEL WAVESHARE)
// =============================================================================
#define TFT_MOSI    4    // LCD_DIN  - LCD data pin
#define TFT_SCLK    5    // LCD_CLK  - LCD clock pin
#define TFT_DC      6    // LCD_DC   - LCD command/data selection
#define TFT_CS      7    // LCD_CS   - LCD chip selection
#define TFT_RST     14   // LCD_RST  - LCD reset pin
#define TFT_BL      15   // LCD_BL   - LCD backlight

// Pas de MISO (lecture non utilisee)
#define TFT_MISO    -1

// =============================================================================
// SPI FREQUENCY
// =============================================================================
#define SPI_FREQUENCY       40000000   // 40 MHz (max ST7789)
#define SPI_READ_FREQUENCY  20000000   // 20 MHz
#define SPI_TOUCH_FREQUENCY  2500000   // Non utilise (touch = I2C)

// =============================================================================
// FONTS
// =============================================================================
#define LOAD_GLCD   // Font 1 - 8px
#define LOAD_FONT2  // Font 2 - 16px
#define LOAD_FONT4  // Font 4 - 26px
#define LOAD_FONT6  // Font 6 - 48px (chiffres)
#define LOAD_FONT7  // Font 7 - 7-segment 48px
#define LOAD_FONT8  // Font 8 - 75px (chiffres)
#define LOAD_GFXFF  // FreeFonts

#define SMOOTH_FONT

// =============================================================================
// NOTES IMPORTANTES
// =============================================================================
// 
// GPIO RESERVES LCD (NE PAS UTILISER):
//   4, 5, 6, 7, 14, 15
//
// GPIO TOUCH I2C (PARTAGEABLES):
//   8 (SCL), 18 (SDA), 22 (INT)
//   21 (RST) marque NC = Not Connected, utilisable
//
// GPIO SD CARD (SI NON UTILISEE = LIBRES):
//   19 (MISO), 20 (CS)
//
// GPIO IMU QMI8658 (SI NON UTILISE = LIBRES):
//   1 (INT1), 2 (INT2)
//
// GPIO ADC DISPONIBLES:
//   0, 1, 2, 3 (seuls ADC sur ESP32-C6)
//
// GPIO DIGITAL LIBRES:
//   16, 17 (UART0, libre si USB Serial)
//   21 (Touch RST NC)
//   23 (libre)
//   19, 20 (si SD Card non utilisee)
//
// =============================================================================
// FIN User_Setup.h
// =============================================================================
