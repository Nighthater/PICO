/*
██████╗░███████╗██████╗░██╗░░░██╗██╗░░░░░██╗░░░░░██╗██╗░░░██╗███╗░░░███╗
██╔══██╗██╔════╝██╔══██╗╚██╗░██╔╝██║░░░░░██║░░░░░██║██║░░░██║████╗░████║
██████╦╝█████╗░░██████╔╝░╚████╔╝░██║░░░░░██║░░░░░██║██║░░░██║██╔████╔██║
██╔══██╗██╔══╝░░██╔══██╗░░╚██╔╝░░██║░░░░░██║░░░░░██║██║░░░██║██║╚██╔╝██║
██████╦╝███████╗██║░░██║░░░██║░░░███████╗███████╗██║╚██████╔╝██║░╚═╝░██║
╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚══════╝╚═╝░╚═════╝░╚═╝░░░░░╚═╝

Beryllium Core

PLANNED:
- Disco FX
- Photo FX 

COMPLETED:
- Color Cycle
- FX Police (US)
- FX Police (DE)
- Fire FX

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
const int mainMenuItems = 8;
String mainMenu[mainMenuItems] = {
  "RGB",
  "HSV",
  "ColorCycle",
  "Police FX DE",
  "Police FX US",
  "Disco FX",
  "Photo FX ",
  "Fire FX"
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
  "Velocity",
  "Value",
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

  int coord = 64;
  for(int i=coord; i>32; i--)
  {
    dispDrawLogoStart(i);
    delay(1);
  }

  dispDrawLogoEnd(32);
  delay(1500);

  
  
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
		case 3: PROG_POLICE_DE(); break;
		case 4: PROG_POLICE_US(); break;
		case 5: PROG_DISCO(); break;
		case 6: PROG_PHOTO(); break;
		case 7: PROG_FIRE(); break;
	
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
    
    if(digitalRead(SIDE_MENU) == LOW)  {  delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break;} // Quit to previous
    
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
	
	if(digitalRead(SIDE_MENU) == LOW)  { delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; } // Quit to previous
	
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

	if(digitalRead(SIDE_MENU) == LOW)  { delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; delay(50);} // Quit to previous
	
	dispDrawSubMenuCOLORCYCLE(ColorCycleMenu[ColorCycleMenuSelected], velocity);
	
	
	delay(20);
  }
}

void PROG_POLICE_DE()
{
  unsigned long TimeSinceStart;
  TimeSinceStart = millis();
  while(true)
  {
    dispDrawSubMenuPOLICEDE();
	
	if(millis() - TimeSinceStart <= 50)
	{
    r = 0;
    g = 0;
    b = 255; 
	}
	
	if(millis() - TimeSinceStart > 50 and millis() - TimeSinceStart <= 100)
	{
    r = 0;
    g = 0;
    b = 0;   
	}
	
	if(millis() - TimeSinceStart > 100 and millis() - TimeSinceStart <= 150)
	{
	  r = 0;
    g = 0;
    b = 255;
	}
	
	
	if(millis() - TimeSinceStart > 150)
	{
	   r = 0;
     g = 0;
     b = 0;
	}
	
	if(millis() - TimeSinceStart > 350){	TimeSinceStart = millis(); } // Start Again

  delay(10);
  
  for(int i = 0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  } 
	pixels.show();
	if(digitalRead(SIDE_MENU) == LOW)  { delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; delay(50); } // Quit to previous
	
  }
}

void PROG_POLICE_US()
{
  unsigned long TimeSinceStart;
  TimeSinceStart = millis();
  while(true)
  {
    dispDrawSubMenuPOLICEUS();
  
  if(millis() - TimeSinceStart <= 50) {    r = 0;    g = 0;    b = 255;   }// BLUE1
  if(millis() - TimeSinceStart > 50  and millis() - TimeSinceStart <= 100)  {    r = 0;    g = 0;    b = 0;    }  // PAUSE 1
  if(millis() - TimeSinceStart > 100 and millis() - TimeSinceStart <= 150)  {    r = 0;    g = 0;    b = 255;  }  // BLUE2
  if(millis() - TimeSinceStart > 150 and millis() - TimeSinceStart <= 200)  {    r = 0;    g = 0;    b = 0;    }  // PAUSE 2
  if(millis() - TimeSinceStart > 200 and millis() - TimeSinceStart <= 250)  {    r = 0;    g = 0;    b = 255;  }  // BLUE3
  if(millis() - TimeSinceStart > 250 and millis() - TimeSinceStart <= 450)  {    r = 0;    g = 0;    b = 0;    }  // LONGPAUSE
  if(millis() - TimeSinceStart > 450 and millis() - TimeSinceStart <= 500)  {    r = 255;    g = 0;    b = 0;  }  // RED1
  if(millis() - TimeSinceStart > 500  and millis() - TimeSinceStart <= 550)  {    r = 0;    g = 0;    b = 0;    }  // PAUSE 1
  if(millis() - TimeSinceStart > 550 and millis() - TimeSinceStart <= 600)  {    r = 255;    g = 0;    b = 0;  }  // RED2
  if(millis() - TimeSinceStart > 600  and millis() - TimeSinceStart <= 650)  {    r = 0;    g = 0;    b = 0;    }  // PAUSE 2
  if(millis() - TimeSinceStart > 650 and millis() - TimeSinceStart <= 700)  {    r = 255;    g = 0;    b = 0;  }  // RED3
  
  if(millis() - TimeSinceStart > 700)  {     r = 0;     g = 0;     b = 0;  } // Turn OFF FINAL
  
  if(millis() - TimeSinceStart > 900){  TimeSinceStart = millis(); } // Start Again

  delay(10);
  
  for(int i = 0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  } 
  pixels.show();
    if(digitalRead(SIDE_MENU) == LOW)  {delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; delay(50);} // Quit to previous
  }
}


void PROG_DISCO()
{
  uint8_t LightLevelRED[NUMPIXELS];
  uint8_t LightLevelBLU[NUMPIXELS];
  uint8_t LightLevelGRN[NUMPIXELS];
  int DELAY_TIME = 20;
  int cooling = 10;

  // Percentage for Lightflash event
  int SPARKING = 30;
  
  while(true)
  {
    

    // Step 1: Cool down every pixel a little uniformly
    for (int i = 0; i < NUMPIXELS; i++) {
      LightLevelRED[i] = max(0, LightLevelRED[i] - cooling);
      LightLevelBLU[i] = max(0, LightLevelBLU[i] - cooling);
      LightLevelGRN[i] = max(0, LightLevelGRN[i] - cooling);
    }


    // Step 2: Sparking - random number of pixels will get hotter
    if (random(0, 100) < SPARKING)
    {
      int amount = random (0,NUMPIXELS);
      int startPixel = random (0,31-amount);
      int LightlevelRED = random (20,255);
      int LightlevelBLU = random (20,255);
      int LightlevelGRN = random (20,255);
      
      for (int i = startPixel; i < startPixel+amount; i++) {
        {
          LightLevelRED[i] = LightlevelRED;
          LightLevelBLU[i] = LightlevelBLU;
          LightLevelGRN[i] = LightlevelGRN;
        }
      }
    }

    // Step 3: Set pixel colors based on heat
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, LightLevelRED[i], LightLevelGRN[i], LightLevelBLU[i]);
    }
    pixels.show();
    delay(DELAY_TIME);

    if(digitalRead(SIDE_MENU) == LOW)  {delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; delay(50);} // Quit to previous

    dispDrawSubMenuDISCO();
    
  }
}

void PROG_PHOTO()
{
  uint8_t LightLevel[NUMPIXELS];
  int DELAY_TIME = 20;
  int cooling = 0;

  // Percentage for Lightflash event
  int SPARKING = 25;
  
  while(true)
  {
    

    // Step 1: Cool down every pixel a little uniformly
    cooling = random(5, 30);
    for (int i = 0; i < NUMPIXELS; i++) {
      LightLevel[i] = max(0, LightLevel[i] - cooling);
    }


    // Step 2: Sparking - random number of pixels will get hotter
    if (random(0, 100) < SPARKING)
    {
      int amount = random (0,NUMPIXELS);
      int startPixel = random (0,31-amount);
      int Lightlevel = random (20,255);
      
      for (int i = startPixel; i < startPixel+amount; i++) {
        {
          LightLevel[i] = Lightlevel; // Yeah I know, its bad...
        }
      }
    }

    // Step 3: Set pixel colors based on heat
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, LightLevel[i], LightLevel[i], LightLevel[i]);
    }
    pixels.show();
    delay(DELAY_TIME);

    if(digitalRead(SIDE_MENU) == LOW)  {delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; delay(50);} // Quit to previous

    dispDrawSubMenuPHOTO();
    
  }
}

void PROG_FIRE()
{
  // Fire color palette (in RGB format)
  const uint32_t FIRE_COLORS[] = {
    Adafruit_NeoPixel::Color(255, 25, 0),   // 1000K
    Adafruit_NeoPixel::Color(255, 60, 0),   // 1100K
    Adafruit_NeoPixel::Color(255, 83, 0),   // 1200K
    Adafruit_NeoPixel::Color(255, 93, 0),   // 1300K
    Adafruit_NeoPixel::Color(255, 101, 0),  // 1400K
    Adafruit_NeoPixel::Color(255, 109, 0),  // 1500K
    Adafruit_NeoPixel::Color(255, 125, 0),  // 1600K
    Adafruit_NeoPixel::Color(255, 131, 0),  // 1700K
    Adafruit_NeoPixel::Color(255, 146, 0),  // 1800K
    Adafruit_NeoPixel::Color(255, 161, 15),  // 1900K
    Adafruit_NeoPixel::Color(255, 170, 30), // 2500K
};
  static uint8_t heat[NUMPIXELS];

  int COOLING = 70;
  int SPARKING = 5;
  int DELAY_TIME = 20;
  
  while(true)
  {
    // Step 1: Cool down every pixel a little
    for (int i = 0; i < NUMPIXELS; i++) {
      heat[i] = max(0, heat[i] - random(5, 15));
    }
    // Step 2: Sparking - random pixel will get hotter
    for (int i = 0; i < NUMPIXELS; i++) {
      if (random(0, 100) < SPARKING) {
        heat[i] = 255;
      }
    }

    // Step 3: Set pixel colors based on heat
    for (int i = 0; i < NUMPIXELS; i++) {
      uint32_t color = FIRE_COLORS[heat[i] / 25];  // Map heat value to the color palette
      pixels.setPixelColor(i, color);
    }
    pixels.show();
    delay(DELAY_TIME);
    if(digitalRead(SIDE_MENU) == LOW)  {delay(50); pixels.clear(); delay(50); pixels.show(); delay(50); break; delay(50);} // Quit to previous

    dispDrawSubMenuFIRE();
  }
}
//-------------------------------------------------------------------------------------------
// GRAPHICS DEPARTMENT
void dispDrawLogoStart(int coord)
{
  int heightOffset = 0;
  display.clearDisplay();

  display.fillRect(coord-22,39+heightOffset,24,24,WHITE);
  display.fillRect(coord,17+heightOffset,24,24,WHITE);

  display.fillRect(coord-18,43+heightOffset,16,16,BLACK);
  display.fillRect(coord+4,21+heightOffset,16,16,BLACK);

  display.fillRect(coord-14,47+heightOffset,8,8,WHITE);
  display.fillRect(coord+8,25+heightOffset,8,8,WHITE);
  
  display.display();
}

void dispDrawLogoEnd(int coord)
{
  int heightOffset = 0;
  display.clearDisplay();

  display.fillRect(coord-22,39+heightOffset,24,24,WHITE);
  display.fillRect(coord,17+heightOffset,24,24,WHITE);

  display.fillRect(coord-18,43+heightOffset,16,16,BLACK);
  display.fillRect(coord+4,21+heightOffset,16,16,BLACK);

  display.fillRect(coord-14,47+heightOffset,8,8,WHITE);
  display.fillRect(coord+8,25+heightOffset,8,8,WHITE);

  display.setTextColor(WHITE);

  
  display.setCursor(64,35+heightOffset);
  display.print("TINKERPAD");
  display.display();
}



void dispDrawMainMenu()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  // Print "MainMenu" at the Top
  display.setCursor(11,4);
  display.print("Main Menu - Pg. ");
  display.print(mainMenuSelected+1);
  
  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);
  display.fillRoundRect(7,25,110,30,5,WHITE);
  display.fillRoundRect(8,26,108,25,5,BLACK);
  
  // Print menuItem
  display.setCursor(25,35);
  display.print(mainMenu[mainMenuSelected]);
  

  
  display.display();
}

void dispDrawSubMenuRGB(String subMenuItem)
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Print "RGBMenu" at the Top
  display.setCursor(11,4);
  display.print("RGB Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);
  display.fillRoundRect(76,20,52,44,5,WHITE);
  display.fillRoundRect(77,21,50,38,5,BLACK);

  display.fillRoundRect(10,29,30,20,5,WHITE);
  display.fillRoundRect(11,30,28,16,5,BLACK);

  display.setCursor(2,19);
  display.print("Now editing:");

  // Print menuItems
  display.setCursor(22,35);
  display.print(subMenuItem);
  display.setCursor(80,25);
  display.print("R: ");
  display.print(r);
  display.setCursor(80,35);
  display.print("G: ");
  display.print(g);
  display.setCursor(80,45);
  display.print("B: ");
  display.print(b);
  display.display();
}

void dispDrawSubMenuHSV(String subMenuItem)
{
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Print "HSVMenu" at the Top
  display.setCursor(11,4);
  display.print("HSV Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);
  display.fillRoundRect(76,20,52,44,5,WHITE);
  display.fillRoundRect(77,21,50,38,5,BLACK);

  display.fillRoundRect(10,29,30,20,5,WHITE);
  display.fillRoundRect(11,30,28,16,5,BLACK);

  display.setCursor(2,19);
  display.print("Now editing:");
  
  // Print menuItems
  display.setCursor(23,35);
  display.print(subMenuItem);
  display.setCursor(80,25);
  display.print("H: ");
  display.print(int(h*360));
  display.setCursor(80,35);
  display.print("S: ");
  display.print(int(s*100));
  display.setCursor(80,45);
  display.print("V: ");
  display.print(int(v*100));
  display.display();
}

void dispDrawSubMenuCOLORCYCLE(String subMenuItem, int velocity)
{
  display.clearDisplay();

  // Print "HSVMenu" at the Top
  display.setCursor(11,4);
  display.print("Cycle Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);
  display.fillRoundRect(66,20,62,44,5,WHITE);
  display.fillRoundRect(67,21,60,38,5,BLACK);

  display.fillRoundRect(2,29,60,20,5,WHITE);
  display.fillRoundRect(3,30,58,16,5,BLACK);

  display.setCursor(2,19);
  display.print("Editing:");
    
  // Print menuItems
  display.setCursor(5,35);
  display.print(subMenuItem);
  display.setCursor(70,25);
  display.print("Vel.: ");
  display.print(velocity);
  display.setCursor(70,35);
  display.print("Val.: ");
  display.print(int(v*100));
  display.setCursor(70,45);
  display.print("Hue: ");
  display.print(int(h*360));
  display.display();
}

void dispDrawSubMenuPOLICEDE()
{
  display.clearDisplay();

  // Print "Menu" at the Top
  display.setCursor(11,4);
  display.print("Police DE");

  display.setCursor(25,35);
  display.print("No Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);
  
  display.display();
}

void dispDrawSubMenuPOLICEUS()
{
  display.clearDisplay();

  // Print "Menu" at the Top
  display.setCursor(11,4);
  display.print("Police US");

  display.setCursor(25,35);
  display.print("No Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);


  display.display();
}

void dispDrawSubMenuPHOTO()
{
  display.clearDisplay();

  // Print "Menu" at the Top
  display.setCursor(11,4);
  display.print("Photo FX");

  display.setCursor(25,35);
  display.print("No Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);


  display.display();
}

void dispDrawSubMenuDISCO()
{
  display.clearDisplay();

  // Print "Menu" at the Top
  display.setCursor(11,4);
  display.print("Disco FX");

  display.setCursor(25,35);
  display.print("No Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);


  display.display();
}

void dispDrawSubMenuFIRE()
{
  display.clearDisplay();

  // Print "Menu" at the Top
  display.setCursor(11,4);
  display.print("Fire FX");

  display.setCursor(25,35);
  display.print("No Settings");

  // Draw Stuff
  display.drawFastHLine(0,15,128,WHITE);


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
