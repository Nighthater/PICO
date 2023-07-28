/*
██╗░░██╗██╗░░░██╗██████╗░██████╗░░█████╗░░██████╗░███████╗███╗░░██╗
██║░░██║╚██╗░██╔╝██╔══██╗██╔══██╗██╔══██╗██╔════╝░██╔════╝████╗░██║
███████║░╚████╔╝░██║░░██║██████╔╝██║░░██║██║░░██╗░█████╗░░██╔██╗██║
██╔══██║░░╚██╔╝░░██║░░██║██╔══██╗██║░░██║██║░░╚██╗██╔══╝░░██║╚████║
██║░░██║░░░██║░░░██████╔╝██║░░██║╚█████╔╝╚██████╔╝███████╗██║░╚███║
╚═╝░░╚═╝░░░╚═╝░░░╚═════╝░╚═╝░░╚═╝░╚════╝░░╚═════╝░╚══════╝╚═╝░░╚══╝

Hydrogen Core

- Simplest Software to control the PICO RGB Light with
- Colors can only be selected using HSV Colors
- There are no further advanced features currently in use
- The Display is currently not in use

MIT License

Copyright (c) 2023 NIGHTHATER

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


//█▄▄ ▄▀█ █▀ █ █▀▀   █░░ █ █▄▄ █▀█ ▄▀█ █▀█ █ █▀▀ █▀
//█▄█ █▀█ ▄█ █ █▄▄   █▄▄ █ █▄█ █▀▄ █▀█ █▀▄ █ ██▄ ▄█


#include "math.h"
#include <Wire.h>

//OLED DISPLAY SSD1306 | by Adafruit
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <splash.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//█▀▄ █▀▀ █▀▀ █░░ ▄▀█ █▀█ ▄▀█ ▀█▀ █ █▀█ █▄░█ █▀
//█▄▀ ██▄ █▄▄ █▄▄ █▀█ █▀▄ █▀█ ░█░ █ █▄█ █░▀█ ▄█

//Pins
int DPAD_UP     = 7;
int DPAD_DOWN   = 6;
int DPAD_LEFT   = 4;
int DPAD_RIGHT  = 5;
int SIDE_MENU   = 3;
int SIDE_PAUSE  = 15;

//OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//NEOPIXELS
#define PIN       28 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 32 // Popular NeoPixel ring size

float h, s, v;
int   r, g, b;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // Initialize Buttons
  pinMode(DPAD_UP, INPUT_PULLUP);
  pinMode(DPAD_DOWN, INPUT_PULLUP);
  pinMode(DPAD_LEFT, INPUT_PULLUP);
  pinMode(DPAD_RIGHT, INPUT_PULLUP);
  pinMode(SIDE_MENU, INPUT_PULLUP);
  pinMode(SIDE_PAUSE, INPUT_PULLUP);
  
  // Initialize Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("FATAL ERROR 0x2B - SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
  
  // Initialize Neopixels
  pixels.begin();
  pixels.clear();
}

void loop() {
  // pixels.clear(); // Set all pixel colors to 'off'
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  // pixels.setPixelColor(Pixel#, pixels.Color(0, 150, 0));
  // pixels.show();   // Send the updated pixel colors to the hardware.
  
  // Show Display
  //TODO
  
  // Wait for BTN Input
  if(digitalRead(DPAD_UP) == LOW)    { h = h + 1/360; }
  if(digitalRead(DPAD_DOWN) == LOW)  { h = h - 1/360; }
  if(digitalRead(DPAD_LEFT) == LOW)  { s = s - 1/100; }
  if(digitalRead(DPAD_RIGHT) == LOW) { s = s + 1/100; }
  if(digitalRead(SIDE_MENU) == LOW)  { v = v + 1/100; }
  if(digitalRead(SIDE_PAUSE) == LOW) { v = v - 1/100; }

  // Boundaries
  if(h>1.0){h=0.0;}
  if(h<0.0){h=1.0;}
  if(s>1.0){s=1.0;}
  if(s<0.0){s=0.0;}
  if(v>1.0){v=1.0;}
  if(v<0.0){v=0.0;}
  
  
  // CONVERT TO RGB
  hsvToRgb(h,s,v,r,g,b);
  // UPDATE LEDS
  
  for(int i = 0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void hsvToRgb(float h, float s, float v, int& r, int& g, int& b) {
  float c = v * s;
  float x = c * (1 - abs(fmod((h * 6), 2) - 1));
  float m = v - c;

  float r1, g1, b1;

  if (h >= 0 && h < 1/6.0) {
    r1 = c;
    g1 = x;
    b1 = 0;
  } else if (h >= 1/6.0 && h < 2/6.0) {
    r1 = x;
    g1 = c;
    b1 = 0;
  } else if (h >= 2/6.0 && h < 3/6.0) {
    r1 = 0;
    g1 = c;
    b1 = x;
  } else if (h >= 3/6.0 && h < 4/6.0) {
    r1 = 0;
    g1 = x;
    b1 = c;
  } else if (h >= 4/6.0 && h < 5/6.0) {
    r1 = x;
    g1 = 0;
    b1 = c;
  } else {
    r1 = c;
    g1 = 0;
    b1 = x;
  }

  r = (r1 + m) * 255;
  g = (g1 + m) * 255;
  b = (b1 + m) * 255;
}
