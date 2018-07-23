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

#define MaxMapW 78 //78
#define MaxMapH 32 //32

class World {
  public:
  byte MapWidth = 0;
  byte MapHeight = 0;
  byte CurrentLoadedMap = 0;
  
  byte splashMemory[MaxMapW][MaxMapH];
  byte splashMemoryColor[(int)ceil(MaxMapW*MaxMapH/8)];

  void SMSetPaintValueNoColor (byte Paint0, byte Paint1, byte Paint2, byte Paint3, byte setX, byte setY) {
    if(getTile(setX,setY) == 0) {
      return; 
    }
    splashMemory[setX][setY] = (Paint3 << 6) + (Paint2 << 4) + (Paint1 << 2) + Paint0;
  }
  
  void SMSetPaintValue (byte Paint0, byte Paint1, byte Paint2, byte Paint3, byte setX, byte setY, byte color) {
    //TODO: instead of resetting value to 0, reduce the value each call and change the color once the values reach 0
    /*if(SMGetColor(setX,setY)!=color) {
      SMSetPaintValueNoColor(0,0,0,0,setX,setY);
      SMSetColor(setX, setY, color);
      return;
    }*/
    
    SMSetColor(setX, setY, color);
    
    if(getTile(setX,setY) == 0) {
      return; 
    }
    splashMemory[setX][setY] = (Paint3 << 6) + (Paint2 << 4) + (Paint1 << 2) + Paint0;
  }
  
  void SMSetColor (byte setX, byte setY, byte Color) {
    int16_t offset = Div8(setX + (setY * MaxMapW));
    byte i = (setX + (setY * MaxMapW)) % 8;
  
    byte mask = 1 << i;
    if(Color) {
      splashMemoryColor[offset] |= mask;
    } else {
      splashMemoryColor[offset] &= ~mask;
    }
  }
  
  byte SMGetColor (byte getX, byte getY) {
    uint16_t offset = Div8(getX + (getY * MaxMapW));
    byte i = (getX + (getY * MaxMapW)) % 8;
    return ((splashMemoryColor[offset] >> i) & B00000001);
  }
  
  byte SMGetRaw (byte getX, byte getY) {
    return splashMemory[getX][getY];
  }

  byte SMGetPaintValueAt (byte getX, byte getY, byte index) {
    if(index == 0) {
      return splashMemory[getX][getY] & B00000011;
    } else if(index == 1) {
      return (splashMemory[getX][getY] >> 2) & B00000011;
    } else if(index == 2) {
      return (splashMemory[getX][getY] >> 4) & B00000011;
    } else if(index == 3) {
      return (splashMemory[getX][getY] >> 6) & B00000011;
    } else if(index == 4) {
      return MergeWalls(splashMemory[getX][getY] & B00000011,(splashMemory[getX][getY] >> 2) & B00000011);
    } else if(index == 5) {
      return MergeWalls((splashMemory[getX][getY] >> 2) & B00000011,(splashMemory[getX][getY] >> 4) & B00000011);
    } else if(index == 6) {
      return MergeWalls((splashMemory[getX][getY] >> 4) & B00000011,(splashMemory[getX][getY] >> 6) & B00000011);
    } else if(index == 7) {
      return MergeWalls((splashMemory[getX][getY] >> 6) & B00000011,splashMemory[getX][getY] & B00000011);
    }
  }

  void SMResetMap () {
    for(byte x = 0; x < MapWidth; x++) {
      for(byte y = 0; y < MapHeight; y++) {
        splashMemory[x][y] = 0;
      }
    }
  }
  
  int16_t MergeWalls (int16_t a, int16_t b) {
    return (a+b)/2;
  }
  
  int16_t PixelInCollider (byte PIMX, byte PIMY, byte PICX, byte PICY) { //PIM = Pos in map (0-255), PIC = Pos in cube (0-8)
    byte gt = getTile(PIMX+Div8(PICX),PIMY+Div8(PICY));
    if(gt == 0) return 0;
    if(gt == 1) return 1;
    if(gt > 1) return gb.display.getBitmapPixel(Colliders[TilesParams_Array[gt*TileParamsCount+0]],PICX%8,PICY%8);
  }

