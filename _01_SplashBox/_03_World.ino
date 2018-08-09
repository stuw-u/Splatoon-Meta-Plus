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


// WORLD MANAGER
/////////////////

#define MaxMapW 80 //78
#define MaxMapH 32 //32

class World {
  public:
  byte MapWidth = 0;
  byte MapHeight = 0;
  byte CurrentLoadedMap = 0;
  
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
  
  int16_t PixelInCollider (byte PIMX, byte PIMY, byte PICX, byte PICY) { //PIM = Pos in map (0-255), PIC = Pos in cube (0-8)
    byte gt = GetTile(PIMX+Div8(PICX),PIMY+Div8(PICY));
    if(gt == 0) return 0;
    if(gt == 1) return 1;
    if(gt > 1) return gb.display.getBitmapPixel(Colliders[TilesParams_Array[gt*TileParamsCount+0]],PICX%8,PICY%8);
  }

  byte GetTile(byte x, byte y){
    return GetMap[CurrentLoadedMap][MapHeader + (x+y*MapWidth)];
  }

  void Draw (uint8_t RenderMode) {
    int16_t xMin = Div8(cameraX);
    int16_t xMax = Div8(LCDWIDTH) + Div8(cameraX) + 2;
    int16_t yMin = Div8(cameraY);
    int16_t yMax = Div8(LCDHEIGHT) + Div8(cameraY) + 2;
    if(RenderMode == 0) {
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
    }
    if(cpuLoad > 95) {
      yMin++;
      yMax--;
      xMax--;
    }
    
    for(int16_t y = yMin; y < yMax; y++) {
      for(int16_t x = xMin; x < xMax; x++ ) {
        byte gt = GetTile(x,y);
        if(gt == 0) {
          continue;
        }
        
        Tiles.setFrame(GetTile(x,y));
        gb.display.drawImage(Mul8(x) - cameraX, Mul8(y) - cameraY,Tiles);

        if(RenderMode == 1) {
          continue;
        }
        
        if(TilesParams_Array[gt*TileParamsCount+0] == 1 && SMGetRaw(x,y) != 0) {
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
                if(TilesParams_Array[GetTile(x+x1,y+y1)*5+0] == 0) {

                  //Ink
                  setColorToGroup(cC);
                  
                  if(x1<0) {
                    if(y1==0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[V7], ROTCCW, NOFLIP);
                    }
                  }
                  if(x1>0) {
                    if(y1==0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[V3], ROTCW, NOFLIP);
                    }
                  }
                  if(x1==0) {
                    if(y1<0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[V1], NOROT, NOFLIP);
                    } else if(y1>0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[V5], ROT180, NOFLIP);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  void Update (uint8_t RenderMode) {
    Draw(RenderMode);
  }
};

World world;
