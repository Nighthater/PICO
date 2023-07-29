/*
██╗░░██╗███████╗██╗░░░░░██╗██╗░░░██╗███╗░░░███╗
██║░░██║██╔════╝██║░░░░░██║██║░░░██║████╗░████║
███████║█████╗░░██║░░░░░██║██║░░░██║██╔████╔██║
██╔══██║██╔══╝░░██║░░░░░██║██║░░░██║██║╚██╔╝██║
██║░░██║███████╗███████╗██║╚██████╔╝██║░╚═╝░██║
╚═╝░░╚═╝╚══════╝╚══════╝╚═╝░╚═════╝░╚═╝░░░░░╚═╝

Helium Core

- Software to control the PICO RGB Light with an GUI
- Colors can only be selected using HSV Colors
- There are no further advanced features currently in use

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
#include <Arduino.h>

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
int   r_old, g_old, b_old;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);

// MENU

int mainMenuSelected = 0;
const int mainMenuItems = 2;
String mainMenu[mainMenuItems] = {
  "RGB",
  "HSV"
};

int RGBMenuSelected = 0;
const int RGBMenuItems = 3;
String RGBMenu[RGBMenuItems] = {
  "R",
  "G",
  "B"
};

int HSVMenuSelected = 0;
const int HSVMenuItems = 3;
String HSVMenu[HSVMenuItems] = {
  "H",
  "S",
  "V"
};


void setup() {
  // Initialize Buttons
  pinMode(DPAD_UP, INPUT_PULLUP);
  pinMode(DPAD_DOWN, INPUT_PULLUP);
  pinMode(DPAD_LEFT, INPUT_PULLUP);
  pinMode(DPAD_RIGHT, INPUT_PULLUP);
  pinMode(SIDE_MENU, INPUT_PULLUP);
  pinMode(SIDE_PAUSE, INPUT_PULLUP);
  Serial.begin(9600);
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
  // Wait for BTN Input
  PROG_MAINMENU();
  
}


void dispDrawMainMenu()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  // Draw Stuff
  // Print "MainMenu" at the Top
  // Center Cursor
  display.drawFastHLine(0,15,128,WHITE);
  display.setCursor(11,25);
  // Print menuItem
  display.print(mainMenu[mainMenuSelected]);
  display.display();
}

void dispDrawSubMenu(String subMenuItem)
{
  display.clearDisplay();
  // Draw Stuff
  // Print "MainMenu" at the Top
  // Center Cursor
  display.setCursor(11,25);
  // Print menuItem
  display.print(subMenuItem);
  display.setCursor(50,25);
  display.print("r: ");
  display.print(r);
  display.setCursor(50,35);
  display.print("g: ");
  display.print(g);
  display.setCursor(50,45);
  display.print("b: ");
  display.print(b);
  display.display();
}

void PROG_MAINMENU()
{
  while(true)
  {
    if(digitalRead(DPAD_UP) == LOW) // Scroll UP
    { 
      delay(200);
      
      mainMenuSelected = mainMenuSelected + 1;
    } 
    
    if(digitalRead(DPAD_DOWN) == LOW)  // Scroll DOWN
    { 
      delay(200);
      
      mainMenuSelected = mainMenuSelected - 1;
    } 
	
    if (mainMenuSelected >= mainMenuItems) {
      mainMenuSelected = 0;
    }
    if (mainMenuSelected < 0) {
      mainMenuSelected = mainMenuItems - 1;
    }
    
    if(digitalRead(DPAD_RIGHT) == LOW) // Enters Next Menu
    { 
      delay(10);
      switch (mainMenuSelected)
      {
        case 0: PROG_RGBMENU();
        case 1: PROG_HSVMENU();
      }
      
    } 
    
    //if(digitalRead(DPAD_LEFT) == LOW)  { delay(10);} // Does nothing, is already in main menu
    //if(digitalRead(SIDE_MENU) == LOW)  { delay(10);} // Does nothing, is already in main menu
    //if(digitalRead(SIDE_PAUSE) == LOW) { delay(10);} // Does nothing, nothing to pause
    
    
    dispDrawMainMenu();
    
    delay(50);
  }  
}

void PROG_RGBMENU()
{
  while(true)
  {
    if(digitalRead(DPAD_LEFT) == LOW) // Decrease selected Variable
    { 
      delay(25);

      switch (RGBMenuSelected)
      {
        case 0: {r = r - 1; break;}
        case 1: {g = g - 1; break;}
        case 2: {b = b - 1; break;}
      }
      // Error Check
      if(r<0){r=0;}
      if(g<0){g=0;}
      if(b<0){b=0;}

      for(int i = 0; i<NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
      }  
      pixels.show();
    } 
    
    if(digitalRead(DPAD_RIGHT) == LOW) // Increase selected Variable
    { 
      delay(25);

    
      switch (RGBMenuSelected)
      {
        case 0: {r = r + 1; break;}
        case 1: {g = g + 1; break;}
        case 2: {b = b + 1; break;}
      }
      // Error Check

      if(r>255){r=255;}
      if(g>255){g=255;}
      if(b>255){b=255;}

      for(int i = 0; i<NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
      }  
      pixels.show();
	  } 

    if(digitalRead(DPAD_UP) == LOW) // Scroll UP
    { 
      delay(200);
      if(RGBMenuSelected >= RGBMenuItems - 1) {RGBMenuSelected = 0;}
      else {RGBMenuSelected = RGBMenuSelected + 1;}
    } 
    
    if(digitalRead(DPAD_DOWN) == LOW)  // Scroll DOWN
    { 
      delay(200);
      if(RGBMenuSelected < 0) {RGBMenuSelected = RGBMenuItems - 1;}
      else {RGBMenuSelected = RGBMenuSelected - 1;}
    } 
    
    if(digitalRead(SIDE_MENU) == LOW)  { break; } // Quit to previous
    
    dispDrawSubMenu(RGBMenu[RGBMenuSelected]);
  }
}

void PROG_HSVMENU()
{
  //dispDrawSubMenu();
}
