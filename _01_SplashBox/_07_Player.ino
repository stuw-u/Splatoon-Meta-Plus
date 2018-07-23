class Player :
  public Object {
  public:
    bool InputControl = true;
    int8_t Live = 100;
    uint8_t Refill = 100;
    uint16_t RespawnTimer = 0;
    byte PlayerCode = 0;

    int8_t Kid2SquidFrames = 0;
    
    byte PlayerGender = 0;
    byte PlayerHaircut = 0;

    bool ShootCall = false;
    
    int8_t LVelX = 0;
    int8_t LVelY = 0;
    bool LastGrounded = false;
    int16_t Lastvy = 0;
    
    int8_t PlayerDir = 1; //Was Going Right
    bool IsSwiming = false;
    byte blinkEye = 60;
    byte eyeSize = 2;
    byte hat = 0;
    byte DuckPosY = 0;

    byte PlayerColor = 0;
    short GroundPoundTime = 0;
    
    bool LeftInk = false;
    bool BottomInk = false;
    bool RightInk = false;

    bool EBottomInk = false;

    uint32_t InkPoints = 0;
    
    int16_t targetX = 0;
    int16_t targetY = 0;

    bool A_PRESSED;
    bool A_HOLD;
    bool B_PRESSED;
    bool B_HOLD;
    
    bool UP_HOLD;
    bool DOWN_HOLD;
    bool LEFT_HOLD;
    bool RIGHT_HOLD;
    bool DOWN_PRESSED;

    bool Last_DOWN_HOLD;
    bool Last_A_HOLD;

    virtual int16_t getWidth() {
      if(IsSwiming) {
        return 10;
      } else {
        return 10; //10
      }
    };
    virtual int16_t getHeight() {
      if(IsSwiming) {
        return 6;
      } else {
        return 14; //14
      }
    };

    void MoveUpdate () {
      if(FreezePlayers) {
        return;
      }

      byte doorCode = TilesParams_Array[world.getTile(Div8(Div8(x)+4),Div64(y)+1)*5+1];
      if(doorCode == 12 || doorCode == 13 || doorCode == 14) {
        DoorWarning = true;
        if(gb.buttons.pressed(BUTTON_UP)) {
          playSFX(7,6);
          if(doorCode == 12) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            GameState = 5;
          } else if(doorCode == 13) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            GameState = 2;
          } else if(doorCode == 14) {
            AnimationTimer = 0;
            AnimationTimer2 = 0;
            AnimationTimer3 = 0;
            AnimationTimer4 = 0;
            AnimationTimer5 = 0;
            GameState = 3;
          }
        }
      } else {
        DoorWarning = false;
      }
      
      if(!InputControl) {
        ControlUpdate();
      } else {
        A_HOLD = gb.buttons.repeat(BUTTON_A,0);
        if(Weapons[mainWeapon][0] != 1) {
          B_HOLD = gb.buttons.repeat(BUTTON_B,0);
          DOWN_HOLD = gb.buttons.repeat(BUTTON_DOWN,0);
        } else {
          //B_HOLD = gb.buttons.repeat(BUTTON_B,0);
          B_HOLD = false;
          if(gb.buttons.repeat(BUTTON_B,0)) {
            DOWN_HOLD = false;
          } else {
            DOWN_HOLD = gb.buttons.repeat(BUTTON_DOWN,0);
          }
        }
        A_PRESSED = gb.buttons.pressed(BUTTON_A);
        B_PRESSED = gb.buttons.pressed(BUTTON_B);

        UP_HOLD = gb.buttons.repeat(BUTTON_UP,0);
        LEFT_HOLD = gb.buttons.repeat(BUTTON_LEFT,0);
        RIGHT_HOLD = gb.buttons.repeat(BUTTON_RIGHT,0);
        DOWN_PRESSED = gb.buttons.pressed(BUTTON_DOWN);
      }

      if(gb.buttons.repeat(BUTTON_UP,9) && !gb.buttons.pressed(BUTTON_UP) && LEFT_HOLD && RIGHT_HOLD) {
        particleManager.spawnParticle(Div8(x)+random(-3,6),Div8(y),5,colorGroup,PlayerColor);
      }
      
      BottomInk = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div64(y)+2,0,world.MapHeight-1),0) > 0 
      && world.SMGetColor(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div64(y)+2,0,world.MapHeight-1)) == PlayerColor;

      if(BottomInk) {
        if(AnimationTimer2%5==0) {
          Live = constrain(Live+1,0,100);
        }
      } else {
        if(AnimationTimer2%6==0) {
          Live = constrain(Live+1,0,100);
        }
      }
      
      RightInk = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4)+1,0,world.MapWidth-1),constrain(Div8(Div8(y)+4),0,world.MapHeight-1),3) > 0 
      && world.SMGetColor(constrain(Div8(Div8(x)+4)+1,0,world.MapWidth-1),constrain(Div8(Div8(y)+4),0,world.MapHeight-1)) == PlayerColor;
      
      LeftInk = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4)-1,0,world.MapWidth-1),constrain(Div8(Div8(y)+4),0,world.MapHeight-1),1) > 0 
      && world.SMGetColor(constrain(Div8(Div8(x)+4)-1,0,world.MapWidth-1),constrain(Div8(Div8(y)+4),0,world.MapHeight-1)) == PlayerColor;
  
      EBottomInk = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div64(y)+2,0,world.MapHeight-1),0) > 0 
      && world.SMGetColor(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div64(y)+2,0,world.MapHeight-1)) != PlayerColor;

      //Kid to Squid to Kid collision
      if(DOWN_HOLD && !Last_DOWN_HOLD && (!A_HOLD || (A_HOLD&&!Last_A_HOLD))) {
        Kid2SquidFrames = 0;
        y+=64;
      }
      if(!DOWN_HOLD && Last_DOWN_HOLD && !A_HOLD) {
        Kid2SquidFrames = 0;
        y-=64;
      }
      if(A_HOLD&&!Last_A_HOLD && DOWN_HOLD) {
        Kid2SquidFrames = 0;
        y-=64;
      }
      if(!A_HOLD&&Last_A_HOLD && DOWN_HOLD) {
        Kid2SquidFrames = 0;
        y+=64;
      }
      if(!A_HOLD&&Last_A_HOLD && DOWN_HOLD&&!Last_DOWN_HOLD) {
        Kid2SquidFrames = 0;
        y-=64;
      }
      if(A_HOLD&&!Last_A_HOLD && !DOWN_HOLD&&Last_DOWN_HOLD) {
        Kid2SquidFrames = 0;
        y-=64;
      }

      //Put ink under player when shooting
      if(ShootCall) {
        inkX = Div64(x);
        inkY = Div64(y);
        
        inkX = constrain(inkX,0,world.MapWidth-1);
        inkY = constrain(inkY,0,world.MapHeight-1);

        if(world.SMGetPaintValueAt(inkX,inkY+2,0) <= 0) {
          V0 = constrain(world.SMGetPaintValueAt(inkX,inkY+2,0)+1, 0, 3);
        } else {
          V0 = constrain(world.SMGetPaintValueAt(inkX,inkY+2,0), 0, 3);
        }
        V1 = constrain(world.SMGetPaintValueAt(inkX,inkY+2,1), 0, 3);
        V2 = constrain(world.SMGetPaintValueAt(inkX,inkY+2,2), 0, 3);
        V3 = constrain(world.SMGetPaintValueAt(inkX,inkY+2,3), 0, 3);

        world.SMSetPaintValue(
          constrain(V0, 0, 3),
          constrain(V1, 0, 3),
          constrain(V2, 0, 3),
          constrain(V3, 0, 3),
          constrain(inkX,0,world.MapWidth-1),
          constrain(inkY+2,0,world.MapHeight-1),
          PlayerColor
        );
      }

      //Stairs and slopes handeling
      if(LEFT_HOLD) {
        IsSwiming = true;
        byte col = TilesParams_Array[world.getTile(Div8(Div8(x)-1),Div8(Div8(y)+11))*TileParamsCount+0];
        if(col == 7) {
          x -= 8;
          y -= 8;
        } else if(col == 0) {
          col = TilesParams_Array[world.getTile(Div8(Div8(x)+4),Div8(Div8(y)+16))*TileParamsCount+0];
          if(col == 7) {
            x -= 8;
            y -= 8;
          }
        }
      }
      if(RIGHT_HOLD) {
        byte col = TilesParams_Array[world.getTile(Div8(Div8(x)+12),Div8(Div8(y)+11))*TileParamsCount+0];
        if(col == 6) {
          x += 8;
          y -= 8;
        } else if(col == 0) {
          col = TilesParams_Array[world.getTile(Div8(Div8(x)+4),Div8(Div8(y)+16))*TileParamsCount+0];
          if(col == 6) {
            x += 8;
            y -= 8;
          }
        }
      }

      //Swim PhysX
      ////////////
      if(DOWN_HOLD && !A_HOLD) {
        bool BottomInkSquid = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div8(Div8(y))+1,0,world.MapHeight-1),0) > 0 
        && world.SMGetColor(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div8(Div8(y))+1,0,world.MapHeight-1)) == PlayerColor;

        bool BottomEInkSquid = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div8(Div8(y))+1,0,world.MapHeight-1),0) > 0 
        && world.SMGetColor(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div8(Div8(y))+1,0,world.MapHeight-1)) != PlayerColor;

        if(BottomEInkSquid && PlayerCode == 0) {
          shakeTimeLeft = 1;
          shakeAmplitude = 1;

          setLightColorToGroup(!revertColors);
          for(uint8_t x = 0; x < 2; x++) {
            for(uint8_t y = 0; y < 4; y++) {
              if(random(0,2)==0) {
                gb.lights.drawPixel(x,y);
              }
            }
          }
        }

        if(BottomInkSquid&&IsGroundedDown) {
          if(PlayerCode == 0) {
            setLightColorToGroup(revertColors);
            gb.lights.drawPixel(0,3);
            gb.lights.drawPixel(1,3);
          }
          
          if(AnimationTimer2%2==0) {
            Live = constrain(Live+1,0,100);
          }
          if(AnimationTimer2%1==0) {
            Refill = constrain(Refill+2,0,100);
          }
          if(LVelY == 0) {
            particleManager.spawnParticle(Div8(x)+1,Div8(y)+3,3,colorGroup,PlayerColor);
            if(!isOffScreen())
              playSFX(4,0);
          }
          LVelY = constrain(LVelY+1, -5, 5);

          if(PlayerCode == 0) {
            setLightColorToGroup(revertColors);
            for(uint8_t x = 0; x < 2; x++) {
              for(int8_t y = constrain(LVelY,0,3); y >= 0; y--) {
                if(random(0,4) != 0 || LVelY == 5) {
                  gb.lights.drawPixel(x,y);
                }
              }
            }
          }
        } else {
          if(LVelY > 0) LVelY = constrain(LVelY-2, 0, 5);
          if(LVelY < 0) LVelY = constrain(LVelY+2, -5, 0);
        }

        if(LeftInk&&LEFT_HOLD) {
          if(AnimationTimer2%2==0) {
            Live = constrain(Live+1,0,100);
          }
          if(AnimationTimer2%1==0) {
            Refill = constrain(Refill+2,0,100);
          }
          LVelX = constrain(LVelX-1, -4, 4);
        } else if(RightInk&&RIGHT_HOLD) {
          if(AnimationTimer2%2==0) {
            Live = constrain(Live+1,0,100);
          }
          if(AnimationTimer2%1==0) {
            Refill = constrain(Refill+2,0,100);
          }
          LVelX = constrain(LVelX+1, -4, 4);
        } else {
          if(LVelX > 0) LVelX = constrain(LVelX-2, 0, 4);
          if(LVelX < 0) LVelX = constrain(LVelX+2, -4, 0);
        }
        
        ShootCall = false;
        IsSwiming = true;
        if(RIGHT_HOLD && !IsGroundedRight) {
          if(BottomInkSquid) {
            if(vx - PSwimAcceleration > -PSwimMaxSpeed) {
              vx -= PSwimAcceleration;
            }
          } else if(BottomEInkSquid) {
            if(vx - PESwimAcceleration > -PESwimMaxSpeed) {
              vx -= PESwimAcceleration;
            }
          } else {
            if(vx - PGSwimAcceleration > -PGSwimMaxSpeed) {
              vx -= PGSwimAcceleration;
            }
          }
          PlayerDir = 1;
        } else if(RIGHT_HOLD && IsGroundedRight && RightInk) {
          vy = PSwimWallClimb;
          PlayerDir = 1;
        }
        if(LEFT_HOLD && !IsGroundedLeft) {
          if(BottomInkSquid) {
            if(vx + PSwimAcceleration < PSwimMaxSpeed) {
              vx += PSwimAcceleration;
            }
          } else if(BottomEInkSquid) {
            if(vx + PESwimAcceleration < PESwimMaxSpeed) {
              vx += PESwimAcceleration;
            }
          } else {
            if(vx + PGSwimAcceleration < PGSwimMaxSpeed) {
              vx += PGSwimAcceleration;
            }
          }
          PlayerDir = -1;
        } else if(LEFT_HOLD && IsGroundedLeft && LeftInk) {
          vy = PSwimWallClimb;
          PlayerDir = -1;
        }
        if((IsGroundedRight&&RightInk) || (IsGroundedLeft&&LeftInk)) {
          vy = max(vy-PSwimWallReleaseAcceleration,-PSwimWallReleaseMaxSpeed);
        }
        if(IsGroundedDown && gb.buttons.pressed(BUTTON_B)) {
          if(BottomEInkSquid) {
            vy = PEJumpForceSquid;
          } else {
            vy = PJumpForceSquid;
          }
        }
        if(!RIGHT_HOLD && !LEFT_HOLD) {
          vx = vx * 0.75f;
        }
        return;
      } else {
        IsSwiming = false;
      }
      
      //setColorToGroup(world.SMGetColor(constrain((x/SCALE+4)/8,0,world.MapWidth-1),constrain((y/SCALE)/8+2,0,world.MapHeight-1)));
      //gb.display.fillRect(toScreenX(constrain((x/SCALE+4)/8,0,world.MapWidth-1)*8),toScreenY(constrain((y/SCALE)/8+2,0,world.MapHeight-1)*8),8,8);
  
      //Mouvement SlowDown
      if(!RIGHT_HOLD && !LEFT_HOLD && !A_HOLD) {
        vx = vx * 0.75f;
      }
      
      if(EBottomInk) {
        if(PlayerCode == 0) {
          shakeTimeLeft = 1;
          shakeAmplitude = 1;

          setLightColorToGroup(!revertColors);
          for(uint8_t x = 0; x < 2; x++) {
            for(uint8_t y = 0; y < 4; y++) {
              if(random(0,2)==0) {
                gb.lights.drawPixel(x,y);
              }
            }
          }
        }
        
        vx = constrain(vx,-PERestrictedSpeed,PERestrictedSpeed);
      }
  
      //Gravity
      if(UP_HOLD) {
        vy = max(vy-3,-22);
      }

      if(true) {
        //NORMAL PhysX
        //////////////
        
        if(LVelY < 0) {
          LVelY++;
        } else if(LVelY > 0) {
          LVelY--;
        }
        
        if(LVelX < 0) {
          LVelX++;
        } else if(LVelX > 0) {
          LVelX--;
        }

        //MOVING
        if(RIGHT_HOLD && !A_HOLD) {
          if(EBottomInk) {
            if(vx - PEWalkAcceleration > -PEWalkMaxSpeed) {
              vx -= PEWalkAcceleration;
            }
          } else {
            if(vx - PWalkAcceleration > -PWalkMaxSpeed) {
              vx -= PWalkAcceleration;
            }
          }
          PlayerDir = 1;
        }
        if(LEFT_HOLD && !A_HOLD) {
          if(EBottomInk) {
            if(vx + PEWalkAcceleration < PEWalkMaxSpeed) {
              vx += PEWalkAcceleration;
            }
          } else {
            if(vx + PWalkAcceleration < PWalkMaxSpeed) {
              vx += PWalkAcceleration;
            }
          }
          PlayerDir = -1;
        }
      }
  
      //Mouvement Particle
      //if(GoingRight && VelocityX > 0) {
      //  WalkParticle = 2+(blinkEye/60.0F*2);
      //}
      //
      //if(!GoingRight && VelocityX < 0) {
      //  WalkParticle = 2+(blinkEye/60.0F*2);
      //}

      if(Lastvy > 40 && !LastGrounded && IsGroundedDown) {
        if(!isOffScreen())
          playSFX(1,1);
      } else if(!LastGrounded && IsGroundedDown) {
        if(!isOffScreen())
          playSFX(1,0);
      }
  
      //Jumps
      if(B_PRESSED && Object::IsGroundedDown  && !A_HOLD) {
        if(!isOffScreen())
          playSFX(0,0);
        if(EBottomInk) {
          vy = PEJumpForce;
        } else {
          vy = PJumpForce;
        }
      }
      if(B_PRESSED && Object::IsGroundedRight && !Object::IsGroundedDown  && !A_HOLD) { 
        vx = PWallJumpForceX;
        vy = PWallJumpForceY;
      }
      if(B_PRESSED && Object::IsGroundedLeft && !Object::IsGroundedDown  && !A_HOLD) { 
        vx = -PWallJumpForceX;
        vy = PWallJumpForceY;
      }
  
      //GroundPound
      if(DOWN_PRESSED && GroundPoundTime == 0 && !Object::IsGroundedDown  && !A_HOLD && !IsSwiming) { 
        GroundPoundTime = 1;
      }
      if(B_PRESSED && GroundPoundTime > 0 && !Object::IsGroundedDown  && !A_HOLD) {
        if(PlayerDir == 1) {
          vx = -33;
          vy = 76;
        } else {
          vx = 33;
          vy = 76;
        }
        GroundPoundTime = 0;
      }
      if(Object::IsGroundedDown && GroundPoundTime > 0) {
        GroundPoundTime = 0;
        if(!BottomInk) {
          vy = 36;
        }
      }
      if(GroundPoundTime > 0) {
        vx *= 0.7F;
        if(GroundPoundTime < 2) {
          vy = 28;
        } else if(GroundPoundTime > 3 && GroundPoundTime < 5) {
          vy = 0;
        }
        if(GroundPoundTime > 6) {
          vy = constrain(vy - 45, -127, 127);
        }
        GroundPoundTime++;
      }
    }




    

    byte MoveTimer = 0;
    byte State = 4;/*2*/               //STATE-- 0: Goto Target(Position), 1: Escape Target(Position), 2: Ink(Area), 3: Attack(Player), 4: Goto Target(Player), 5: Escape Target(Player)
    uint16_t ReachingTimer = 25;/*25*/ 
    uint8_t TargetPlayerId = 0; //-1

    //Squidbagging:
    /*LEFT_HOLD = false;
    RIGHT_HOLD = false;
    if(AnimationTimer%8<4) {
      DOWN_HOLD = false;
    } else {
      DOWN_HOLD = true;
    }
    MoveTimer = 0;*/

    void ControlUpdate () {

      //Action Triggerer -- Find what to do when an action ends and were are the players
      ///////////////////////////////////

      //0: When area done inked
      //1: --
      //2: After traveling, attack done/target escaped/attack cancelled
      //3: When a ennemie is close
      //4: No current attack and a close teammates needs some help
      //5: When two player are attacking
      
      if(abs(targetX)+abs(targetY) < 32) {
        if(State == 0 || State == 4) {
          State = 2;
          ReachingTimer = random(81,123);
        }
      }
      if(abs(targetX)+abs(targetY) > 1024) {
        if(State == 1 || State == 5 || State == 3) {
          State = 2;
          ReachingTimer = random(81,123);
        }
      }
      if(ReachingTimer<=0) {
        if(State == 2) {
          State = 0;
          ReachingTimer = random(95,240);
          targetX = random(0,world.MapWidth*8);
          targetY = random(0,world.MapHeight*8);
          TargetPlayerId = -1;
        } else if(State == 3) {
          if(random(0,4)==0) {
            State = 2;
            ReachingTimer = random(81,123);
          } else {
            ReachingTimer = random(95,240);
          }
        } else if(State == 0 || State == 1 || State == 4 || State == 5) {
          State = 2;
          ReachingTimer = random(45,80);
        }
      }



      //Destination not reached yet
      /////////////////////////////
      if(ReachingTimer>0 && (State == 0 || State == 1 || State == 3 || State == 4 || State == 5)) {
        if(State == 1 || State == 5) {
          targetX = -targetX;
          targetY = -targetY;
        }
        
        DOWN_HOLD = false;
        
        LEFT_HOLD = false;
        RIGHT_HOLD = false;
        
        if(targetY > 0) {
          if(random(0,14) == 0 && IsGroundedDown) {
            B_PRESSED = true;
          }
        } else {
          if(random(0,26) == 0 && IsGroundedDown) {
            B_PRESSED = true;
          }
        }

        if(targetY > 0) {
          if(random(0,6) == 0 && IsGroundedRight && !IsGroundedDown) {
            B_PRESSED = true;
            LEFT_HOLD = true;
            PlayerDir = -PlayerDir;
            MoveTimer = random(8,14);
          }
          if(random(0,6) == 0 && IsGroundedLeft && !IsGroundedDown) { 
            B_PRESSED = true;
            RIGHT_HOLD = true;
            PlayerDir = -PlayerDir;
            MoveTimer = random(8,14);
          }
        } else {
          if(random(0,16) == 0 && IsGroundedRight && !IsGroundedDown) {
            B_PRESSED = true;
            LEFT_HOLD = true;
            PlayerDir = -PlayerDir;
            MoveTimer = random(8,14);
          } else if(random(0,16) == 0 && IsGroundedLeft && !IsGroundedDown) { 
            B_PRESSED = true;
            RIGHT_HOLD = true;
            PlayerDir = -PlayerDir;
            MoveTimer = random(8,14);
          }
        }

        if(IsGroundedDown) {
          if(IsGroundedLeft) {
            if(random(0,45) == 0) {
              B_PRESSED = true;
            }
          } else if(IsGroundedRight) {
            if(random(0,45) == 0) {
              B_PRESSED = true;
            }
          }
        }
  
        if(State == 3) {
          ShootCall = true;

          if(random(0,12) == 0) {
            float aimingAngleV = atan2(targetX,targetY)+1.5708F;
            
            bulletsManager.spawnBullet(
              Object::x-7+(21*((PlayerDir+1)/2)),
              Object::y+5,
              (int)(cos(aimingAngleV)*110.0F),
              (int)(sin(aimingAngleV)*110.0F),
              PlayerColor,
              PlayerCode,
              3
            );
          } 
        } else {
          ShootCall = false;
        }

        bool BottomInkSquid = world.SMGetPaintValueAt(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div8(Div8(y))+1,0,world.MapHeight-1),0) > 0 
        && world.SMGetColor(constrain(Div8(Div8(x)+4),0,world.MapWidth-1),constrain(Div8(Div8(y))+1,0,world.MapHeight-1)) == PlayerColor;
        if((LeftInk || BottomInk || RightInk || BottomInkSquid) && !ShootCall) {
          DOWN_HOLD = true;
        }
        
        if(MoveTimer <= 0) {
          if(targetX < 0) {
            if(random(0,12) != 0) {
              MoveTimer = random(40,60);
              PlayerDir = -1;
            } else {
              MoveTimer = random(5,9);
              PlayerDir = 1;
            }
          } else {
            if(random(0,12) != 0) {
              MoveTimer = random(40,60);
              PlayerDir = 1;
            } else {
              MoveTimer = random(5,9);
              PlayerDir = -1;
            }
          }
        }

        if(targetY > 0) {
          if(PlayerDir < 0) {
            if(TilesParams_Array[world.getTile(constrain(Div8(Div8(x)+0),0,world.MapWidth-1),constrain(Div8(Div8(y))+2,0,world.MapHeight-1))*5+0] == 0) {
              B_PRESSED = true;
            }
          } else {
            if(TilesParams_Array[world.getTile(constrain(Div8(Div8(x)+8),0,world.MapWidth-1),constrain(Div8(Div8(y))+2,0,world.MapHeight-1))*5+0] == 0) {
              B_PRESSED = true;
            }
          }
        }
  
        if(MoveTimer > 0) {
          if(PlayerDir == 1) {
            RIGHT_HOLD = true;
            LEFT_HOLD = false;
          } else {
            LEFT_HOLD = true;
            RIGHT_HOLD = false;
          }
          MoveTimer--;
        }

        if(State == 1 || State == 5) {
          targetX = -targetX;
          targetY = -targetY;
        }

        ReachingTimer--;
      } else



      //Ink area
      //////////
      if(ReachingTimer>0 && State == 2) {
        ShootCall = true;

        if(AnimationTimer2%8 == 0) {
          float aimingAngleV = fmod((AnimationTimer2/20.0F),(PI*2.0F));

          if(aimingAngleV < PI) {
            PlayerDir = -1;
          } else {
            PlayerDir = 1;
          }
          
          bulletsManager.spawnBullet(
            Object::x-(7*8)+(21*((PlayerDir+1)/2)*8),
            Object::y+(5*8),
            (int)(cos(aimingAngleV-(PI/2.0F))*110.0F),
            (int)(sin(aimingAngleV-(PI/2.0F))*110.0F),
            PlayerColor,
            PlayerCode,
            3
          );
        }
        
        ReachingTimer--;
      } else {
        ShootCall = false;
      }
    }

    void BlinkingUpdate () {
      blinkEye--;
      if(blinkEye == 0) {
        eyeSize = 0;
        blinkEye = 60;
      } else if(blinkEye == 1) {
        eyeSize = 1;
      } else {
        eyeSize = 2;
      }
    }

    void Draw () {
      if(isOffScreen())
        return; //skip boxes which are out of the screen

      setPaletteToColorGroup(PlayerColor,colorGroup);
      
      int8_t sizeX = InklingF.width();
      int8_t sizeY = InklingF.height();
      uint8_t playerImageID = 0;

      if(IsSwiming) {
        if(Kid2SquidFrames < 2) {
          switch(Kid2SquidFrames) {
            case 0:
            playerImageID = 14;
            break;
            case 1:
            playerImageID = 15;
            break;
            case 2:
            playerImageID = 13;
            break;
          }
          Kid2SquidFrames++;
        } else if(!IsGroundedDown) {
          if(vy > 70) {
            playerImageID = 11;
          } else if(vy < -70) {
            playerImageID = 13;
          } else {
            playerImageID = 12;
          }
        } else {
          playerImageID = 10;
        }

        if(abs(LVelX) > 1) {
          if(PlayerDir == 1) {
            if(PlayerGender == 0) {
              InklingF.setFrame(playerImageID);
              gb.display.drawImage(toScreenX(Div8(x)-8+LVelX+(sizeX-((sizeX*PlayerDir)/3*(4-abs(LVelX))))),toScreenY(Div8(y)-7-8+LVelY),InklingF,(sizeX*PlayerDir)/3*(4-abs(LVelX)),(sizeY));
            } else {
              InklingM.setFrame(playerImageID);
              gb.display.drawImage(toScreenX(Div8(x)-8+LVelX+(sizeX-((sizeX*PlayerDir)/3*(4-abs(LVelX))))),toScreenY(Div8(y)-7-8+LVelY),InklingM,(sizeX*PlayerDir)/3*(4-abs(LVelX)),(sizeY));
            }
          } else {
            if(PlayerGender == 0) {
              InklingF.setFrame(playerImageID);
              gb.display.drawImage(toScreenX(Div8(x)-8+LVelX),toScreenY(Div8(y)-7-8+LVelY),InklingF,(sizeX*PlayerDir)/3*(4-abs(LVelX)),(sizeY));
            } else {
              InklingM.setFrame(playerImageID);
              gb.display.drawImage(toScreenX(Div8(x)-8+LVelX),toScreenY(Div8(y)-7-8+LVelY),InklingM,(sizeX*PlayerDir)/3*(4-abs(LVelX)),(sizeY));
            }
          }
        } else {
          if(PlayerGender == 0) {
            InklingF.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8+LVelX),toScreenY(Div8(y)-7-8+LVelY),InklingF,(sizeX*PlayerDir),(sizeY));
          } else {
            InklingM.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8+LVelX),toScreenY(Div8(y)-7-8+LVelY),InklingM,(sizeX*PlayerDir),(sizeY));
          }
        }
      } else {
        if(Kid2SquidFrames < 2) {
          switch(Kid2SquidFrames) {
            case 0:
            playerImageID = 13;
            break;
            case 1:
            playerImageID = 15;
            break;
            case 2:
            playerImageID = 14;
            break;
          }
          Kid2SquidFrames++;

          if(PlayerGender == 0) {
          InklingF.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingF,(sizeX*PlayerDir),(sizeY));
          } else {
            InklingM.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingM,(sizeX*PlayerDir),(sizeY));
          }

          gb.display.colorIndex = palette;
          return;
        }
        
        if(IsGroundedDown && vx == 0) {
          playerImageID = 0;
        }
        if(!IsGroundedDown && vy >= 0) {
          playerImageID = 7;
        }
        if(!IsGroundedDown && vy < 0) {
          playerImageID = 8;
        }
        if(IsGroundedDown && abs(vx) > 5 && (RIGHT_HOLD||LEFT_HOLD)) {
          playerImageID = 1+((blinkEye/AnimWALKSPEED)%3);
        }
        /*if(IsGroundedDown && abs(vx) > 72 && (RIGHT_HOLD||LEFT_HOLD)) {
          playerImageID = 4+((blinkEye/AnimRUNSPEED)%3);
        }*/

        if(ShootCall) {
          playerImageID = 9;
          ShootCall = false;
        }

        if(PlayerGender == 0) {
          switch(PlayerHaircut) {
            case 0:
            InklingF.setFrame(playerImageID);
            H0InklingF.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingF,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H0InklingF,sizeX*PlayerDir,sizeY);
            break;
            case 1:
            InklingF.setFrame(playerImageID);
            H1InklingF.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingF,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H1InklingF,sizeX*PlayerDir,sizeY);
            break;
            case 2:
            InklingF.setFrame(playerImageID);
            H2InklingF.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingF,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H2InklingF,sizeX*PlayerDir,sizeY);
            break;
            case 3:
            InklingF.setFrame(playerImageID);
            H3InklingF.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingF,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H3InklingF,sizeX*PlayerDir,sizeY);
            break;
          }
        } else {
          switch(PlayerHaircut) {
            case 0:
            InklingM.setFrame(playerImageID);
            H0InklingM.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingM,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H0InklingM,sizeX*PlayerDir,sizeY);
            break;
            case 1:
            InklingM.setFrame(playerImageID);
            H1InklingM.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingM,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H1InklingM,sizeX*PlayerDir,sizeY);
            break;
            case 2:
            InklingM.setFrame(playerImageID);
            H2InklingM.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingM,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H2InklingM,sizeX*PlayerDir,sizeY);
            break;
            case 3:
            InklingM.setFrame(playerImageID);
            H3InklingM.setFrame(playerImageID);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),InklingM,sizeX*PlayerDir,sizeY);
            gb.display.drawImage(toScreenX(Div8(x)-8),toScreenY(Div8(y)-7),H3InklingM,sizeX*PlayerDir,sizeY);
            break;
          }
        }
      }

      gb.display.colorIndex = palette;

      if(!IsSwiming && (cpuLoad < 100 && PlayerCode != 0)) {
        if(PlayerDir == 1) {
          gb.display.drawImage(toScreenX(Div8(x)-5)+HatOffset[playerImageID*2],toScreenY(Div8(y)-10)-HatOffset[playerImageID*2+1],HeadgearSprites[hat],16,16);
        } else {
          gb.display.drawImage(toScreenX(Div8(x)-3)-HatOffset[playerImageID*2],toScreenY(Div8(y)-10)-HatOffset[playerImageID*2+1],HeadgearSprites[hat],-16,16);
        }
      }
      
      //gb.display.setColor(BLACK);
      //gb.display.drawRect(toScreenX(x/SCALE),toScreenY(y/SCALE),getWidth(),getHeight());
    }

    void BulletCollision () {
      for(byte i = 0; i < BCOUNT; i++) {
        if(!bulletsManager.bullets[i].IsDead) {
          if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y),getWidth(),getHeight())) {
            if(bulletsManager.bullets[i].color != PlayerColor) {
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              Live-=bulletsManager.bullets[i].Damage;
              bulletsManager.bullets[i].Die();

              if(!isOffScreen() && Live > 0) {
                playSFX(2,0);
              } else if(!isOffScreen()) {
                playSFX(3,0);
              }
              
              if(PlayerCode == 0) {
                shakeTimeLeft += 3;
                shakeAmplitude += 1;
              }
            }
            if(bulletsManager.bullets[i].Owner != PlayerCode) {
              bulletsManager.bullets[i].Die();
            }
          }
        }
      }
    }
    
    void Update () {
      if(Live < 0) {
        Die();
      } else if(RespawnTimer > 0) {
        RespawnTimer--;
        if(RespawnTimer == 0) {
          if(PlayerColor == revertColors) {
            x = ((GetMap[world.CurrentLoadedMap][2]*8)-4)*8;
            y = (GetMap[world.CurrentLoadedMap][3]*8)*8;
          } else {
            x = (GetMap[world.CurrentLoadedMap][0]*8)*8 - Mul8(GetMap[world.CurrentLoadedMap][2])*8 - 64;
            y = (GetMap[world.CurrentLoadedMap][3]*8)*8;
          }
        }
        return;
      }
      
      MoveUpdate();
      LastGrounded = IsGroundedDown;
      Object::Update();
      BlinkingUpdate();
      Draw();
      BulletCollision();

      if(EBottomInk) {
        Live = constrain(Live, -1, 80);
      }

      A_PRESSED = false;
      B_PRESSED = false;
      DOWN_PRESSED = false;
      Last_DOWN_HOLD = DOWN_HOLD;
      Last_A_HOLD = A_HOLD;
      
      Lastvy = vy;
    }

    void Die () {
      particleManager.spawnParticle(x/8,y/8,4,colorGroup,PlayerColor);
      particleManager.spawnParticle(x/8+5,y/8+11,3,colorGroup,!PlayerColor);
      
      RespawnTimer = 80;
      Live = 100;
    }
};

