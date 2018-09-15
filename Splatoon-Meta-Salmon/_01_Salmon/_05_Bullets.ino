int16_t inkX = 0;
int16_t inkY = 0;

class Bullets:
  public Object {
  public:
    int16_t Timer = 0;
    bool IsDead = true;
    byte color = 0;
    byte Owner = 0;
    byte Damage = 7;
    byte Type = 0;
    uint16_t BulletTimeLimit = 300;
    int16_t gravity = 0;
    virtual int16_t getWidth() {
      return 1;
    };
    virtual int16_t getHeight() {
      return 1;
    };
    virtual int16_t getGravity() {
      return gravity;
    };
    virtual int16_t getXFriction() {
      return 0;
    };
    virtual int16_t getYFriction() {
      return 0;
    };
    virtual int16_t getXBounce() {
      return 90;
    };
    virtual int16_t getYBounce() {
      return 10;
    };
    virtual byte getCollisionQuality() {
      return 1;
    };

    void Draw () {
      if(isOffScreen())
        return;

      if(color == 0) {
        gb.display.setColor((ColorIndex)7);
      } else {
        gb.display.setColor((ColorIndex)9);
      }

      if(Timer != 0) {
        setPaletteToColorGroup(color);
        int8_t bulldir = (vx<0)*2-1;
        if(Type == 0) { //Squish
          BulletSquish.setFrame(constrain(Timer/(BulletTimeLimit/4),0,3));
          gb.display.drawImage(toScreenX(Div8(x)-3), toScreenY(Div8(y)-3), BulletSquish, 7*bulldir, 7);
        } else if(Type == 1) { //Flat
          BulletLong.setFrame(constrain(Timer/(BulletTimeLimit/4),0,3));
          gb.display.drawImage(toScreenX(Div8(x)-3), toScreenY(Div8(y)-3), BulletLong, 7*bulldir, 7);
        } if(Type == 2) { //Falling
          FallingBullet.setFrame(constrain(Timer/(BulletTimeLimit/4),0,3));
          gb.display.drawImage(toScreenX(Div8(x)-3), toScreenY(Div8(y)-3), FallingBullet, 7*bulldir, 7);
        } else if(Type == 3) { //Spray
          SprayBullet.setFrame(constrain(Timer/(BulletTimeLimit/4),0,3));
          gb.display.drawImage(toScreenX(Div8(x)-3), toScreenY(Div8(y)-3), SprayBullet, 7*bulldir, 7);
        } else if(Type == 4) { //Ennemy Missile (Green only)
          gb.display.colorIndex = palette;
          gb.display.drawImage(toScreenX(Div8(x)-2), toScreenY(Div8(y)-12), Missile);
        } else if(Type == 5) { //Ennemy Track Missile (Green only)
          gb.display.colorIndex = palette;
          gb.display.colorIndex = palette;
          TrackMissile.setFrame((Timer/3)%2);
          gb.display.drawImage(toScreenX(Div8(x)-2), toScreenY(Div8(y)-2), TrackMissile);
        }
        gb.display.colorIndex = palette;
      }
    }

    void Die () {
      IsDead = true;
      Timer = 0;
    }
    
    void Recreate () {
      IsDead = false;
      collided = false;
    }

    void Update () {
      if(Timer < 0) {
        Timer++;
        return;
      }
      if(Timer < BulletTimeLimit && !IsDead && (Div8(y) < world.MapHeight*8-(world.WaterLevel))) {
        Timer++;
      } else {
        if((Div8(y) >= world.MapHeight*8-(world.WaterLevel)) && !IsDead) {
          particleManager.spawnParticle(Div8(x),Div8(y)-1,1,colorGroup,color);
        }
        Die();
      }
      
      if(collided && !IsDead) {
        if(!isOffScreen())
          playSFX(1,1);
          
        if(IsGroundedDown && Type != 4) {
          particleManager.spawnParticle(Div8(x),Div8(y)-1,2,colorGroup,color);
        }
        if(Type == 4) {
          particleManager.spawnParticle(Div8(x),Div8(y)-1,3,colorGroup,color);
          particleManager.spawnParticle(Div8(x),Div8(y)-1,7,colorGroup,color);
        }
        
        inkX = Div8(x);//+(vx>=0?1:-1)
        inkY = Div8(y);//+(vy>=0?1:-1)
        
        inkX = constrain(inkX,0,(world.MapWidth-1)*8);
        inkY = constrain(inkY,0,(world.MapHeight-1)*8);
        
        for(int8_t xv = -1; xv < 2; xv++) {
          for(int8_t yv = -1; yv < 2; yv++) {
            if(((Div8(inkX+xv*4) == constrain(Div8(inkX+xv*4),0,world.MapWidth-1))&&(Div8(inkY+yv*4) == constrain(Div8(inkY+yv*4),0,world.MapWidth-1))) && TilesParams_Array[world.GetTile(Div8(inkX+xv*4),Div8(inkY+yv*4))*TileParamsCount+0] > 0) {
              
              V0 = world.SMGetPaintValueAt(Div8(inkX+xv*6),Div8(inkY+yv*6),0);
              V1 = world.SMGetPaintValueAt(Div8(inkX+xv*6),Div8(inkY+yv*6),1);
              V2 = world.SMGetPaintValueAt(Div8(inkX+xv*6),Div8(inkY+yv*6),2);
              V3 = world.SMGetPaintValueAt(Div8(inkX+xv*6),Div8(inkY+yv*6),3);
              
              if(xv==-1 && yv==0) {
                if(world.GetTile(Div8(inkX-6),Div8(inkY+0)) != 0) {
                  V1 = 1;
                  //AddPointToPlayer(Owner,V1-world.SMGetPaintValueAt(Div8(inkX-4),Div8(inkY),1));
                }
              }
              if(xv==1 && yv==0) {
                if(world.GetTile(Div8(inkX+6),Div8(inkY+0)) != 0) {
                  V3 = 1;
                  //AddPointToPlayer(Owner,V3-world.SMGetPaintValueAt(Div8(inkX+4),Div8(inkY),3));
                }
              }
              if(yv==1 && xv==0) {
                if(world.GetTile(Div8(inkX+0),Div8(inkY+6)) != 0) {
                  V0 = 1;
                  //AddPointToPlayer(Owner,V0-world.SMGetPaintValueAt(Div8(inkX),Div8(inkY+4),0));
                }
              }
              if(yv==-1 && xv==0) {
                if(world.GetTile((inkX+0)/8,(inkY-6)/8) != 0) {
                  V2 = 1;
                  //AddPointToPlayer(Owner,V2-world.SMGetPaintValueAt(Div8(inkX),Div8(inkY-4),2));
                }
              }

              world.SMSetPaintValue(
                V0,
                V1,
                V2,
                V3,
                constrain(Div8(inkX+xv*6),0,world.MapWidth-1),
                constrain(Div8(inkY+yv*6),0,world.MapHeight-1),
                color
              );
            }
          }
        }
        
        Die();
      }

      if(!IsDead) {
        if(Type == 5) {
          Object::HyperCleanUpdate(true);
        } else {
          Object::Update();
        }

        Draw();
      }
    }
};

class BulletsManager {
  public:
  Bullets bullets[BCOUNT];

  int8_t spawnBullet (int16_t x, int16_t y, int16_t vx, int16_t vy, byte color, byte owner, byte type) {
    for(byte i = 0; i < BCOUNT; i++) {
      if(bullets[i].IsDead) {
        bullets[i].Recreate();
        bullets[i].x = x;
        bullets[i].y = y;
        bullets[i].vx = vx;
        bullets[i].vy = vy;
        bullets[i].color = color;
        bullets[i].Owner = owner;
        bullets[i].Type = type;
        return i;
      }
    }
    return -1;
  }

  void Reset () {
    for(byte i = 0; i < BCOUNT; i++) {
      bullets[i].IsDead = true;
    }
  }

  void Update () {
    for(byte i = 0; i < BCOUNT; i++) {
      bullets[i].Update();
    }
  }
};

BulletsManager bulletsManager;
