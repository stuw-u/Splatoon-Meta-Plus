// MAIN FUNCTIONS
/////////////////

byte floorToBound (byte Value, byte Bound) {
  return floor((double)Value/Bound) * Bound;
}

byte clamp(byte n, byte lower, byte upper) {
    n = ( n > lower ) * n + !( n > lower ) * lower;
    return ( n < upper ) * n + !( n < upper ) * upper;
}

int toScreenX(int x) {
  return x - cameraX;
}

int toScreenY(int y) {
  return y - cameraY;
}

boolean inRay (byte Min, byte Max, byte Value) {
  return Value <= Max && Value >= Min;
}

boolean inRange (byte r, byte v) {
  return inRay(r,r+8,v+2) || inRay(r,r+8,v+6);
}

int ClampInt (int minv, int16_t maxv, int16_t value) {
  if(value < minv)
    return minv;
  else if(value >= maxv)
    return maxv;
  else
    return value;
}


bool isOffScreen(int16_t x, int16_t y, byte width, byte height) {
  return !((toScreenX(x + width) > 0) && (toScreenX(x) < LCDWIDTH) && (toScreenY(y) < LCDHEIGHT) && (toScreenY(y + height) > 0));
}

bool isOffScreenW(int16_t x, int16_t y, byte width, byte height) {
  return ((toScreenX(x + width) > 0) && (toScreenX(x) < LCDWIDTH) && (toScreenY(y) < LCDHEIGHT) && (toScreenY(y + height) > 0));
}


// WORLD MANAGER
/////////////////

#define MaxMapW 30 //78
#define MaxMapH 10 //32

class World {
  public:
  byte MapWidth = 30;
  byte MapHeight = 10;
  int32_t WaterWave = 0;
  
  int16_t Anger = 0;
  
  byte splashMemory[MaxMapW/2][MaxMapH];
  byte splashMemoryColor[(int)ceil(MaxMapW*MaxMapH/8)];

  void SMSetPaintValueNoColor (byte Paint0, byte Paint1, byte Paint2, byte Paint3, byte setX, byte setY) {
    if(GetTile(setX,setY) == 0) return;
    splashMemory[setX][setY] = (Paint3 << 6) + (Paint2 << 4) + (Paint1 << 2) + Paint0;
  }
  
  void SMSetPaintValue (byte Paint0, byte Paint1, byte Paint2, byte Paint3, byte setX, byte setY, byte color) {
    if(GetTile(setX,setY) == 0) return;
    SMSetColor(setX, setY, color); 
    if(setX%2==0) {
      splashMemory[setX/2][setY] = (Paint3 << 3) + (Paint2 << 2) + (Paint1 << 1) + (Paint0 << 0) + (splashMemory[setX/2][setY] & B11110000);
    } else {
      splashMemory[setX/2][setY] = ((Paint3 << 3) + (Paint2 << 2) + (Paint1 << 1) + (Paint0 << 0) << 4) + (splashMemory[setX/2][setY] & B00001111);
    }
  }

  void SMSetPaintAtWallIndex (byte setX, byte setY, byte index, byte color) {
    V0 = SMGetPaintValueAt(setX,setY,0);
    V1 = SMGetPaintValueAt(setX,setY,1);
    V2 = SMGetPaintValueAt(setX,setY,2);
    V3 = SMGetPaintValueAt(setX,setY,3);

    if(index == 0) {
      V0 = 1;
    } else if(index == 1) {
      V1 = 1;
    } else if(index == 2) {
      V2 = 1;
    } else if(index == 3) {
      V3 = 1;
    }
    
    SMSetPaintValue(
      V0,
      V1,
      V2,
      V3,
      constrain(setX,0,MapWidth-1),
      constrain(setY,0,MapHeight-1),
      color
    );
  }
  
  void SMSetColor (byte setX, byte setY, byte Color) {
    if(Color) {
      splashMemoryColor[Div8(setX + (setY * MaxMapW))] |= 1 << ((setX + (setY * MaxMapW)) % 8);
    } else {
      splashMemoryColor[Div8(setX + (setY * MaxMapW))] &= ~(1 << ((setX + (setY * MaxMapW)) % 8));
    }
  }
  
  byte SMGetColor (byte getX, byte getY) {
    return ((splashMemoryColor[Div8(getX + (getY * MaxMapW))] >> ((getX + (getY * MaxMapW)) % 8)) & B00000001);
  }

