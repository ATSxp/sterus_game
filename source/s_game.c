#include "engine/gba.h"
#include "utils.h"
#include "e_player.h"
#include "e_mob.h"

void initGame() {
  T_setMode(0);
  T_enableBg(0);
  T_enableObjs();

  tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

  T_initObjs();

  E_initPlayer(&g_player);
  E_initAllMobs();
}

void updateGame() {
  // Tmp
  if (key_hit(KEY_B)) {
    E_initMob(MOB_ID_COMMON, qran_range(0, 240 - 16), -16);
  }
  
  E_updatePlayer(&g_player);
  E_updateAllMobs();
  T_updateObjs(FALSE);
}

void endGame() {
  clearBullets(g_player.b, 8);
}

Scene g_scene_game = {initGame, updateGame, endGame};
