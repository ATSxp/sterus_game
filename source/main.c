#include "engine/gba.h"
#include "utils.h"
#include "all_scenes.h"

int main() {
  T_init();

  setScene(g_scene_game);

  while (1) {
    T_update();
    updateScene();
  }

  return 0;
}
