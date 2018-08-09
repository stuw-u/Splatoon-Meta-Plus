#include <Backlight.h>
#include <Battery.h>
#include <Buttons.h>
#include <Display.h>
#include <Gamebuino.h>
#include <Sound.h>
#include <SPI.h>
#include <EEPROM.h>

const PROGMEM byte FontA[] {
  3,5,
  B11100000,
  B10100000,
  B11100000,
  B10100000,
  B10100000
};

const PROGMEM byte FontB[] {
  3,5,
  B11100000,
  B10100000,
  B11000000,
  B10100000,
  B11100000
};

const PROGMEM byte FontC[] {
  3,5,
  B11100000,
  B10000000,
  B10000000,
  B10000000,
  B11100000
};

const PROGMEM byte FontD[] {
  3,5,
  B11000000,
  B10100000,
  B10100000,
  B10100000,
  B11000000
};

const PROGMEM byte FontE[] {
  3,5,
  B11100000,
  B10000000,
  B11100000,
  B10000000,
  B11100000
};

const PROGMEM byte FontF[] {
  3,5,
  B11100000,
  B10000000,
  B11100000,
  B10000000,
  B10000000
};

const PROGMEM byte FontG[] {
  3,5,
  B11100000,
  B10000000,
  B10100000,
  B10100000,
  B11100000
};

const PROGMEM byte FontH[] {
  3,5,
  B10100000,
  B10100000,
  B11100000,
  B10100000,
  B10100000
};

const PROGMEM byte FontI[] {
  3,5,
  B11100000,
  B01000000,
  B01000000,
  B01000000,
  B11100000
};

const PROGMEM byte FontJ[] {
  3,5,
  B01100000,
  B10100000,
  B00100000,
  B00100000,
  B11000000
};

const PROGMEM byte FontK[] {
  3,5,
  B10100000,
  B10100000,
  B11000000,
  B10100000,
  B10100000
};

const PROGMEM byte FontL[] {
  3,5,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B11100000
};

const PROGMEM byte FontM[] {
  3,5,
  B10100000,
  B11100000,
  B11100000,
  B10100000,
  B10100000
};

const PROGMEM byte FontN[] {
  3,5,
  B10100000,
  B11100000,
  B11100000,
  B11100000,
  B10100000
};

const PROGMEM byte FontO[] {
  3,5,
  B11100000,
  B10100000,
  B10100000,
  B10100000,
  B11100000
};

const PROGMEM byte FontP[] {
  3,5,
  B11100000,
  B10100000,
  B11100000,
  B10000000,
  B10000000
};

const PROGMEM byte FontQ[] {
  3,5,
  B11100000,
  B10100000,
  B10100000,
  B11100000,
  B00100000
};

const PROGMEM byte FontR[] {
  3,5,
  B11100000,
  B10100000,
  B11000000,
  B10100000,
  B10100000
};

const PROGMEM byte FontS[] {
  3,5,
  B11100000,
  B10000000,
  B11100000,
  B00100000,
  B11100000
};

const PROGMEM byte FontT[] {
  3,5,
  B11100000,
  B01000000,
  B01000000,
  B01000000,
  B01000000
};

const PROGMEM byte FontU[] {
  3,5,
  B10100000,
  B10100000,
  B10100000,
  B10100000,
  B11100000
};

const PROGMEM byte FontV[] {
  3,5,
  B10100000,
  B10100000,
  B10100000,
  B10100000,
  B01000000
};

const PROGMEM byte FontW[] {
  3,5,
  B10100000,
  B10100000,
  B11100000,
  B11100000,
  B10100000
};

const PROGMEM byte FontX[] {
  3,5,
  B10100000,
  B10100000,
  B01000000,
  B10100000,
  B10100000
};

const PROGMEM byte FontY[] {
  3,5,
  B10100000,
  B10100000,
  B01000000,
  B01000000,
  B01000000
};

const PROGMEM byte FontZ[] {
  3,5,
  B11100000,
  B10100000,
  B01000000,
  B00100000,
  B11100000
};

const PROGMEM byte Font0[] {
  3,5,
  B11100000,
  B10100000,
  B10100000,
  B10100000,
  B11100000
};

const PROGMEM byte Font1[] {
  3,5,
  B01000000,
  B11000000,
  B01000000,
  B01000000,
  B01000000
};

