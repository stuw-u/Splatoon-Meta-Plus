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
  world.Initialize(gb.save.get(difficulty+8));
  salmonidManager.cooldown = 80;
  salmonidManager.wave = 0;
  metawave = 0;
  salmonidManager.nextRequest = -1;
  
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

  PausedTimer = -12;
  IsPaused = true;


  Background.setFrame(difficulty%2);
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

  IsPlaying = true;
  GameState = 0;
  difficulty = 0;
  difficultyB = 0;
  AnimationTimer = 0;
  AnimationTimer2 = 0;
  AnimationTimer3 = 0;
  AnimationTimer4 = 0;
  AnimationTimer5 = 0;
  AnimationTimer6 = 0;
  AnimationTimer7 = 0;
  AnimationTimer8 = 0;
  PrepareMap();
  gb.sound.play("S/SLMN_INTRO.wav");
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
        gb.display.print(enText[4]);
        gb.display.setFontSize(1);
      } else if(AnimationTimer8 == 2) {
        //GLITCH
        AnimationTimer = 0;
        gb.display.setColor((ColorIndex)3);
        gb.display.fill();
        gb.display.setColor((ColorIndex)6);
        gb.display.setFontSize(2);
        gb.display.setCursor(2,2);
        gb.display.print(enText[4]);
        
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
        for(byte i = 0; i < strlen(enText[5]); i++ ) {
          if(enText[1][i] != ' ' && enText[5][i] != '.') {
            gb.display.print(randomC[random(0,strlen(randomC))]);
          }
        }
      } else if(AnimationTimer8 > 4) {
        gb.display.setColor((ColorIndex)6);
        gb.display.setCursor(2,14);
        gb.display.print(enText[5]);
      }

      if(AnimationTimer8 == 12 || AnimationTimer8 == 14 || AnimationTimer8 == 15 || AnimationTimer8 >= 17) {
        gb.display.setCursor(1,52);
        if(AnimationTimer == 0) {
          gb.display.setCursor(5,52);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,52,1+2,52+2);
          gb.display.drawLine(1,52+4,1+2,52+2);
          if(gb.buttons.released(BUTTON_A)) {
            //EXIT
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
        gb.display.print(enText[6]);
        gb.display.setCursor(1,58);
        if(AnimationTimer == 1) {
          gb.display.setCursor(5,58);
          gb.display.setColor((ColorIndex)10);
          gb.display.drawLine(1,58,1+2,58+2);
          gb.display.drawLine(1,58+4,1+2,58+2);
          if(gb.buttons.released(BUTTON_A)) {
            gb.sound.stop(0);
            gb.sound.play("SLMN_INTRO.wav");
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            IsPlaying = true;
            PrepareMap();
            GameState = 0;
          }
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        gb.display.print(enText[7]);

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
        if(PausedTimer == -13) {
          gb.display.fill();
          if(player.mainPlayer.RespawnTimer > 0) {
            player.mainPlayer.RespawnTimer--;
            PausedTimer++;
          }
          return;
        }
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

            if((gb.buttons.pressed(BUTTON_DOWN) || (gb.buttons.timeHeld(BUTTON_DOWN) > 8 && gb.buttons.repeat(BUTTON_DOWN,3))) && PausedSelection+1 < 3) {
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
      if(player.mainPlayer.RespawnTimer > 0 && PausedTimer >= 11) {
        PausedTimer = -13;
        IsPaused = true;
        return;
      }
      if(!(player.mainPlayer.RespawnTimer > 0 && PausedTimer < 0)) {
        PausedTimer++;
      }
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
      cannonX = (-(cameraX-17*4)/8);
      aimX = (-(cameraX-17*7)/12);
      salmonX = (-(cameraX-17*8)/16);
      
      googlyEyesX = ((cameraX-17*4)/64);
      googlyEyesY = ((cameraY)/16);
      
      gb.display.colorIndex = paletteBG;
      Background.setFrame(0);
      gb.display.drawImage(0,0,Background);
      gb.display.colorIndex = palette;
      
      gb.display.drawImage(19+salmonX, 45, Boss_Bottom);
      if(world.WaterWave/16%2==0) {
        gb.display.drawImage(22+salmonX, 13, Boss_Closed);
        gb.display.setColor((ColorIndex)6);
        gb.display.drawPixel(22+salmonX+11,15+14);
        gb.display.drawPixel(22+salmonX+24,15+14);
        gb.display.setColor((ColorIndex)3);
        gb.display.drawPixel(22+salmonX+11+googlyEyesX,14+14+googlyEyesY);
        gb.display.drawPixel(22+salmonX+24+googlyEyesX,14+14+googlyEyesY);
      } else {
        gb.display.drawImage(22+salmonX, 13, Boss_Open);
        gb.display.setColor((ColorIndex)6);
        gb.display.drawPixel(22+salmonX+11,15+12);
        gb.display.drawPixel(22+salmonX+24,15+12);
        gb.display.setColor((ColorIndex)3);
        gb.display.drawPixel(22+salmonX+11+googlyEyesX,14+12+googlyEyesY);
        gb.display.drawPixel(22+salmonX+24+googlyEyesX,14+12+googlyEyesY);
      }
      
      gb.display.drawImage(4+cannonX, 22, Boss_Canon, 11, 42);
      if(Bam1 > 0) {
        gb.display.drawImage(7+cannonX, 21, Boss_CanonSpark, 9, 9);
        Bam1--;
      }
      gb.display.drawImage(65+cannonX, 22, Boss_Canon, -11, 42);
      if(Bam2 > 0) {
        gb.display.drawImage(64+cannonX, 21, Boss_CanonSpark, -9, 9);
        Bam2--;
      }

      if(Rocket) {
        if(RocketY < 28) {
          gb.display.drawImage(36+aimX, 22, HitbackRocket_Aim);
          gb.display.drawImage(36+aimX, -9+RocketY, HitbackRocket_Return);
        } else {
          RedSparkEyes.setFrame(RocketY-28);
          if(world.WaterWave/16%2==0) {
            gb.display.drawImage(22+salmonX+11+googlyEyesX-7,14+14+googlyEyesY-7,RedSparkEyes);
            gb.display.drawImage(22+salmonX+24+googlyEyesX-7,14+14+googlyEyesY-7,RedSparkEyes);
          } else {
            gb.display.drawImage(22+salmonX+11+googlyEyesX-7,14+12+googlyEyesY-7,RedSparkEyes);
            gb.display.drawImage(22+salmonX+24+googlyEyesX-7,14+12+googlyEyesY-7,RedSparkEyes);
          }
        }
        if(RocketY > 25 && RocketY < 25+4) {
          gb.display.drawImage(36+aimX-3, 22-3, HitSpark_White);
        }
        
        if(RocketY < 28+13) {
          RocketY++;
        } else {
          RocketY = 0;
          Rocket = false;
          if(metawave == 3) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            AnimationTimer6 = 0;
            AnimationTimer7 = 0;
            IsPlaying = false;
            GameState = 2;
            gb.sound.play("S/SLMN_END.wav");
          }
        }
      }
    } else {
      gb.display.setColor((ColorIndex)12);
      gb.display.fill();
    }
    
    player.UpdateGlobal();
    salmonidManager.UpdateGlobal();
    world.Update(0);
    salmonidManager.UnSpawnedGlobalUpdate();
    bulletsManager.Update();
    for(byte i = 0; i < BCOUNT; i++) {
      if(bulletsManager.bullets[i].Type == 5) {
        bulletsManager.bullets[i].vx = constrain(bulletsManager.bullets[i].vx + -(player.mainPlayer.x - bulletsManager.bullets[i].x)/(15+bulletsManager.bullets[i].Timer/7), -70, 70);
        bulletsManager.bullets[i].vy = constrain(bulletsManager.bullets[i].vy + -((player.mainPlayer.y - Mul8(14)) - bulletsManager.bullets[i].y)/(15+bulletsManager.bullets[i].Timer/7), -70, 70);
      }
    }
    DrawCursor();

    particleManager.Update();

    if(PausedTimer == 0) {
      if((Weapons[mainWeapon][0]) == 2 && gb.buttons.repeat(BUTTON_A,0)) {
        cameraX = (cameraX*5+(Div8(player.mainPlayer.x)+4-(LCDWIDTH/2) + ((curX-LCDWIDTH/2)/6)*10 + (-player.mainPlayer.vx/5)))/6;
        cameraY = (cameraY*4+(Div8(player.mainPlayer.y)+4-(LCDHEIGHT/2) + ((curY-LCDWIDTH/2)/6)*12 + (-player.mainPlayer.vy/5)))/5;
      } else {
        cameraX = (cameraX*5+(Div8(player.mainPlayer.x)+4-(LCDWIDTH/2) + ((curX-LCDWIDTH/2)/4)*3 + (-player.mainPlayer.vx/5)))/6;
        cameraY = (cameraY*4+(Div8(player.mainPlayer.y)+4-(LCDHEIGHT/2) + ((curY-LCDWIDTH/2)/6)*4 + (-player.mainPlayer.vy/5)))/5;
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

      /*if(world.FlagEggs >= world.MaxEggs && world.FlagGolden >= world.MaxGolden && !TutorialMode) {
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

        AnimationTimer8++;
      }*/
     

      
      gb.display.setColor((ColorIndex)7);
      byte size = player.mainPlayer.Refill*7/100;
      byte fillsize = constrain(size,0,7);
      if(player.mainPlayer.Refill <= 0) {
        fillsize = 0;
      }
      gb.display.fillRect(74,55,5,fillsize);
      for(int8_t y = fillsize-1; y >= 0; y--) {
        gb.display.drawFastHLine(74,61-y,5);
      }
      if(fillsize < 0) {
        return;
      }
      gb.display.setColor((ColorIndex)10);
      gb.display.drawFastHLine(74,54,5);
      gb.display.drawFastHLine(74,62,5);
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
