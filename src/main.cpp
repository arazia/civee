#include "Engine.h"

int main() {
  if (Engine::get().init()) {
    Engine::get().run();
  }

  return 0;
}
