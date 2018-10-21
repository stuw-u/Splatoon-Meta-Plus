byte activeSalmons = 0;

class Salmonid:
  public Object {
  public:
  byte type = 0;
  int16_t Live = 0;
  byte BLive = 0;
  int16_t LiveS1 = 0;
  int16_t LiveS2 = 0;
  byte cooldown = 0;
  byte cooldown2 = 0;
  byte turningCooldown = 0;
  int8_t dir = 1;
  bool alive = false;
  bool hasSpawned = false;
  
  int8_t bobbing = 0;
  int8_t bob = 0;
  
  //0: Spoon buddy
  //1: Normal
  //2: Normal 2.0
  //3: Fat ones
  //4: Boss Maws dude
  //5: Mechanic asshole //No hurt when down, attack white thing
  //6: Flying asshole
  //7: Robot guy
  //8: The one with a bomb on his head
  //9: Tower
  //10: EggDrop
  //11: GoldenEggDrop
  
  virtual int16_t getYBounce() {
    if(type == 10 || type == 11) {
      return 65;
    }
    return 40;
  };

  virtual int16_t getGravity() {
    if(type == 10 || type == 11) {
      return 0;
    }
    if(type == 6) {
      return 0;
    }
    return 55;
  };

  virtual int16_t getWidth() {
    if(type == 0) {
      return 8;
    } else if(type == 1) {
      return 11;
    } else if(type == 2) {
      return 11;
    } else if(type == 3) {
      return 31;
    } else if(type == 4) {
      if(cooldown < 19) {
        return 23;
      } else {
        return 4;
      }
    } else if(type == 5) {
      return 23;
    } else if(type == 6) {
      return 12;
    } else if(type == 7) {
      return 0;
    } else if(type == 8) {
      return 23;
    } else if(type == 9) {
      return 24;
    } else if(type == 10 || type == 11) {
      return 10;
    }
  };
  
  virtual int16_t getHeight() {
    return 1;
  };

  int16_t getSHeight() {
    if(type == 0) {
      return 8;
    } else if(type == 1) {
      return 13;
    } else if(type == 2) {
      return 13;
    } else if(type == 3) {
      return 29;
    } else if(type == 4) {
      if(cooldown < 19) {
        return 23;
      } else {
        return 4;
      }
    } else if(type == 5) {
      return 19;
    } else if(type == 6) {
      return 11;
    } else if(type == 7) {
      return 0;
    } else if(type == 8) {
      return 23;
    } else if(type == 9) {
      byte r = 69;
      if((BLive&B00010000) == 0) {
        r-=9;
      }
      if((BLive&B00001000) == 0) {
        r-=12;
      }
      if((BLive&B00000100) == 0) {
        r-=9;
      }
      if((BLive&B00000010) == 0) {
        r-=14;
      }
      if((BLive&B00000001) == 0) {
        r-=12;
      }
      return r;
    } else if(type == 10 || type == 11) {
      return 10;
    }
  };

  int16_t getLive () {
    byte boost = 32 + difficultyB*10;
    if(type == 0) {
      return 60*boost/32;
    } else if(type == 1) {
      return 140*boost/32;
    } else if(type == 2) {
      return 140*boost/32;
    } else if(type == 3) {
      return 310*boost/32;
    } else if(type == 4) {
      return 420*boost/32;
    } else if(type == 5) {
      return 180*boost/32;
    } else if(type == 6) {
      return 1;
    } else if(type == 7) {
      return 0;
    } else if(type == 8) {
      return 210*boost/32;
    } else if(type == 10 || type == 11) {
      return 60*boost/32;
    }
  }

  byte getBLive () {
    if(type == 9) {
      return B00011111;
    } else {
      return 0;
    }
  }

  int16_t getLiveSecond () {
    if(type == 5) {
      return 280;
    } else if(type == 6) {
      return 90;
    } else if(type == 9) {
      return 90;
    } else {
      return 0;
    }
  }

  byte getAttackDamage () {
    if(type == 0) {
      return 17;
    } else if(type == 1) {
      return 27;
    } else if(type == 2) {
      return 27;
    } else if(type == 3) {
      return 39;
    } else if(type == 4) {
      return 101;
    } else if(type == 5) {
      return 42;
    } else if(type == 6) {
      return 0;
    } else if(type == 7) {
      return 55;
    } else if(type == 8) {
      return 0;
    } else if(type == 9) {
      return 35;
    } else if(type == 10 || type == 11) {
      return 0;
    }
  }

  byte getPowerEggDrop () {
    if(type == 0) {
      return 1;
    } else if(type == 1) {
      return 2;
    } else if(type == 2) {
      return 2;
    } else if(type == 3) {
      return 3;
    } else if(type == 4) {
      return 2;
    } else if(type == 5) {
      return 2;
    } else if(type == 6) {
      return 2;
    } else if(type == 7) {
      return 2;
    } else if(type == 8) {
      return 2;
    } else if(type == 9) {
      return 2;
    } else if(type == 10) {
      return 3;
    } else {
      return 0;
    }
  }

  byte getGoldenEggDrop () {
    if(type == 0) {
      return 0;
    } else if(type == 1) {
      return 0;
    } else if(type == 2) {
      return 0;
    } else if(type == 3) {
      return 0;
    } else if(type == 4) {
      return 1;
    } else if(type == 5) {
      return 1;
    } else if(type == 6) {
      return 1;
    } else if(type == 7) {
      return 2;
    } else if(type == 8) {
      return 2;
    } else if(type == 9) {
      return 2;
    } else if(type == 11) {
      return 2;
    } else {
      return 0;
    }
  }

  uint16_t getSpeed () {
    if(TutorialMode && type == 1) {
      return 0;
    }
    
    if(type == 0) {
      return 110;
    } else if(type == 1) {
      return 93;
    } else if(type == 2) {
      return 93;
    } else if(type == 3) {
      return 85;
    } else if(type == 5) {
      return 42;
    } else if(type == 8) {
      return 14;
    } else if(type == 9) {
      return 7;
    }
  }
  
  int8_t getAnger () {
    if(type == 0) {
      return 3;
    } else if(type == 1) {
      return 8;
    } else if(type == 2) {
      return 8;
    } else if(type == 3) {
      return 12;
    } else if(type == 4) {
      return -18;
    } else if(type == 5) {
      return -22;
    } else if(type == 8) {
      return -32;
    } else if(type == 9) {
      return -26;
    }
    return 0;
  }

  void UnSpawnedUpdate () {
    if(type == 10 || type == 11) {
      hasSpawned = true;
      return;
    }
    if(!hasSpawned) {
      Object::UpdateGrounding(0);
    }
    if(Div8(y)<(world.MapHeight*8-6*8) && !(IsGroundedUp||IsGroundedDown||IsGroundedLeft||IsGroundedUp)) {
      hasSpawned = true;
    }
    if(type == 4) {
      y -= 3*8;
      if(Div8(y)<(world.MapHeight*8-6*8)) {
        if(Div64(x) < world.MapWidth/2) {
          x += 8;
        } else {
          x -= 8;
        } 
      }
      return;
    }
    if((Div8(y)>(world.MapHeight*8-6*8) || (IsGroundedUp||IsGroundedDown||IsGroundedLeft||IsGroundedUp)) && !hasSpawned) {
      y -= 3*8;
      if(Div8(y)<(world.MapHeight*8-6*8)) {
        if(Div64(x) < world.MapWidth/2) {
          x += 8;
        } else {
          x -= 8;
        } 
      }
      Draw();
      return;
    }
    if(Div8(y)<0) {
      Kill(false);
    }
  }

  void Update () {
    bob += bobbing/3;
    bobbing *= 8;
    bobbing /= 10;
    bobbing += (0 - bob)*3;
    
    if(turningCooldown == 0 && (type != 5 || cooldown == 0)) {
      if((player.mainPlayer.x + player.mainPlayer.getWidth()/2*8) < x) {
        dir = -1;
      } else {
        dir = 1;
      }
      if(type != 4) {
        turningCooldown = 12;
      } else {
        turningCooldown = 3;
      }
    } else {
      if(turningCooldown > 0) {
        turningCooldown--;
      }
    }

    if(type == 5 && cooldown == 0) {
      if(IsGroundedDown) {
        world.SMSetPaintAtWallIndex(constrain(Div64(x),0,world.MapWidth-1), constrain(Div64(y)+1,0,world.MapWidth-1), 0, 0);
      }
      if(IsGroundedLeft) {
        world.SMSetPaintAtWallIndex(constrain(Div8(Div8(x)-8),0,world.MapWidth-1), constrain(Div64(y),0,world.MapWidth-1), 1, 0);
      }
      if(IsGroundedRight) {
        world.SMSetPaintAtWallIndex(constrain(Div8(Div8(x)+8),0,world.MapWidth-1), constrain(Div64(y),0,world.MapWidth-1), 3, 0);
      }
    } else if(turningCooldown%3 == 0 && type != 4 && type != 10 && type != 11) {
      if(IsGroundedDown) {
        world.SMSetPaintAtWallIndex(constrain(Div64(x),0,world.MapWidth-1), constrain(Div64(y)+1,0,world.MapWidth-1), 0, 1);
      }
      if(IsGroundedLeft) {
        world.SMSetPaintAtWallIndex(constrain(Div8(Div8(x)-8),0,world.MapWidth-1), constrain(Div64(y),0,world.MapWidth-1), 1, 1);
      }
      if(IsGroundedRight) {
        world.SMSetPaintAtWallIndex(constrain(Div8(Div8(x)+8),0,world.MapWidth-1), constrain(Div64(y),0,world.MapWidth-1), 3, 1);
      }
    }

    for(byte i = 0; i < BCOUNT; i++) {
      if(!bulletsManager.bullets[i].IsDead) {
        if(type == 6 && cooldown < 39) {
          if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x)-12,Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y)-16,9,16)) {
            particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
            playSFX(2,0);
            LiveS1-=bulletsManager.bullets[i].Damage;
            bulletsManager.bullets[i].Die();
          }
          if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x)+9,Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y)-16,9,16)) {
            particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
            playSFX(2,0);
            LiveS2-=bulletsManager.bullets[i].Damage;
            bulletsManager.bullets[i].Die();
          }
        }
        if(type == 8) {
          //BODY
          if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),Div8(x)-16,Div8(y)-40,36,51)) {
            playSFX(2,1);
            particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
            bulletsManager.bullets[i].Die();
          }

          //BALOON
          
        } else if(type == 9 && BLive > 0) {
          if(LiveS1 > 0) {
            if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y)-getSHeight(),getWidth(),getSHeight()) && bulletsManager.bullets[i].color != 1) {
              playSFX(2,0);
              LiveS1-=bulletsManager.bullets[i].Damage;
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              bulletsManager.bullets[i].Die();
            }
          }
          if(LiveS1<=0) {
            if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y)-getSHeight(),getWidth(),getSHeight())) {
              playSFX(2,1);
              if((BLive&B00000001) == 1<<0) {
                BLive &= ~(1 << 0);
              } else
              if((BLive&B00000010) == 1<<1) {
                BLive &= ~(1 << 1);
              } else
              if((BLive&B00000100) == 1<<2) {
                BLive &= ~(1 << 2);
              } else
              if((BLive&B00001000) == 1<<3) {
                BLive &= ~(1 << 3);
              } else
              if((BLive&B00010000) == 1<<4) {
                BLive &= ~(1 << 4);
              }
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              bulletsManager.bullets[i].Die();
            }
            LiveS1 = getLiveSecond();
          }
        } else
        if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y)-getSHeight(),getWidth(),getSHeight())) {
          if(bulletsManager.bullets[i].color != 1) {
            if(type == 4 && cooldown < 17/*cooldown < 21*/) {
              cooldown += 2;

              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              Live-=bulletsManager.bullets[i].Damage;
              bulletsManager.bullets[i].Die();

              if(!isOffScreen() && Live > 0) {
                playSFX(2,0);
              } else if(!isOffScreen()) {
                world.Anger+=getAnger();
                world.Anger = constrain(world.Anger, 0, 100);
                playSFX(3,0);
              }
            } else if(type == 4) {
              if(!isOffScreen() && Live > 0) {
                playSFX(2,1);
              }
            } else if(type == 5 && cooldown > 0 && (bulletsManager.bullets[i].vx > 0 == dir < 0)) {
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              Live-=bulletsManager.bullets[i].Damage;
              if(IsGroundedDown) {
                vx += dir*-getSpeed()*0.5F;
              }
              bulletsManager.bullets[i].Die();
              cooldown += 3;

              if(!isOffScreen() && Live > 0) {
                playSFX(2,0);
              } else if(!isOffScreen()) {
                world.Anger+=getAnger();
                world.Anger = constrain(world.Anger, 0, 100);
                playSFX(3,0);
              } 
            } else if(type == 5) {
              if(!isOffScreen() && Live > 0) {
                particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
                bulletsManager.bullets[i].Die();
                playSFX(2,1);
              }
              if(cooldown == 0) {
                LiveS1-=bulletsManager.bullets[i].Damage;
                bulletsManager.bullets[i].Die();
                if(LiveS1 < 0) {
                  playSFX(3,0);
                  LiveS1 = getLiveSecond();
                  cooldown = 180;
                }
              }
            } else if(type == 6) {
              playSFX(2,1);
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
            } else if(type == 10 || type == 11) {
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              Live-=bulletsManager.bullets[i].Damage;
              bulletsManager.bullets[i].Die();
  
              if(!isOffScreen() && Live > 0) {
                playSFX(2,0);
              } else if(!isOffScreen()) {
                playSFX(3,0);
              }
            } else {
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              Live-=bulletsManager.bullets[i].Damage;
              if(random(0,5)==0) {
                vy += 60;
              }
              if(type != 3) {
                if(bulletsManager.bullets[i].vx < 0) {
                  vx += -(15);
                } else {
                  vx += (15);
                }
              } else {
                if(bulletsManager.bullets[i].vx < 0) {
                  vx += -(5);
                } else {
                  vx += (5);
                }
              }
              bulletsManager.bullets[i].Die();

              bobbing = 25;
  
              if(!isOffScreen() && Live > 0) {
                playSFX(2,0);
              } else if(!isOffScreen()) {
                world.Anger+=getAnger();
                world.Anger = constrain(world.Anger, 0, 100);
                playSFX(3,0);
              }
            }
          }
          bulletsManager.bullets[i].Die();
        }
      }
    }

    //Attack and collision of basic type salmonid 0-3
    if(type == 0 || type == 1 || type == 2 || type == 3 || type == 9) {
      if(type != 3) {
        if(cooldown > 0) {
          cooldown--;
        }
      }
      bool PushRange = abs((Div8(player.mainPlayer.x)+player.mainPlayer.getWidth()/2) - (Div8(x)+getWidth()/2)) < (getWidth()/3)+0 && abs((Div8(player.mainPlayer.y)+player.mainPlayer.getHeight()/2) - (Div8(y)+getSHeight()/4)) < getSHeight()/2+1;
      bool AttackRange = abs((Div8(player.mainPlayer.x)+player.mainPlayer.getWidth()/2) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+4 && abs((Div8(player.mainPlayer.y)+player.mainPlayer.getHeight()/2) - (Div8(y)+getSHeight()/4)) < getSHeight()/2+4;
      bool InWater = Div8(y)-(getSHeight()/2) >= world.MapHeight*8-(world.WaterLevel);
      if(type == 3 && AttackRange || cooldown < 20) {
        if(cooldown > 0) {
          cooldown--;
        }
      }
      if(cooldown == 0 && IsGroundedDown && AttackRange && player.mainPlayer.RespawnTimer <= 0 && type != 9) {
        vy += 40;
        cooldown = 21;
        player.mainPlayer.Live -= getAttackDamage()*(1+world.Anger/100.0F*0.3F);
        hitAnim = 3;
        shakeAmplitude = 2;
        shakeTimeLeft = 3;
      }
      if(cooldown == 0 && type == 9) {
        cooldown = 154;
        int8_t bulletID = bulletsManager.spawnBullet(
          x - 8*1, y-getSHeight()*8, //Position x, y
          0,0,      //Velocity vx, vy
          1,1,      //Color, Owner
          5         //Type
        );
        if(bulletID!=-1) {
          bulletsManager.bullets[bulletID].Timer = 0;
          bulletsManager.bullets[bulletID].gravity = 10;
          bulletsManager.bullets[bulletID].Damage = 48;
          bulletsManager.bullets[bulletID].BulletTimeLimit = 120;
        }
      }
      //knockback not working
      if(PushRange) {
        if(type == 3) {
          vx = constrain(-dir*-90,-140,140);
          player.mainPlayer.vx += -dir*120;
        } else {
          vx = constrain(-dir*-60,-140,140);
          player.mainPlayer.vx += -dir*60;
        }
      } else if((vx < 0 && dir == 1) || (vx > 0 && dir == -1) || (abs(vx/VFORCE) <= 1)) {
        if(InWater) {
          vx = dir*-(getSpeed()*2)*(1+world.Anger/8/100);
        } else {
          vx = dir*-getSpeed()*(1+world.Anger/8/100);
        }
      }
      if(InWater) {
        vy += 15;
      }
      
      Object::CleanUpdate(true);
      Draw();
      
      if((IsGroundedLeft || IsGroundedRight) && IsGroundedDown) {
        vy = 90;
      }
      if((IsGroundedLeft || IsGroundedRight) && !IsGroundedDown && vy < 0) {
        vy += 110;
      }
    } else if(type == 5) {
      if(cooldown > 0) {
        cooldown--;
      }
      if(cooldown2 > 0) {
        cooldown2--;
      }
      bool PushRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+1 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()/2+1;
      bool AttackRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+4 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()/2+4;
      if(cooldown > 0) {
        AttackRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+0 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()/2+2;
      }
      bool InWater = Div8(y)-(getSHeight()/2) >= world.MapHeight*8-(world.WaterLevel);
      if(cooldown2 == 0 && IsGroundedDown && AttackRange && player.mainPlayer.RespawnTimer <= 0 && cooldown == 0) {
        cooldown2 = 32;
        player.mainPlayer.Live -= getAttackDamage()*(1+world.Anger/100.0F*0.3F);
        hitAnim = 3;
        shakeAmplitude = 2;
        shakeTimeLeft = 3;
      }
      //knockback not working
      if(cooldown == 0) {
        if(PushRange) {
          vx = constrain(-dir*-getSpeed()*0.5F,-140,140);
          player.mainPlayer.vx += -dir*60;
        } else if((vx < 0 && dir == 1) || (vx > 0 && dir == -1) || (vx/VFORCE == 0)) {
          if(cooldown == 0) {
            if(InWater) {
              vx = dir*-(getSpeed()*2)*(1+world.Anger/8/100);
            } else {
              vx = dir*-getSpeed()*(1+world.Anger/8/100);
            }
          }
        }
      }
      if(InWater) {
        vy += 15;
      }
      
      Object::CleanUpdate(true);
      Draw();
      
      if((IsGroundedLeft || IsGroundedRight) && IsGroundedDown) {
        vy = 80;
      }
      if((IsGroundedLeft || IsGroundedRight) && !IsGroundedDown && vy < 0) {
        vy += 120;
      }
    } else if(type == 4) {
      if(cooldown < 66 && IsGroundedDown) {
        cooldown--;
      } else {
        cooldown = 66;
      }
      bool AttackRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)-4)) < (16/2) && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-20/2)) < 20+4;
      bool PushRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)-4)) < (22/2) && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-24/2)) < 24+4;
      if(AttackRange && cooldown >= 66) {
        cooldown=65;
      }
      bool InWater = Div8(y)-(getSHeight()/2) >= world.MapHeight*8-(world.WaterLevel);
      if(cooldown == 7 && IsGroundedDown && PushRange) {
        player.mainPlayer.vx += abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)-4))*3;
        player.mainPlayer.vy += 50;
      }
      if(cooldown == 7 && IsGroundedDown && AttackRange && player.mainPlayer.RespawnTimer <= 0) {
        player.mainPlayer.Live -= getAttackDamage();
      }
      if(cooldown == 0) {
        cooldown = 66;
      }
      Object::CleanUpdate(true);
      if(cooldown >= 46) {
        if((IsGroundedLeft && -dir) || (IsGroundedRight && dir)) {
          y -= 8;
          vy = 0;
          if(InWater) {
            vx = 100*-dir;
          } else {
            vx = 50*-dir;
          }
        } else if(((IsGroundedLeft && dir) || (IsGroundedRight && -dir)) && !IsGroundedDown) {
          vx = 0;
        } else if(IsGroundedDown) {
          if(InWater) {
            vx = 100*-dir;
          } else {
            vx = 50*-dir;
          }
        } else if(!IsGroundedDown && vy < 0) {
          vx = 0;
        }
      } else {
        vx = 0;
      }
      Draw();
    } else if(type == 6) {
      if(LiveS1 <= 0 && LiveS2 <= 0 && IsGroundedDown) {
        Kill(true);
      }
      if(cooldown > 0) {
        cooldown--;
      }
      if(cooldown == 0) {
        cooldown = 360;
      }
      if(cooldown == 19) {
        //Launch
        if(LiveS1>0) {
          playSFX(5,0);
          int8_t offset = random(-16,17);
          int8_t bulletID = bulletsManager.spawnBullet(
            player.mainPlayer.x + offset*8,8, //Position x, y
            0,0,      //Velocity vx, vy
            1,1,      //Color, Owner
            4         //Type
          );
          if(bulletID!=-1) {
            bulletsManager.bullets[bulletID].Timer = -25;
            bulletsManager.bullets[bulletID].gravity = 55;
            bulletsManager.bullets[bulletID].Damage = 65;
            bulletsManager.bullets[bulletID].BulletTimeLimit = 255;
          }
          particleManager.spawnParticle(Div8(player.mainPlayer.x) + offset,Div8(player.mainPlayer.y)-8,8,0,0);
          offset = random(-16,17);
          bulletID = bulletsManager.spawnBullet(
            player.mainPlayer.x + offset*8,8, //Position x, y
            0,0,      //Velocity vx, vy
            1,1,      //Color, Owner
            4         //Type
          );
          if(bulletID!=-1) {
            bulletsManager.bullets[bulletID].Timer = -50;
            bulletsManager.bullets[bulletID].gravity = 55;
            bulletsManager.bullets[bulletID].Damage = 65;
            bulletsManager.bullets[bulletID].BulletTimeLimit = 255;
          }
          particleManager.spawnParticle(Div8(player.mainPlayer.x) + offset,Div8(player.mainPlayer.y)-8,8,0,0);
        }
        if(LiveS2>0) {
          playSFX(5,0);
          byte offset = random(-16,17);
          int8_t bulletID = bulletsManager.spawnBullet(
            player.mainPlayer.x + offset*8,8, //Position x, y
            0,0,      //Velocity vx, vy
            1,1,      //Color, Owner
            4         //Type
          );
          if(bulletID!=-1) {
            bulletsManager.bullets[bulletID].Timer = -25;
            bulletsManager.bullets[bulletID].gravity = 55;
            bulletsManager.bullets[bulletID].Damage = 65;
            bulletsManager.bullets[bulletID].BulletTimeLimit = 255;
          }
          particleManager.spawnParticle(Div8(player.mainPlayer.x) + offset,Div8(player.mainPlayer.y)-8,8,0,0);
          offset = random(-16,17);
          bulletID = bulletsManager.spawnBullet(
            player.mainPlayer.x + offset*8,8, //Position x, y
            0,0,      //Velocity vx, vy
            1,1,      //Color, Owner
            4         //Type
          );
          if(bulletID!=-1) {
            bulletsManager.bullets[bulletID].Timer = -50;
            bulletsManager.bullets[bulletID].gravity = 55;
            bulletsManager.bullets[bulletID].Damage = 65;
            bulletsManager.bullets[bulletID].BulletTimeLimit = 255;
          }
          particleManager.spawnParticle(Div8(player.mainPlayer.x) + offset,Div8(player.mainPlayer.y)-8,8,0,0);
        }
      }

      if(LiveS1>0 || LiveS2>0) {
        if(abs(Div8(player.mainPlayer.x) - Div8(x)) > 64 || isOffScreenX(0)) {
          if(Div8(player.mainPlayer.x) - Div8(x) > 0) {
            vx += -4;
          } else {
            vx += 4;
          }
        }
        if(abs(Div8(player.mainPlayer.y) - (Div8(y))) > 32 || isOffScreenY(-12)) {
          if(Div8(player.mainPlayer.y) - Div8(y) > 0) {
            vy += -8;
          } else {
            vy += 4;
          }
        } else {
          vy = constrain(vy+1, -127, 8);
        }
      } else {
        vy-=55/VFORCE;
      }
      Object::CleanUpdate(true);
      Draw();
    } else if(type == 8) {
      if(cooldown > 0) {
        cooldown--;
      }
      bool PushRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)-getWidth()/2)) < (getWidth()/2)+1 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()+1;
      bool InWater = Div8(y)-(getSHeight()/2) >= world.MapHeight*8-(world.WaterLevel);
      //knockback not working ?
      if(PushRange) {
        vx = constrain(-dir*-getSpeed()*0.5F,-140,140);
      } else if((vx < 0 && dir == 1) || (vx > 0 && dir == -1) || (vx/VFORCE == 0)) {
        vx = dir*-getSpeed()*(1+world.Anger/8/100);
      }
      if(InWater) {
        vy += 15;
      }
      
      Object::CleanUpdate(true);
      Draw();
      
      if((IsGroundedLeft || IsGroundedRight) && IsGroundedDown) {
        vy = 80;
      }
      if((IsGroundedLeft || IsGroundedRight) && !IsGroundedDown && vy < 0) {
        vy += 120;
      }
    } else if(type == 10 || type == 11) {
      vy = -20;
      Object::Update();
      Draw();
    }


    if((Live < 0 && type != 9) || (BLive == 0 && type == 9)) {
      particleManager.spawnParticle(Div8(x),Div8(y),7,colorGroup,1);
      Kill(true);
    }
  }

  #define HealtbarWidth 10
  #define HealtbarDistance 3

  void Draw () {
    if((!(toScreenX(Div8(x)+ getWidth()) >= 0 && toScreenX(Div8(x)) < LCDWIDTH && toScreenY(Div8(y)) >= 0 && toScreenY(Div8(y)-getSHeight()) < LCDHEIGHT) && type != 8))
      return;

    float Squash = 1.0F + constrain(vy/8, -3, 3)/30.0F+((bob)/55.0F);

    if(type == 0) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-11*Squash), SpoonSalmonid, 11 * -dir, 11*Squash);
    } else if(type == 1) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-16*Squash), NormalSalmonid, 19 * -dir, 16*Squash);
    } else if(type == 3) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-31*Squash), FatSalmonid, 32 * -dir, 31*Squash);
      if(cooldown == 0) {
        FryingPan.setFrame(0);
      } else if(cooldown == 1) {
        FryingPan.setFrame(3);
      } else if(cooldown == 2) {
        FryingPan.setFrame(2);
      } else if(cooldown < 20) {
        FryingPan.setFrame(1);
      } else {
        FryingPan.setFrame(0);
      }
      if(dir == -1) {
        gb.display.drawImage(toScreenX(Div8(x)-6), toScreenY(Div8(y)-31*Squash+18), FryingPan, 18, 9);
      } else {
        gb.display.drawImage(toScreenX(Div8(x)+32+6-18), toScreenY(Div8(y)-31*Squash+18), FryingPan, -18, 9);
      }
    } else if(type == 4) {
      if(cooldown >= 46) {
        gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-4), MawsLight);
      } else
      if(cooldown >= 18) {
        gb.display.drawImage(toScreenX(Div8(x)-5), toScreenY(Div8(y)-14), Maws_Alert);
      } else if(cooldown >= 10) {
        Maws.setFrame(0);
        gb.display.drawImage(toScreenX(Div8(x)-14), toScreenY(Div8(y)-(17-cooldown)*3+(17-cooldown)/2), Maws);
      } else if(cooldown <= 9) {
        Maws.setFrame(1);
        gb.display.drawImage(toScreenX(Div8(x)-14), toScreenY(Div8(y)-((cooldown)*2+(cooldown/10*7))), Maws);
      }
    } else if(type == 5) {
      MechanicSalmon.setFrame((byte)(cooldown == 0));
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-19), MechanicSalmon, 23 * -dir, 19);
    } else if(type == 6) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-11), FlyingSalmon);
      if(LiveS1>0) {
        gb.display.drawImage(toScreenX(Div8(x)+12), toScreenY(Div8(y)-16), FlyingSalmonLaunchers);
        if(cooldown == 24) {
          particleManager.spawnParticle(Div8(x)+14,Div8(y)-8,7,0,0);
        }
        if(cooldown < 39) {
          if(cooldown > 26) {
            gb.display.drawImage(toScreenX(Div8(x)+12), toScreenY(Div8(y)-16), FlyingSalmonCover, 9, cooldown-26);
          } else if(cooldown < 13) {
            gb.display.drawImage(toScreenX(Div8(x)+12), toScreenY(Div8(y)-16), FlyingSalmonCover, 9, 13-cooldown);
          }
        } else {
          gb.display.drawImage(toScreenX(Div8(x)+12), toScreenY(Div8(y)-16), FlyingSalmonCover, 9, 13);
        }
      }
      if(LiveS2>0) {
        gb.display.drawImage(toScreenX(Div8(x)-9), toScreenY(Div8(y)-16), FlyingSalmonLaunchers);
        if(cooldown == 24) {
          particleManager.spawnParticle(Div8(x)-11,Div8(y)-8,7,0,0);
        }
        if(cooldown < 39) {
          if(cooldown > 26) {
            gb.display.drawImage(toScreenX(Div8(x)-9), toScreenY(Div8(y)-16), FlyingSalmonCover, 9, cooldown-26);
          } else if(cooldown < 13) {
            gb.display.drawImage(toScreenX(Div8(x)-9), toScreenY(Div8(y)-16), FlyingSalmonCover, 9, 13-cooldown);
          }
        } else {
          gb.display.drawImage(toScreenX(Div8(x)-9), toScreenY(Div8(y)-16), FlyingSalmonCover, 9, 13);
        }
      }
    } else if(type == 7) {
      
    } else if(type == 8) {
      if(-dir == 1) {
        if(!isOffScreenW(toScreenX(Div8(x)+0-16-3), toScreenY(Div8(y)-51-3), 19, 17)) {
          gb.display.drawImage(toScreenX(Div8(x)+0-16-3), toScreenY(Div8(y)-51-3), SteelHead_0, 19, 17);
        }
        if(!isOffScreenW(toScreenX(Div8(x)+16-16), toScreenY(Div8(y)-51), 20, 14)) {
          gb.display.drawImage(toScreenX(Div8(x)+16-16), toScreenY(Div8(y)-51), SteelHead_01, 20, 14);
        }
        if(!isOffScreenW(toScreenX(Div8(x)+0-16), toScreenY(Div8(y)-37), 16, 17)) {
          gb.display.drawImage(toScreenX(Div8(x)+0-16), toScreenY(Div8(y)-37), SteelHead_10, 16, 17);
        }
        if(!isOffScreenW(toScreenX(Div8(x)+16-16), toScreenY(Div8(y)-37), 21, 17)) {
          gb.display.drawImage(toScreenX(Div8(x)+16-16), toScreenY(Div8(y)-37), SteelHead_11, 21, 17);
        }
        if(!isOffScreenW(toScreenX(Div8(x)+4-16), toScreenY(Div8(y)-20), 12, 20)) {
          gb.display.drawImage(toScreenX(Div8(x)+4-16), toScreenY(Div8(y)-20), SteelHead_02, 12, 20);
        }
        if(!isOffScreenW(toScreenX(Div8(x)+0), toScreenY(Div8(y)-20), 23, 20)) {
          gb.display.drawImage(toScreenX(Div8(x)+0), toScreenY(Div8(y)-20), SteelHead_12, 23, 20);
        }
      } else {
        if(!isOffScreenW(toScreenX(Div8(x)), toScreenY(Div8(y)-51-3), 19, 17)) {
          gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-51-3), SteelHead_0, -19, 17);
        }
        if(!isOffScreenW(toScreenX(Div8(x)-20), toScreenY(Div8(y)-51), 20, 14)) {
          gb.display.drawImage(toScreenX(Div8(x)-20), toScreenY(Div8(y)-51), SteelHead_01, -20, 14);
        }
        if(!isOffScreenW(toScreenX(Div8(x)), toScreenY(Div8(y)-37), 16, 17)) {
          gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-37), SteelHead_10, -16, 17);
        }
        if(!isOffScreenW(toScreenX(Div8(x)-21), toScreenY(Div8(y)-37), 21, 17)) {
          gb.display.drawImage(toScreenX(Div8(x)-21), toScreenY(Div8(y)-37), SteelHead_11, -21, 17);
        }
        if(!isOffScreenW(toScreenX(Div8(x)), toScreenY(Div8(y)-20), 12, 20)) {
          gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-20), SteelHead_02, -12, 20);
        }
        if(!isOffScreenW(toScreenX(Div8(x)-23), toScreenY(Div8(y)-20), 23, 20)) {
          gb.display.drawImage(toScreenX(Div8(x)-23), toScreenY(Div8(y)-20), SteelHead_12, -23, 20);
        }
      }
    } else if(type == 9) {
      byte h = 0;
      h+=6;
      gb.display.drawImage(toScreenX(Div8(x)+4), toScreenY(Div8(y)-h), TowerSalmon_Base);
      if((BLive&B00000001) == 1<<0) {
        h+=12;
        h+=(5*Squash)-5;
        gb.display.drawImage(toScreenX(Div8(x)+0), toScreenY(Div8(y)-h), TowerSalmon_4);
      }
      if((BLive&B00000010) == 1<<1) {
        h+=14;
        h+=(5*Squash)-5;
        gb.display.drawImage(toScreenX(Div8(x)+1), toScreenY(Div8(y)-h), TowerSalmon_3);
      }
      if((BLive&B00000100) == 1<<2) {
        h+=9;
        h+=(5*Squash)-5;
        gb.display.drawImage(toScreenX(Div8(x)+1), toScreenY(Div8(y)-h), TowerSalmon_2);
      }
      if((BLive&B00001000) == 1<<3) {
        h+=12;
        h+=(5*Squash)-5;
        gb.display.drawImage(toScreenX(Div8(x)+3), toScreenY(Div8(y)-h), TowerSalmon_1);
      }
      if((BLive&B00010000) == 1<<4) {
        h+=9;
        h+=(5*Squash)-5;
        gb.display.drawImage(toScreenX(Div8(x)+2), toScreenY(Div8(y)-h), TowerSalmon_0);
      }
      h+=17;
      gb.display.drawImage(toScreenX(Div8(x)+2), toScreenY(Div8(y)-h), TowerSalmon_Head, -19 * dir, 17);
    } else if(type == 10) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-9), EggDropCrate);
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-21), EggDropBalloon);
    } else if(type == 11) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-9), GoldenEggDropCrate);
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-21), EggDropBalloon);
    }
  }

  void OverDraw () {
    if(!(toScreenX(Div8(x)+ getWidth()) >= 0 && toScreenX(Div8(x)) < LCDWIDTH && toScreenY(Div8(y)) >= 0 && toScreenY(Div8(y)-getSHeight()) < LCDHEIGHT))
      return;

    float Squash = 1.0F + constrain(vy/8, -3, 3)/30.0F;

    if(type == 0) {
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-11*Squash-HealtbarDistance), 11);
    } else if(type == 1) {
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-16*Squash-HealtbarDistance), 19);
    } else if(type == 3) {
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-31*Squash-HealtbarDistance), 32);
    } else if(type == 4 && cooldown < 19) {
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-28-HealtbarDistance), 4);
    } else if(type == 5) {
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-19*Squash-HealtbarDistance), 19);
    } else if(type == 8) {
      DrawHealtBarAt(toScreenX(Div8(x)-16), toScreenY(Div8(y)-51*Squash-HealtbarDistance), 51);
    }
  }

  void DrawHealtBarAt (int8_t x, int8_t y, byte salmonWidth) {
    gb.display.setColor((ColorIndex)7);
    gb.display.fillRect(x+((salmonWidth-HealtbarWidth)/2), y, HealtbarWidth, 2);
    gb.display.setColor((ColorIndex)9);
    gb.display.fillRect(x+((salmonWidth-HealtbarWidth)/2), y, HealtbarWidth*Live/getLive(), 2);
  }

  void Kill (bool DropEgg) {
    if(TutorialMode && type == 1) {
      AnimationTimer6 = 5;
      AnimationTimer7 = 0;
    }
    if(TutorialMode && type == 4) {
      AnimationTimer6 = 5;
      AnimationTimer7 = 6;
    }
    
    activeSalmons--;
    alive = false;
    int8_t eggDir = 1;
    if(x < player.mainPlayer.x) {
      eggDir = -1;
    } else {
      eggDir = 1;
    }
    
    if(DropEgg) {
      world.WaterLevel -= 3;
      
      for(byte n = 0; n < getPowerEggDrop(); n++) {
        eggManager.Spawn(Div8(x) + (getWidth()/2) + random(-4,5), Div8(y)-(getSHeight()/2) + random(-4,5), 0, eggDir);
      }
      for(byte n = 0; n < getGoldenEggDrop(); n++) {
        eggManager.Spawn(Div8(x) + (getWidth()/2) + random(-4,5), Div8(y)-(getSHeight()/2) + random(-4,5), 1, eggDir);
      }
    }
  }
};

