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

boolean isOffScreen(int16_t x, int16_t y, byte width, byte height) {
  return !((toScreenX(x + width) > 0) && (toScreenX(x) < LCDWIDTH) && (toScreenY(y) < LCDHEIGHT) && (toScreenY(y + height) > 0));
}


// WORLD MANAGER
/////////////////

#define MaxMapW 75 //78
#define MaxMapH 25 //32

#define MapTileGroupW 15
#define MapTileGroupH 5

class World {
  public:
  byte MapWidth = 75;
  byte MapHeight = 25;
  
  uint16_t WaterLevel = 16;
  uint32_t WaterWave = 0;
  bool FlagSet = false;
  int8_t FlagWave = 0;
  byte FlagEggs = 0;
  byte FlagGolden = 0;
  byte Anger = 0;

  byte MaxEggs = 20;
  byte MaxGolden = 5;

  byte mapTileGroups[MapTileGroupW][MapTileGroupH+1]; //Last row meant to store themes
  //B Theme Theme Theme Tile Tile Tile Tile Tile
  
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
    byte Theme = ThemeTileTypeGetTheme(mapTileGroups[x/5][y/5]);
    byte TileGroup = ThemeTileTypeGetTileType(mapTileGroups[x/5][y/5]);
    if(Theme == 0) {
      return TileGroup;
    } else if(Theme > 0) {
      return Themes[Theme-1][1+TileGroup * TilesGroupCount + ((x%5) + (y%5) * 5)];
    }
  }

  byte GetTileNoObstacle(int16_t x, int16_t y){
    if(x<0||y<0||x>=MaxMapW||y>=MaxMapH) {
      return 1;
    }
    byte Theme = ThemeTileTypeGetTheme(mapTileGroups[x/5][y/5]);
    byte TileGroup = ThemeTileTypeGetTileType(mapTileGroups[x/5][y/5]);
    if(Theme == 0) {
      return TileGroup;
    } else if(Theme > 0) {
      if(Themes[Theme-1][1+TileGroup * TilesGroupCount + 25] == 0) {
        return Themes[0][1];
      } else if(Themes[Theme-1][1+TileGroup * TilesGroupCount + 25] == 1) {
        return Themes[Theme-1][1+TileGroup * TilesGroupCount + ((x%5) + (y%5) * 5)];
      } else {
        return 0;
      }
    }
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

    if(!isOffScreen(MapWidth*4, 8, 23, 32)) {
      if(FlagGolden > 5) {
        gb.display.drawImage((MapWidth*4 + 10) - cameraX, (8 + 14) - cameraY, Golden_Large);
        gb.display.drawImage((MapWidth*4 + 6) - cameraX, (8 + 16) - cameraY, Golden_Large);
        gb.display.drawImage((MapWidth*4 + 10) - cameraX, (8 + 22) - cameraY, Golden_Large);
        gb.display.drawImage((MapWidth*4 + 5) - cameraX, (8 + 22) - cameraY, Golden_Large);
      } else if(FlagGolden > 3) {
        gb.display.drawImage((MapWidth*4 + 6) - cameraX, (8 + 16) - cameraY, Golden_Large);
        gb.display.drawImage((MapWidth*4 + 10) - cameraX, (8 + 22) - cameraY, Golden_Large);
        gb.display.drawImage((MapWidth*4 + 5) - cameraX, (8 + 22) - cameraY, Golden_Large);
      } else if(FlagGolden > 1) {
        gb.display.drawImage((MapWidth*4 + 10) - cameraX, (8 + 22) - cameraY, Golden_Large);
        gb.display.drawImage((MapWidth*4 + 5) - cameraX, (8 + 22) - cameraY, Golden_Large);
      } else if(FlagGolden > 0) {
        gb.display.drawImage((MapWidth*4 + 5) - cameraX, (8 + 22) - cameraY, Golden_Large);
      }
      gb.display.drawImage(MapWidth*4 - cameraX, 8 - cameraY, Basket);
    }
    if(!isOffScreen((MapWidth/2*8 - 12), 10, 15, 24)) {
      if(FlagSet && FlagWave >= 0) {
        Flag.setFrame(1+FlagWave/2);
        FlagWave++;
        if(FlagWave > 3*2) {
          FlagWave = 0;
        }
      } else {
        if(FlagWave < 0) {
          FlagWave++;
        }
        Flag.setFrame(0);
      }
      gb.display.drawImage((MapWidth/2*8 - 12) - cameraX, 10 - cameraY, Flag);
    }
    
    for(int16_t y = yMin; y < yMax; y++) {
      for(int16_t x = xMin; x < xMax; x++ ) {
        byte gt = GetTile(x,y);
        if(gt == 0 || Mul8(y) >= MapHeight*8-(WaterLevel-8)) {
          continue;
        }
        
        Tiles.setFrame(GetTile(x,y));
        gb.display.drawImage(Mul8(x) - cameraX, Mul8(y) - cameraY, Tiles);

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

                  randomSeed((x+y*MaxMapW) + (x1+y1*8));

                  //Ink
                  if(cC == 0) {
                    gb.display.setColor((ColorIndex)7);
                  } else {
                    gb.display.setColor((ColorIndex)9);
                  }
                  
                  if(x1<0) {
                    if(y1==0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V7*random(1,4)], ROTCCW, NOFLIP);
                    }
                  }
                  if(x1>0) {
                    if(y1==0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V3*random(1,4)], ROTCW, NOFLIP);
                    }
                  }
                  if(x1==0) {
                    if(y1<0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V1*random(1,4)], NOROT, NOFLIP);
                    } else if(y1>0) {
                      gb.display.drawBitmap(Mul8(x) - cameraX, Mul8(y) - cameraY, Ink[V5*random(1,4)], ROT180, NOFLIP);
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

    for(int16_t x = xMin; x < xMax; x++ ) {
      int8_t c = cos(WaterWave)*2;
      if(toScreenY(MapHeight*8-(WaterLevel + c)) < LCDHEIGHT && toScreenY(MapHeight*8-(WaterLevel + c)) >= 0) {
        gb.display.drawImage(toScreenX(Mul8(x)), toScreenY(MapHeight*8-(WaterLevel + c)), Water);
        gb.display.setColor((ColorIndex)11);
        gb.display.fillRect(toScreenX(Mul8(x)), toScreenY(MapHeight*8-(WaterLevel + c)) + 8, 8, LCDHEIGHT-(toScreenY(MapHeight*8-(WaterLevel + c)) + 8));
      }
      WaterWave++;
      if(WaterWave % 520 == 0) {
        WaterLevel++;
      }
    }
  }

  
  byte StuitableTile[8];
  byte CFullfilled = 0;
  byte Seed = 0;
  
  void Initialize (byte S) {
    FlagSet = false;
    FlagWave = 0;
    MaxEggs = random(18,38);
    MaxGolden = random(4,8);
    WaterWave = 0;
    WaterLevel = 16;
    Anger = 0;
    randomSeed(S);
    Seed = S;
    
    byte Cut = random(3,MapTileGroupW-3);
    byte ThemeA = random(0,5);
    byte ThemeB = random(0,5);
    if(ThemeB == ThemeA) {
      ThemeB = ThemeA+2;
      if(ThemeB>=5) {
        ThemeB -= 5;
      }
    }
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      mapTileGroups[x][MapTileGroupH] = (ThemeA)*(x>Cut) + (ThemeB)*(x<=Cut);
    }
    int8_t h = 0;
    bool MiddleAtMaxHeight = false;
    bool EndAtMinHeight = false;
    while(!(MiddleAtMaxHeight && EndAtMinHeight)) {
      h = 0;
      MiddleAtMaxHeight = false;
      EndAtMinHeight = false;
      for(int8_t x = 0; x < MapTileGroupW; x++) {
        int8_t rand0 = random(0,5);
        int8_t rand1 = random(-1,2);
        for(int8_t y = 0; y < MapTileGroupH; y++) {
          /*(x*1.0F/MapTileGroupW)+(y*1.0F/MapTileGroupH) < 1.0F && y != MapTileGroupH-1*/
          mapTileGroups[x][y] = ThemeTileTypeMerge(0,y >= MapTileGroupH-1-h);
        }
        if(rand0>2 && x+1!=MapTileGroupW) {
          h = constrain(h+constrain(rand1,-1,1),0,MapTileGroupH-2);
        }
        if(x==MapTileGroupW/2-1) {
          if(h == MapTileGroupH-2) {
            MiddleAtMaxHeight = true;
          }
        }
        if(x==MapTileGroupW-1) {
          if(h == 0) {
            EndAtMinHeight = true;
          }
        }
      }
    }

    //Phase two!!
    //Theme 0 codes: 0-Air 1-Ground 2-Stair 4-Partial, 5-Full, 7-Deco, 8-Underground


    //Basic p. platform, stair setup
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        //Current Block is Available
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 0 && x+1<MapTileGroupW && y+1<MapTileGroupH) {
          //The block to the > is ground
          //The block bellow is ground
          if(ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) == 1 && ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 1) {
            //Set to stair
            mapTileGroups[x][y] = ThemeTileTypeMerge(0,2);
            if(x-1>=0) {
              if(ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) == 1) {
                //Set as Stair or Partial
                mapTileGroups[x][y] = 4;
              }
            }
          }
        }
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 0 && x-1>=0 && y+1<MapTileGroupH) {
          //The block to the < is ground
          //The block bellow is ground
          if(ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) == 1 && ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 1) {
            //Set to partial
            mapTileGroups[x][y] = 4;
          }
        }
      }
    }

    //Full Support
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        //Current Block is Available
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 0 && x+1<MapTileGroupW && x-1>=0 && y+1<MapTileGroupH) {
          //Blocks on both sids aren't ground and the one bellow is
          if(ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 1) {
            byte rand4 = random(0,4);
            if(rand4 == 0) {
              //Set as full platform
              mapTileGroups[x][y] = ThemeTileTypeMerge(0,5);
            }
          }
        } else
        //Current Block is Ground
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 1 && x+1<MapTileGroupW && x-1>=0 && y+1<MapTileGroupH) {
          //Blocks on both sids aren't ground and the one bellow is
          if(ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 1) {
            byte rand4 = random(0,5);
            if(rand4 == 3) {
              //Set as full platform
              mapTileGroups[x][y] = ThemeTileTypeMerge(0,5);
            }
          }
        } else
        //Current Block is Ground
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 1 && y+1>=MapTileGroupH) {
          //Block above is not ground and the one bellow is out of range
          if(ThemeTileTypeGetTileType(mapTileGroups[x][y-1]) != 1) {
            //Set as full platform
            mapTileGroups[x][y] = ThemeTileTypeMerge(0,5);
          }
        }
      }
    }

    //Full platform stacking
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 0 && x+1<MapTileGroupW && x-1>=0 && y+1<MapTileGroupH) {
          //Blocks on both sids aren't ground and the one bellow is F. Platform
          if(ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 5) {
            byte rand4 = random(0,4);
            if(rand4 == 0) {
              //Set as full platform
              mapTileGroups[x][y] = ThemeTileTypeMerge(0,5);
            } else if(rand4 == 1) {
              mapTileGroups[x][y] = ThemeTileTypeMerge(0,4);
            }
          }
        }
      }
    }

    //Deco
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        //Current Block is Available
        if(ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 0 && x+1<MapTileGroupW && x-1>=0 && y+1<MapTileGroupH) {
          //Blocks on both sids aren't ground and the one bellow is Ground or Full platform
          if(ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) != 1 && 
          (ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 1 || ThemeTileTypeGetTileType(mapTileGroups[x][y+1]) == 5)) {
            byte rand5 = random(0,3);
            if(rand5 == 0) {
              //Set as deco
              mapTileGroups[x][y] = ThemeTileTypeMerge(0,7);
            }
          }
        }
      }
    }

    mapTileGroups[MapTileGroupW/2][0] = 0;

    //Ground no underground
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) == 0 && ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 1) {
          
          bool AL;
          bool AR;
          bool AU;
          if(y-1>=0) {
            if(ThemeTileTypeGetTheme(mapTileGroups[x][y-1]) == 0) {
              AU = (ThemeTileTypeGetTileType(mapTileGroups[x][y-1]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x][y-1]) != 8);
            } else {
              AU = (Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y-1])*TilesGroupCount+25] != 0);
            }
          }
          if(x-1>=0) {
            if(ThemeTileTypeGetTheme(mapTileGroups[x-1][y]) == 0) {
              AL = (ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x-1][y]) != 8);
            } else {
              AL = (Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x-1][y])*TilesGroupCount+25] != 0);
            }
          }
          if(x+1<MapTileGroupW) {
            if(ThemeTileTypeGetTheme(mapTileGroups[x+1][y]) == 0) {
              AR = (ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) != 1 && ThemeTileTypeGetTileType(mapTileGroups[x+1][y]) != 8);
            } else {
              AR = (Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x+1][y])*TilesGroupCount+25] != 0);
            }
          }

          if(!AU) {
            //Underground Check
            mapTileGroups[x][y] = ThemeTileTypeMerge(0,8);
          } else {
            if(!AL && !AR) {
              //Normal Ground
              CFullfilled = 0;
              for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
                if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 0) {
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 0) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 1) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 8 && random(0,5) == 0) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 9 && random(0,5) == 0) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                }
              }
              if(CFullfilled > 0) {
                mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
              }
            } else if(AL && AR) {
              //Two Side Ground
              CFullfilled = 0;
              for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
                if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 0) {
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 2) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 3) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                }
              }
              if(CFullfilled > 0) {
                mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
              }
            } else if(AL && !AR) {
              //LeftSide Ground
              CFullfilled = 0;
              for(byte i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
                if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 0) {
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 4) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 5) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                }
              }
              if(CFullfilled > 0) {
                mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
              }
            } else if(!AL && AR) {
              //RightSide Ground
              CFullfilled = 0;
              for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
                if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 0) {
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 6) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 7) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                }
              }
              if(CFullfilled > 0) {
                mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
              }
            }
          }
        }
      }
    }

    //Stairs and partials and deco
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) == 0 && ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 2) {
          CFullfilled = 0;
          for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
            if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 2) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 1) {
                StuitableTile[CFullfilled] = i;
                CFullfilled++;
              }
            }
            if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 3) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 0) {
                StuitableTile[CFullfilled] = i;
                CFullfilled++;
              }
            }
            if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 3) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 1) {
                if(ThemeTileTypeGetTheme(mapTileGroups[x][y+1]) != 0 && Themes[0][1+ThemeTileTypeGetTileType(mapTileGroups[x][y+1])*TilesGroupCount+25] == 0) {
                  StuitableTile[CFullfilled] = i;
                  CFullfilled++;
                }
              }
            }
          }
          if(CFullfilled > 0) {
            mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
          }
        }
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) == 0 && ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 4) {
          CFullfilled = 0;
          for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
            if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 2) {
              StuitableTile[CFullfilled] = i;
              CFullfilled++;
            }
          }
          if(CFullfilled > 0) {
            mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
          }
        }
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) == 0 && ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 7) {
          mapTileGroups[x][y] = ThemeTileTypeMerge(1,0);
          if(ThemeTileTypeGetTheme(mapTileGroups[x][y+1]) != 0) {
            if(Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y+1])*TilesGroupCount+25] == 0) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y+1])*TilesGroupCount+26] != 8 && Themes[0][1+ThemeTileTypeGetTileType(mapTileGroups[x][y+1])*TilesGroupCount+26] != 9) {
                CFullfilled = 0;
                for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 4) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                }
                if(CFullfilled > 0) {
                  mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
                }
              }
            }
          } else {
            CFullfilled = 0;
            for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 4) {
                StuitableTile[CFullfilled] = i;
                CFullfilled++;
              }
            }
            if(CFullfilled > 0) {
              mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
            }
          }
        }
      }
    }

    //Full platform
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) == 0 && ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 5) {
          CFullfilled = 0;
          for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
            if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 1) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 0) {
                StuitableTile[CFullfilled] = i;
                CFullfilled++;
              }
            }
          }
          if(CFullfilled > 0) {
            mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
          }
        }
      }
    }

    //Underground and dip checks
    for(int8_t x = 0; x < MapTileGroupW; x++) {
      for(int8_t y = 0; y < MapTileGroupH; y++) {
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) == 0 && ThemeTileTypeGetTileType(mapTileGroups[x][y]) == 8) {
          if(ThemeTileTypeGetTheme(mapTileGroups[x][y-1]) != 0 && Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y-1])*TilesGroupCount+25] == 0) {
            if((Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y-1])*TilesGroupCount+26])%2==1) {
              CFullfilled = 0;
              for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
                if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 0) {
                  if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 10) {
                    StuitableTile[CFullfilled] = i;
                    CFullfilled++;
                  }
                }
              }
              if(CFullfilled > 0) {
                mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
              }
            } else {
              mapTileGroups[x][y] = ThemeTileTypeMerge(0,1);
            }
          } else {
            mapTileGroups[x][y] = ThemeTileTypeMerge(0,1);
          }
        }
        if(ThemeTileTypeGetTheme(mapTileGroups[x][y]) != 0 && Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y])*TilesGroupCount+25] == 0 &&
        (Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y])*TilesGroupCount+26] == 8 || Themes[0][1+ThemeTileTypeGetTileType(mapTileGroups[x][y])*TilesGroupCount+26] == 9)) {
          if(ThemeTileTypeGetTheme(mapTileGroups[x][y-1]) == 0 || (ThemeTileTypeGetTheme(mapTileGroups[x][y-1]) != 0 &&
          (Themes[mapTileGroups[x][MapTileGroupH]][1+ThemeTileTypeGetTileType(mapTileGroups[x][y])*TilesGroupCount+25] == 3 && (Themes[0][1+ThemeTileTypeGetTileType(mapTileGroups[x][y])*TilesGroupCount+26] == 0)))) {
            continue;
          } else {
            CFullfilled = 0;
            for(uint16_t i = 0; i < Themes[mapTileGroups[x][MapTileGroupH]][0]; i++) {
              if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+25] == 0) {
                if(Themes[mapTileGroups[x][MapTileGroupH]][1+i*TilesGroupCount+26] == 0) {
                  StuitableTile[CFullfilled] = i;
                  CFullfilled++;
                }
              }
            }
            if(CFullfilled > 0) {
              mapTileGroups[x][y] = ThemeTileTypeMerge(mapTileGroups[x][MapTileGroupH]+1,StuitableTile[random(0,CFullfilled)]);
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
