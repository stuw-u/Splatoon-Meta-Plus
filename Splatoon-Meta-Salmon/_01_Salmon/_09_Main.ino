// MAIN STUFF
////////////////

bool WeaponRecharged = false;
uint32_t WeaponResetTimer = 0;

void ClampCamera () { //Clamp the camera in the world
  if(world.MapWidth < 11) {
    cameraX = (LCDWIDTH - Mul8(world.MapWidth))/2;
  } else {
    cameraX = ClampInt(0,(world.MapWidth*8)-LCDWIDTH,cameraX);
  }
  cameraY = ClampInt(0,(world.MapHeight*8)-LCDHEIGHT,cameraY);
}

void PrepareMap () {
  world.SMResetMap();
  world.Initialize(random(1,999));
  eggManager.Init();
  salmonidManager.cooldown = 180;
  
  player.mainPlayer.x = world.GetSpawnPositionX();
  player.mainPlayer.y = world.GetSpawnPositionY();

  cameraX = world.MapWidth*8/2-LCDWIDTH/2+8;
  cameraY = 0;
  
  player.mainPlayer.vx = 0;
  player.mainPlayer.vy = 0;

  player.Initialize();
  bulletsManager.Reset();

  AnimationTimer2 = 1800; //Min*60*fps = Match Lenght
  AnimationTimer = STARTLENGHT;

  gb.sound.play("S/SLMN_INTRO.wav");
  PausedTimer = -12;
  IsPaused = true;

  if(TutorialMode) {
    bulletsManager.spawnBullet(Mul64(12), Mul64(3), 0, -32, 1, 1, 0);
    bulletsManager.spawnBullet(Mul64(13), Mul64(3), 0, -32, 1, 1, 0);
    bulletsManager.spawnBullet(Mul64(14), Mul64(3), 0, -32, 1, 1, 0);

    salmonidManager.Spawn(Mul8(31), Mul8(3), 1);
  }
}

void DrawCursor() {
  if(player.mainPlayer.RespawnTimer > 0) {
    return;
  }
  
  if(gb.buttons.repeat(BUTTON_RIGHT,0) && !gb.buttons.repeat(BUTTON_A,0)) {
    if(!LastDirection && curX<48) {
      curX = 84 - curX;
    }
    LastDirection = true;
  }

  if(gb.buttons.repeat(BUTTON_LEFT,0) && !gb.buttons.repeat(BUTTON_A,0)) {
    if(LastDirection && curX>48) {
      curX = 84 - curX;
    }
    LastDirection = false;
  }

  if(gb.buttons.repeat(BUTTON_A,0) || gb.buttons.released(BUTTON_A)) {
    player.mainPlayer.ShootCall = IsPlaying;
    if(gb.buttons.repeat(BUTTON_UP,0)) {
      curY-=4;
      curY = constrain(curY, 0+5, LCDHEIGHT-4);
    }
    if(gb.buttons.repeat(BUTTON_LEFT,0)) {
      curX-=4;
      curX = constrain(curX, 0+5, LCDWIDTH-4);
    }
    if(gb.buttons.repeat(BUTTON_RIGHT,0)) {
      curX+=4;
      curX = constrain(curX, 0+5, LCDWIDTH-4);
    }
    if(gb.buttons.repeat(BUTTON_DOWN,0)) {
      curY+=4;
      curY = constrain(curY, 0+5, LCDHEIGHT-4);
    }

    if(IsPlaying) {
      switch(Weapons[mainWeapon][0]) {
        case 0: //--Classic Shooters
        if(gb.buttons.repeat(BUTTON_A,Weapons[mainWeapon][5]) && !gb.buttons.pressed(BUTTON_A)) {
          float aimingAngle = 0;
          for(uint8_t b = 0; b < Weapons[mainWeapon][8]; b++) {
            int16_t rootX = Div8(player.mainPlayer.x);
            int16_t rootY = Div8(player.mainPlayer.y);
            int16_t targetX = cameraX+curX;
            int16_t targetY = cameraY+curY;
  
            if(player.mainPlayer.PlayerDir == 1) {
              //root = + 22,9 from player
              rootX+=14;
              rootY+=5;
            } else if(player.mainPlayer.PlayerDir == -1) {
              //root = + 2,9 from player
              rootX-=7;
              rootY+=5;
            }
  
            if(player.mainPlayer.PlayerDir == 1) {
              aimingAngle = atan2(rootX-targetX,rootY-targetY)+1.5708F+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F); //In radians
            } else {
              aimingAngle = atan2(rootX-targetX,rootY-targetY)+1.5708F+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F); //In radians
            }
            bool shoot = true;
            if(player.mainPlayer.PlayerDir == 1) {
              if(aimingAngle-1.5708F != constrain(aimingAngle-1.5708F,-PI,0)) {
                shoot = false;
              }
            } else {
              if(aimingAngle-1.5708F != constrain(aimingAngle-1.5708F,0,PI)) {
                shoot = false;
              }
            }
  
            if(shoot && (player.mainPlayer.Refill >= Weapons[mainWeapon][10])) {
              player.mainPlayer.Refill -= Weapons[mainWeapon][10];
              gb.display.setColor(BLACK);
              gb.display.drawLine(toScreenX(rootX),toScreenY(rootY),toScreenX(targetX),toScreenY(targetY));

              playSFX(6,2);
              int8_t bulletID = bulletsManager.spawnBullet(
                rootX*8+16,
                rootY*8+16,
                -(int16_t)(cos(aimingAngle)*Weapons[mainWeapon][1]+random(-Weapons[mainWeapon][2]*10,Weapons[mainWeapon][2]*10)/10.0F),
                (int16_t)(sin(aimingAngle)*Weapons[mainWeapon][1]+random(-Weapons[mainWeapon][2]*10,Weapons[mainWeapon][2]*10)/10.0F),
                player.mainPlayer.PlayerColor,
                0,
                0
              );
              if(bulletID!=-1) {
                bulletsManager.bullets[bulletID].gravity = Weapons[mainWeapon][3];
                bulletsManager.bullets[bulletID].Damage = Weapons[mainWeapon][6];
                bulletsManager.bullets[bulletID].BulletTimeLimit = Weapons[mainWeapon][7];
              }
            }
          }
        }
        break;
        case 1: //--Rollers
        if(gb.buttons.held(BUTTON_A,Weapons[mainWeapon][5]) && !(gb.buttons.timeHeld(BUTTON_A) > Weapons[mainWeapon][5]+Weapons[mainWeapon][9])) { //Attack (If not rolling)
          float aimingAngle = 0;
          shakeTimeLeft += 3;
          shakeAmplitude += 2;
          for(uint8_t b = 0; b < Weapons[mainWeapon][8]; b++) {
            if(player.mainPlayer.Refill >= Weapons[mainWeapon][10]) {
              player.mainPlayer.Refill -= Weapons[mainWeapon][10];
            } else {
              break;
            }
            
            int16_t rootX = Div8(player.mainPlayer.x);
            int16_t rootY = Div8(player.mainPlayer.y);
            
            if(player.mainPlayer.PlayerDir == 1) {
              //root = + 22,9 from player
              rootX+=14;
              rootY+=5;
      
              aimingAngle = (0)+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F);
            } else if(player.mainPlayer.PlayerDir == -1) {
              //root = + 2,9 from player
              rootX-=7;
              rootY+=5;
      
              aimingAngle = (PI)+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F);
            }
            playSFX(6,1);
            int8_t bulletID = bulletsManager.spawnBullet(
              rootX*8+16,
              rootY*8+16,
              -(int16_t)(cos(aimingAngle)*(Weapons[mainWeapon][1]+random(-Weapons[mainWeapon][2]*10,Weapons[mainWeapon][2]*10)/10.0F)),
              (int16_t)(sin(aimingAngle)*(Weapons[mainWeapon][1]+random(-Weapons[mainWeapon][2]*10,Weapons[mainWeapon][2]*10)/10.0F))+-Weapons[mainWeapon][11],
              player.mainPlayer.PlayerColor,
              0,
              1
            );
            if(bulletID!=-1) {
              bulletsManager.bullets[bulletID].gravity = Weapons[mainWeapon][3];
              bulletsManager.bullets[bulletID].Damage = Weapons[mainWeapon][6];
              bulletsManager.bullets[bulletID].BulletTimeLimit = Weapons[mainWeapon][7];
            }
          }
        }
        if(gb.buttons.timeHeld(BUTTON_A) > Weapons[mainWeapon][5]+Weapons[mainWeapon][9] && gb.buttons.repeat(BUTTON_A,1)) { //Rolling (After attack length)
          if(player.mainPlayer.Refill >= 1) {
            player.mainPlayer.Refill -= (AnimationTimer2%3!=1);
          } else {
            break;
          }
          float aimingAngle = 0;
          for(uint8_t b = 0; b < Weapons[mainWeapon][8]/2; b++) {
            int16_t rootX = Div8(player.mainPlayer.x);
            int16_t rootY = Div8(player.mainPlayer.y);
            
            if(player.mainPlayer.PlayerDir == 1) {
              //root = + 22,9 from player
              rootX+=14;
              rootY+=5;
      
              aimingAngle = (0)+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F);
            } else if(player.mainPlayer.PlayerDir == -1) {
              //root = + 2,9 from player
              rootX-=7;
              rootY+=5;
      
              aimingAngle = (PI)+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F);
            }
            int8_t bulletID = bulletsManager.spawnBullet(
              rootX*8+16,
              rootY*8+16,
              0,
              -Weapons[mainWeapon][1],
              player.mainPlayer.PlayerColor,
              0,
              2
            );
            if(bulletID!=-1) {
              bulletsManager.bullets[bulletID].gravity = Weapons[mainWeapon][3];
              bulletsManager.bullets[bulletID].Damage = Weapons[mainWeapon][6]*2;
              bulletsManager.bullets[bulletID].BulletTimeLimit = Weapons[mainWeapon][7]/5;
            }
          }
        }
        break;
        case 2: //--Chargers
        if(gb.buttons.held(BUTTON_A,Weapons[mainWeapon][5])) {
          playSFX(2,1);
        }
        if(gb.buttons.repeat(BUTTON_A,0)) {
          WeaponResetTimer += 1;
        }
        if(WeaponResetTimer > Weapons[mainWeapon][5]) { //Recharge Indicator
          int16_t rootX = Div8(player.mainPlayer.x);
          int16_t rootY = Div8(player.mainPlayer.y);
          int16_t targetX = cameraX+curX;
          int16_t targetY = cameraY+curY;
          if(player.mainPlayer.PlayerDir == 1) {
            //root = + 22,9 from player
            rootX+=14;
            rootY+=5;
          } else if(player.mainPlayer.PlayerDir == -1) {
            //root = + 2,9 from player
            rootX-=7;
            rootY+=5;
          }
  
          gb.display.setColor((ColorIndex)3);
          gb.display.drawLine(toScreenX(rootX),toScreenY(rootY),toScreenX(targetX),toScreenY(targetY));
          WeaponRecharged = true;
        }
        if(WeaponRecharged && gb.buttons.released(BUTTON_A) && (player.mainPlayer.Refill >= Weapons[mainWeapon][10])) { //Held for a certain length then released
          player.mainPlayer.Refill -= Weapons[mainWeapon][10];
          WeaponRecharged = false;
          WeaponResetTimer = 0;
          
          float aimingAngle = 0;
          float driftX = 0;
          float driftY = 0;
          int16_t rootX = Div8(player.mainPlayer.x);
          int16_t rootY = Div8(player.mainPlayer.y);
          int16_t targetX = cameraX+curX;
          int16_t targetY = cameraY+curY;
          if(player.mainPlayer.PlayerDir == 1) {
            //root = + 22,9 from player
            rootX+=14;
            rootY+=5;
          } else if(player.mainPlayer.PlayerDir == -1) {
            //root = + 2,9 from player
            rootX-=7;
            rootY+=5;
          }
          if(player.mainPlayer.PlayerDir == 1) {
            aimingAngle = atan2(rootX-targetX,rootY-targetY)+1.5708F+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F); //In radians
          } else {
            aimingAngle = atan2(rootX-targetX,rootY-targetY)+1.5708F+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F); //In radians
          }
          if(player.mainPlayer.PlayerDir == 1) {
            if(aimingAngle-1.5708F != constrain(aimingAngle-1.5708F,-PI,0)) {
              break;
            }
          } else {
            if(aimingAngle-1.5708F != constrain(aimingAngle-1.5708F,0,PI)) {
              break;
            }
          }
          shakeTimeLeft += 3;
          shakeAmplitude += 1;
          playSFX(5,0);
          for(uint8_t b = 0; b < Weapons[mainWeapon][8]; b++) {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawLine(toScreenX(rootX),toScreenY(rootY),toScreenX(targetX),toScreenY(targetY));
  
            driftX += cos(aimingAngle)*(Weapons[mainWeapon][1]/3.0F);
            driftY += sin(aimingAngle)*(Weapons[mainWeapon][1]/3.0F);
  
            //gb.display.setColor(RED);
            //gb.display.drawRect(toScreenX((rootX+2+driftX/SCALE)/8*8),toScreenY((rootY+2-driftY/SCALE)/8*8),8,8);
            if(TilesParams_Array[world.GetTile((rootX+2+(driftX/8))/8,(rootY+2-(driftY/8))/8)] != 0) {
              break;
            }
            
            int8_t bulletID = bulletsManager.spawnBullet(
              rootX*8+16+(int16_t)driftX,
              rootY*8+16-(int16_t)driftY,
              -(int16_t)(cos(aimingAngle)*50),
              (int16_t)(sin(aimingAngle)*50),
              player.mainPlayer.PlayerColor,
              0,
              2
            );
            if(bulletID!=-1) {
              bulletsManager.bullets[bulletID].gravity = Weapons[mainWeapon][3];
              bulletsManager.bullets[bulletID].Damage = Weapons[mainWeapon][6];
              bulletsManager.bullets[bulletID].BulletTimeLimit = Weapons[mainWeapon][7];
            }
          }
        }
        break;
        case 3: //--Rush attack weapons
        if(gb.buttons.held(BUTTON_A,Weapons[mainWeapon][5])) {
          if(player.mainPlayer.Refill >= Weapons[mainWeapon][10]) {
            player.mainPlayer.Refill -= Weapons[mainWeapon][10];
          } else {
            break;
          }

          playSFX(6,0);
          float aimingAngle = 0;
          for(uint8_t b = 0; b < Weapons[mainWeapon][8]; b++) {
            int16_t rootX = Div8(player.mainPlayer.x);
            int16_t rootY = Div8(player.mainPlayer.y);
            
            if(player.mainPlayer.PlayerDir == 1) {
              //root = + 22,9 from player
              rootX+=14;
              rootY+=5;
      
              aimingAngle = (0)+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F);
            } else if(player.mainPlayer.PlayerDir == -1) {
              //root = + 2,9 from player
              rootX-=7;
              rootY+=5;
      
              aimingAngle = (PI)+(random(-Weapons[mainWeapon][4],Weapons[mainWeapon][4])/50.0F);
            }
            int8_t bulletID = bulletsManager.spawnBullet(
              rootX*8+16,
              rootY*8+16,
              -(int16_t)(cos(aimingAngle)*(Weapons[mainWeapon][1]+random(-Weapons[mainWeapon][2]*10,Weapons[mainWeapon][2]*10)/10.0F)),
              (int16_t)(sin(aimingAngle)*(Weapons[mainWeapon][1]+random(-Weapons[mainWeapon][2]*10,Weapons[mainWeapon][2]*10)/10.0F))+-Weapons[mainWeapon][11],
              player.mainPlayer.PlayerColor,
              0,
              3
            );
            if(bulletID!=-1) {
              bulletsManager.bullets[bulletID].gravity = Weapons[mainWeapon][3];
              bulletsManager.bullets[bulletID].Damage = Weapons[mainWeapon][6];
              bulletsManager.bullets[bulletID].BulletTimeLimit = Weapons[mainWeapon][7];
            }
          }
        }
        break;
      }
    }

    gb.display.setColor(BLACK);
    gb.display.fillRect(curX-4,curY-5,7,9);
    gb.display.fillRect(curX-5,curY-4,9,7);

    gb.display.setColor(WHITE); //Was Invert
    gb.display.drawBitmap(curX-4,curY-4,Cursor);
    gb.display.drawBitmap(84-curX-4,curY-4,Cursor);
  } else {
    gb.display.setColor(BLACK);
    gb.display.drawBitmap(curX-4,curY-4,Cursor);

    WeaponRecharged = false;
  }
}