  byte SMGetPaintValueAt (byte getX, byte getY, byte index) {
    if(index == 0) {
      return (splashMemory[getX/2][getY] >> 0 + (4 * (getX % 2))) & B00000001;
    } else if(index == 1) {
      return (splashMemory[getX/2][getY] >> 1 + (4 * (getX % 2))) & B00000001;
    } else if(index == 2) {
      return (splashMemory[getX/2][getY] >> 2 + (4 * (getX % 2))) & B00000001;
    } else if(index == 3) {
      return (splashMemory[getX/2][getY] >> 3 + (4 * (getX % 2))) & B00000001;
    } else if(index == 4) {
      return MergeWalls((splashMemory[getX/2][getY] >> 0 + (4 * (getX % 2))) & B00000001, (splashMemory[getX/2][getY] >> 1 + (4 * (getX % 2))) & B00000001);
    } else if(index == 5) {
      return MergeWalls((splashMemory[getX/2][getY] >> 1 + (4 * (getX % 2))) & B00000001, (splashMemory[getX/2][getY] >> 2 + (4 * (getX % 2))) & B00000001);
    } else if(index == 6) {
      return MergeWalls((splashMemory[getX/2][getY] >> 2 + (4 * (getX % 2))) & B00000001, (splashMemory[getX/2][getY] >> 3 + (4 * (getX % 2))) & B00000001);
    } else if(index == 7) {
      return MergeWalls((splashMemory[getX/2][getY] >> 3 + (4 * (getX % 2))) & B00000001, (splashMemory[getX/2][getY] >> 0 + (4 * (getX % 2))) & B00000001);
    }
  }

  byte SMGetRaw (byte getX, byte getY) {
    return (splashMemory[getX/2][getY] >> 4 * (getX % 2)) & B00001111;
  }

  void SMResetMap () {
    for(byte x = 0; x < MaxMapW/2; x++) {
      for(byte y = 0; y < MaxMapH; y++) {
        splashMemory[x][y] = 0;
      }
    }
  }
  
  int16_t MergeWalls (int16_t a, int16_t b) {
    return a|b;
  }
  
  int16_t PixelInCollider (byte PIMX, byte PIMY, byte PICX, byte PICY, byte t) { //PIM = Pos in map (0-255), PIC = Pos in cube (0-8)
    byte gt = 0;
    if(t==0) {
      gt = GetTile(PIMX+Div8(PICX),PIMY+Div8(PICY));
    } else if(t == 2) {
      return 0;
    } else {
      gt = GetTileNoObstacle(PIMX+Div8(PICX),PIMY+Div8(PICY));
    }
    if(gt == 0) return 0;
    if(gt == 1) return 1;
    if(gt > 1) return gb.display.getBitmapPixel(Colliders[TilesParams_Array[gt*TileParamsCount+0]],PICX%8,PICY%8);
  }

  byte ThemeTileTypeMerge (byte Theme, byte TileType) {
    return ((Theme & B00000111) << 5) + (TileType & B00011111);
  }

  byte ThemeTileTypeGetTheme (byte Source) {
    return (Source >> 5) & B00000111;
  }

  byte ThemeTileTypeGetTileType (byte Source) {
    return Source & B00011111;
  }

  byte GetTile(int16_t x, int16_t y){
    if(x<0||y<0||x>=MaxMapW||y>=MaxMapH) {
      return 1;
    }
    return BossArena[x+y*MapWidth];
  }

  byte GetTileNoObstacle(int16_t x, int16_t y){
    if(x<0||y<0||x>=MaxMapW||y>=MaxMapH) {
      return 1;
    }
    return BossArena[x+y*MapWidth];
  }

  uint16_t GetSpawnPositionX () {
    return (MapWidth/2-2)*64+32;
  }

  uint16_t GetSpawnPositionY () {
    return (2*64);
  }