  byte getTile(byte x, byte y){
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
        byte gt = getTile(x,y);
        if(gt == 0) {
          continue;
        }

        //gb.display.setColor(WHITE);
        //gb.display.fillRect(CPosX + x*8, CPosY + y*8, 8, 8);
        //gb.display.setColor(BLACK);
        //gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, mapTiles[getTile(x,y)]);
        Tiles.setFrame(getTile(x,y));
        gb.display.drawImage(Mul8(x) - cameraX, Mul8(y) - cameraY,Tiles);

        if(RenderMode == 1) {
          continue;
        }
        
        if(TilesParams_Array[gt*TileParamsCount+0] == 1 && SMGetRaw(x,y) != 0) {
          //V = DebugBytes;
          cC = SMGetColor(x,y);
          V1 = SMGetPaintValueAt(x,y,0);
          V3 = SMGetPaintValueAt(x,y,1);
          V5 = SMGetPaintValueAt(x,y,2);
          V7 = SMGetPaintValueAt(x,y,3);
          
          //V4 = SMGetPaintValueAt(x,y,4);
          //V6 = SMGetPaintValueAt(x,y,5);
          //V0 = SMGetPaintValueAt(x,y,6);
          //V2 = SMGetPaintValueAt(x,y,7);
          
          for(int8_t x1 = -1; x1 < 2; x1++) {
            for(int8_t y1 = -1; y1 < 2; y1++) {
              if(!(x+x1<0 || x+x1>=MapWidth || y+y1<0 || y+y1>=MapHeight)) {
                if(abs(x1)+abs(y1) > 1) {
                  continue;
                } 
                if(TilesParams_Array[getTile(x+x1,y+y1)*5+0] == 0) {

                  //Ink
                  setColorToGroup(cC);

                  if(x1<0) {
                    if(y1<0) {
                      /*if(getTile(x+-1,y+0) != 0 && getTile(x+0,y+-1) != 0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[4+V6], ROTCCW, NOFLIP); //WORKS //Kinda confirmed?
                      }*/
                    } else if(y1>0) {
                      /*if(getTile(x+-1,y+0) != 0 && getTile(x+0,y+1) != 0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[4+V0], ROT180, NOFLIP); //WORKS
                      }*/
                    } else if(y1==0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[V7], ROTCCW, NOFLIP);
                    }
                  }
                  if(x1>0) {
                    if(y1<0) {
                      /*if(getTile(x+1,y+0) != 0 && getTile(x+0,y+-1) != 0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[4+V4], NOROT, NOFLIP); //WORKS
                      }*/
                    } else if(y1>0) {
                      /*if(getTile(x+0,y+1) != 0 && getTile(x+1,y+0) != 0) {
                        gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, splashes[4+V2], ROTCW, NOFLIP); //WORKS //Kinda confirmed?
                      }*/
                    } else if(y1==0) {
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
          /*for(int8_t x1 = -1; x1 < 2; x1++) {
            for(int8_t y1 = -1; y1 < 2; y1++) {
              if(!(x+x1<0 || x+x1>=MapWidth || y+y1<0 || y+y1>=MapHeight)) {
                if(getTile(x+x1,y+y1)==0) {
                  //Outline
                  gb.display.setColor(BLACK);
                  if(x1<0) {
                    if(y1<0) {
                      if(getTile(x+-1,y+0) == 1 && getTile(x+0,y+-1) == 1) {
                        gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+4+3]);
                      }
                    } else if(y1>0) {
                      if(getTile(x+-1,y+0) == 1 && getTile(x+0,y+1) == 1) {
                        gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+4+0]);
                      }
                    } else if(y1==0) {
                      gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+3]);
                    }
                  }
                  if(x1>0) {
                    if(y1<0) {
                      if(getTile(x+1,y+0) == 1 && getTile(x+0,y+-1) == 1) {
                        gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+4+2]);
                      }
                    } else if(y1>0) {
                      if(getTile(x+0,y+1) == 1 && getTile(x+1,y+0) == 1) {
                        gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+4+1]);
                      }
                    } else if(y1==0) {
                      gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+1]);
                    }
                  }
                  if(x1==0) {
                    if(y1<0) {
                      gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+0]);
                    } else if(y1>0) {
                      gb.display.drawBitmap(x*8 - cameraX, y*8 - cameraY, outlines[2+2]);
                    }
                  }
                }
              }
            }
          }*/
        }
      }
    }
  }

  void Update (uint8_t RenderMode) {
    Draw(RenderMode);
  }
};

World world;