void DrawUI() {
  gb.display.setColor(WHITE);
  gb.display.fillRect(0,0,84,6);
  
  gb.display.setColor(BLACK);
  gb.display.setCursor(0,0);
  //gb.display.print(gb.getFreeRam());
  //gb.display.print(player.mainPlayer.Live);
  gb.display.print(cpuLoad);

  lastLoad[0] = lastLoad[1];
  lastLoad[1] = lastLoad[2];
  lastLoad[2] = lastLoad[3];
  lastLoad[3] = lastLoad[4];
  lastLoad[4] = lastLoad[5];
  lastLoad[5] = lastLoad[6];
  lastLoad[6] = lastLoad[7];
  lastLoad[7] = gb.getCpuLoad();
  
  cpuLoad = Div8(lastLoad[0]+lastLoad[1]+lastLoad[2]+lastLoad[3]+lastLoad[4]+lastLoad[5]+lastLoad[6]+lastLoad[7]);
}

void setup() {
  gb.begin();
  //SerialUSB.begin(9600);
  gb.setFrameRate(25);
  gb.display.setFont(SquidSquare);
  gb.display.colorIndex = palette;
  player.mainPlayer.PlayerGender = SelectedGender;
  player.mainPlayer.PlayerHaircut = SelectedHaircut;
  mainWeapon = gb.save.get(2);

  gb.sound.play("S/SLMN_LOOP.wav");
}

byte Mode = 0;
long r1 = 0;
long r2 = 0;

void CrossDraw(byte x, byte y) {
  gb.display.drawPixel(x,y);
  gb.display.drawPixel(x+1,y+1);
  gb.display.drawPixel(x+1,y-1);
  gb.display.drawPixel(x-1,y+1);
  gb.display.drawPixel(x-1,y-1);
}

