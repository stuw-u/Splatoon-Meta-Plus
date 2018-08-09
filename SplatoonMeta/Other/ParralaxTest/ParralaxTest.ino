#include <Gamebuino-Meta.h>
#include <math.h>

#define LCDWIDTH 160
#define LCDHEIGHT 128

#define DISTANCEBACKG 5

int cameraX, cameraY;
int backGX, backGY;

const uint8_t ForegroundBrick_Array[] = {
  9, 9, 1, 0, 0, 3, 1,
  0x22,0x22,0x22,0x22,0x32,0x01,0x12,0x01,0x12,0xb0,0x22,0x22,0x22,0x22,0xb2,0x12,0x01,0x12,0x01,0xb1,0x22,0x22,0x22,0x22,0xb2,0x01,0x12,0x01,0x12,0xb0,0x22,0x22,0x22,0x22,0xb2,0x12,0x01,0x12,0x01,0xb1,0x3b,0xbb,0xbb,0xbb,0xb3,
};
Image ForegroundBrick = Image(ForegroundBrick_Array);

const byte BackgroundEmpty[] =
{
  8,8,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};
const byte BackgroundTower[] =
{
  8,8,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110,
  B01111110
};
const byte BackgroundTowerFull[] =
{
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
const byte BackgroundTowerRight[] =
{
  8,8,
  B01111111,
  B01111111,
  B01111111,
  B01111111,
  B01111111,
  B01111111,
  B01111111,
  B01111111
};

const byte BackgroundTowerLeft[] =
{
  8,8,
  B11111110,
  B11111110,
  B11111110,
  B11111110,
  B11111110,
  B11111110,
  B11111110,
  B11111110
};

const byte BackgroundTowerRightCorner[] =
{
  8,8,
  B00000000,
  B00000000,
  B00000010,
  B00000110,
  B00001110,
  B00011110,
  B00111110,
  B01111110
};

const byte BackgroundTowerLeftCorner[] =
{
  8,8,
  B00000000,
  B00000000,
  B01000000,
  B01100000,
  B01110000,
  B01111000,
  B01111100,
  B01111110
};

const byte* backgroundTiles[] = {
  BackgroundEmpty,
  BackgroundTower,
  BackgroundTowerFull,
  BackgroundTowerRight,
  BackgroundTowerLeft,
  BackgroundTowerRightCorner,
  BackgroundTowerLeftCorner
};

const byte Map[] PROGMEM = {
  16,10,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,
  0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,
  0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,
  0,1,1,0,0,0,1,1,1,1,0,0,1,1,0,0,
  0,1,1,0,0,0,1,1,1,1,0,0,1,1,0,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

byte getTile(byte x, byte y){
  return pgm_read_byte(Map + (
    2 + 
    (x%pgm_read_byte(0+Map))+
    constrain(y,-1,pgm_read_byte(1+Map)-1)*pgm_read_byte(0+Map)
  ));
}

const byte Background[] PROGMEM = {
  16,10,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,
  0,0,6,0,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,1,0,0,6,1,0,0,0,0,0,1,0,5,0,
  0,5,1,1,0,1,1,0,0,0,0,0,1,1,1,0,
  0,1,3,4,0,3,4,5,0,1,0,0,1,3,4,0,
  0,1,3,4,0,3,2,4,0,1,0,0,3,2,4,0,
  2,2,2,4,0,3,2,4,0,1,0,3,2,2,2,2,
  2,2,2,2,2,2,2,2,2,4,0,3,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};

byte getBackgroundTile(byte x, byte y){
  return pgm_read_byte(Background + (
    2 + 
    (x%pgm_read_byte(0+Background))+
    constrain(y,-1,pgm_read_byte(1+Background)-1)*pgm_read_byte(0+Background)
  ));
}

Color palette[] = {  
  (Color)0xf779,
  (Color)0xacd0,
  (Color)0x730b,
  (Color)0x2923,
  (Color)0xb15a,
  (Color)0xc9ae,
  (Color)0xe985,
  (Color)0xfc03,
  (Color)0xf5a2,
  (Color)0x8624,
  (Color)0x34a4,
  (Color)0x3291,
  (Color)0x3415,
  (Color)0x461b,
  (Color)0xf62f,
  (Color)0xcc47
};

void setup() {
  gb.begin();
  gb.display.colorIndex = palette;
}

void loop() {
  if(!gb.update()) {
    return;
  }

  gb.display.setColor(LIGHTBLUE);
  gb.display.fill();
  
  if(gb.buttons.repeat(BUTTON_UP,0)) {
    if(gb.buttons.repeat(BUTTON_B,0)) {
      backGY-=3;
    }
    cameraY-=3;
  }
  if(gb.buttons.repeat(BUTTON_DOWN,0)) {
    if(gb.buttons.repeat(BUTTON_B,0)) {
      backGY+=3;
    }
    cameraY+=3;
  }
  if(gb.buttons.repeat(BUTTON_LEFT,0)) {
    if(gb.buttons.repeat(BUTTON_B,0)) {
      backGX-=3;
    }
    cameraX-=3;
  }
  if(gb.buttons.repeat(BUTTON_RIGHT,0)) {
    if(gb.buttons.repeat(BUTTON_B,0)) {
      backGX+=3;
    }
    cameraX+=3;
  }

  int xMin = cameraX / 8;
  int xMax = LCDWIDTH / 8 + cameraX / 8 + 2;
  int yMin = cameraY / 8;
  int yMax = LCDHEIGHT / 8 + cameraY / 8 + 2;

  int xMinB = cameraX/DISTANCEBACKG / 8 + backGX;
  int xMaxB = LCDWIDTH / 8 + cameraX/DISTANCEBACKG / 8 + backGX;
  int yMinB = cameraY/DISTANCEBACKG / 8 + backGY; //The last value represent the background decaling
  int yMaxB = LCDHEIGHT / 8 + cameraY/DISTANCEBACKG / 8 + backGY; //The last value represent the background decaling

  gb.display.setColor(BLUE);
  for(int y = yMinB; y < yMaxB; y++) {
    for(int x = xMinB; x < xMaxB; x++ ) {
      gb.display.drawBitmap(x*8 - cameraX/DISTANCEBACKG, y*8 - cameraY/DISTANCEBACKG, backgroundTiles[getBackgroundTile(x,y)]);
    }
  }
  
  for(int y = yMin; y < yMax; y++) {
    for(int x = xMin; x < xMax; x++ ) {
      if(getTile(x,y)!=0) {
        gb.display.drawImage(x*8 - cameraX, y*8 - cameraY, ForegroundBrick);
      }
    }
  }
}
