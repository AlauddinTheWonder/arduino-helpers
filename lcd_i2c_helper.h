#include <LiquidCrystal_I2C.h>

#define LCD_ADDR 0x27   // I2C Address for LCD. Generally its 0x27
#define EN 2            // Enable bit
#define RW 1            // Read/Write bit
#define RS 0            // Register select bit

// D4, D5, D6, D7 LCD data lines pin mapping of the extender module
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define BACKLIGHT_PIN 3
#define BACKLIGHT_POL POSITIVE

LiquidCrystal_I2C lcd(LCD_ADDR, EN, RW, RS, D4, D5, D6, D7, BACKLIGHT_PIN, BACKLIGHT_POL);
