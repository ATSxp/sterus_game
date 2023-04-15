#include <string.h>

#include "hud.h"
#include "e_item.h"
#include "e_player.h"

#include "gfx_hp_p.h"
#include "gfx_items.h"

#define HUD_CBB 0
#define HUD_SBB 31

u8 g_inv_cursor;
s8 g_inv_p[MAX_INVENTORY] = {0};
TSprite *icon_inv = NULL;

void initHud(Player p) {
  g_inv_cursor = 0;

  GRIT_CPY(&pal_bg_bank[2], gfx_hp_pPal);
  GRIT_CPY(&tile_mem[0][30], gfx_hp_pTiles);

  REG_BG0CNT = BG_CBB(0) | BG_SBB(31);

  icon_inv = T_addObj(
      SCREEN_WIDTH - 16, SCREEN_HEIGHT - 16, 
      OBJ_16X16, ITEM_TID_BASE + 4, 0, 0, 
      SET_GFX_OBJ(FALSE, gfx_items)
    );
}

void updateHud(Player p) {
  int ii;
  u32 dist;

  if (p.dead) {
    se_fill(&se_mem[HUD_SBB][HUD_CBB], SE_ID(0));

    if (p.spr)
      tte_init_se_default(0, BG_CBB(1) | BG_SBB(HUD_SBB));

    REM_SPR(icon_inv);

    return;
  }

  for (ii = 0; ii < p.hp; ii++)
    se_plot(&se_mem[HUD_SBB][HUD_CBB], ii + 1, 0, SE_ID(30) | SE_PALBANK(2));

  if (p.hp != PLAYER_MAX_HP) {
    dist = clamp(PLAYER_MAX_HP - p.hp, 0, PLAYER_MAX_HP + 1);

    for (ii = 0; ii < dist; ii++)
      se_plot(&se_mem[HUD_SBB][HUD_CBB], 8 - (ii == dist ? 1 : ii), 0, SE_ID(31) | SE_PALBANK(2));
  }

  // Inventory
  g_inv_p[0] = g_player.i_health;

  if (g_inv_p[g_inv_cursor] != 0) {
    T_showObj(icon_inv);
    T_setTileObj(icon_inv, 128 + (g_inv_cursor != 0 ? 4 << g_inv_cursor : 4 ));
  } else
    T_hideObj(icon_inv);

  for (ii = 0; ii < MAX_INVENTORY; ii++)
    if (g_inv_p[ii] == 0) return;

  if (key_hit(KEY_L) && g_inv_cursor > 0) {
    do {
      g_inv_cursor = (g_inv_cursor - 1) % MAX_INVENTORY;
    } while(g_inv_p[g_inv_cursor] == 0);

  } else if (key_hit(KEY_R) && g_inv_cursor < MAX_INVENTORY - 1) {
    do {
      g_inv_cursor = (g_inv_cursor + 1) % MAX_INVENTORY;
    } while(g_inv_p[g_inv_cursor] == 0);

  }

}
