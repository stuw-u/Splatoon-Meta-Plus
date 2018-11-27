// INITIALIZATION
/////////////////

#include <Gamebuino-Meta.h>
#include <math.h>

//Save stuff

//0: Coin
//2: Level
//3: WeaponId
//4: WeaponIdSalmon
//5: Missions Unlocked (Byte, Each bit represed 1 mission)
//6: Missions Unlocked (A_B) (Byte, Each bit represed 1 mission)
//7: Eggs Count

//8: Seed0
//9: Seed1
//10: Seed2
//11: Seed3
//12: Seed4
//13: Seed5
//14: Seed6

//15: Difficulty
//16: DifficultyB

//17: Gender
//18: Haircuts

extern const uint8_t SquidSquare[];

#define LCDWIDTH 80
#define LCDHEIGHT 64

#define AnimWALKSPEED 4
#define AnimRUNSPEED 2

//#define SCALE 8
#define VFORCE 5

#define BCOUNT 7
#define PCOUNT 5

#define STARTLENGHT 90
#define STARTLENGHT2 130
#define ENDSCREENLENGHT 110

#define MapHeader 5

//Player mouvement
#define PSwimAcceleration 18 //How fast the player will reach the max speed when swiming in it's own ink
#define PSwimMaxSpeed 116 //The maximum swiming speed 
#define PESwimAcceleration 6 //How fast the player will reach the max speed when swiming in the ennemie ink
#define PESwimMaxSpeed 48 //The maximum swiming speed 
#define PGSwimAcceleration 7 //How fast the player will reach the max speed when squidding on the ground
#define PGSwimMaxSpeed 62 //The maximum squid speed 
#define PSwimWallClimb 84 //How fast the player can swim up a wall
#define PSwimWallReleaseAcceleration 3 //How fast the player reach the max swim up a wall speed
#define PSwimWallReleaseMaxSpeed 18 //The maximum releasing speed when swimming up a wall

#define PJumpForceSquid 134 //The squid jump force
#define PEJumpForceSquid 26 //The jump force when on ennemy ink

#define PERestrictedSpeed 45 //The maximum velocity you can reach on ennemy ink
#define PWalkMaxSpeed 84
#define PWalkAcceleration 9
#define PEWalkMaxSpeed 35
#define PEWalkAcceleration 4

#define PJumpForce 157 //The inkling jump force
#define PEJumpForce 58 //The jump force when on ennemy ink
#define PWallJumpForceX 68 //The inkling horizontal wall jump force
#define PWallJumpForceY 104 //The inkling vertical wall jump force

byte UseBackgroundInGame = true;
byte cinematicSkip = 0;

byte revertColors = 0;
int16_t colorGroup = 1;

int16_t cameraX, cameraY, shakeTimeLeft, shakeAmplitude;

uint8_t cpuLoad = 0;
uint8_t lastLoad[8];
uint8_t difficulty = 0;
uint8_t difficultyB = 0;

uint16_t zoneM1 = 0;
uint16_t zoneM2 = 0;

//UI and Screen parameters
bool IsPlaying = false;//true; //Starts game (GameState define mode, 0:Classic, 1:Rainmaker, 2:Zones, 3:Tower, 4:Clam, 5:SalmonRun)
uint8_t GameState = 0; //0: TitleScreen, //1: Inkopolis, //2: WeaponShop, //3: HatShop, //4: GearSelect, //5: PlaySelection, //6: SalmonRunSelection, //7: CharSelect
bool FreezePlayers = false;
bool TutorialMode = false;
int16_t AnimationTimer;
int32_t AnimationTimer2;
int32_t AnimationTimer3;
int32_t AnimationTimer4;
int32_t AnimationTimer5;
int32_t AnimationTimer6;
int32_t AnimationTimer7;
int32_t AnimationTimer8;
bool PartialRendering = false;

byte Missions = 0;
byte MissionsB = 0;

bool slmKnock = false;

byte hitAnim = 0;

bool IsPaused = false;
int32_t PausedTimer = 0;
byte PausedSelection = 0;

byte SelectedGender = 2; //0: F, 1: M, 2: N/A
byte SelectedHaircut = 6; //4: N/A

bool LastDirection = true;
byte curX = 48;
byte curY = 24;

byte* D;
byte V0 = 0;
byte V1 = 0;
byte V2 = 0;
byte V3 = 0;
byte V4 = 0;
byte V5 = 0;
byte V6 = 0;
byte V7 = 0;
byte cC = 0;