  void Draw (uint8_t RenderMode) {
    int16_t xMin = Div8(cameraX);
    int16_t xMax = Div8(LCDWIDTH) + Div8(cameraX) + 2;
    int16_t yMin = Div8(cameraY);
    int16_t yMax = Div8(LCDHEIGHT) + Div8(cameraY) + 2;
    /*if(RenderMode == 0) {
      if(constrain(GetMap[CurrentLoadedMap][2],xMin,xMax) == GetMap[CurrentLoadedMap][2]) {
        if(constrain(GetMap[CurrentLoadedMap][3],yMin,yMax) == GetMap[CurrentLoadedMap][3]) {
          gb.display.drawImage(toScreenX(GetMap[CurrentLoadedMap][2]*8-4),toScreenY(GetMap[CurrentLoadedMap][3]*8+9),SpawnBoard);
        }
      }
      if(constrain(GetMap[CurrentLoadedMap][0]-GetMap[CurrentLoadedMap][2],xMin,xMax) == GetMap[CurrentLoadedMap][0]-GetMap[CurrentLoadedMap][2]) {
        if(constrain(GetMap[CurrentLoadedMap][3],yMin,yMax) == GetMap[CurrentLoadedMap][3]) {
          gb.display.drawImage(toScreenX((GetMap[CurrentLoadedMap][0]-GetMap[CurrentLoadedMap][2])*8-12),toScreenY(GetMap[CurrentLoadedMap][3]*8+9),SpawnBoard,-SpawnBoard.width(),SpawnBoard.height());
        }
      }
    }*/
    if(cpuLoad > 95) {
      yMin++;
      yMax--;
      xMax--;
    }
    if(PartialRendering) {
      yMin+=3;
      yMax-=4;
    }
    
    for(int16_t y = yMin; y < yMax; y++) {
      for(int16_t x = xMin; x < xMax; x++ ) {
        byte gt = GetTile(x,y);
        byte gc = TilesParams_Array[gt*TileParamsCount+0];
        if(gt == 0) {
          continue;
        }

        if(gt <= 9 || gt == 13) {
          //Quick noshadow/noink draw
          if(gt <= 9) {
            NoShadow.setFrame(gt-1);
          } else {
            NoShadow.setFrame(9);
          }
          gb.display.drawImage(Mul8(x) - cameraX, Mul8(y) - cameraY, NoShadow);
          continue;
        }

        if(gt == 10 || gt == 11 || gt == 12) {
          Tiles.setFrame(gt-10);
        } else {
          Tiles.setFrame(gt-11);
        }
        gb.display.drawImage(Mul8(x) - cameraX, Mul8(y) - cameraY, Tiles);

        if(RenderMode == 1) {
          continue;
        }
        
        if(gc > 0) {
          cC = SMGetColor(x,y);
          V1 = SMGetPaintValueAt(x,y,0);
          V3 = SMGetPaintValueAt(x,y,1);
          V5 = SMGetPaintValueAt(x,y,2);
          V7 = SMGetPaintValueAt(x,y,3);
          
          for(int8_t x1 = -1; x1 < 2; x1++) {
            for(int8_t y1 = -1; y1 < 2; y1++) {
              if(!(x+x1<0 || x+x1>=MapWidth || y+y1<0 || y+y1>=MapHeight)) {
                if(abs(x1)+abs(y1) > 1) {
                  continue;
                } 
                if(TilesParams_Array[GetTile(x+x1,y+y1)*TileParamsCount+0] == 0) {

                  //Ink
                  if(cC == 0) {
                    gb.display.setColor((ColorIndex)7);
                  } else {
                    gb.display.setColor((ColorIndex)9);
                  }

                  randomSeed((x+y*MaxMapW) + (x1+y1*8));
                  byte ii = getInkIndex(gc,x1,-y1);

                  if(gc != 10) {
                    if(x1<0) {
                      if(y1==0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V7*ii], getRot(gc,x1,y1,ii), getFlip(gc,x1,-y1,ii));
                      }
                    }
                    if(x1>0) {
                      if(y1==0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V3*ii], getRot(gc,x1,y1,ii), getFlip(gc,x1,-y1,ii));
                      } 
                    }
                    if(x1==0) {
                      if(y1<0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V1*ii], getRot(gc,x1,y1,ii), getFlip(gc,x1,-y1,ii));
                      } else if(y1>0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V5*ii], getRot(gc,x1,y1,ii), getFlip(gc,x1,-y1,ii));
                      }
                    }
                  }
                }
              }
            }
          }
          randomSeed(WaterWave);
        }
      }
    }

    WaterWave++;
  }
  
  void Initialize (byte S) {
    Anger = 0;
    randomSeed(S);
  }
  
  void Update (uint8_t RenderMode) {
    Draw(RenderMode);
  }
};

World world;
