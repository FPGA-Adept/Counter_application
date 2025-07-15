/*
Author: Kristo Siiskonen (FPGA-Adept)
Device: Lilygo T-Display-S3 (ESP32-S3R8 with an ST7789 LCD display)
Message: Declarations needed for driving a LCD display and hardware interrupts.
*/

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <TimerOne.h>
#include <mutex>


//----------Hardware interrupt definitions----------
// Onboard button pin numbers
#define button_r 14
#define button_l 0
// Buttonstates for hardware interrupt counter
volatile bool button_state_r;
volatile bool button_state_l;
// Global variable to store the amount of presses
int button_presses = 0;
// Mutex to prevent possible race conditions coming from the use of a global variable
std::mutex button_press_mutex;
//----------Hardware interrupt definitions----------


//----------Display definitions----------
// GPIO pins
#define LCDReset 5
#define LCDChipSelect 6
#define LCDDataCommand 7
#define LCDWrite 8
#define LCDRead 9
#define LCDPower 15
#define LCDBlackLight 38
#define LCDDataBit0 39
#define LCDDataBit1 40
#define LCDDataBit2 41
#define LCDDataBit3 42
#define LCDDataBit4 45
#define LCDDataBit5 46
#define LCDDataBit6 47
#define LCDDataBit7 48

// LCD properties
#define LCDWidth 170
#define LCDHeight 320
#define LCDRotation 1 //0=normal, 1=90 degrees clockwise, 2=upside down, 3=90 degrees counterclockwise
#define LCDIPS true

#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3
// Connecting power pin
#define GFX_EXTRA_PRE_INIT()    \
    {                           \
        pinMode(LCDPower, OUTPUT);    \
        digitalWrite(LCDPower, HIGH); \
    }

// Connecting correct pins to a bus for the LCD display
Arduino_DataBus *bus = new Arduino_ESP32PAR8Q(
    LCDDataCommand,
    LCDChipSelect,
    LCDWrite,
    LCDRead,
    LCDDataBit0,
    LCDDataBit1,
    LCDDataBit2,
    LCDDataBit3,
    LCDDataBit4,
    LCDDataBit5,
    LCDDataBit6,
    LCDDataBit7);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    LCDReset,
    LCDRotation,
    LCDIPS,
    LCDWidth,
    LCDHeight,
    35, //col offset 1
    0, //row offset 1
    35, //col offset 2
    0); //row offset 2
//----------Display definitions----------



/*
// UNUSED TODO: get application to Light-Sleep
#include <esp_task_wdt.h>
#include <esp_system.h> // For ESP.restart()

// UNUSED
#include <LowPower.h>


  //----------Watchdog timer setup----------
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 0,
    .idle_core_mask = (1 << cores) - 1,
    .trigger_panic = false
  };
  esp_task_wdt_init();
  esp_task_wdt_add(*buttonRightInterrupt(), buttonLeftInterrupt()*);
  //----------Watchdog timer setup----------
  */