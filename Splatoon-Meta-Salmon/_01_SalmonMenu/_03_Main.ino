uint16_t GetGoldenEgg () {
  return gb.save.get(7);
}

void AddGoldenEgg (uint16_t Add) {
  gb.save.set(7, (gb.save.get(7) + Add));
}

void ResetGoldenEgg () {
  gb.save.set(7, (int32_t)0);
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

void setup() {
  gb.begin();
  gb.setFrameRate(25);
  gb.display.setFont(SquidSquare);
  gb.display.colorIndex = palette;
  gb.sound.play("S/SLMN_LOOP.wav");

  GameState = 0;
  MissionLoad();
}

void CrossDraw(byte x, byte y) {
  gb.display.drawPixel(x,y);
  gb.display.drawPixel(x+1,y+1);
  gb.display.drawPixel(x+1,y-1);
  gb.display.drawPixel(x-1,y+1);
  gb.display.drawPixel(x-1,y-1);
}

void loop () {
  while(!gb.update());

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
        AnimationTimer+=2;
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
  } else






  //Map
  if(GameState == 1) {
    if(!MissionReadIndex(0,0)) {
      //Tutorial
      /*AnimationTimer = 0;
      AnimationTimer2 = 0;
      AnimationTimer3 = 0;
      AnimationTimer4 = 0;
      AnimationTimer5 = 0;
      AnimationTimer6 = 0;
      AnimationTimer7 = 0;
      TutorialMode = true;
      IsPlaying = true;
      PrepareMap();
      GameState = 0;*/
      gb.save.set(5, -1);
      gb.save.set(6, (int32_t)0);
      gb.bootloader.game("ShoalArcade/SALMON.bin");
      return;
    }
    gb.display.setColor((ColorIndex)11);
    if(AnimationTimer == 11) {
      gb.display.setColor((ColorIndex)8);
      MissionLoad();
    }
    gb.display.fill();
    if(AnimationTimer < 9) {
      gb.display.drawImage(16+(49-(49*128/9*AnimationTimer)/128)/2, 5+(56-(56*128/9*AnimationTimer)/128)/2, Map, (49*128/9*AnimationTimer)/128, (49*128/9*AnimationTimer)/128);
    } else if(AnimationTimer < 14) {
      if(AnimationTimer == 9 || AnimationTimer == 10) {
        gb.display.drawImage(16, 5, Map);
      }
      if(AnimationTimer == 11) {
        gb.display.colorIndex = GlitchPalette;
        gb.display.drawImage(16, 5, Map);
        gb.display.colorIndex = palette;
      }
      if(AnimationTimer == 12) {
        gb.display.drawImage(16, 5, Map, 0, 0, 45, 10);
        gb.display.drawImage(13, 15, Map, 0, 10, 44, 2);
        gb.display.drawImage(16, 17, Map, 0, 12, 45, 5);
        gb.display.drawImage(22, 22, Map, 0, 17, 47, 11);
        gb.display.drawImage(16, 33, Map, 0, 28, 48, 5);
        gb.display.drawImage(25, 38, Map, 0, 33, 49, 5);
        gb.display.drawImage(16, 42, Map, 0, 38, 49, 18);
      }
      if(AnimationTimer == 13) {
        gb.display.drawImage(16, 5, Map, 0, 0, 49, 36);
        gb.display.drawImage(10, 41, Map, 0, 35, 49, 6);
        gb.display.drawImage(16, 5, Map, 0, 42, 49, 13);
        gb.display.setColor((ColorIndex)0);
        gb.display.fillRect(9, 21, 35, 5);
        gb.display.fillRect(42, 9, 27, 5);
        gb.display.fillRect(54, 53, 10, 6);
      }
      if(AnimationTimer == 14) {
        gb.display.drawImage(16, 5, Map);
        gb.display.setColor((ColorIndex)3);
        gb.display.fillRect(43, 0, 10, 12);
        gb.display.fillRect(8, 25, 13, 7);
        gb.display.fillRect(46, 41, 22, 6);
      }
      if(AnimationTimer >= 15) {
        gb.display.drawImage(16, 5, Map);
      }
      AnimationTimer2 = 16;
    } else if(AnimationTimer < 20) {
      gb.display.drawImage(AnimationTimer2, 5, Map);
      AnimationTimer2 = (AnimationTimer2*80 + 3*20)/100;
    } else if(AnimationTimer >= 20) {
      gb.display.drawImage(3, 5, Map);
      if(AnimationTimer >= 20 && MissionReadIndex(0,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(41,19);
      }
      if(AnimationTimer >= 21 && MissionReadIndex(1,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(47,42);
      }
      if(AnimationTimer >= 22 && MissionReadIndex(2,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(25,44);
      }
      if(AnimationTimer >= 23 && MissionReadIndex(3,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(17,53);
      }
      if(AnimationTimer >= 24 && MissionReadIndex(4,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(15,14);
      }
      if(AnimationTimer >= 25 && MissionReadIndex(5,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(12,26);
      }
      if(AnimationTimer >= 26 && MissionReadIndex(6,0)) {
        gb.display.setColor((ColorIndex)6);
        CrossDraw(12,26);
      }
      if(AnimationTimer == 28 && MissionReadIndex(7,0)) {
        gb.display.setColor((ColorIndex)0);
        gb.display.drawFastVLine(29,8,5);
        gb.display.drawFastHLine(26,11,5);
      }
      if(AnimationTimer == 29 && MissionReadIndex(7,0)) {
        gb.display.setColor((ColorIndex)0);
        gb.display.drawFastVLine(29,9,3);
        gb.display.drawFastHLine(27,11,3);
      }
      if(AnimationTimer >= 28 && MissionReadIndex(7,0)) {
        gb.display.setColor((ColorIndex)7);
        gb.display.fillRect(28,10,3,3);
      }
      if(AnimationTimer >= 27  && MissionReadIndex(7,0)) {
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
          if(MissionReadIndex(AnimationTimer4,0)) {
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
          if(MissionReadIndex(AnimationTimer4,1)) {
            gb.display.setColor((ColorIndex)7);
            gb.display.print("UNLOCK");
          } else {
            gb.display.setColor((ColorIndex)6);
            gb.display.print("LOCK");
          }

          gb.display.drawImage(51,58,Golden_Small);
          gb.display.setCursor(57,58);
          gb.display.setColor((ColorIndex)0);
          gb.display.print(GetGoldenEgg());
            
        } else {
          if(AnimationTimer7 == 0 && gb.buttons.released(BUTTON_A) && MissionReadIndex(AnimationTimer4,0)) {
            AnimationTimer7 = 1;
            AnimationTimer8 = 1;
          } else
          if(AnimationTimer7 > 0 && gb.buttons.released(BUTTON_B)) {
            AnimationTimer7 = 0;
          } else
          if(AnimationTimer7 > 0 && gb.buttons.released(BUTTON_A) && (MissionReadIndex(AnimationTimer4,AnimationTimer8-1))) {
            //ENTER
            /*difficulty = AnimationTimer4;
            difficultyB = AnimationTimer8-1;
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
            GameState = 0;*/
            gb.save.set(5, AnimationTimer4);
            gb.save.set(6, AnimationTimer8-1);
            gb.save.set(18, SelectedGender);
            gb.save.set(17, SelectedHaircut);
            gb.bootloader.game("ShoalArcade/SALMON.bin");
          }
          
          if(AnimationTimer7 == 0) {
            gb.display.drawImage(50,0,StartMission);
            gb.display.drawImage(52,55,Icon_Menu);
            if(MissionReadIndex(AnimationTimer4,0)) {
              gb.display.drawImage(72,55,Icon_A);
              gb.display.drawImage(72,46,Icon_B);
            } else {
              gb.display.drawImage(72,55,Icon_B);
            }
          } else if(AnimationTimer7 > 0) {
            if(gb.buttons.pressed(BUTTON_UP)) {
              AnimationTimer8 = 1;
            } else if(gb.buttons.pressed(BUTTON_DOWN)) {
              AnimationTimer8 = 2;
            }
            
            gb.display.drawImage(50,12,StartMission);
            gb.display.setColor((ColorIndex)13);
            gb.display.fillRect(50,0,30,28);
            gb.display.drawImage(52,55,Icon_Menu);
            if(MissionReadIndex(AnimationTimer4,0)) {
              gb.display.drawImage(72,55,Icon_A);
              gb.display.drawImage(72,46,Icon_B);
            } else {
              gb.display.drawImage(72,55,Icon_B);
            }

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
          if(gb.buttons.pressed(BUTTON_DOWN) && MissionReadIndex(1,0)) {
            AnimationTimer4=1;
          }
          if(gb.buttons.pressed(BUTTON_LEFT) && MissionReadIndex(6,0)) {
            AnimationTimer4=6;
          }
        } else
        if(AnimationTimer4 == 1) {
          AnimationTimer5 = 48;
          AnimationTimer6 = 42;
          if(gb.buttons.pressed(BUTTON_UP) && MissionReadIndex(0,0)) {
            AnimationTimer4=0;
          }
          if(gb.buttons.pressed(BUTTON_LEFT) && MissionReadIndex(2,0)) {
            AnimationTimer4=2;
          }
        } else
        if(AnimationTimer4 == 2) {
          AnimationTimer5 = 25;
          AnimationTimer6 = 44;
          if(gb.buttons.pressed(BUTTON_DOWN) && MissionReadIndex(3,0)) {
            AnimationTimer4=3;
          }
          if(gb.buttons.pressed(BUTTON_LEFT) && MissionReadIndex(3,0)) {
            AnimationTimer4=3;
          }
          if(gb.buttons.pressed(BUTTON_RIGHT) && MissionReadIndex(1,0)) {
            AnimationTimer4=1;
          }
          if(gb.buttons.pressed(BUTTON_UP) && MissionReadIndex(5,0)) {
            AnimationTimer4=5;
          }
        } else
        if(AnimationTimer4 == 3) {
          AnimationTimer5 = 17;
          AnimationTimer6 = 53;
          if(gb.buttons.pressed(BUTTON_UP) && MissionReadIndex(5,0)) {
            AnimationTimer4=5;
          }
          if(gb.buttons.pressed(BUTTON_RIGHT) && MissionReadIndex(2,0)) {
            AnimationTimer4=2;
          }
        } else
        if(AnimationTimer4 == 4) {
          AnimationTimer5 = 16;
          AnimationTimer6 = 13;
          if(gb.buttons.pressed(BUTTON_DOWN) && MissionReadIndex(5,0)) {
            AnimationTimer4=5;
          }
          if(gb.buttons.pressed(BUTTON_LEFT) && MissionReadIndex(5,0)) {
            AnimationTimer4=5;
          }
          if(gb.buttons.pressed(BUTTON_RIGHT) && MissionReadIndex(6,0)) {
            AnimationTimer4=6;
          }
        } else
        if(AnimationTimer4 == 5) {
          AnimationTimer5 = 12;
          AnimationTimer6 = 25;
          if(gb.buttons.pressed(BUTTON_DOWN) && MissionReadIndex(3,0)) {
            AnimationTimer4=3;
          }
          if(gb.buttons.pressed(BUTTON_UP) && MissionReadIndex(4,0)) {
            AnimationTimer4=4;
          }
        } else
        if(AnimationTimer4 == 6) {
          AnimationTimer5 = 30;
          AnimationTimer6 = 10;
          if(gb.buttons.pressed(BUTTON_LEFT) && MissionReadIndex(4,0)) {
            AnimationTimer4=4;
          }
          if(gb.buttons.pressed(BUTTON_RIGHT) && MissionReadIndex(0,0)) {
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
  } else







  //PlayerSelect
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
            AnimationTimer2 = 55;
          }
        } else if(AnimationTimer == 5) {
          if(AnimationTimer7 == 0) {
            AnimationTimer3 = 55;
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
        //difficulty = AnimationTimer8;
        /*AnimationTimer = 0;
        AnimationTimer2 = 0;
        AnimationTimer3 = 0;
        AnimationTimer4 = 0;
        AnimationTimer5 = 0;
        AnimationTimer6 = 0;
        AnimationTimer7 = 0;*/
        //TutorialMode = true;
        //IsPlaying = true;
        //IsPlaying = false;
        //PrepareMap();
        //player.mainPlayer.PlayerGender = SelectedGender;
        //player.mainPlayer.PlayerHaircut = SelectedHaircut;
        //player.mainPlayer.hat = 0;
        gb.save.set(18, SelectedGender);
        gb.save.set(17, SelectedHaircut);
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
  }/* else*/



  


  //Gear Selection
  /*if(GameState == 4) {
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
  }*/
}
