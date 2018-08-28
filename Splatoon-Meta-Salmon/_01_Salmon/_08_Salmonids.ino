byte activeSalmons = 0;

class Salmonid:
  public Object {
  public:
  byte type = 0;
  int16_t Live = 0;
  int16_t LiveS1 = 0;
  int16_t LiveS2 = 0;
  byte cooldown = 0;
  byte cooldown2 = 0;
  byte turningCooldown = 0;
  int8_t dir = 1;
  bool alive = false;
  bool hasSpawned = false;
  
  //0: Spoon buddy
  //1: Normal
  //2: Normal 2.0
  //3: Fat ones
  //4: Boss Maws dude
  //5: Mechanic asshole
  //6: Flying asshole
  //7: Robot guy
  //8: The one with a bomb on his head
  
  virtual int16_t getYBounce() {
    return 40;
  };

  virtual int16_t getGravity() {
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
    }
  };

  int16_t getLive () {
    if(type == 0) {
      return 60;
    } else if(type == 1) {
      return 140;
    } else if(type == 2) {
      return 140;
    } else if(type == 3) {
      return 310;
    } else if(type == 4) {
      return 420;
    } else if(type == 5) {
      return 180;
    } else if(type == 6) {
      return 1;
    }
  }

  int16_t getLiveSecond () {
    if(type == 5) {
      return 280;
    } else if(type == 6) {
      return 90;
    } else {
      return 0;
    }
  }

  byte getAttackDamage () {
    if(type == 0) {
      return 27;
    } else if(type == 1) {
      return 37;
    } else if(type == 2) {
      return 37;
    } else if(type == 3) {
      return 49;
    } else if(type == 4) {
      return 101;
    } else if(type == 5) {
      return 42;
    } else if(type == 6) {
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
    }
  }

  byte getSpeed () {
    if(type == 0) {
      return 80;
    } else if(type == 1) {
      return 73;
    } else if(type == 2) {
      return 73;
    } else if(type == 3) {
      return 55;
    } else if(type == 5) {
      return 42;
    }
  }

  int8_t getAnger () {
    if(type == 0) {
      return 2;
    } else if(type == 1) {
      return 4;
    } else if(type == 2) {
      return 4;
    } else if(type == 3) {
      return 8;
    } else if(type == 4) {
      return -18;
    } else if(type == 4) {
      return -22;
    } 
  }

  void UnSpawnedUpdate () {
    if(!hasSpawned) {
      Object::UpdateGrounding(0);
    }
    if(Div8(y)<(world.MapHeight*8-10*8) && !(IsGroundedUp||IsGroundedDown||IsGroundedLeft||IsGroundedUp)) {
      hasSpawned = true;
    }
    if(type == 4) {
      y -= 3*8;
      return;
    }
    if((Div8(y)>(world.MapHeight*8-10*8) || (IsGroundedUp||IsGroundedDown||IsGroundedLeft||IsGroundedUp)) && !hasSpawned) {
      y -= 3*8;
      Draw();
      return;
    }
  }

  void Update () {
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

    if(turningCooldown%3 == 0 && type != 4) {
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
        if(gb.collidePointRect(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),Div8(x),Div8(y)-getSHeight(),getWidth(),getSHeight())) {
          if(bulletsManager.bullets[i].color != 1) {
            if(type == 4 && cooldown < 19) {
              cooldown += 3;

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
                  cooldown = 90;
                }
              }
            } else if(type == 6) {
              playSFX(2,1);
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
            } else {
              particleManager.spawnParticle(Div8(bulletsManager.bullets[i].x),Div8(bulletsManager.bullets[i].y),0,colorGroup,bulletsManager.bullets[i].color);
              Live-=bulletsManager.bullets[i].Damage;
              if(IsGroundedDown) {
                vx += -dir*-getSpeed()*0.4F;
              }
              bulletsManager.bullets[i].Die();
  
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
    if(type == 0 || type == 1 || type == 2 || type == 3) {
      if(cooldown > 0) {
        cooldown--;
      }
      bool PushRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+1 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()+1;
      bool AttackRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+4 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()+4;
      bool InWater = Div8(y)-(getSHeight()/2) >= world.MapHeight*8-(world.WaterLevel);
      if(cooldown == 0 && IsGroundedDown && AttackRange && player.mainPlayer.RespawnTimer <= 0) {
        vy += 60;
        cooldown = 12;
        player.mainPlayer.Live -= getAttackDamage()*(1+world.Anger/100.0F*0.3F);
      }
      //knockback not working
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
    } else if(type == 5) {
      if(cooldown > 0) {
        cooldown--;
      }
      if(cooldown2 > 0) {
        cooldown2--;
      }
      bool PushRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+1 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()+1;
      bool AttackRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+4 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()+4;
      if(cooldown > 0) {
        AttackRange = abs((Div8(player.mainPlayer.x)/*+player.mainPlayer.getWidth()/2*/) - (Div8(x)+getWidth()/2)) < (getWidth()/2)+0 && abs((Div8(player.mainPlayer.y)/*-player.mainPlayer.getHeight()*/) - (Div8(y)-getSHeight()/2)) < getSHeight()+0;
      }
      bool InWater = Div8(y)-(getSHeight()/2) >= world.MapHeight*8-(world.WaterLevel);
      if(cooldown2 == 0 && IsGroundedDown && AttackRange && player.mainPlayer.RespawnTimer <= 0) {
        cooldown2 = 32;
        player.mainPlayer.Live -= getAttackDamage()*(1+world.Anger/100.0F*0.3F);
      }
      //knockback not working
      if(PushRange) {
        vx = constrain(-dir*-getSpeed()*0.5F,-140,140);
      } else if((vx < 0 && dir == 1) || (vx > 0 && dir == -1) || (vx/VFORCE == 0)) {
        if(cooldown == 0) {
          vx = dir*-getSpeed()*(1+world.Anger/8/100);
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
      if(cooldown >= 26) {
        if((IsGroundedLeft && -dir) || (IsGroundedRight && dir)) {
          y -= 8;
          vy = 0;
          vx = 50*-dir;
        } else if(((IsGroundedLeft && dir) || (IsGroundedRight && -dir)) && !IsGroundedDown) {
          vx = 0;
        } else if(IsGroundedDown) {
          vx = 50*-dir;
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
    }


    if(Live < 0) {
      particleManager.spawnParticle(Div8(x),Div8(y),7,colorGroup,1);
      Kill(true);
    }
  }

  #define HealtbarWidth 10
  #define HealtbarDistance 3

  void Draw () {
    if(!(toScreenX(Div8(x)+ getWidth()) >= 0 && toScreenX(Div8(x)) < LCDWIDTH && toScreenY(Div8(y)) >= 0 && toScreenY(Div8(y)-getSHeight()) < LCDHEIGHT))
      return;

    float Squash = 1.0F + constrain(vy/8, -3, 3)/30.0F;

    if(type == 0) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-11*Squash), SpoonSalmonid, 11 * -dir, 11*Squash);
    } else if(type == 1) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-16*Squash), NormalSalmonid, 19 * -dir, 16*Squash);
    } else if(type == 3) {
      gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-31*Squash), FatSalmonid, 32 * -dir, 31*Squash);
    } else if(type == 4) {
      if(cooldown >= 18) {
        gb.display.drawImage(toScreenX(Div8(x)), toScreenY(Div8(y)-4), MawsLight);
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
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-23-HealtbarDistance), 23);
    } else if(type == 5) {
      DrawHealtBarAt(toScreenX(Div8(x)), toScreenY(Div8(y)-19*Squash-HealtbarDistance), 19);
    }
  }

  void DrawHealtBarAt (int8_t x, int8_t y, byte salmonWidth) {
    gb.display.setColor((ColorIndex)7);
    gb.display.fillRect(x+((salmonWidth-HealtbarWidth)/2), y, HealtbarWidth, 2);
    gb.display.setColor((ColorIndex)9);
    gb.display.fillRect(x+((salmonWidth-HealtbarWidth)/2), y, HealtbarWidth*Live/getLive(), 2);
  }

  void Kill (bool DropEgg) {
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

#define SalmonidCount 5
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
    if((cooldown == 0 || activeSalmons == 0) && abs(Div64(player.mainPlayer.x) - zonePositionA) < 12) {
      //zonePosition
      zonePositionA = 2;
      if(Div8(world.MapHeight*8-world.WaterLevel) > world.MapHeight-5) {
        for(int8_t x = world.MapWidth/2-1; x >= 0; x--) {
          if(world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) == 0) {
            zonePositionA = x;
            break;
          }
        }
      }
      
      cooldown = 170 + random(0, 220);
      SpawnBatchAt(zonePositionA);
    } else if((cooldown == 0 || activeSalmons == 0) && abs(Div64(player.mainPlayer.x) - zonePositionB) < 12) {
      //zonePosition
      zonePositionB = world.MapWidth-2;
      if(Div8(world.MapHeight*8-world.WaterLevel) > world.MapHeight-5) {
        for(int8_t x = world.MapWidth/2; x < world.MapWidth; x++) {
          if(world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) == 0) {
            zonePositionB = x;
            break;
          }
        }
      }
      
      cooldown = 170 + random(0, 220);
      SpawnBatchAt(zonePositionB);
    } else {
      if(cooldown == 0) {
        cooldown = 170 + random(0, 220);

        zonePositionA = 2;
        if(Div8(world.MapHeight*8-world.WaterLevel) > world.MapHeight-5) {
          for(int8_t x = world.MapWidth/2-1; x >= 0; x--) {
            if(world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) == 0) {
              zonePositionA = x;
              break;
            }
          }
        }

        zonePositionB = world.MapWidth-2;
        if(Div8(world.MapHeight*8-world.WaterLevel) > world.MapHeight-5) {
          for(int8_t x = world.MapWidth/2; x < world.MapWidth; x++) {
            if(world.GetTile(x,Div8(world.MapHeight*8-world.WaterLevel)) == 0) {
              zonePositionB = x;
              break;
            }
          }
        }
      }
      cooldown--;
    }
  }

  void SpawnBatchAt(byte zonePosition) {
    byte r = 0;//random(0,3);
      if(r==0) {
        bool AlreadyFatGuys = false;
        for(byte i = 0; i < SalmonidCount; i++) {
          if(salmonids[i].alive && salmonids[i].type == /*3*/6) {
            AlreadyFatGuys = true;
            break;
          }
        }
        if(!AlreadyFatGuys) {
          Spawn(zonePosition*8-8, world.MapHeight*8-16, /*3*/6);
        } else {
          //Spawn(zonePosition*8-8, world.MapHeight*8-16, 1);
        }
      } if(r==1) {
        Spawn(zonePosition*8-8, world.MapHeight*8-16, 0);
        Spawn(zonePosition*8-8, world.MapHeight*8-16, 1);
      } else {
        Spawn(zonePosition*8-8, world.MapHeight*8-16, random(0,2));
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

  void Spawn (int16_t x, int16_t y, byte type) {
    for(byte i = 0; i < SalmonidCount; i++) {
      if(!salmonids[i].alive) {
        activeSalmons++;
        salmonids[i].x = Mul8(x);
        salmonids[i].y = Mul8(y);
        salmonids[i].type = type;
        salmonids[i].alive = true;
        salmonids[i].Live = salmonids[i].getLive();
        salmonids[i].LiveS1 = salmonids[i].getLiveSecond();
        salmonids[i].LiveS2 = salmonids[i].getLiveSecond();
        salmonids[i].cooldown = 0;
        salmonids[i].cooldown2 = 0;
        salmonids[i].dir = 1;
        salmonids[i].hasSpawned = false;
        return;
      }
    }
  }
};

SalmonidManager salmonidManager;
