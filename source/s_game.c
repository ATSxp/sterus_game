#include "engine/gba.h"
#include "utils.h"
#include "e_player.h"
#include "e_mob.h"
#include "level.h"
#include "bg.h"
#include "hud.h"
#include "e_item.h"

void initGame() {
  T_setMode(0);
  T_enableBg(0);
  T_enableBg(1);
  T_enableBg(2);
  T_enableObjs();

  T_initObjs();

  L_initLevel(&g_levels[LEVEL_01]);

  E_initPlayer(&g_player);
  initHud(g_player);

  E_initItem(ITEM_ID_HEALTH, 16, 0);
}

void updateGame() {
  // Tmp
  if (key_hit(KEY_B))
    E_initMob(MOB_ID_COMMON, 240, -16, MOB_MOVE_ZIGZAG);

  L_updateLevel(&g_levels[LEVEL_01]);
  
  E_updatePlayer(&g_player);
  E_updateAllMobs();
  E_updateAllItems();

  updateHud(g_player);
  T_updateObjs(FALSE);
}

void endGame() {
  clearBullets(g_player.b, 8);
  E_clearAllMobs();

  T_disableObjs();
}

Scene g_scene_game = {initGame, updateGame, endGame};