const PROGMEM byte Font2[] {
  3,5,
  B11000000,
  B10100000,
  B00100000,
  B01000000,
  B11100000
};

const PROGMEM byte Font3[] {
  3,5,
  B11100000,
  B00100000,
  B11100000,
  B00100000,
  B11100000
};

const PROGMEM byte Font4[] {
  3,5,
  B10100000,
  B10100000,
  B11100000,
  B00100000,
  B00100000
};

const PROGMEM byte Font5[] {
  3,5,
  B11100000,
  B10100000,
  B11000000,
  B00100000,
  B11000000
};

const PROGMEM byte Font6[] {
  3,5,
  B11100000,
  B10000000,
  B11100000,
  B10100000,
  B11100000
};

const PROGMEM byte Font7[] {
  3,5,
  B11100000,
  B00100000,
  B01000000,
  B01000000,
  B01000000
};

const PROGMEM byte Font8[] {
  3,5,
  B11100000,
  B10100000,
  B11100000,
  B10100000,
  B11100000
};

const PROGMEM byte Font9[] {
  3,5,
  B11100000,
  B10100000,
  B11100000,
  B00100000,
  B00100000
};

const PROGMEM byte FontDOT[] {
  3,5,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B01000000
};

const PROGMEM byte FontEXDOT[] {
  3,5,
  B01000000,
  B01000000,
  B01000000,
  B00000000,
  B01000000
};

const PROGMEM byte FontINDOT[] {
  3,5,
  B11100000,
  B00100000,
  B01000000,
  B00000000,
  B01000000
};

const PROGMEM byte FontCOMMA[] {
  3,5,
  B00000000,
  B00000000,
  B00000000,
  B01000000,
  B10000000
};

const byte* font[] = {
  FontA,
  FontB,
  FontC,
  FontD,
  FontE,
  FontF,
  FontG,
  FontH,
  FontI,
  FontJ,
  FontK,
  FontL,
  FontM,
  FontN,
  FontO,
  FontP,
  FontQ,
  FontR,
  FontS,
  FontT,
  FontU,
  FontV,
  FontW,
  FontX,
  FontY,
  FontZ,
  Font0,
  Font1,
  Font2,
  Font3,
  Font4,
  Font5,
  Font6,
  Font7,
  Font8,
  Font9,
  FontDOT,
  FontEXDOT,
  FontINDOT,
  FontCOMMA
};

const PROGMEM byte ShopUIArrowL[] {
  3,5,
  B00100000,
  B01100000,
  B11100000,
  B01100000,
  B00100000
};

const PROGMEM byte ShopUIArrowR[] {
  3,5,
  B10000000,
  B11000000,
  B11100000,
  B11000000,
  B10000000
};

const PROGMEM byte ShopUICoin[] {
  4,4,
  B01100000,
  B10110000,
  B10110000,
  B01100000
};

const PROGMEM byte ShopPlayer0[] {
  8,8,
  B11111111,
  B11111111,
  B11101101,
  B11101101,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};

const PROGMEM byte ShopPlayer1[] {
  8,8,
  B11111111,
  B11111111,
  B11101101,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};

const PROGMEM byte ShopPlayer2[] {
  8,8,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};

const PROGMEM byte ShopStrangeGuy[] {
  8,5,
  B11111111,
  B11111111,
  B10110111,
  B10110111,
  B11111111
};

const PROGMEM byte ShopStrangeGuyHat[] {
  8,9,
  B00000000,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00100100,
  B01111110
};

const PROGMEM byte ShopStrangeGuyHatOutline[] {
  8,9,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110
};

const PROGMEM byte ShopSpeachBubble[] {
  16,8,
  B11111111,B11000000,
  B00001111,B11100000,
  B00000001,B11110000,
  B00000000,B00011000,
  B00000000,B00000000,
  B00000000,B00000000,
  B00000000,B00000000,
  B00000000,B00000000,
  B00000000,B00000000
};

const PROGMEM byte ShopSpeakSizeList[] {
  1,2,3,2,1,2,1,2,3,4,5,4,3,4,5,4,3,2,1
};

const byte* shop[] = {
  ShopUIArrowR,     //0
  ShopUIArrowL,     //1
  ShopUICoin,       //2
  ShopPlayer0,      //3
  ShopPlayer1,      //4
  ShopPlayer2,      //5
  ShopStrangeGuy,   //6
  ShopStrangeGuyHat,//7
  ShopSpeachBubble,  //8
  ShopStrangeGuyHatOutline //9
};