#define PLAYER_C 4

class PlayersOperator {
  public:
  Player mainPlayer;
  Player players[PLAYER_C-1];

  void Initialize () {
    revertColors = random(0,2);
    mainPlayer.InputControl = true;
    for(uint8_t i = 0; i < PLAYER_C; i++) {
      if(i != 0) {
        players[i-1].blinkEye = random(60);
        players[i-1].InputControl = false;

        if(i < PLAYER_C/2) {
          players[i-1].PlayerDir = 1;
          players[i-1].PlayerColor = revertColors;
          players[i-1].x = ((GetMap[world.CurrentLoadedMap][2]*8)-4)*8;
          players[i-1].y = (GetMap[world.CurrentLoadedMap][3]*8)*8;
        } else {
          players[i-1].PlayerDir = -1;
          players[i-1].PlayerColor = 1-revertColors;
          players[i-1].x = (GetMap[world.CurrentLoadedMap][0]*8)*8 - (GetMap[world.CurrentLoadedMap][2]*8)*8- 64;
          players[i-1].y = (GetMap[world.CurrentLoadedMap][3]*8)*8;
        }

        players[i-1].PlayerCode = i;
        players[i-1].PlayerGender = random(0,2);
        players[i-1].PlayerHaircut = random(0,4);
        players[i-1].hat = random(0,HEADGEARCount+1);
        players[i-1].InkPoints = 0;
        players[i-1].Live = 100;
        players[i-1].Refill = 100;
        players[i-1].IsSwiming = false;

        players[i-1].State = 4;/*2*/               //STATE-- 0: Goto Target(Position), 1: Escape Target(Position), 2: Ink(Area), 3: Attack(Player), 4: Goto Target(Player), 5: Escape Target(Player)
        players[i-1].ReachingTimer = 25;/*25*/ 
        players[i-1].TargetPlayerId = 0; //-1
      } else {
        mainPlayer.PlayerColor = revertColors;
        mainPlayer.PlayerCode = 0;
        mainPlayer.InkPoints = 0;
        mainPlayer.Live = 100;
        mainPlayer.Refill = 100;
        mainPlayer.IsSwiming = false;
      }
    }
  }

