#include <Gamebuino-Meta.h>

void setup() {
  gb.begin();
}

void loop() {
  if(gb.update()) {
    return;
  }
}