//Main variable
Gamebuino gb;

//Shop Variables
bool isShopping = false;
byte ShopID = 0;
short ShopValue = 0;
short ShopValue1 = 0;
short ShopValue2 = 0;
short ShopValue3 = 0;
short ShopValue4 = 0;
short ShopValue5 = 0;

byte ShopEyeTimerLength = 60;
byte ShopEyeSize = 2;
byte ShopEyeTimer = 0;

byte ShopSpeakTimeLength = 18;
byte ShopSpeakSize = 1;
byte ShopSpeakTimer = 0;

void setup() {
  gb.begin();
  initGame();

  enterShop(0);
}

void loop() {
  if(gb.update()) {
    
    if(isShopping) {
      if(ShopID == 0) {
        gb.display.setColor(BLACK);
        
        gb.display.drawBitmap(2,1,font[18]);
        gb.display.drawBitmap(2+4,1,font[7]);
        gb.display.drawBitmap(2+4*2,1,font[14]);
        gb.display.drawBitmap(2+4*3,1,font[15]);

        gb.display.drawFastHLine(0,7,34);
        gb.display.drawFastHLine(0+34+36,7,14);

        gb.display.fillRect(35,5,34,28);
        gb.display.setColor(WHITE);
        gb.display.drawRect(36,6,32,26);
        gb.display.setColor(BLACK);
        gb.display.drawBitmap(58,33,shop[8]);
        
        gb.display.drawBitmap(1,9,shop[1]);
        gb.display.drawBitmap(1+6,9,shop[0]);

        gb.display.drawBitmap(3,48-11,shop[3+(2-ShopEyeSize)]);
        
        ShopEyeTimer--;
        if(ShopEyeTimer == 0) {
          ShopEyeSize = 0;
          ShopEyeTimer = ShopEyeTimerLength;
        } else if(ShopEyeTimerLength == 1) {
          ShopEyeSize = 0;
        } else if(ShopEyeTimerLength == 2) {
          ShopEyeSize = 1;
        } else if(ShopEyeTimerLength == 3) {
          ShopEyeSize = 1;
        } else {
          ShopEyeSize = 2;
        }

        ShopSpeakSize = pgm_read_byte(ShopSpeakSizeList + ShopSpeakTimer);
        gb.display.fillRect(84-11,48-3-26,8,26);
        gb.display.fillRect(84-(11-8)-1,48-3-26-ShopSpeakSize,1,ShopSpeakSize);
        gb.display.drawBitmap(84-11,48-3-5-26-ShopSpeakSize,shop[6]);
        gb.display.setColor(WHITE);
        gb.display.drawBitmap(84-11,48-3-5-26-ShopSpeakSize-9,shop[9]);
        gb.display.setColor(BLACK);
        gb.display.drawBitmap(84-11,48-3-5-26-ShopSpeakSize-9,shop[7]);

        ShopSpeakTimer++;
        if(ShopSpeakTimer > ShopSpeakTimeLength) {
          ShopSpeakTimer = 0;
        }

        gb.display.drawRect(1,15,32,21);
        gb.display.drawRect(1,15,11,11);
        gb.display.drawRect(11,15,22,9);
        gb.display.drawBitmap(11+2,15+2,shop[2]);

        ShopValue = 896;

        ShopValue1 = (ShopValue)/100;
        ShopValue2 = (ShopValue-ShopValue1*100)/10;
        ShopValue3 = (ShopValue-ShopValue1*100-ShopValue2*10)/1;
        
        gb.display.drawBitmap(11+2+5,15+2,font[26+ShopValue1]);
        gb.display.drawBitmap(11+2+5+4,15+2,font[26+ShopValue2]);
        gb.display.drawBitmap(11+2+5+4*2,15+2,font[26+ShopValue3]);
      }
    }
  }
}

//Shop Functions
void enterShop (byte ID) {
  isShopping = true;
  ShopID = ID;
}

void exitShop () {
  isShopping = false;
}

//Main Functions
void initGame() {
  gb.titleScreen(F("Splash Box - Shops"));
  gb.battery.show = false;
  gb.setFrameRate(15);
}