Color paletteAllWhite[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xf779, //(Color)0xacd0 Grey
  (Color)0xf779, //(Color)0x730b Dark Grey
  (Color)0xacd0, //(Color)0x2923 Black
  (Color)0xf779, //(Color)0xb15a Purple
  (Color)0xf779, //(Color)0xc9ae Magenta
  (Color)0xf779, //(Color)0xe985 Red
  (Color)0xf779, //(Color)0xfc03 Orange
  (Color)0xf779, //(Color)0xf5a2 Yellow
  (Color)0xf779, //(Color)0x8624 Green
  (Color)0xf779, //(Color)0x34a4 Dark Green
  (Color)0xf779, //(Color)0x3291 Dark Blue
  (Color)0xf779, //(Color)0x3415 Blue
  (Color)0xf779, //(Color)0x461b Light Blue
  (Color)0xf779, //(Color)0xf62f Beige (Light Skin)
  (Color)0xf779  //(Color)0xcc47 Brown (Dark Skin)
};

Color paletteFlipBlackGrey[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0x2923, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0xacd0, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color paletteFade0[] = {  
  (Color)0x3415, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3291, //(Color)0x3415 Blue
  (Color)0x3291, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color paletteFade1[] = {  
  (Color)0x461b, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3291, //(Color)0x3415 Blue
  (Color)0x3415, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color GlitchPalette[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0x461b, //(Color)0xacd0 Grey
  (Color)0xc9ae, //(Color)0x730b Dark Grey
  (Color)0xf62f, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x3415, //(Color)0x8624 Green
  (Color)0x3291, //(Color)0x34a4 Dark Green
  (Color)0xf5a2, //(Color)0x3291 Dark Blue
  (Color)0xfc03, //(Color)0x3415 Blue
  (Color)0xe985, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0x2923  //(Color)0xcc47 Brown (Dark Skin)
};

Color palette[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color paletteBG[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3af2, //(Color)0x3291 Dark Blue
  (Color)0x33d7, //(Color)0x3415 Blue
  (Color)0x34f9, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color palettePYellow[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xfc03, //(Color)0xe985 Red
  (Color)0xe985, //(Color)0xfc03 Orange
  (Color)0xf5a2, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color palettePOrange[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xf5a2, //(Color)0xfc03 Orange
  (Color)0xfc03, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color palettePRed[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xf5a2, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0xb15a, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xe985, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color palettePMagenta[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xf5a2, //(Color)0xc9ae Magenta
  (Color)0xe985, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0xc9ae, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color palettePBlue[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0x730b, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0x3291, //(Color)0xf5a2 Yellow
  (Color)0x8624, //(Color)0x8624 Green
  (Color)0x34a4, //(Color)0x34a4 Dark Green
  (Color)0xf5a2, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

Color palettePGreen[] = {  
  (Color)0xf779, //(Color)0xf779 White
  (Color)0xacd0, //(Color)0xacd0 Grey
  (Color)0x730b, //(Color)0x730b Dark Grey
  (Color)0x2923, //(Color)0x2923 Black
  (Color)0xb15a, //(Color)0xb15a Purple
  (Color)0xc9ae, //(Color)0xc9ae Magenta
  (Color)0x34a4, //(Color)0xe985 Red
  (Color)0xfc03, //(Color)0xfc03 Orange
  (Color)0x8624, //(Color)0xf5a2 Yellow
  (Color)0xf5a2, //(Color)0x8624 Green
  (Color)0xe985, //(Color)0x34a4 Dark Green
  (Color)0x3291, //(Color)0x3291 Dark Blue
  (Color)0x3415, //(Color)0x3415 Blue
  (Color)0x461b, //(Color)0x461b Light Blue
  (Color)0xf62f, //(Color)0xf62f Beige (Light Skin)
  (Color)0xcc47  //(Color)0xcc47 Brown (Dark Skin)
};

void setPaletteToColorGroup (byte Color) {
  if(Color == 0) {
    gb.display.colorIndex = palettePOrange;
  } else {
    gb.display.colorIndex = palettePGreen;
  }
}

void setColorToGroup (byte cC) {
  if(cC == 0) {
    gb.display.setColor((ColorIndex)7);
  } else {
    gb.display.setColor((ColorIndex)9);
  }
}

void setLightColorToGroup (byte cC) {
  if(cC == 0) {
    gb.lights.setColor((ColorIndex)7);
  } else {
    gb.lights.setColor((ColorIndex)9);
  }
}

template<typename T> T Mul8 (T v) {
    return v << 3;
    //return v*8;
}

template<typename T> T Div8 (T v) {
    return v >> 3;
    //return v/8;
}

template<typename T> T Mul64 (T v) {
    return v << 6;
    //return v*64;
}

template<typename T> T Div64 (T v) {
    return v >> 6;
    //return v/64;
}
