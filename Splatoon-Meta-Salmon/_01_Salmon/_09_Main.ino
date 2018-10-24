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

void MissionUpload () {
  gb.save.set(5, Missions);
  gb.save.set(6, MissionsB);
}

void MissionLoad () {
  Missions = gb.save.get(5);
  MissionsB = gb.save.get(6);
}

byte MissionReadIndex (byte Index, byte AB) {
  if(AB == 0) {
    return (Missions >> Index) & B00000001;
  } else {
    return (MissionsB >> Index) & B00000001;
  }
}

void MissionWriteIndex (byte Index, byte AB, byte Value) {
  if(AB == 0) {
    if(Value == 1) {
      Missions |= (1<<Index);
    } else {
      Missions &= ~(1<<Index);
    }
  } else {
    if(Value == 1) {
      MissionsB |= (1<<Index);
    } else {
      MissionsB &= ~(1<<Index);
    }
  }
}

uint16_t GetGoldenEgg () {
  return gb.save.get(7);
}

void AddGoldenEgg (uint16_t Add) {
  gb.save.set(7, (gb.save.get(7) + Add));
}

void ResetGoldenEgg () {
  gb.save.set(7, (int32_t)0);
}

void PrepareMap () {
  world.SMResetMap();
  world.Initialize(gb.save.get(difficulty+8));
  eggManager.Init();
  salmonidManager.cooldown = 80;
  
  player.mainPlayer.x = world.GetSpawnPositionX();
  player.mainPlayer.y = world.GetSpawnPositionY();

  cameraX = world.MapWidth*8/2-LCDWIDTH/2+8;
  cameraY = 0;

  salmonidManager.zonePositionA = 0;
  salmonidManager.zonePositionB = world.MapWidth-1;
  salmonidManager.Clean();
  
  player.mainPlayer.vx = 0;
  player.mainPlayer.vy = 0;

  player.Initialize();
  bulletsManager.Reset();

  AnimationTimer2 = 1800; //Min*60*fps = Match Lenght
  AnimationTimer = STARTLENGHT;

  gb.sound.play("S/SLMN_INTRO.wav");
  PausedTimer = -12;
  IsPaused = true;


  Background.setFrame(difficulty%2);
  if(TutorialMode) {
    Background.setFrame(2);
    bulletsManager.spawnBullet(Mul64(12), Mul64(3), 0, -32, 1, 1, 0);
    bulletsManager.spawnBullet(Mul64(13), Mul64(3), 0, -32, 1, 1, 0);
    bulletsManager.spawnBullet(Mul64(14), Mul64(3), 0, -32, 1, 1, 0);

    salmonidManager.Spawn(Mul8(31), Mul8(3), 1, 1);
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
  gb.setFrameRate(25);
  gb.display.setFont(SquidSquare);
  gb.display.colorIndex = palette;
  player.mainPlayer.PlayerGender = /*SelectedGender*/gb.save.get(17);
  player.mainPlayer.PlayerHaircut = /*SelectedHaircut*/gb.save.get(18);
  mainWeapon = gb.save.get(2);
  MissionLoad();

  if(gb.save.get(8) == 0) {
    gb.save.set(8,random(1,255));
    gb.save.set(9,random(1,255));
    gb.save.set(10,random(1,255));
    gb.save.set(11,random(1,255));
    gb.save.set(12,random(1,255));
    gb.save.set(13,random(1,255));
    gb.save.set(14,random(1,255));
  }

  IsPlaying = true;
  GameState = 0;
  if(gb.save.get(15) == -1) {
    TutorialMode = true;
  } else {
    difficulty = gb.save.get(15);
  }
  difficultyB = gb.save.get(16);
  AnimationTimer = 0;
  AnimationTimer2 = 0;
  AnimationTimer3 = 0;
  AnimationTimer4 = 0;
  AnimationTimer5 = 0;
  AnimationTimer6 = 0;
  AnimationTimer7 = 0;
  AnimationTimer8 = 0;
  PrepareMap();
}

byte Mode = 0;
long r1 = 0;
long r2 = 0;

void loop () {
  while(!gb.update());
  gb.lights.clear();
  gb.display.setFont(SquidSquare);
  if(!IsPlaying) {
    if(GameState == 1) {
      gb.save.set(19, 1);
      gb.bootloader.game("ShoalArcade/SALMONMENU.bin");
    }
    
    if(GameState == 2) {
      gb.display.setColor((ColorIndex)3);
      gb.display.fill();
      if(AnimationTimer8 == 1 || AnimationTimer8 >= 3) {
        gb.display.setColor((ColorIndex)7);
        gb.display.setFontSize(2);
        gb.display.setCursor(2,2);
        gb.display.print(enText[33]);
        gb.display.setFontSize(1);
      } else if(AnimationTimer8 == 2) {
        //GLITCH
        AnimationTimer = 0;
        gb.display.setColor((ColorIndex)3);
        gb.display.fill();
        gb.display.setColor((ColorIndex)6);
        gb.display.setFontSize(2);
        gb.display.setCursor(2,2);
        gb.display.print(enText[33]);
        
        gb.display.drawImage(5,4+14,gb.display,0,4,58,3);
        gb.display.drawImage(6,9+14,gb.display,0,9,58,1);
        gb.display.setColor((ColorIndex)3);
        gb.display.fillRect(2,4,58,3);
        gb.display.fillRect(2,9,58,1);
        gb.display.drawImage(5,4,gb.display,5,4+14,58,3);
        gb.display.drawImage(6,9,gb.display,6,9+14,58,1);
        gb.display.fillRect(2,14,58,10);
        gb.display.setFontSize(1);
      }
      
      if(AnimationTimer8 == 4) {
        gb.display.setColor((ColorIndex)6);
        gb.display.setCursor(2,14);
        for(byte i = 0; i < strlen(enText[34]); i++ ) {
          if(enText[1][i] != ' ' && enText[34][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 4) {
        gb.display.setColor((ColorIndex)6);
        gb.display.setCursor(2,14);
        gb.display.print(enText[34]);
      }
      
      if(AnimationTimer8 == 5) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,20);
        for(byte i = 0; i < strlen(enText[35]); i++ ) {
          if(enText[1][i] != ' ' && enText[35][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 5) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,20);
        gb.display.print(enText[35]);
        gb.display.print(world.FlagEggs);
      }

      if(AnimationTimer8 == 6) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,26);
        for(byte i = 0; i < strlen(enText[36]); i++ ) {
          if(enText[1][i] != ' ' && enText[36][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 6) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,26);
        gb.display.print(enText[36]);
        gb.display.print(world.FlagGolden);
      }

      if(AnimationTimer8 == 12 || AnimationTimer8 == 14 || AnimationTimer8 == 15 || AnimationTimer8 >= 17) {
        gb.display.setCursor(1,40);
        if(AnimationTimer == 0) {
          gb.display.setCursor(5,40);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,40,1+2,40+2);
          gb.display.drawLine(1,40+4,1+2,40+2);
          if(gb.buttons.released(BUTTON_A)) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer7 = 0;
            AnimationTimer8 = 0;
            GameState = 1;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[38]);
        gb.display.setCursor(1,46);
        if(AnimationTimer == 1) {
          gb.display.setCursor(5,46);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,46,1+2,46+2);
          gb.display.drawLine(1,46+4,1+2,46+2);
          if(gb.buttons.released(BUTTON_A)) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            TutorialMode = false;
            IsPlaying = true;
            PrepareMap();
            GameState = 0;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[39]);
        gb.display.setCursor(1,52);
        if(AnimationTimer == 2) {
          gb.display.setCursor(5,52);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,52,1+2,52+2);
          gb.display.drawLine(1,52+4,1+2,52+2);
          if(gb.buttons.released(BUTTON_A)) {
            difficulty++;
            if(difficulty == 6) {
              gb.save.set(16, (int32_t)0);
              gb.bootloader.game("ShoalArcade/SALMONBOSS.bin");
            }
            difficultyB = 0;
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            TutorialMode = false;
            IsPlaying = true;
            PrepareMap();
            GameState = 0;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[40]);
        if(difficultyB == 0) {
          gb.display.print(difficulty+2);
          gb.display.print(".A");
        } else {
          gb.display.print(difficulty+2);
          gb.display.print(".A");
        }
        gb.display.setCursor(1,58);
        if(AnimationTimer == 3) {
          gb.display.setCursor(5,58);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,58,1+2,58+2);
          gb.display.drawLine(1,58+4,1+2,58+2);
          if(gb.buttons.released(BUTTON_A) && difficultyB == 0) {
            difficultyB = 1;
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            TutorialMode = false;
            IsPlaying = true;
            PrepareMap();
            GameState = 0;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[40]);
        if(difficultyB == 0) {
          gb.display.print(difficulty+1);
          gb.display.print(".B");
        } else {
          gb.display.print(enText[41]);
          gb.display.print(difficulty+1);
        }

        if(gb.buttons.repeat(BUTTON_UP, 5)) {
          AnimationTimer--;
        }
        if(gb.buttons.repeat(BUTTON_DOWN, 5)) {
          AnimationTimer++;
        }
        AnimationTimer = constrain(AnimationTimer, 0, 3);
      }
      
      if(AnimationTimer8 < 20) {
        AnimationTimer8++;
      }
    }
    


    if(GameState == 3) {
      gb.display.setColor((ColorIndex)3);
      gb.display.fill();
      if(AnimationTimer8 == 1 || AnimationTimer8 >= 3) {
        gb.display.setColor((ColorIndex)7);
        gb.display.setFontSize(2);
        gb.display.setCursor(2,2);
        gb.display.print(enText[33]);
        gb.display.setFontSize(1);
      } else if(AnimationTimer8 == 2) {
        //GLITCH
        AnimationTimer = 0;
        gb.display.setColor((ColorIndex)3);
        gb.display.fill();
        gb.display.setColor((ColorIndex)6);
        gb.display.setFontSize(2);
        gb.display.setCursor(2,2);
        gb.display.print(enText[33]);
        
        gb.display.drawImage(5,4+14,gb.display,0,4,58,3);
        gb.display.drawImage(6,9+14,gb.display,0,9,58,1);
        gb.display.setColor((ColorIndex)3);
        gb.display.fillRect(2,4,58,3);
        gb.display.fillRect(2,9,58,1);
        gb.display.drawImage(5,4,gb.display,5,4+14,58,3);
        gb.display.drawImage(6,9,gb.display,6,9+14,58,1);
        gb.display.fillRect(2,14,58,10);
        gb.display.setFontSize(1);
      }
      
      if(AnimationTimer8 == 4) {
        gb.display.setColor((ColorIndex)6);
        gb.display.setCursor(2,14);
        for(byte i = 0; i < strlen(enText[37]); i++ ) {
          if(enText[1][i] != ' ' && enText[37][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 4) {
        gb.display.setColor((ColorIndex)6);
        gb.display.setCursor(2,14);
        gb.display.print(enText[37]);
      }
      
      if(AnimationTimer8 == 5) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,20);
        for(byte i = 0; i < strlen(enText[35]); i++ ) {
          if(enText[1][i] != ' ' && enText[35][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 5) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,20);
        gb.display.print(enText[35]);
        gb.display.print(world.FlagEggs);
      }

      if(AnimationTimer8 == 6) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,26);
        for(byte i = 0; i < strlen(enText[36]); i++ ) {
          if(enText[1][i] != ' ' && enText[36][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 6) {
        gb.display.setColor((ColorIndex)2);
        gb.display.setCursor(2,26);
        gb.display.print(enText[36]);
        gb.display.print(world.FlagGolden);
      }

      if(AnimationTimer8 == 12 || AnimationTimer8 == 14 || AnimationTimer8 == 15 || AnimationTimer8 >= 17) {
        gb.display.setCursor(1,52);
        if(AnimationTimer == 0) {
          gb.display.setCursor(5,52);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,52,1+2,52+2);
          gb.display.drawLine(1,52+4,1+2,52+2);
          if(gb.buttons.released(BUTTON_A)) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            IsPlaying = false;
            GameState = 1;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[38]);
        gb.display.setCursor(1,58);
        if(AnimationTimer == 1) {
          gb.display.setCursor(5,58);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,58,1+2,58+2);
          gb.display.drawLine(1,58+4,1+2,58+2);
          if(gb.buttons.released(BUTTON_A)) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            TutorialMode = false;
            IsPlaying = true;
            PrepareMap();
            GameState = 0;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[39]);
        

        if(gb.buttons.repeat(BUTTON_UP, 5)) {
          AnimationTimer--;
        }
        if(gb.buttons.repeat(BUTTON_DOWN, 5)) {
          AnimationTimer++;
        }
        AnimationTimer = constrain(AnimationTimer, 0, 1);
      }
      
      if(AnimationTimer8 < 20) {
        AnimationTimer8++;
      }
    }
  }
  
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
              if(gb.buttons.released(BUTTON_A)) {
                //Exit to map
                playSFX(7,2);
                AnimationTimer = 0;
                AnimationTimer2 = 0;
                AnimationTimer3 = 0;
                AnimationTimer4 = 0;
                AnimationTimer5 = 0;
                AnimationTimer6 = 0;
                AnimationTimer7 = 0;
                IsPlaying = false;
                GameState = 1;
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
              if(gb.buttons.released(BUTTON_A)) {
                playSFX(7,2);
                AnimationTimer = 0;
                AnimationTimer2 = 0;
                AnimationTimer3 = 0;
                AnimationTimer4 = 0;
                AnimationTimer5 = 0;
                AnimationTimer6 = 0;
                AnimationTimer7 = 0;
                AnimationTimer8 = 0;
                PrepareMap();
                IsPaused = false;
                PausedTimer = 0;
                return;
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
      gb.display.colorIndex = paletteBG;
      Background.setFrame(difficulty%3);
      gb.display.drawImage(0,0,Background);
      gb.display.colorIndex = palette;
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
        bulletsManager.bullets[i].vx = constrain(bulletsManager.bullets[i].vx + -(player.mainPlayer.x - bulletsManager.bullets[i].x)/(12+bulletsManager.bullets[i].Timer/10), -70, 70);
        bulletsManager.bullets[i].vy = constrain(bulletsManager.bullets[i].vy + -((player.mainPlayer.y - Mul8(8)) - bulletsManager.bullets[i].y)/(12+bulletsManager.bullets[i].Timer/10), -70, 70);
      }
    }
    if(!TutorialMode || AnimationTimer6 > 0) {
      DrawCursor();
    }

    particleManager.Update();

    zoneM1 = (zoneM1*9+salmonidManager.zonePositionA*8)/10;
    zoneM2 = (zoneM2*9+salmonidManager.zonePositionB*8)/10;
    if(!isOffScreen(zoneM1,(world.MapHeight*8-(world.WaterLevel))-12,11,16) && world.WaterLevel>40) {
      gb.display.drawImage(toScreenX(zoneM1),toScreenY(world.MapHeight*8-(world.WaterLevel)-20),SpawnLights);
    }

    if(!isOffScreen(zoneM2,(world.MapHeight*8-(world.WaterLevel))-12,11,16) && world.WaterLevel>40) {
      gb.display.drawImage(toScreenX(zoneM2),toScreenY(world.MapHeight*8-(world.WaterLevel)-20),SpawnLights);
    }

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

      gb.display.setColor((ColorIndex)0);
      for(byte i = 0; i < hitAnim; i++) {
        gb.display.drawRect(0+i,0+i,80-i*2,64-i*2);
      }
      if(hitAnim > 0) {
        hitAnim--;
      }
      gb.display.colorIndex = palette;

      if(world.FlagEggs >= world.MaxEggs && world.FlagGolden >= world.MaxGolden && !TutorialMode) {
        //Ending Cinematics!
        if(AnimationTimer8 > 220) {
          AddGoldenEgg(world.FlagGolden);
          if(difficultyB == 0 && difficulty < 6) {
            MissionWriteIndex(difficulty,1,1);
            MissionWriteIndex(difficulty+1,0,1);
          } else if(difficultyB == 1 && difficulty < 6) {
            MissionWriteIndex(difficulty+1,0,1);
          }
          MissionUpload();
          AnimationTimer = 0;
          AnimationTimer8 = 0;
          IsPlaying = false;
          GameState = 2;
          return;
        }

        if(AnimationTimer8 == 0) {
          gb.sound.play("S/SLMN_END.wav");
        }
        AnimationTimer8++;
      }

      if(world.WaterLevel > world.MapHeight*8-6*8 && !TutorialMode) {
        AnimationTimer = 0;
        AnimationTimer8 = 0;
        IsPlaying = false;
        GameState = 3;
        return;
      }
      
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
        //sprintf(text,"%02d",salmonidManager.cooldown);
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
              salmonidManager.Spawn(Mul8(50), Mul8(3), 4, 1);
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
              MissionWriteIndex(0,0,1);
              MissionUpload();
              AnimationTimer = 0;
              AnimationTimer2 = 0;
              AnimationTimer3 = 0;
              AnimationTimer4 = 0;
              AnimationTimer5 = 0;
              AnimationTimer6 = 0;
              AnimationTimer7 = 0;
              TutorialMode = false;
              IsPlaying = false;
              GameState = 1;
            }
          }
        }
      }
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
    }
  }