  void UpdateGlobal () {
    for(int16_t i = 0; i < PLAYER_C; i++) {
      if(i == 0) {
        mainPlayer.Update();
      } else {
        players[i-1].Update();

        if(AnimationTimer2%10 == 0) {
          //Analize surrouding player:
          //- Close player needs help and no one's nearby
          //- How many ennemies are nearby and am I attacked
          //- Which ennemie is the closest

          //Goal:
          //AttackPlayer/3: When a ennemie is close
          //GotoPlayer/4: No current attack and a close teammates needs some help
          //EscapePlayer/5: When two player are attacking

          byte ennemieCountNearby = 0;
          int8_t closestEnnemie = -1;
          uint16_t closestRange = 65535;
          for(int16_t e = 0; e < PLAYER_C; e++) {
            if(e==0) {
              if(players[i-1].PlayerColor != mainPlayer.PlayerColor && mainPlayer.RespawnTimer > 0) {
                uint16_t Range = abs(players[i-1].x/8-mainPlayer.x/8)+abs(players[i-1].y/8-mainPlayer.y/8);
                if(Range < 128) {
                  ennemieCountNearby++;
                  if(Range < closestRange) {
                    closestEnnemie = e;
                  }
                }
              }
            } else {
              if(players[i-1].PlayerColor != players[e-1].PlayerColor && players[e-1].RespawnTimer > 0) {
                uint16_t Range = abs(players[i-1].x/8-players[e-1].x/8)+abs(players[i-1].y/8-players[e-1].y/8);
                if(Range < 128) {
                  ennemieCountNearby++;
                  if(Range < closestRange) {
                    closestEnnemie = e;
                  }
                }
              }
            }
          }

          if(ennemieCountNearby > 0 && closestEnnemie != -1) {
            if(ennemieCountNearby > 1) {
              //AttackClosestEnnemie
              players[i-1].State = 3;
              players[i-1].ReachingTimer = random(75,100);
              players[i-1].TargetPlayerId = closestEnnemie;
            } else {
              //EscapeClosestEnnemie
              players[i-1].State = 5;
              players[i-1].ReachingTimer = random(100,150);
              players[i-1].TargetPlayerId = closestEnnemie;
            }
          } else if(ennemieCountNearby <= 0) {
            for(int16_t e = 0; e < PLAYER_C; e++) {
              if(e==0) {
                if(players[i-1].PlayerColor == mainPlayer.PlayerColor && mainPlayer.RespawnTimer > 0) {
                  if(mainPlayer.Live < 50) {
                    //GotoPlayer
                    players[i-1].State = 4;
                    players[i-1].ReachingTimer = random(100,150);
                    players[i-1].TargetPlayerId = e;
                  }
                }
              } else {
                if(players[i-1].PlayerColor == players[e-1].PlayerColor && players[e-1].RespawnTimer > 0) {
                  if(players[e-1].Live < 50) {
                    //GotoPlayer
                    players[i-1].State = 4;
                    players[i-1].ReachingTimer = random(100,150);
                    players[i-1].TargetPlayerId = e;
                  }
                }
              }
            }
          }
          
        }
        if((players[i-1].State == 3 || players[i-1].State == 4 || players[i-1].State == 5) && players[i-1].TargetPlayerId != -1) {
          if(players[i-1].TargetPlayerId==0) {
            if(mainPlayer.RespawnTimer <= 0) {
              
              players[i-1].State = 2;
              players[i-1].ReachingTimer = random(81,123);
              
              players[i-1].TargetPlayerId = -1;
            } else {
              players[i-1].targetX = Div8(mainPlayer.x)-Div8(players[i-1].x);
              players[i-1].targetY = Div8(mainPlayer.y)-Div8(players[i-1].y);
            }
          } else {
            if(players[players[i-1].TargetPlayerId-1].RespawnTimer <= 0) {
              
              players[i-1].State = 2;
              players[i-1].ReachingTimer = random(81,123);

              players[i-1].TargetPlayerId = -1;
            } else {
              players[i-1].targetX = Div8(players[players[i-1].TargetPlayerId-1].x)-Div8(players[i-1].x);
              players[i-1].targetY = Div8(players[players[i-1].TargetPlayerId-1].y)-Div8(players[i-1].y);
            }
          }
        }
      }
    }
  }

  void UpdateMain () {
    mainPlayer.Update();
  }
};

PlayersOperator player;

void AddPointToPlayer(byte playerCode, byte points) {
  if(playerCode == 0) {
    player.mainPlayer.InkPoints+=points;
  } else {
    player.players[playerCode-1].InkPoints+=points;
  }
}

