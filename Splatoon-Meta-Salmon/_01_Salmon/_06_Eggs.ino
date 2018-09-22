class Egg:
  public Object {
  public:
  byte type = 0;
  bool collected;
  bool alive = false;
  uint16_t timer = 0;

  int8_t dir = 1;

  virtual int16_t getWidth() {
    return 1;
  };
  
  virtual int16_t getHeight() {
    return 1;
  };

  virtual int16_t getXFriction() {
    return 62;
  };
  virtual int16_t getYFriction() {
    return 62 * (type==0);
  };

  virtual int16_t getGravity() {
    if(type == 0) {
      return 55;
    } else if(type == 1) {
      return 7;
    }
  };
  
  virtual int16_t getYBounce() {
    if(type == 0 && !collected) {
      return 101;
    } else if(type == 1) {
      return 60;
    } else if(type == 0 && collected) {
      return 40;
    }
  };

  bool isOffScreenEgg() {
    return (toScreenX(Div8(x-3)) + toScreenX(Div8(x) + 7) < 0) || (toScreenX(Div8(x)) > LCDWIDTH) || (toScreenY(Div8(y) - 7) + toScreenY(Div8(y-3) + 7 - 6) < 0) || (toScreenY(Div8(y) - 6) > LCDHEIGHT);
  }
  
  void Update () {
    if(type == 0 && !collected) {
      collected = true;
      vx = 40*dir;
    }
    if(!collected) {
      Object::Update();
    } else {
      Object::CleanUpdate(false);
    }
    Draw();
    if(IsGroundedLeft) {
      dir = -1;
    }
    if(IsGroundedRight) {
      dir = 1;
    }

    vx = constrain(vx, -80, 80);
    vy = constrain(vy, -80, 80);
    
    if(world.PixelInCollider(Div64(x),Div64(y),Div8(x)%8,Div8(y)%8,0) == 1) {
      y -= Mul8(2);
    }
    if(Div8(y) >= world.MapHeight*8-(world.WaterLevel) && !collected) {
      Kill();
    }
    if(Div64(x) == 36 && type == 0 && collected && !TutorialMode) {
      Kill();
      world.FlagEggs++;
      playSFX(2,1);
    }
    if(Div64(x) == 22 && type == 0 && collected && TutorialMode) {
      Kill();
      world.FlagEggs++;
      playSFX(2,1);
    }

    if(type == 2 || type == 3) {
      vy = -15;
    }
    
    timer--;
    if(timer <= 0) {
      Kill();
    }
  }

  void Kill () {
    alive = false;
    particleManager.spawnParticle(Div8(x)+3, Div8(y)-3, 1, 0, 0);
  }

  void Draw () {
    if(isOffScreenEgg())
        return;
    
    if(type == 0) {
      gb.display.drawImage(toScreenX(Div8(x)),toScreenY(Div8(y)-6),Egg_Large);
    } else if(type == 1) {
      gb.display.drawImage(toScreenX(Div8(x)),toScreenY(Div8(y)-6),Golden_Large);
      if(random(0,30)%7 == 0) {
        particleManager.spawnParticle(Div8(x)+random(0,6), Div8(y)-6+random(0,6), 0, 0, 0);
      }
    }
  }
};

#define MaxEgg 6
class EggManager {
  public:
  Egg eggs[MaxEgg];

  void Init () {
    for(byte i = 0; i < MaxEgg; i++) {
      eggs[i].type = 0;
      eggs[i].alive = false;
      eggs[i].collected = false;
      eggs[i].timer = 1600;
    }
  }

  void UpdateGlobal () {
    for(byte i = 0; i < MaxEgg; i++) {
      if(eggs[i].alive) {
        eggs[i].Update();
      }
    }
  }

  void Spawn (int16_t x, int16_t y, byte type, int8_t dir) {
    bool uncollected = false;
    for(byte i = 0; i < MaxEgg; i++) {
      if(!eggs[i].alive || (type == 1 && eggs[i].type == 0)) {
        eggs[i].alive = true;
        eggs[i].x = Mul8(x);
        eggs[i].y = Mul8(y);
        eggs[i].vx = 0;
        eggs[i].vy = 0;
        eggs[i].type = type;
        eggs[i].collected = false;
        eggs[i].timer = 1600;
        eggs[i].dir = dir;
        return;
      } else {
        uncollected = !eggs[i].collected || uncollected;
      }
    }
    if(!uncollected) {
      return;
    }
    byte lowestTimer = 0;
    for(byte i = 1; i < MaxEgg; i++) {
      if(eggs[i].timer < eggs[lowestTimer].timer && !eggs[i].collected) {
        lowestTimer = i;
      }
    }
    eggs[lowestTimer].alive = true;
    eggs[lowestTimer].x = Mul8(x);
    eggs[lowestTimer].y = Mul8(y);
    eggs[lowestTimer].type = type;
    eggs[lowestTimer].collected = false;
    eggs[lowestTimer].timer = 1600;
    if(random(0,2)==0) {
      eggs[lowestTimer].dir = 1;
    } else {
      eggs[lowestTimer].dir = -1;
    }
  }
};

EggManager eggManager;
