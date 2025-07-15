/*
Author: Kristo Siiskonen (FPGA-Adept)
Device: Lilygo T-Display-S3 (ESP32-S3R8 with an ST7789 LCD display)
Message: Hardware interrupts are used to increment and decrement a counter.
         A Software interrupt exists, but is not currently used for anything.
*/

#include <header.h>


void incrementCounter() {
  // a lock_guard wrapper for mutex is used to prevent race conditions
  std::lock_guard<std::mutex> lock_guard(button_press_mutex);

  // Clear last count and increment by 1
  gfx->setTextColor(RGB565_BLACK);
  gfx->setCursor(205, 55);
  gfx->println(button_presses);
  button_presses = button_presses + 1;
} //incrementCounter end


void decrementCounter() {
  // a lock_guard wrapper for mutex is used to prevent race conditions
  std::lock_guard<std::mutex> lock_guard(button_press_mutex);

  //  Clear last count and decrement by 1
  gfx->setTextColor(RGB565_BLACK);
  gfx->setCursor(205, 55);
  gfx->println(button_presses);
  // decrement only if count greater than 0
  if (button_presses != 0){
    button_presses = button_presses - 1;
  }
} //decrementCounter end


void buttonRightInterrupt() {
  // Only trigger this once per press
  button_state_r = digitalRead(button_r);
  if (button_state_r == HIGH){
    incrementCounter();
  }
} //buttonRightInterrupt end


void buttonLeftInterrupt() {
  // Only trigger this once per press
  button_state_l = digitalRead(button_l);
  if (button_state_l == HIGH){
    decrementCounter();
  }
} // buttonLeftInterrupt end


void softwareInterrupt() {
  // TODO
} // softwareInterrupt end


void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");

  //----------Display setup----------
  #ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
  #endif

  if (!gfx->begin()){
    Serial.println("gfx->begin() failed!");
  }

  #ifdef LCDBlackLight
  pinMode(LCDBlackLight, OUTPUT);
  digitalWrite(LCDBlackLight, HIGH);
  #endif
  //----------Display setup----------


  //----------Hardware interrupt setup----------
  // Mapping both buttons to interrupts
  pinMode(button_r, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button_r), buttonRightInterrupt, RISING);
  pinMode(button_l, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button_l), buttonLeftInterrupt, RISING);
  //----------Hardware interrupt setup----------


  //----------Software interrupt setup----------
  // Software interrupt every 1 000 000 microseconds
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(softwareInterrupt);
  //----------Software interrupt setup----------

  Serial.println("Setup end");
}

void loop() {
  //main_process();
  gfx->begin();
  gfx->fillScreen(RGB565_BLACK);

  gfx->setTextSize(2);
  gfx->setTextColor(RGB565_GREEN);
  gfx->setCursor(175, 5);
  gfx->println("Increment ->");
  gfx->setCursor(175,150);
  gfx->println("Decrement ->");

  while(true) {
    gfx->setTextColor(RGB565_BLUE);
    gfx->setCursor(10, 30);
    gfx->println("Please press button,");
    gfx->setCursor(10, 55);
    gfx->println("current presses:");

    gfx->setTextColor(RGB565_GREEN);
    gfx->setCursor(205, 55);
    gfx->println(button_presses);
  }
}