#define SalmonidCount 4
class SalmonidManager {
  public:
  Salmonid salmonids[SalmonidCount];
  uint16_t cooldown = 180;
  byte zonePositionA = 0;
  byte zonePositionB = 0;

  void UpdateGlobal () {
    for(byte i = 0; i < SalmonidCount; i++) {
      if(salmonids[i].alive && salmonids[i].hasSpawned) {
        salmonids[i].Update();
      }
    }
    byte r1 = random(0,190);
    byte r2 = random(0,190);
    if((cooldown == 0 || (activeSalmons <= 1 && cooldown < 50)) && ((abs(Div64(player.mainPlayer.x) - zonePositionA) < 11) || r1==1)) {
      //zonePosition
      if(Div8(world.WaterLevel) < 5) {
        for(int8_t x = 0; x < world.MapWidth/2; x++) {
          if(TilesParams_Array[TileParamsCount*world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) + 1] == 0) {
            zonePositionA = x;
          } else {
            break;
          }
        }
      }
      
      cooldown = ((6-difficulty)*15)+85 + random(0, 20);
      SpawnBatchAt(0, 1);
    } else if((cooldown == 0 || (activeSalmons == 1 && cooldown < 50)) && ((abs(Div64(player.mainPlayer.x) - zonePositionB) < 11) || r2==1)) {
      //zonePosition
      if(Div8(world.WaterLevel) < 5) {
        for(int8_t x = world.MapWidth-1; x >= world.MapWidth/2; x--) {
          if(TilesParams_Array[TileParamsCount*world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) + 0] == 0) {
            zonePositionB = x;
          } else {
            break;
          }
        }
      }
      
