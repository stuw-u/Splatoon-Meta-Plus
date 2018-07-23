int16_t inkX = 0;
int16_t inkY = 0;

class Bullets:
  public Object {
  public:
    uint16_t Timer = 0;
    bool IsDead = true;
    byte color = 0;
    byte Owner = 0;
    byte Damage = 7;
    byte Type = 0;
    uint16_t BulletTimeLimit = 300;
    int16_t gravity = 0;
    virtual int16_t getWidth() {
      return 3;
    };
    virtual int16_t getHeight() {
      return 3;
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
        return; //skip boxes which are out of the screen

      switch(colorGroup) {
        case 0:
        if(color == 0) {
          gb.display.setColor((ColorIndex)8);
        } else {
          gb.display.setColor((ColorIndex)11);
        }
        break;
        case 1:
        if(color == 0) {
          gb.display.setColor((ColorIndex)5);
        } else {
          gb.display.setColor((ColorIndex)9);
        }
        break;
        case 2:
        if(color == 0) {
          gb.display.setColor((ColorIndex)7);
        } else {
          gb.display.setColor((ColorIndex)11);
        }
        break;
        case 3:
        if(color == 0) {
          gb.display.setColor((ColorIndex)5);
        } else {
          gb.display.setColor((ColorIndex)11);
        }
        break;
        case 4:
        if(color == 0) {
          gb.display.setColor((ColorIndex)6);
        } else {
          gb.display.setColor((ColorIndex)11);
        }
        break;
        case 5:
        if(color == 0) {
          gb.display.setColor((ColorIndex)5);
        } else {
          gb.display.setColor((ColorIndex)7);
        }
        break;
        case 6:
        if(color == 0) {
          gb.display.setColor((ColorIndex)9);
        } else {
          gb.display.setColor((ColorIndex)11);
        }
        break;
      }

      if(Timer != 0) {
        setPaletteToColorGroup(color,colorGroup);
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
      if(Timer < BulletTimeLimit && !IsDead) {
        Timer++;
      } else {
        Die();
      }
      
      if(collided && !IsDead) {
        if(!isOffScreen())
          playSFX(1,1);
          
        if(IsGroundedDown) {
          particleManager.spawnParticle(Div8(x),Div8(y)-1,2,colorGroup,color);
        }
        
        inkX = Div8(x);//+(vx>=0?1:-1)
        inkY = Div8(y);//+(vy>=0?1:-1)

        //UpdateGrounding();
        /*if(IsGroundedDown) {
          inkY+=1;
        } else if(IsGroundedUp) {
          inkY-=1;
        } else if(IsGroundedLeft) {
          inkX-=1;
        } else if(IsGroundedRight) {
          inkX+=1;
        }*/
        
        inkX = constrain(inkX,0,(world.MapWidth-1)*8);
        inkY = constrain(inkY,0,(world.MapHeight-1)*8);
        
        for(int8_t xv = -1; xv < 2; xv++) {
          for(int8_t yv = -1; yv < 2; yv++) {
            if(((Div8(inkX+xv*4) == constrain(Div8(inkX+xv*4),0,world.MapWidth-1))&&(Div8(inkY+yv*4) == constrain(Div8(inkY+yv*4),0,world.MapWidth-1))) && TilesParams_Array[world.getTile(Div8(inkX+xv*4),Div8(inkY+yv*4))*TileParamsCount+0] == 1) {
              
              V0 = constrain(world.SMGetPaintValueAt(Div8(inkX+xv*4),Div8(inkY+yv*4),0), 0, 3);
              V1 = constrain(world.SMGetPaintValueAt(Div8(inkX+xv*4),Div8(inkY+yv*4),1), 0, 3);
              V2 = constrain(world.SMGetPaintValueAt(Div8(inkX+xv*4),Div8(inkY+yv*4),2), 0, 3);
              V3 = constrain(world.SMGetPaintValueAt(Div8(inkX+xv*4),Div8(inkY+yv*4),3), 0, 3);
              
              if(xv==-1 && yv==0) {
                if(world.getTile(Div8(inkX-4),Div8(inkY+0)) != 0) {
                  V0 = constrain(world.SMGetPaintValueAt(Div8(inkX-4),Div8(inkY),0), 0, 3);
                  V1 = constrain(world.SMGetPaintValueAt(Div8(inkX-4),Div8(inkY),1)+1+IsGroundedLeft, 0, 3);
                  V2 = constrain(world.SMGetPaintValueAt(Div8(inkX-4),Div8(inkY),2), 0, 3);
                  V3 = constrain(world.SMGetPaintValueAt(Div8(inkX-4),Div8(inkY),3), 0, 3);
                  AddPointToPlayer(Owner,V1-world.SMGetPaintValueAt(Div8(inkX-4),Div8(inkY),1));
                }
              }
              if(xv==1 && yv==0) {
                if(world.getTile(Div8(inkX+4),Div8(inkY+0)) != 0) {
                  V0 = constrain(world.SMGetPaintValueAt(Div8(inkX+4),Div8(inkY),0), 0, 3);
                  V1 = constrain(world.SMGetPaintValueAt(Div8(inkX+4),Div8(inkY),1), 0, 3);
                  V2 = constrain(world.SMGetPaintValueAt(Div8(inkX+4),Div8(inkY),2), 0, 3);
                  V3 = constrain(world.SMGetPaintValueAt(Div8(inkX+4),Div8(inkY),3)+1+IsGroundedRight, 0, 3);
                  AddPointToPlayer(Owner,V3-world.SMGetPaintValueAt(Div8(inkX+4),Div8(inkY),3));
                }
              }
              if(yv==1 && xv==0) {
                if(world.getTile(Div8(inkX+0),Div8(inkY+4)) != 0) {
                  V0 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY+4),0)+1+IsGroundedDown, 0, 3);
                  V1 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY+4),1), 0, 3);
                  V2 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY+4),2), 0, 3);
                  V3 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY+4),3), 0, 3);
                  AddPointToPlayer(Owner,V0-world.SMGetPaintValueAt(Div8(inkX),Div8(inkY+4),0));
                }
              }
              if(yv==-1 && xv==0) {
                if(world.getTile((inkX+0)/8,(inkY-4)/8) != 0) {
                  V0 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY-4),0), 0, 3); 
                  V1 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY-4),1), 0, 3); 
                  V2 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY-4),2)+1+IsGroundedUp, 0, 3); 
                  V3 = constrain(world.SMGetPaintValueAt(Div8(inkX),Div8(inkY-4),3), 0, 3);
                  AddPointToPlayer(Owner,V2-world.SMGetPaintValueAt(Div8(inkX),Div8(inkY-4),2));
                }
              }

              world.SMSetPaintValue(
                constrain(V0, 0, 3),
                constrain(V1, 0, 3),
                constrain(V2, 0, 3),
                constrain(V3, 0, 3),
                constrain(Div8(inkX+xv*4),0,world.MapWidth-1),
                constrain(Div8(inkY+yv*4),0,world.MapHeight-1),
                color
              );
            }
          }
        }

        //gb.display.setColor(RED);
        //gb.display.fillRect(toScreenX(inkX*8),toScreenY(inkY*8),8,8);
        
        Die();
      }

      if(!IsDead) {
        Object::Update();

        Draw();
        /*if(!Object::isOffScreen()) {
          if(color == 0) {
            gb.display.fillRect(toScreenX(x/SCALE), toScreenY(y/SCALE), getWidth(), getHeight());
          } else {
            gb.display.drawRect(toScreenX(x/SCALE), toScreenY(y/SCALE), getWidth(), getHeight());
          }
        }*/
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
