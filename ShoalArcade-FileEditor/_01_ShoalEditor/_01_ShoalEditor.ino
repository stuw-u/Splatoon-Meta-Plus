#include <Gamebuino-Meta.h>
extern const uint8_t SquidSquare[];

const char* Options [] = {
  "SPLATOON META",
  "SALMON RUN",
  "EXIT"
};

const char* OptionsSplatoon [] = {
  "CLEAR LEVELS",
  "CLEAR COINS",
  "PLAY",
  "RETURN"
};

const char* OptionsSalmon [] = {
  "NEW GAME",
  "PLAY",
  "RETURN"
};

const char* OptionsConfirm [] = {
  "CONFIRM",
  "RETURN"
};

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  gb.setFrameRate(25);
  gb.display.setFont(SquidSquare);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!gb.update());
  uint8_t sel = gb.gui.menu("Arcade Editor", Options);
  if(sel == 0) {
    while(true) {
      uint8_t sel_spl = gb.gui.menu("Splatoon Meta", OptionsSplatoon);
      if(sel_spl == 0) {
        uint8_t sel_clr = gb.gui.menu("Clear levels?", OptionsConfirm);
        if(sel_clr == 0) {
          gb.save.set(0, (int32_t)0);
        }
      } else if(sel_spl == 1) {
        uint8_t sel_clr = gb.gui.menu("Clear coins?", OptionsConfirm);
        if(sel_clr == 0) {
          gb.save.set(2, (int32_t)0);
        }
      } else if(sel_spl == 2) {
        uint8_t sel_clr = gb.gui.menu("Play the game?", OptionsConfirm);
        if(sel_clr == 0) {
          gb.bootloader.game("ShoalArcade/SPOONM.bin");
        }
      } else if(sel_spl == 3) {
        break;
      }
    }
  } else if(sel == 1) {
    while(true) {
      uint8_t sel_slm = gb.gui.menu("Salmon Run", OptionsSalmon);
      if(sel_slm == 0) {
        uint8_t sel_clr = gb.gui.menu("Clear Old Game?", OptionsConfirm);
        if(sel_clr == 0) {
          gb.save.set(7, (int32_t)0);
          gb.save.set(5, (int32_t)0);
          gb.save.set(6, (int32_t)0);

          gb.save.set(8,random(1,255));
          gb.save.set(9,random(1,255));
          gb.save.set(10,random(1,255));
          gb.save.set(11,random(1,255));
          gb.save.set(12,random(1,255));
          gb.save.set(13,random(1,255));
          gb.save.set(14,random(1,255));
        }
      } else if(sel_slm == 1) {
        uint8_t sel_clr = gb.gui.menu("Play the game?", OptionsConfirm);
        if(sel_clr == 0) {
          gb.bootloader.game("ShoalArcade/SALMONMENU.bin");
        }
      } else if(sel_slm == 2) {
        break;
      }
    }
  } else if(sel == 2) {
    uint8_t sel_exit = gb.gui.menu("Exit Editor?", OptionsConfirm);
    if(sel_exit == 0) {
      gb.bootloader.game("ShoalArcade/ShoalArcade.bin");
    }
  }
}