      cooldown = ((6-difficulty)*15)+85 + random(0, 20);
      SpawnBatchAt(world.MapWidth-1, -1);
    } else {
      if(cooldown == 0) {
        cooldown = ((6-difficulty)*15)+85 + random(0, 20);

        if(Div8(world.WaterLevel) < 5) {
          for(int8_t x = 0; x < world.MapWidth/2; x++) {
            if(TilesParams_Array[TileParamsCount*world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) + 0] == 0) {
              zonePositionA = x;
            } else {
              break;
            }
          }
        }

        if(Div8(world.WaterLevel) < 5) {
          for(int8_t x = world.MapWidth-1; x >= world.MapWidth/2; x--) {
            if(TilesParams_Array[TileParamsCount*world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) + 0] == 0) {
              zonePositionB = x;
            } else {
              break;
            }
          }
        }
      }
      cooldown--;
    }
  }

  void SpawnBatchAt(byte zonePosition, int8_t dir) {
    if(TutorialMode) {
      return;
    }
    bool Boss = false;
    for(byte i = 0; i < SalmonidCount; i++) {
      if(salmonids[i].alive && (salmonids[i].type == 3 ||  salmonids[i].type == 4 || salmonids[i].type == 5 || salmonids[i].type == 6 || salmonids[i].type == 8 || salmonids[i].type == 9)) {
        Boss = true;
        break;
      }
    }

    //Need help? Here's a drop. Power drop needs high sea level (2/3 <) and < 3/4 eggs max
    //Golden drop needs high sea level (1/2 <) and < 3/4 eggs max
    //Power priority, low chance of happening
    if(random(0,5) == 0) {
      if(world.MaxEggs*3/4 > world.FlagEggs && (25*8)*2/3 < world.WaterLevel) {
        //Spawn a drop near player
        Spawn(Div8(player.mainPlayer.x)+random(-8,9), 4, 10, 1);
        return;
      } else if(world.MaxGolden*3/4 > world.FlagGolden && (25*8)*1/2 < world.WaterLevel) {
        //Spawn a drop near player
        Spawn(Div8(player.mainPlayer.x)+random(-8,9), 4, 11, 1);
        return;
      }
    }

    //

    if(!Boss && random(0,11) != 0) {
      byte salmon = 0;
      if(world.MaxEggs*5/8 > world.FlagEggs && random(0,3) != 0 && (world.FlagEggs < world.MaxEggs)) {
        Spawn(zonePosition*8-8, world.MapHeight*8-16, 3, dir);
      } else {
        if(difficulty == 5 || difficulty == 6) {
          salmon = random(4,8);
          if(salmon == 7) {
            salmon = 9;
          }
          Spawn(zonePosition*8-8, world.MapHeight*8-16, salmon, dir);
        } else
        if(difficulty == 3 || difficulty == 4) {
          salmon = random(4,7);
          Spawn(zonePosition*8-8, world.MapHeight*8-16, salmon, dir);
        } else
        if(difficulty == 1 || difficulty == 2) {
          salmon = random(4,6);
          Spawn(zonePosition*8-8, world.MapHeight*8-16, salmon, dir);
        } else 
        if(difficulty == 0) {
          salmon = random(3,6);
          if(salmon == 4) {
            salmon = 5;
          }
          Spawn(zonePosition*8-8, world.MapHeight*8-16, salmon, dir);
        }
      }
    } else {
      if(random(0,2)==0) {
        Spawn(zonePosition*8-8, world.MapHeight*8-16, 0, dir);
        Spawn(zonePosition*8-8, world.MapHeight*8-16, 1, dir);
      } else {
        Spawn(zonePosition*8-8, world.MapHeight*8-16, random(0,2), dir);
      }
    }
  }

  void UnSpawnedGlobalUpdate () {
    for(byte i = 0; i < SalmonidCount; i++) {
      if(salmonids[i].alive && !salmonids[i].hasSpawned) {
        salmonids[i].UnSpawnedUpdate();
      }
      if(salmonids[i].alive) {
        salmonids[i].OverDraw();
      }
    }
  }

  void Spawn (int16_t x, int16_t y, byte type, int8_t dir) {
    for(byte i = 0; i < SalmonidCount; i++) {
      if(!salmonids[i].alive) {
        activeSalmons++;
        salmonids[i].x = Mul8(x);
        salmonids[i].y = Mul8(y);
        salmonids[i].type = type;
        salmonids[i].alive = true;
        salmonids[i].Live = salmonids[i].getLive();
        salmonids[i].BLive = salmonids[i].getBLive();
        salmonids[i].LiveS1 = salmonids[i].getLiveSecond();
        salmonids[i].LiveS2 = salmonids[i].getLiveSecond();
        salmonids[i].cooldown = 0;
        salmonids[i].cooldown2 = 0;
        salmonids[i].dir = dir;
        salmonids[i].hasSpawned = false;
        salmonids[i].bobbing = 0;
        salmonids[i].bob = 0;
        return;
      }
    }
  }

  void Clean () {
    for(byte i = 0; i < SalmonidCount; i++) {
      salmonids[i].alive = false;
    }
    activeSalmons = 0;
  }
};

SalmonidManager salmonidManager;