void loop () {
  while(!gb.update());
  if(true) {
    gb.lights.clear();
    gb.display.setFont(SquidSquare);
    if(!IsPlaying) {


      //Title
      if(GameState == 0) {
        gb.display.setColor((ColorIndex)3);
        gb.display.fill();
        if(AnimationTimer >= 16 && AnimationTimer < 16+3) {
          gb.display.setFontSize(1);
          gb.display.setCursor(2,2);
          if(AnimationTimer == 16+0) {
            gb.display.setColor((ColorIndex)11);
          } else if(AnimationTimer == 16+1) {
            gb.display.setColor((ColorIndex)12);
          } else if(AnimationTimer == 16+2) {
            gb.display.setColor((ColorIndex)13);
          }
          gb.display.print(enText[7]);
        }
        if(AnimationTimer >= 16+3 && AnimationTimer < 16+3+16) {
          gb.display.setFontSize(1);
          gb.display.setCursor(2,2);
          gb.display.setColor((ColorIndex)0);
          gb.display.print(enText[7]);
        }
        if(AnimationTimer >= 16+3+16 && AnimationTimer < 16+3+16+3) {
          gb.display.setFontSize(1);
          gb.display.setCursor(2,2);
          if(AnimationTimer == 16+3+16+0) {
            gb.display.setColor((ColorIndex)13);
          } else if(AnimationTimer == 16+3+16+1) {
            gb.display.setColor((ColorIndex)12);
          } else if(AnimationTimer == 16+3+16+2) {
            gb.display.setColor((ColorIndex)11);
          }
          gb.display.print(enText[7]);
        }
        if(AnimationTimer > 64 && AnimationTimer < 128) {
          //11 -- 48
          if(AnimationTimer < 64+11) {
            AnimationTimer = 64+11;
          }
          if(AnimationTimer == 64+48) {
            AnimationTimer = 64+64;
          }
          gb.display.drawImage(3,16,Title);
          gb.display.drawImage(0,64-(AnimationTimer-64),InkTitle);
          gb.display.drawImage(32,64-(AnimationTimer-64),InkTitle);
          gb.display.drawImage(64,64-(AnimationTimer-64),InkTitle);
          gb.display.setColor((ColorIndex)3);
          gb.display.fillRect(0,0,32,(64-(AnimationTimer-64)));
          gb.display.fillRect(32,0,32,(64-(AnimationTimer-64)));
          gb.display.fillRect(64,0,32,(64-(AnimationTimer-64)));
        }
        
        if(AnimationTimer < 128) {
          AnimationTimer++;
        } else if(AnimationTimer > 128) {
          AnimationTimer-=64;
          gb.display.drawImage(3,16,Title);
          gb.display.drawImage(0,(AnimationTimer-64),InkTitle);
          gb.display.drawImage(32,(AnimationTimer-64),InkTitle);
          gb.display.drawImage(64,(AnimationTimer-64),InkTitle);
          if(AnimationTimer >= 124) {
            gb.display.setColor((ColorIndex)2);
          } else {
            gb.display.setColor((ColorIndex)3);
          }
          gb.display.fillRect(0,0,32,((AnimationTimer-64)));
          gb.display.fillRect(32,0,32,((AnimationTimer-64)));
          gb.display.fillRect(64,0,32,((AnimationTimer-64)));
          AnimationTimer+=64;

          if(AnimationTimer > 128+64) {
            AnimationTimer = 0;
            AnimationTimer8 = 64;
            GameState = 7;
            return;
          } else {
            AnimationTimer++;
          }
        } else {
          gb.display.drawImage(3,16,Title);
        }

        if(gb.buttons.pressed(BUTTON_A) || gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) {
          IsPaused = true;
        }
        if(IsPaused && AnimationTimer > 16+3+16+3 && AnimationTimer == 128) {
          IsPaused = false;
          AnimationTimer = 129;
        }
      }




      //Map
      if(GameState == 1) {
        gb.display.setColor((ColorIndex)11);
        gb.display.fill();
        if(AnimationTimer < 9) {
          gb.display.drawImage(16+(49-(49*128/9*AnimationTimer)/128)/2, 5+(56-(56*128/9*AnimationTimer)/128)/2, Map, (49*128/9*AnimationTimer)/128, (49*128/9*AnimationTimer)/128);
        } else if(AnimationTimer < 14) {
          gb.display.drawImage(16, 5, Map);
          AnimationTimer2 = 16;
        } else if(AnimationTimer < 20) {
          gb.display.drawImage(AnimationTimer2, 5, Map);
          AnimationTimer2 = (AnimationTimer2*80 + 3*20)/100;
        } else if(AnimationTimer >= 20) {
          gb.display.drawImage(3, 5, Map);
          if(AnimationTimer >= 20) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(41,19);
          }
          if(AnimationTimer >= 21) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(47,42);
          }
          if(AnimationTimer >= 22) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(25,44);
          }
          if(AnimationTimer >= 23) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(17,53);
          }
          if(AnimationTimer >= 24) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(15,14);
          }
          if(AnimationTimer >= 25) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(12,26);
          }
          if(AnimationTimer >= 26) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(12,26);
          }
          if(AnimationTimer == 28) {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawFastVLine(29,8,5);
            gb.display.drawFastHLine(26,11,5);
          }
          if(AnimationTimer == 29) {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawFastVLine(29,9,3);
            gb.display.drawFastHLine(27,11,3);
          }
          if(AnimationTimer >= 28) {
            gb.display.setColor((ColorIndex)7);
            gb.display.fillRect(28,10,3,3);
          }
          if(AnimationTimer >= 27) {
            gb.display.setColor((ColorIndex)6);
            CrossDraw(29,11);
          }
          if(AnimationTimer >= 31) {
            if(AnimationTimer == 31) {
              gb.display.colorIndex = paletteFade0;
            } else
            if(AnimationTimer == 32) {
              gb.display.colorIndex = paletteFade1;
            } else {
              gb.display.colorIndex = palette;
            }
            if(gb.buttons.repeat(BUTTON_MENU,0)) {
              gb.display.setColor((ColorIndex)3);
              gb.display.fillRect(50,0,30,64);

              gb.display.setCursor(52,2);
              gb.display.setColor((ColorIndex)0);
              gb.display.print("START");
              gb.display.setCursor(52,2+6);
              gb.display.print("MISSION");
              gb.display.setCursor(52,2+6+6);
              gb.display.setColor((ColorIndex)1);
              gb.display.print("SECTOR");
              gb.display.setCursor(52,2+6+6+6);
              gb.display.setColor((ColorIndex)2);
              gb.display.print(AnimationTimer4+1);
              gb.display.print(".A");
              gb.display.setCursor(52,2+6+6+6+6);
              if(true) {
                gb.display.setColor((ColorIndex)7);
                gb.display.print("UNLOCK");
              } else {
                gb.display.setColor((ColorIndex)6);
                gb.display.print("LOCK");
              }
              //LOCK/UNLOCKED
              gb.display.setCursor(52,2+6+6+6+6+6);
              gb.display.setColor((ColorIndex)2);
              gb.display.print(AnimationTimer4+1);
              gb.display.print(".B");
              gb.display.setCursor(52,2+6+6+6+6+6+6);
              //LOCK/UNLOCKED
              if(false) {
                gb.display.setColor((ColorIndex)7);
                gb.display.print("UNLOCK");
              } else {
                gb.display.setColor((ColorIndex)6);
                gb.display.print("LOCK");
              }

              gb.display.drawImage(51,58,Golden_Small);
              gb.display.setCursor(56,58);
              gb.display.setColor((ColorIndex)0);
              gb.display.print("354");
              
              
            } else {
              if(AnimationTimer7 == 0 && gb.buttons.released(BUTTON_A)) {
                AnimationTimer7 = 1;
                AnimationTimer8 = 1;
              } else
              if(AnimationTimer7 > 0 && gb.buttons.released(BUTTON_B)) {
                AnimationTimer7 = 0;
              } else
              if(AnimationTimer7 > 0 && gb.buttons.released(BUTTON_A)) {
                //ENTER
                AnimationTimer = 0;
                AnimationTimer2 = 0;
                AnimationTimer3 = 0;
                AnimationTimer4 = 0;
                AnimationTimer5 = 0;
                AnimationTimer6 = 0;
                AnimationTimer7 = 0;
                TutorialMode = false;//true;
                IsPlaying = true;
                PrepareMap();
                player.mainPlayer.PlayerGender = SelectedGender;
                player.mainPlayer.PlayerHaircut = SelectedHaircut;
                player.mainPlayer.hat = 0;
                GameState = 0;
              }
              
              if(AnimationTimer7 == 0) {
                gb.display.drawImage(50,0,StartMission);
                gb.display.drawImage(72,46,Icon_B);
                gb.display.drawImage(52,55,Icon_Menu);
                gb.display.drawImage(72,55,Icon_A);
              } else if(AnimationTimer7 > 0) {
                if(gb.buttons.pressed(BUTTON_UP)) {
                  AnimationTimer8 = 1;
                } else if(gb.buttons.pressed(BUTTON_DOWN)) {
                  AnimationTimer8 = 2;
                }
                
                gb.display.drawImage(50,12,StartMission);
                gb.display.setColor((ColorIndex)13);
                gb.display.fillRect(50,0,30,28);
                gb.display.drawImage(72,46,Icon_B);
                gb.display.drawImage(52,55,Icon_Menu);
                gb.display.drawImage(72,55,Icon_A);

                gb.display.setCursor(52,2);
                gb.display.setColor((ColorIndex)0);
                gb.display.print("START");
                gb.display.setCursor(52,2+6);
                gb.display.print("MISSION");
                gb.display.drawFastHLine(50,14,30);

                if(AnimationTimer8==1) {
                  gb.display.setColor((ColorIndex)0);
                  gb.display.drawLine(52,16,54,18);
                  gb.display.drawLine(54,18,52,20);
                  gb.display.setCursor(57,16);
                  gb.display.print(AnimationTimer4+1);
                  gb.display.print(".A");
                } else {
                  gb.display.setColor((ColorIndex)12);
                  gb.display.setCursor(52,16);
                  gb.display.print(AnimationTimer4+1);
                  gb.display.print(".A");
                }

                if(AnimationTimer8==2) {
                  gb.display.setColor((ColorIndex)0);
                  gb.display.drawLine(52,16+6,54,18+6);
                  gb.display.drawLine(54,18+6,52,20+6);
                  gb.display.setCursor(57,16+6);
                  gb.display.print(AnimationTimer4+1);
                  gb.display.print(".B");
                } else {
                  gb.display.setColor((ColorIndex)12);
                  gb.display.setCursor(52,16+6);
                  gb.display.print(AnimationTimer4+1);
                  gb.display.print(".B");
                }
              }
            }
            
            
            
            gb.display.colorIndex = palette;
          }
        }
        if(AnimationTimer < 33) {
          AnimationTimer++;
        } else {
          /*if(gb.buttons.repeat(BUTTON_UP,1)) {
            AnimationTimer3-=1;
          }
          if(gb.buttons.repeat(BUTTON_DOWN,1)) {
            AnimationTimer3+=1;
          }
          if(gb.buttons.repeat(BUTTON_LEFT,1)) {
            AnimationTimer2-=1;
          }
          if(gb.buttons.repeat(BUTTON_RIGHT,1)) {
            AnimationTimer2+=1;
          }*/
          if(AnimationTimer7 == 0) {
            if(AnimationTimer4 == 0) {
              AnimationTimer5 = 42;
              AnimationTimer6 = 19;
              if(gb.buttons.pressed(BUTTON_DOWN)) {
                AnimationTimer4=1;
              }
              if(gb.buttons.pressed(BUTTON_LEFT)) {
                AnimationTimer4=6;
              }
            } else
            if(AnimationTimer4 == 1) {
              AnimationTimer5 = 48;
              AnimationTimer6 = 42;
              if(gb.buttons.pressed(BUTTON_UP)) {
                AnimationTimer4=0;
              }
              if(gb.buttons.pressed(BUTTON_LEFT)) {
                AnimationTimer4=2;
              }
            } else
            if(AnimationTimer4 == 2) {
              AnimationTimer5 = 25;
              AnimationTimer6 = 44;
              if(gb.buttons.pressed(BUTTON_DOWN)) {
                AnimationTimer4=3;
              }
              if(gb.buttons.pressed(BUTTON_LEFT)) {
                AnimationTimer4=3;
              }
              if(gb.buttons.pressed(BUTTON_RIGHT)) {
                AnimationTimer4=1;
              }
              if(gb.buttons.pressed(BUTTON_UP)) {
                AnimationTimer4=5;
              }
            } else
            if(AnimationTimer4 == 3) {
              AnimationTimer5 = 17;
              AnimationTimer6 = 53;
              if(gb.buttons.pressed(BUTTON_UP)) {
                AnimationTimer4=5;
              }
              if(gb.buttons.pressed(BUTTON_RIGHT)) {
                AnimationTimer4=2;
              }
            } else
            if(AnimationTimer4 == 4) {
              AnimationTimer5 = 16;
              AnimationTimer6 = 13;
              if(gb.buttons.pressed(BUTTON_DOWN)) {
                AnimationTimer4=5;
              }
              if(gb.buttons.pressed(BUTTON_LEFT)) {
                AnimationTimer4=5;
              }
              if(gb.buttons.pressed(BUTTON_RIGHT)) {
                AnimationTimer4=6;
              }
            } else
            if(AnimationTimer4 == 5) {
              AnimationTimer5 = 12;
              AnimationTimer6 = 25;
              if(gb.buttons.pressed(BUTTON_DOWN)) {
                AnimationTimer4=3;
              }
              if(gb.buttons.pressed(BUTTON_UP)) {
                AnimationTimer4=4;
              }
            } else
            if(AnimationTimer4 == 6) {
              AnimationTimer5 = 30;
              AnimationTimer6 = 10;
              if(gb.buttons.pressed(BUTTON_LEFT)) {
                AnimationTimer4=4;
              }
              if(gb.buttons.pressed(BUTTON_RIGHT)) {
                AnimationTimer4=0;
              }
            }
          }
          gb.display.setColor((ColorIndex)3);
          gb.display.fillTriangle((AnimationTimer2/8)-4,(AnimationTimer3/8)-4,(AnimationTimer2/8)+4,(AnimationTimer3/8)-4,  (AnimationTimer2/8),(AnimationTimer3/8));
          gb.display.setColor((ColorIndex)0);
          gb.display.fillTriangle((AnimationTimer2/8)-2,(AnimationTimer3/8)-3, (AnimationTimer2/8)+2,(AnimationTimer3/8)-3,  (AnimationTimer2/8),(AnimationTimer3/8)-1);
          AnimationTimer2 = (AnimationTimer2*8+(AnimationTimer5*8)*2)/10;
          AnimationTimer3 = (AnimationTimer3*8+(AnimationTimer6*8)*2)/10;
        }
      }




      //Player Selection
      //////////////////
      if(GameState == 7) {
        gb.display.setColor((ColorIndex)3);
        gb.display.fill();

        if(SelectedGender == 2) {
  
          int8_t sizeX = DInklingF.width();
          int8_t sizeY = DInklingF.height();
          gb.display.drawImage(-2,LCDHEIGHT-43-(sin(AnimationTimer2/55.0F*PI)*6.0F),DInklingF,sizeX*1,sizeY);
          if(AnimationTimer == 0) {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawBitmap(12,LCDHEIGHT-44-(sin(AnimationTimer2/55.0F*PI)*12.0F),ArrowOutlineUI);
            gb.display.setColor((ColorIndex)(!gb.buttons.repeat(BUTTON_A,0)*3));
            gb.display.drawBitmap(12,LCDHEIGHT-44-(sin(AnimationTimer2/55.0F*PI)*12.0F),ArrowUI);
          }
          
          gb.display.drawImage(LCDWIDTH-44,LCDHEIGHT-43-(sin(AnimationTimer3/55.0F*PI)*6.0F),DInklingM,sizeX*-1,sizeY);
          if(AnimationTimer == 1) {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawBitmap(LCDWIDTH-24,LCDHEIGHT-44-(sin(AnimationTimer3/55.0F*PI)*12.0F),ArrowOutlineUI);
            gb.display.setColor((ColorIndex)(!gb.buttons.repeat(BUTTON_A,0)*3));
            gb.display.drawBitmap(LCDWIDTH-24,LCDHEIGHT-44-(sin(AnimationTimer3/55.0F*PI)*12.0F),ArrowUI);
          }
          gb.display.setColor((ColorIndex)1);
          gb.display.fillRect(0,0,80,AnimationTimer8);
          gb.display.colorIndex = paletteFlipBlackGrey;
          gb.display.drawImage(0,AnimationTimer8,InkTitle);
          gb.display.drawImage(32,AnimationTimer8,InkTitle);
          gb.display.drawImage(64,AnimationTimer8,InkTitle);
          gb.display.colorIndex = palette;
          gb.display.setFontSize(1);
          gb.display.setColor((ColorIndex)0);
          gb.display.setCursor(1,1);
          gb.display.print(enText[8]);
          
          if(gb.buttons.pressed(BUTTON_LEFT)) {
            playSFX(7,0);
            AnimationTimer = 0;
            if(AnimationTimer2 == 0) {
              AnimationTimer2 = 55;
            }
          }
          if(gb.buttons.pressed(BUTTON_RIGHT)) {
            playSFX(7,0);
            AnimationTimer = 1;
            if(AnimationTimer3 == 0) {
              AnimationTimer3 = 55;
            }
          }

          if(gb.buttons.pressed(BUTTON_A)) {
            playSFX(7,1);
            SelectedGender = AnimationTimer;
            
            AnimationTimer = 0;
            AnimationTimer2 = 55;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            AnimationTimer8 = 64;
          }
        } else {
          if(SelectedGender == 0) {
            InklingF.setFrame(0);
            H0InklingF.setFrame(0);
            H1InklingF.setFrame(0);
            H2InklingF.setFrame(0);
            H3InklingF.setFrame(0);
            gb.display.drawImage(1,LCDHEIGHT-22-22-(sin(AnimationTimer2/55.0F*PI)*6.0F),InklingF);
            gb.display.drawImage(1+19,LCDHEIGHT-22-22-(sin(AnimationTimer3/55.0F*PI)*6.0F),InklingF);
            gb.display.drawImage(1+19*2,LCDHEIGHT-22-22-(sin(AnimationTimer4/55.0F*PI)*6.0F),InklingF);
            gb.display.drawImage(1+19*3,LCDHEIGHT-22-22-(sin(AnimationTimer5/55.0F*PI)*6.0F),InklingF);
            gb.display.drawImage(1,LCDHEIGHT-22-(sin(AnimationTimer6/55.0F*PI)*6.0F),InklingF);
            gb.display.drawImage(1+19,LCDHEIGHT-22-(sin(AnimationTimer7/55.0F*PI)*6.0F),InklingF);
            gb.display.colorIndex = palettePOrange;
            gb.display.drawImage(1,LCDHEIGHT-22-22-(sin(AnimationTimer2/55.0F*PI)*6.0F),H0InklingF);
            gb.display.drawImage(1+19,LCDHEIGHT-22-22-(sin(AnimationTimer3/55.0F*PI)*6.0F),H1InklingF);
            gb.display.drawImage(1+19*2,LCDHEIGHT-22-22-(sin(AnimationTimer4/55.0F*PI)*6.0F),H2InklingF);
            gb.display.drawImage(1+19*3,LCDHEIGHT-22-22-(sin(AnimationTimer5/55.0F*PI)*6.0F),H3InklingF);
            gb.display.drawImage(1,LCDHEIGHT-22-(sin(AnimationTimer6/55.0F*PI)*6.0F),H4InklingF);
            gb.display.drawImage(1+19,LCDHEIGHT-22-(sin(AnimationTimer7/55.0F*PI)*6.0F),H5InklingF);
            Headgear.setFrame(13);
            byte Lift = (0 == 1 && 0 == 3);
            gb.display.drawImage(1+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer2/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
            Lift = (1 == 1 && 1 == 3);
            gb.display.drawImage(1+19+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer3/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
            Lift = (1 == 1 && 2 == 3);
            gb.display.drawImage(1+19*2+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer4/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
            Lift = (1 == 1 && 3 == 3);
            gb.display.drawImage(1+19*3+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer5/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
          } else if(SelectedGender == 1) {
            InklingM.setFrame(0);
            H0InklingM.setFrame(0);
            H1InklingM.setFrame(0);
            H2InklingM.setFrame(0);
            H3InklingM.setFrame(0);
            gb.display.drawImage(1,LCDHEIGHT-22-22-(sin(AnimationTimer2/55.0F*PI)*6.0F),InklingM);
            gb.display.drawImage(1+19,LCDHEIGHT-22-22-(sin(AnimationTimer3/55.0F*PI)*6.0F),InklingM);
            gb.display.drawImage(1+19*2,LCDHEIGHT-22-22-(sin(AnimationTimer4/55.0F*PI)*6.0F),InklingM);
            gb.display.drawImage(1+19*3,LCDHEIGHT-22-22-(sin(AnimationTimer5/55.0F*PI)*6.0F),InklingM);
            gb.display.drawImage(1,LCDHEIGHT-22-(sin(AnimationTimer6/55.0F*PI)*6.0F),InklingM);
            gb.display.drawImage(1+19,LCDHEIGHT-22-(sin(AnimationTimer7/55.0F*PI)*6.0F),InklingM);
            gb.display.colorIndex = palettePOrange;
            gb.display.drawImage(1,LCDHEIGHT-22-22-(sin(AnimationTimer2/55.0F*PI)*6.0F),H0InklingM);
            gb.display.drawImage(1+19,LCDHEIGHT-22-22-(sin(AnimationTimer3/55.0F*PI)*6.0F),H1InklingM);
            gb.display.drawImage(1+19*2,LCDHEIGHT-22-22-(sin(AnimationTimer4/55.0F*PI)*6.0F),H2InklingM);
            gb.display.drawImage(1+19*3,LCDHEIGHT-22-22-(sin(AnimationTimer5/55.0F*PI)*6.0F),H3InklingM);
            gb.display.drawImage(1,LCDHEIGHT-22-(sin(AnimationTimer6/55.0F*PI)*6.0F),H4InklingM);
            gb.display.drawImage(1+19,LCDHEIGHT-22-(sin(AnimationTimer7/55.0F*PI)*6.0F),H5InklingM);
            Headgear.setFrame(13);
            byte Lift = (1 == 1 && 0 == 3);
            gb.display.drawImage(1+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer2/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
            Lift = (1 == 1 && 1 == 3);
            gb.display.drawImage(1+19+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer3/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
            Lift = (1 == 1 && 2 == 3);
            gb.display.drawImage(1+19*2+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer4/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
            Lift = (1 == 1 && 3 == 3);
            gb.display.drawImage(1+19*3+3-HatOffset[0],LCDHEIGHT-22-22-(sin(AnimationTimer5/55.0F*PI)*6.0F)-3-HatOffset[0]-Lift,Headgear,16,16);
          }
          gb.display.colorIndex = palette;

          if(AnimationTimer < 4) {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawBitmap(1+(AnimationTimer*19)+6,LCDHEIGHT-32-22,ArrowOutlineUI);
            gb.display.setColor((ColorIndex)(!gb.buttons.repeat(BUTTON_A,0)*3));
            gb.display.drawBitmap(1+(AnimationTimer*19)+6,LCDHEIGHT-32-22,ArrowUI);
          } else {
            gb.display.setColor((ColorIndex)0);
            gb.display.drawBitmap(1+((AnimationTimer-4)*19)+6,LCDHEIGHT-32,ArrowOutlineUI);
            gb.display.setColor((ColorIndex)(!gb.buttons.repeat(BUTTON_A,0)*3));
            gb.display.drawBitmap(1+((AnimationTimer-4)*19)+6,LCDHEIGHT-32,ArrowUI);
          }

          gb.display.setColor((ColorIndex)1);
          gb.display.fillRect(0,0,80,AnimationTimer8);
          gb.display.colorIndex = paletteFlipBlackGrey;
          gb.display.drawImage(0,AnimationTimer8,InkTitle);
          gb.display.drawImage(32,AnimationTimer8,InkTitle);
          gb.display.drawImage(64,AnimationTimer8,InkTitle);
          gb.display.colorIndex = palette;
          gb.display.setFontSize(1);
          gb.display.setColor((ColorIndex)0);
          gb.display.setCursor(1,1);
          gb.display.print(enText[9]);

          if(gb.buttons.repeat(BUTTON_LEFT,4)) {
            playSFX(7,0);
            AnimationTimer--;
            AnimationTimer = constrain(AnimationTimer,0,5);
            if(AnimationTimer == 0) {
              if(AnimationTimer2 == 0) {
                AnimationTimer2 = 55;
              }
            } else if(AnimationTimer == 1) {
              if(AnimationTimer3 == 0) {
                AnimationTimer3 = 55;
              }
            } else if(AnimationTimer == 2) {
              if(AnimationTimer4 == 0) {
                AnimationTimer4 = 55;
              }
            } else if(AnimationTimer == 3) {
              if(AnimationTimer5 == 0) {
                AnimationTimer5 = 55;
              }
            } else if(AnimationTimer == 4) {
              if(AnimationTimer6 == 0) {
                AnimationTimer6 = 55;
              }
            } else if(AnimationTimer == 5) {
              if(AnimationTimer7 == 0) {
                AnimationTimer7 = 55;
              }
            }
          }
          if(gb.buttons.repeat(BUTTON_RIGHT,4)) {
            playSFX(7,0);
            AnimationTimer++;
            AnimationTimer = constrain(AnimationTimer,0,5);
            if(AnimationTimer == 0) {
              if(AnimationTimer2 == 0) {
                AnimationTimer2 = 55;
              }
            } else if(AnimationTimer == 1) {
              if(AnimationTimer3 == 0) {
                AnimationTimer3 = 55;
              }
            } else if(AnimationTimer == 2) {
              if(AnimationTimer4 == 0) {
                AnimationTimer4 = 55;
              }
            } else if(AnimationTimer == 3) {
              if(AnimationTimer5 == 0) {
                AnimationTimer5 = 55;
              }
            } else if(AnimationTimer == 4) {
              if(AnimationTimer6 == 0) {
                AnimationTimer6 = 55;
              }
            } else if(AnimationTimer == 5) {
              if(AnimationTimer7 == 0) {
                AnimationTimer7 = 55;
              }
            }
          }
          if(gb.buttons.repeat(BUTTON_DOWN,4) && AnimationTimer<2) {
            playSFX(7,0);
            AnimationTimer+=4;
            if(AnimationTimer == 4) {
              if(AnimationTimer6 == 0) {
                AnimationTimer6 = 55;
              }
            } else if(AnimationTimer == 5) {
              if(AnimationTimer7 == 0) {
                AnimationTimer7 = 55;
              }
            }
            AnimationTimer = constrain(AnimationTimer,0,5);
          }
          if(gb.buttons.repeat(BUTTON_UP,4) && AnimationTimer>3) {
            playSFX(7,0);
            AnimationTimer-=4;
            if(AnimationTimer == 4) {
              if(AnimationTimer6 == 0) {
                AnimationTimer6 = 55;
              }
            } else if(AnimationTimer == 5) {
              if(AnimationTimer7 == 0) {
                AnimationTimer7 = 55;
              }
            }
            AnimationTimer = constrain(AnimationTimer,0,5);
          }
          if(gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) {
            playSFX(7,3);
            AnimationTimer = SelectedGender;
            SelectedGender = 2;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            AnimationTimer8 = 64;
          }
          if(gb.buttons.pressed(BUTTON_A)) {
            playSFX(7,2);
            SelectedHaircut = AnimationTimer;
            gb.sound.stop(0);
            difficulty = AnimationTimer8;
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            //TutorialMode = true;
            //IsPlaying = true;
            IsPlaying = false;
            //PrepareMap();
            player.mainPlayer.PlayerGender = SelectedGender;
            player.mainPlayer.PlayerHaircut = SelectedHaircut;
            player.mainPlayer.hat = 0;
            GameState = 1;
            return;
          }
        }
        if(AnimationTimer2 > 0) {
          AnimationTimer2 -= 9;
        }
        AnimationTimer2 = constrain(AnimationTimer2,0,255);
        if(AnimationTimer3 > 0) {
          AnimationTimer3 -= 9;
        }
        AnimationTimer3 = constrain(AnimationTimer3,0,255);
        if(AnimationTimer4 > 0) {
          AnimationTimer4 -= 9;
        }
        AnimationTimer4 = constrain(AnimationTimer4,0,255);
        if(AnimationTimer5 > 0) {
          AnimationTimer5 -= 9;
        }
        AnimationTimer5 = constrain(AnimationTimer5,0,255);
        if(AnimationTimer6 > 0) {
          AnimationTimer6 -= 9;
        }
        AnimationTimer6 = constrain(AnimationTimer6,0,255);
        if(AnimationTimer7 > 0) {
          AnimationTimer7 -= 9;
        }
        AnimationTimer7 = constrain(AnimationTimer7,0,255);

        if(AnimationTimer8 > 7) {
          AnimationTimer8 = ((AnimationTimer8 * 80) + (7*20))/100;
        }
      } else





      


      //Gear Selection Screen
      ///////////////////////
      if(GameState == 4) {
        gb.display.setColor((ColorIndex)3);
        gb.display.fill();

        gb.display.setColor((ColorIndex)(8-gb.buttons.repeat(BUTTON_A,0)));
        gb.display.cursorX = 1;
        gb.display.cursorY = 1;
        gb.display.print("WEAPON N-");
        gb.display.setColor((ColorIndex)(1*gb.buttons.repeat(BUTTON_A,0)));
        gb.display.print(AnimationTimer2);

        gb.display.drawImage(72,1,UIElement_1);
        gb.display.drawImage(76,1,UIElement_2);

        if(gb.buttons.repeat(BUTTON_LEFT,7)) {
          playSFX(7,0);
          AnimationTimer2 = constrain(AnimationTimer2-1,0,WeaponCount);
        }

        if(gb.buttons.repeat(BUTTON_RIGHT,7)) {
          playSFX(7,0);
          AnimationTimer2 = constrain(AnimationTimer2+1,0,WeaponCount);
        }

        gb.display.cursorX = 1;
        gb.display.cursorY = 7;
        gb.display.setColor((ColorIndex)8);
        gb.display.print("TYPE: ");
        gb.display.setColor((ColorIndex)0);
        if(Weapons[AnimationTimer2][0] == 0) {
          gb.display.print("CLASSIC");
        } else if(Weapons[AnimationTimer2][0] == 1) {
          gb.display.print("ROLLERS");
        } else if(Weapons[AnimationTimer2][0] == 2) {
          gb.display.print("CHARGERS");
        } else if(Weapons[AnimationTimer2][0] == 3) {
          gb.display.print("MASH ATTACK");
        }

        gb.display.cursorX = 1;
        gb.display.cursorY = 13;
        gb.display.setColor((ColorIndex)8);
        gb.display.print("ATTACK: ");
        gb.display.setColor((ColorIndex)0);
        gb.display.print(Weapons[AnimationTimer2][6]*Weapons[AnimationTimer2][8]);

        gb.display.cursorX = 1;
        gb.display.cursorY = 19;
        gb.display.setColor((ColorIndex)8);
        gb.display.print("FORCE: ");
        gb.display.setColor((ColorIndex)0);
        gb.display.print(Weapons[AnimationTimer2][1]);

        gb.display.cursorX = 1;
        gb.display.cursorY = 25;
        gb.display.setColor((ColorIndex)8);
        gb.display.print("INTERVAL: ");
        gb.display.setColor((ColorIndex)0);
        gb.display.print(Weapons[AnimationTimer2][5]);

        gb.display.cursorX = 1;
        gb.display.cursorY = 31;
        gb.display.setColor((ColorIndex)8);
        gb.display.print("INK PER ATTACK: ");
        gb.display.setColor((ColorIndex)0);
        gb.display.print(Weapons[AnimationTimer2][10]);

        gb.display.cursorX = 1;
        gb.display.cursorY = 37;
        gb.display.setColor((ColorIndex)8);
        gb.display.print("SPRAY: ");
        gb.display.setColor((ColorIndex)0);
        gb.display.print(Weapons[AnimationTimer2][2]+Weapons[AnimationTimer2][4]);
        
        if(gb.buttons.pressed(BUTTON_MENU) || gb.buttons.released(BUTTON_A)) {
          if(gb.buttons.released(BUTTON_A)) {
            playSFX(4,1);
            mainWeapon = AnimationTimer2;
            gb.save.set(2,mainWeapon);
          }
          
          AnimationTimer = STARTLENGHT2;
          AnimationTimer2 = 0;
          AnimationTimer3 = 0;
          AnimationTimer4 = 0;
          AnimationTimer5 = 0;
          GameState = 1;
        }
      }
    }



    //Turf War
    //////////
    if(IsPlaying && GameState == 0) {
      
      if(IsPaused) {
        if(PausedTimer < 0) {
          gb.display.setColor((ColorIndex)3);
          gb.display.fillRect(0,0,80,VerticalClosing[abs(PausedTimer+1)]);
          gb.display.fillRect(0,64-VerticalClosing[abs(PausedTimer+1)],80,VerticalClosing[abs(PausedTimer+1)]);
          gb.display.fillRect(0,VerticalClosing[abs(PausedTimer+1)],HorizontalClosing[abs(PausedTimer+1)],(64-(VerticalClosing[abs(PausedTimer+1)]*2)));
          gb.display.fillRect(80-HorizontalClosing[abs(PausedTimer+1)],VerticalClosing[abs(PausedTimer+1)],HorizontalClosing[abs(PausedTimer+1)],(64-(VerticalClosing[abs(PausedTimer+1)]*2)));
          if(PausedTimer < -6) {
            PartialRendering = true;
          } else {
            PartialRendering = false;
          }
          
          if(PausedTimer == -1) {
            IsPaused = false;
            PausedTimer = 0;
          }
        } else if(PausedTimer <= 12) {
          gb.display.setColor((ColorIndex)3);
          gb.display.fillRect(0,0,80,VerticalClosing[PausedTimer]);
          gb.display.fillRect(0,64-VerticalClosing[PausedTimer],80,VerticalClosing[PausedTimer]);
          gb.display.fillRect(0,VerticalClosing[PausedTimer],HorizontalClosing[PausedTimer],(64-(VerticalClosing[PausedTimer]*2)));
          gb.display.fillRect(80-HorizontalClosing[PausedTimer],VerticalClosing[PausedTimer],HorizontalClosing[PausedTimer],(64-(VerticalClosing[PausedTimer]*2)));
        } else if(PausedTimer > 12) {
          gb.display.setColor((ColorIndex)3);
          gb.display.fill();

          if(PausedTimer == 14) {
            //NORMAL
            gb.display.setColor((ColorIndex)6);
            gb.display.setFontSize(2);
            gb.display.setCursor(2,2);
            gb.display.print(enText[0]);
            
          } else if(PausedTimer == 15) {
            //GLITCH
            gb.display.setColor((ColorIndex)3);
            gb.display.fill();
            gb.display.setColor((ColorIndex)6);
            gb.display.setFontSize(2);
            gb.display.setCursor(2,2);
            gb.display.print(enText[0]);
            
            gb.display.drawImage(5,4+14,gb.display,0,4,58,3);
            gb.display.drawImage(6,9+14,gb.display,0,9,58,1);
            gb.display.setColor((ColorIndex)3);
            gb.display.fillRect(2,4,58,3);
            gb.display.fillRect(2,9,58,1);
            gb.display.drawImage(5,4,gb.display,5,4+14,58,3);
            gb.display.drawImage(6,9,gb.display,6,9+14,58,1);
            gb.display.fillRect(2,14,58,10);
          } else if(PausedTimer > 15) {
            if(gb.buttons.released(BUTTON_MENU)) {
              PausedTimer = -13;
            }
            
            //NORMAL
            gb.display.setColor((ColorIndex)6);
            gb.display.setFontSize(2);
            gb.display.setCursor(2,2);
            gb.display.print(enText[0]);
            gb.display.setFontSize(1);

            if(PausedTimer <= 17) {
              //Bugged Continue
              gb.display.setColor((ColorIndex)7);
              gb.display.drawLine(2,14,4,16);
              gb.display.drawLine(2,18,3,17);
              
              gb.display.setCursor(6,14);
              for(byte i = 0; i < strlen(enText[1]); i++ ) {
                if(enText[1][i] != ' ' && enText[1][i] != '.') {
                  gb.display.print(randomC[random(0,strlen(randomC))]);
                }
              }
              gb.display.setColor((ColorIndex)6);
            } else if(PausedTimer <= 19) {
              //Continue
              //Bugged Return to map
              gb.display.setColor((ColorIndex)7);
              gb.display.drawLine(2,14,4,16);
              gb.display.drawLine(2,18,3,17);
              
              gb.display.setCursor(6,14);
              gb.display.print(enText[1]);
              
              gb.display.setColor((ColorIndex)6);
              gb.display.setCursor(2,20);
              for(byte i = 0; i < strlen(enText[2]); i++ ) {
                if(enText[1][i] != ' ' && enText[2][i] != '.') {
                  gb.display.print(randomC[random(0,strlen(randomC))]);
                }
              }
            } else if(PausedTimer <= 21) {
              //Continue 
              //Return to map
              //Bugged Restart mission
              gb.display.setColor((ColorIndex)7);
              gb.display.drawLine(2,14,4,16);
              gb.display.drawLine(2,18,3,17);
              
              gb.display.setCursor(6,14);
              gb.display.print(enText[1]);
              
              gb.display.setColor((ColorIndex)6);
              gb.display.setCursor(2,20);
              gb.display.print(enText[2]);

              gb.display.setCursor(2,26);
              for(byte i = 0; i < strlen(enText[3]); i++ ) {
                if(enText[1][i] != ' ' && enText[3][i] != '.') {
                  gb.display.print(randomC[random(0,strlen(randomC))]);
                }
              }
            } else if(PausedTimer <= 23) {
              //Continue 
              //Return to map
              //Restart mission
              //Bugged Mark area as
              gb.display.setColor((ColorIndex)7);
              gb.display.drawLine(2,14,4,16);
              gb.display.drawLine(2,18,3,17);
              
              gb.display.setCursor(6,14);
              gb.display.print(enText[1]);
              
              gb.display.setColor((ColorIndex)6);
              gb.display.setCursor(2,20);
              gb.display.print(enText[2]);

              gb.display.setCursor(2,26);
              gb.display.print(enText[3]);

              gb.display.setCursor(2,32);
              for(byte i = 0; i < strlen(enText[4]); i++ ) {
                if(enText[1][i] != ' ' && enText[4][i] != '.') {
                  gb.display.print(randomC[random(0,strlen(randomC))]);
                }
              }
              PausedSelection = 0;
            } else if(PausedSelection<4) {
              //NORMAL
              gb.display.setColor((ColorIndex)7);
              if(gb.buttons.repeat(BUTTON_A,0)) {
                gb.display.setColor((ColorIndex)8);
              }
              gb.display.drawLine(2,14+(PausedSelection*6),4,16+(PausedSelection*6));
              gb.display.drawLine(2,18+(PausedSelection*6),3,17+(PausedSelection*6));
              gb.display.setColor((ColorIndex)6);

              if(PausedSelection==0) {
                gb.display.setColor((ColorIndex)7);
                if(gb.buttons.repeat(BUTTON_A,0)) {
                  gb.display.setColor((ColorIndex)8);
                }
                if(gb.buttons.released(BUTTON_A)) {
                  PausedTimer = -13;
                }
              }
              gb.display.setCursor(2+(4*(PausedSelection==0)),14);
              gb.display.print(enText[1]);
              gb.display.setColor((ColorIndex)6);

              if(PausedSelection==1) {
                gb.display.setColor((ColorIndex)7);
                if(gb.buttons.repeat(BUTTON_A,0)) {
                  gb.display.setColor((ColorIndex)8);
                }
              }
              gb.display.setCursor(2+(4*(PausedSelection==1)),20);
              gb.display.print(enText[2]);
              gb.display.setColor((ColorIndex)6);

              if(PausedSelection==2) {
                gb.display.setColor((ColorIndex)7);
                if(gb.buttons.repeat(BUTTON_A,0)) {
                  gb.display.setColor((ColorIndex)8);
                }
              }
              gb.display.setCursor(2+(4*(PausedSelection==2)),26);
              gb.display.print(enText[3]);
              gb.display.setColor((ColorIndex)6);

              if(PausedSelection==3) {
                gb.display.setColor((ColorIndex)7);
                if(gb.buttons.repeat(BUTTON_A,0)) {
                  gb.display.setColor((ColorIndex)8);
                }
                if(gb.buttons.released(BUTTON_A)) {
                  PausedSelection = 4;
                }
              }
              gb.display.setCursor(2+(4*(PausedSelection==3)),32);
              gb.display.print(enText[4]);
              gb.display.setColor((ColorIndex)6);

              if((gb.buttons.pressed(BUTTON_DOWN) || (gb.buttons.timeHeld(BUTTON_DOWN) > 8 && gb.buttons.repeat(BUTTON_DOWN,3))) && PausedSelection+1 < 4) {
                PausedSelection++;
              }
              if((gb.buttons.pressed(BUTTON_UP) || (gb.buttons.timeHeld(BUTTON_UP) > 8 && gb.buttons.repeat(BUTTON_UP,3))) && PausedSelection > 0) {
                PausedSelection--;
              }
            } else if(PausedSelection > 3) {
              //NORMAL

              
              gb.display.setCursor(2+(4*(PausedSelection==0)),14);
              gb.display.print(enText[1]);
              gb.display.setColor((ColorIndex)6);
              
              gb.display.setCursor(2+(4*(PausedSelection==1)),20);
              gb.display.print(enText[2]);
              gb.display.setColor((ColorIndex)6);
              
              gb.display.setCursor(2+(4*(PausedSelection==2)),26);
              gb.display.print(enText[3]);
              gb.display.setColor((ColorIndex)6);

              gb.display.setColor((ColorIndex)7);
              gb.display.setCursor(2+(4*(PausedSelection==3)),32);
              gb.display.print(enText[4]);
              gb.display.setColor((ColorIndex)6);

              gb.display.setColor((ColorIndex)9);
              if(gb.buttons.repeat(BUTTON_A,0)) {
                gb.display.setColor((ColorIndex)8);
              }
              gb.display.drawLine(6,14+(PausedSelection*6),4+4,16+(PausedSelection*6));
              gb.display.drawLine(6,18+(PausedSelection*6),3+4,17+(PausedSelection*6));
              gb.display.setColor((ColorIndex)10);

              if(PausedSelection==4) {
                gb.display.setColor((ColorIndex)9);
                if(gb.buttons.repeat(BUTTON_A,0)) {
                  gb.display.setColor((ColorIndex)8);
                }
                if(gb.buttons.released(BUTTON_A)) {
                  
                }
              }
              gb.display.setCursor(6+(4*(PausedSelection==4)),38);
              gb.display.print(enText[5]);
              gb.display.setColor((ColorIndex)10);

              if(PausedSelection==5) {
                gb.display.setColor((ColorIndex)9);
                if(gb.buttons.repeat(BUTTON_A,0)) {
                  gb.display.setColor((ColorIndex)8);
                }
                if(gb.buttons.released(BUTTON_A)) {
                  
                }
              }
              gb.display.setCursor(6+(4*(PausedSelection==5)),44);
              gb.display.print(enText[6]);
              gb.display.setColor((ColorIndex)10);


              if(gb.buttons.released(BUTTON_B)) {
                PausedSelection = 3;
              }

              if((gb.buttons.pressed(BUTTON_DOWN) || (gb.buttons.timeHeld(BUTTON_DOWN) > 8 && gb.buttons.repeat(BUTTON_DOWN,3))) && PausedSelection+1 < 6) {
                PausedSelection++;
              }
              if((gb.buttons.pressed(BUTTON_UP) || (gb.buttons.timeHeld(BUTTON_UP) > 8 && gb.buttons.repeat(BUTTON_UP,3))) && PausedSelection > 4) {
                PausedSelection--;
              }
            }
          }
        }
        gb.display.setFontSize(1);
        PausedTimer++;
        if(PausedTimer-1 >= 0) {
          return;
        }
      } else {
        if(gb.buttons.released(BUTTON_MENU)) {
          IsPaused = true;
          PausedTimer = 0;
        }
      }
      
      if(UseBackgroundInGame && !PartialRendering) {
        Background.setFrame(0);
        gb.display.drawImage(0,0,Background);
      } else {
        gb.display.setColor((ColorIndex)12);
        gb.display.fill();
      }
      
      player.UpdateGlobal();
      salmonidManager.UpdateGlobal();
      world.Update(0);
      salmonidManager.UnSpawnedGlobalUpdate();
      eggManager.UpdateGlobal();
      bulletsManager.Update();
      for(byte i = 0; i < BCOUNT; i++) {
        if(bulletsManager.bullets[i].Type == 5) {
          bulletsManager.bullets[i].vx = constrain(bulletsManager.bullets[i].vx + -(player.mainPlayer.x - bulletsManager.bullets[i].x)/15, -70, 70);
          bulletsManager.bullets[i].vy = constrain(bulletsManager.bullets[i].vy + -(player.mainPlayer.y - bulletsManager.bullets[i].y)/15, -70, 70);
        }
      }
      if(!TutorialMode || AnimationTimer6 > 0) {
        DrawCursor();
      }

      particleManager.Update();

      if(PausedTimer == 0) {
        if((Weapons[mainWeapon][0]) == 2 && gb.buttons.repeat(BUTTON_A,0)) {
          cameraX = (cameraX*5+(Div8(player.mainPlayer.x)+4-(LCDWIDTH/2) + ((curX-LCDWIDTH/2)/6)*10 + (-player.mainPlayer.vx/5)))/6;
          cameraY = (cameraY*4+(Div8(player.mainPlayer.y)+4-(LCDHEIGHT/2) + ((curY-LCDWIDTH/2)/6)*12 + (-player.mainPlayer.vy/5)))/5;
        } else if(!Tutorial) {
          cameraX = (cameraX*5+(Div8(player.mainPlayer.x)+4-(LCDWIDTH/2) + ((curX-LCDWIDTH/2)/4)*3 + (-player.mainPlayer.vx/5)))/6;
          cameraY = (cameraY*4+(Div8(player.mainPlayer.y)+4-(LCDHEIGHT/2) + ((curY-LCDWIDTH/2)/6)*4 + (-player.mainPlayer.vy/5)))/5;
        } else {
          cameraX = (cameraX*5+(Div8(player.mainPlayer.x)+4-(LCDWIDTH/2) + ((curX-LCDWIDTH/2)/4)*3 + (-player.mainPlayer.vx/5)))/6;
          cameraY = (cameraY*4+(Div8(player.mainPlayer.y)+4-(LCDHEIGHT/2) + ((curY-LCDWIDTH/2+16)/6)*4 + (-player.mainPlayer.vy/5)))/5;
        }
      }
      
      if(shakeTimeLeft > 0) {
        shakeTimeLeft--;
        cameraX += random(-1, 2) * shakeAmplitude;
        cameraY += random(-1, 2) * shakeAmplitude;
      } else {
        shakeAmplitude = 0;
      }

      ClampCamera();
  
      /*if(AnimationTimer > 0) {
        } else if(AnimationTimer > STARTLENGHT/3) {
        } else {
          if(AnimationTimer > (STARTLENGHT/3/2)) {
            //gb.display.drawImage(17,25,READY_ICON);
            //gb.display.drawImage(17,25+14,READY_INK,READY_INK.width(),READY_INK.height()*(STARTLENGHT/3-AnimationTimer)/(STARTLENGHT/3/2));
          } else {
            //gb.display.drawImage(27,25,GO_ICON);
            //gb.display.drawImage(27+1,25+14,GO_INK,GO_INK.width(),GO_INK.height()*(STARTLENGHT/3/2-AnimationTimer)/(STARTLENGHT/3/2));
            FreezePlayers = false;
            if(AnimationTimer == 1) {
              startMusic();
            }
          }
        }
        
        AnimationTimer = 0;//--;
        AnimationTimer = constrain(AnimationTimer,0,65535);
        if(AnimationTimer <= 0) {
          gb.display.setColor(BLACK);
          gb.display.cursorX = 0;
          gb.display.cursorY = 0;
          gb.display.fontSize = 1;
          gb.display.print(F(""));
        }*/
    
        gb.display.setColor((ColorIndex)9);
        if(player.mainPlayer.Live < 95) {
          gb.display.drawRect(0,0,80,64);
        }
        if(player.mainPlayer.Live < 55) {
          gb.display.drawRect(2,2,76,60);
        }
        if(player.mainPlayer.Live < 35) {
          gb.display.drawRect(4,4,72,56);
        }
        gb.display.colorIndex = palette;
        
        if(!TutorialMode || AnimationTimer6 > 3) {
          EggSlot.setFrame(player.mainPlayer.HasGoldenEgg);
          gb.display.drawImage(1,LCDHEIGHT-8,EggSlot);
          gb.display.drawImage(1,1,SalmonHud);
          gb.display.setColor((ColorIndex)0);
          char text[2];

          //Eggs
          gb.display.cursorX = 12;
          gb.display.cursorY = 2;
          sprintf(text,"%02d",world.FlagEggs);
          gb.display.print(text);
          gb.display.print("/");
          sprintf(text,"%02d",world.MaxEggs);
          gb.display.print(text);

          //Golden Eggs
          gb.display.cursorX = 55;
          gb.display.cursorY = 2;
          sprintf(text,"%02d",world.FlagGolden);
          gb.display.print(text);
          gb.display.print("/");
          sprintf(text,"%02d",world.MaxGolden);
          gb.display.print(text);

          //Anger
          gb.display.setColor((ColorIndex)7);
          gb.display.fillRect(10,11,world.Anger*28/100,3);

        
          gb.display.setColor((ColorIndex)7);
          gb.display.drawRect(74,54,5,9);
        }

        
        if(!TutorialMode || AnimationTimer6 > 2) {
          gb.display.setColor((ColorIndex)7);
          byte size = player.mainPlayer.Refill*7/100;
          byte fillsize = constrain(size,0,7);
          if(player.mainPlayer.Refill <= 0) {
            fillsize = 0;
          }
          //gb.display.fillRect(74,55,5,fillsize);
          for(int8_t y = fillsize-1; y >= 0; y--) {
            gb.display.drawFastHLine(74,61-y,5);
          }
          if(fillsize < 0) {
            return;
          }
          gb.display.setColor((ColorIndex)10);
          gb.display.drawFastHLine(74,54,5);
          gb.display.drawFastHLine(74,62,5);
        }

        if(TutorialMode) {
          gb.display.setColor((ColorIndex)0, (ColorIndex)3);
          if(AnimationTimer6 == 0) {
            gb.display.setCursor(1,64-1-6-1-6);
            if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
              AnimationTimer7 = 1;
            }
            if(AnimationTimer7 == 0) {
              gb.display.drawImage(80-1-18, 1, ArrowUp_Push);
              gb.display.print(enText[10]);
            } else {
              gb.display.drawImage(80-1-18, 1, B_Push);
              gb.display.print(enText[11]);
            }
          }
          
          if(AnimationTimer6 == 1) {
            gb.display.setCursor(1,64-1-6-1-6-1-6);
            gb.display.drawImage(80-1-18, 1, A_Push);
            gb.display.print(enText[12]);
            if(gb.buttons.released(BUTTON_A) && PausedTimer == 0) {
              AnimationTimer6 = 2;
              AnimationTimer7 = 0;
            }
          }
          
          if(AnimationTimer6 == 2) {
            gb.display.setCursor(1,64-1-6-1-6-1-6);
            if(AnimationTimer7 == 0 && Div64(player.mainPlayer.x) > 11) {
              AnimationTimer7 = 1;
            }
            if(AnimationTimer7 == 1) {
              gb.display.drawImage(80-1-18, 1, A_Push);
              gb.display.print(enText[13]);

              if(gb.buttons.released(BUTTON_A) && PausedTimer == 0) {
                AnimationTimer7 = 2;
              }
            } else
            if(AnimationTimer7 == 2) {
              gb.display.drawImage(80-1-18, 1, A_Push);
              gb.display.print(enText[14]);

              if(gb.buttons.released(BUTTON_A) && PausedTimer == 0) {
                AnimationTimer7 = 3;
              }
            } else
            if(AnimationTimer7 == 3) {
              gb.display.drawImage(80-1-18, 1, ArrowUp_Push);
              gb.display.print(enText[15]);

              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 4;
              }
            } else
            if(AnimationTimer7 == 4) {
              gb.display.drawImage(80-1-18, 1, ArrowDown_Push);
              gb.display.print(enText[16]);

              if(gb.buttons.released(BUTTON_DOWN) && PausedTimer == 0) {
                AnimationTimer7 = 5;
              }
            } else
            if(AnimationTimer7 == 5) {
              gb.display.setCursor(1,64-1-6-1-6);
              gb.display.drawImage(80-1-18, 1, ArrowUp_Push);
              gb.display.print(enText[17]);

              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer6 = 3;
                AnimationTimer7 = 0;
              }
            }
          }
          
          if(AnimationTimer6 == 3) {
            gb.display.setCursor(1,64-1-6-1-6-1-6);

            if(AnimationTimer7 == 0 && Div64(player.mainPlayer.x) > 17) {
              AnimationTimer7 = 1;
            }

            if(AnimationTimer7 == 1) {
              gb.display.print(enText[18]);
              if(Div64(player.mainPlayer.x) > 22) {
                AnimationTimer6 = 4;
                AnimationTimer7 = 0;
              }
            }
          }

          if(AnimationTimer6 == 4) {
            if(AnimationTimer7 == 0 && Div64(player.mainPlayer.x) > 24) {
              AnimationTimer7 = 1;
            }
            
            if(AnimationTimer7 == 1){
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[19]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 2;
              }
            } else
            if(AnimationTimer7 == 2) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              //gb.display.drawImage(80-1-18, 1, ArrowUp_Push);
              gb.display.print(enText[20]);
              /*if(gb.buttons.released(BUTTON_A) && PausedTimer == 0) {
                AnimationTimer6 = 5;
                AnimationTimer7 = 0;
              }*/
            }
          }

          if(AnimationTimer6 == 5) {
            if(AnimationTimer7 == 0) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.print(enText[21]);
              if(Div64(player.mainPlayer.x) < 23) {
                AnimationTimer7 = 1;
              }
            } else
            if(AnimationTimer7 == 1) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.print(enText[22]);
              if(Div64(player.mainPlayer.x) > 26) {
                AnimationTimer7 = 2;
              }
            } else
            if(AnimationTimer7 == 2) {
              if(Div64(player.mainPlayer.x) > 42) {
                salmonidManager.Spawn(Mul8(50), Mul8(3), 4);
                AnimationTimer7 = 3;
              }
            } else
            if(AnimationTimer7 == 3) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[23]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 4;
              }
            } else
            if(AnimationTimer7 == 4) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1-8, ArrowUp_Push);
              gb.display.print(enText[24]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 5;
              }
            } else
            if(AnimationTimer7 == 5) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.print(enText[25]);
            } else
            if(AnimationTimer7 == 6) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[26]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 7;
              }
            } else
            if(AnimationTimer7 == 7) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[27]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 8;
              }
            } else
            if(AnimationTimer7 == 8) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[28]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 9;
              }
            }  else
            if(AnimationTimer7 == 9) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[29]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 10;
              }
            } else
            if(AnimationTimer7 == 10) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[30]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 11;
              }
            } else
            if(AnimationTimer7 == 11) {
              if(Div64(player.mainPlayer.x) < 23) {
                AnimationTimer7 = 12;
              }
            } else
            if(AnimationTimer7 == 12) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[31]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                AnimationTimer7 = 13;
              }
            } else
            if(AnimationTimer7 == 13) {
              gb.display.setCursor(1,64-1-6-1-6-1-6);
              gb.display.drawImage(80-1-18, 1+8, ArrowUp_Push);
              gb.display.print(enText[32]);
              if(gb.buttons.released(BUTTON_UP) && PausedTimer == 0) {
                
              }
            }
          }
        }
        
        /*if(AnimationTimer2 < 45) {
          FreezePlayers = true;
          byte sX = constrain(STOP_ICON.width()*(45-AnimationTimer2)/10,0,STOP_ICON.width());
          byte sY = constrain(STOP_ICON.height()*(45-AnimationTimer2)/10,0,STOP_ICON.height());
          gb.display.drawImage(40-sX/2,32-sY/2,STOP_ICON, sX, sY);
          //Done!
        }*/ 

        if(PausedTimer < 0) {
          gb.display.setColor((ColorIndex)3);
          gb.display.fillRect(0,0,80,VerticalClosing[abs(PausedTimer+1)]);
          gb.display.fillRect(0,64-VerticalClosing[abs(PausedTimer+1)],80,VerticalClosing[abs(PausedTimer+1)]);
          gb.display.fillRect(0,VerticalClosing[abs(PausedTimer+1)],HorizontalClosing[abs(PausedTimer+1)],(64-(VerticalClosing[abs(PausedTimer+1)]*2)));
          gb.display.fillRect(80-HorizontalClosing[abs(PausedTimer+1)],VerticalClosing[abs(PausedTimer+1)],HorizontalClosing[abs(PausedTimer+1)],(64-(VerticalClosing[abs(PausedTimer+1)]*2)));
          if(PausedTimer == -1) {
            IsPaused = false;
            PausedTimer = 0;
          }
        }
        /*if(AnimationTimer2 > 0) {
          //AnimationTimer2-=1; //1
        } else {
          AnimationTimer2=0;
          AnimationTimer3 = 1;

          AlphaScore = 0;
          BetaScore = 0;
        }*/
      }
    }
  }
