/*
██████╗░███████╗██████╗░██╗░░░██╗██╗░░░░░██╗░░░░░██╗██╗░░░██╗███╗░░░███╗
██╔══██╗██╔════╝██╔══██╗╚██╗░██╔╝██║░░░░░██║░░░░░██║██║░░░██║████╗░████║
██████╦╝█████╗░░██████╔╝░╚████╔╝░██║░░░░░██║░░░░░██║██║░░░██║██╔████╔██║
██╔══██╗██╔══╝░░██╔══██╗░░╚██╔╝░░██║░░░░░██║░░░░░██║██║░░░██║██║╚██╔╝██║
██████╦╝███████╗██║░░██║░░░██║░░░███████╗███████╗██║╚██████╔╝██║░╚═╝░██║
╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚══════╝╚═╝░╚═════╝░╚═╝░░░░░╚═╝

Beryllium Core

- Software to control the PICO RGB Light with an GUI
- Colors can only be selected using HSV and RGB Colors
- Color Cycle

PLANNED:
- FX Police (US)
- FX Police (DE)
- Disco FX
- Photo FX 
- Fire FX
- 

COMPLETED:

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
const int mainMenuItems = 3;
String mainMenu[mainMenuItems] = {
  "RGB",
  "HSV",
  "ColorCycle"
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

int ColorCycleMenuSelected = 0;
const int ColorCycleMenuItems = 2;
String ColorCycleMenu[ColorCycleMenuItems] = {
  "Speed",
  "Luminosity",
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

//-------------------------------------------------------------------------------------------
// PROGRAMMING LOGIC

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
	
	if(mainMenuSelected >= mainMenuItems) {mainMenuSelected = 0;}
	if(mainMenuSelected <= -1) {mainMenuSelected = mainMenuItems-1;}
    
    if(digitalRead(DPAD_RIGHT) == LOW) // Enters Next Menu
    { 
      delay(10);
      switch (mainMenuSelected)
      {
        case 0: PROG_RGBMENU(); break;
        case 1: PROG_HSVMENU(); break;
		    case 2: PROG_COLORCYCLE(); break;
      }
      
    } 
    
	if(digitalRead(SIDE_MENU) == LOW)  { mainMenuSelected = 0; delay(200);} // Resets back to normal Menu
	
    //if(digitalRead(DPAD_LEFT) == LOW)  { delay(10);} // Does nothing, is already in main menu
    
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
      
      RGBMenuSelected = RGBMenuSelected + 1;
    } 
    
    if(digitalRead(DPAD_DOWN) == LOW)  // Scroll DOWN
    { 
      delay(200);
      
      RGBMenuSelected = RGBMenuSelected - 1;
    } 
  
  if(RGBMenuSelected >= RGBMenuItems) {RGBMenuSelected = 0;}
  if(RGBMenuSelected <= -1) {RGBMenuSelected = RGBMenuItems-1;}
    
    if(digitalRead(SIDE_MENU) == LOW)  { break; } // Quit to previous
    
    dispDrawSubMenuRGB(RGBMenu[RGBMenuSelected]);
  }
}

void PROG_HSVMENU()
{
  while(true)
  {
    if(digitalRead(DPAD_LEFT) == LOW) // Decrease selected Variable
    { 
      delay(20);

      switch (HSVMenuSelected)
      {
        case 0: {h = h - 1.0/360.0; break;}
        case 1: {s = s - 0.01; break;}
        case 2: {v = v - 0.01; break;}
      }
      // Error Check
      if(h<0.0){h=0.0;}
      if(s<0.0){s=0.0;}
      if(v<0.0){v=0.0;}
	  
	  hsvToRgb(h,s,v,r,g,b);

      for(int i = 0; i<NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
      }  
      pixels.show();
    } 
	
	if(digitalRead(DPAD_RIGHT) == LOW) // Increase selected Variable
    { 
      delay(20);

    
      switch (HSVMenuSelected)
      {
        case 0: {h = h + 1.0/360.0; break;}
        case 1: {s = s + 0.01; break;}
        case 2: {v = v + 0.01; break;}
      }
      // Error Check

      if(h>1.0){h=1.0;}
      if(s>1.0){s=1.0;}
      if(v>1.0){v=1.0;}
	  
	  hsvToRgb(h,s,v,r,g,b);

      for(int i = 0; i<NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
      }  
      pixels.show();
	} 
	

    if(digitalRead(DPAD_UP) == LOW) // Scroll UP
    { 
      delay(200);
      
      HSVMenuSelected = HSVMenuSelected + 1;
    } 
    
    if(digitalRead(DPAD_DOWN) == LOW)  // Scroll DOWN
    { 
      delay(200);
      
      HSVMenuSelected = HSVMenuSelected - 1;
    } 
  
  if(HSVMenuSelected >= HSVMenuItems) {HSVMenuSelected = 0;}
  if(HSVMenuSelected <= -1) {HSVMenuSelected = HSVMenuItems-1;}
	
	if(digitalRead(SIDE_MENU) == LOW)  { break; } // Quit to previous
	
	dispDrawSubMenuHSV(HSVMenu[HSVMenuSelected]);
  }
}

void PROG_COLORCYCLE()
{
  int velocity = 10;
  float luminosity = 0.1;
  
  while(true)
  {
    if(digitalRead(DPAD_LEFT) == LOW) // Decrease selected Variable
    { 
      delay(25);

      switch (ColorCycleMenuSelected)
      {
        case 0: {velocity = velocity - 1; break;}
        case 1: {luminosity = luminosity - 0.01; break;}
      }
      // Error Check
      if(velocity < 0){velocity = 0;}
      if(luminosity<0.0){luminosity=0.0;}
    } 
	
	if(digitalRead(DPAD_RIGHT) == LOW) // Increase selected Variable
    { 
      delay(25);

      switch (ColorCycleMenuSelected)
      {
        case 0: {velocity = velocity + 1; break;}
        case 1: {luminosity = luminosity + 0.01; break;}
      }
      // Error Check
      if(velocity > 100){velocity = 100;}
      if(luminosity>1.0){luminosity=1.0;}
    } 
	
	if(digitalRead(DPAD_UP) == LOW) // Scroll UP
    { 
      delay(200);
      
      ColorCycleMenuSelected = ColorCycleMenuSelected + 1;
    } 
    
    if(digitalRead(DPAD_DOWN) == LOW)  // Scroll DOWN
    { 
      delay(200);
      
      ColorCycleMenuSelected = ColorCycleMenuSelected - 1;
    } 
	
	// Error Checking
	if(ColorCycleMenuSelected >= ColorCycleMenuItems) {ColorCycleMenuSelected = 0;}
	if(ColorCycleMenuSelected <= -1) {ColorCycleMenuSelected = ColorCycleMenuItems - 1;}
	
	h = h + 1.0/360.0*velocity;
	s = 1.0;
	v = luminosity;

  if(h>1.0){h=0.0;}

	// Colorcycle
	hsvToRgb(h,s,v,r,g,b);

    for(int i = 0; i<NUMPIXELS; i++)
    {
	  pixels.setPixelColor(i, pixels.Color(r, g, b));
    }  
    pixels.show();

	if(digitalRead(SIDE_MENU) == LOW)  { break; delay(200);} // Quit to previous
	
	dispDrawSubMenuCOLORCYCLE(ColorCycleMenu[ColorCycleMenuSelected], velocity);
	
	
	delay(20);
  }
}

//-------------------------------------------------------------------------------------------
// GRAPHICS DEPARTMENT

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

void dispDrawSubMenuRGB(String subMenuItem)
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

void dispDrawSubMenuHSV(String subMenuItem)
{
  display.clearDisplay();
  // Draw Stuff
  // Print "MainMenu" at the Top
  // Center Cursor
  display.setCursor(11,25);
  // Print menuItem
  display.print(subMenuItem);
  display.setCursor(50,25);
  display.print("h: ");
  display.print(int(h*360));
  display.setCursor(50,35);
  display.print("s: ");
  display.print(int(s*100));
  display.setCursor(50,45);
  display.print("v: ");
  display.print(int(v*100));
  display.display();
}

void dispDrawSubMenuCOLORCYCLE(String subMenuItem, int velocity)
{
  display.clearDisplay();
  // Draw Stuff
  // Print "MainMenu" at the Top
  // Center Cursor
  display.setCursor(11,25);
  // Print menuItem
  display.print(subMenuItem);
  display.setCursor(50,25);
  display.print("speed: ");
  display.print(velocity);
  display.setCursor(50,35);
  display.print("lum.: ");
  display.print(int(v*100));
  display.setCursor(50,45);
  display.print("hue: ");
  display.print(int(h*360));
  display.display();
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
