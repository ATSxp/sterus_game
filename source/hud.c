#include "hud.h"

#include "gfx_hp_p.h"

void initHud(Player p) {
  int ii;

  GRIT_CPY(&pal_bg_bank[2], gfx_hp_pPal);
  GRIT_CPY(&tile_mem[0][30], gfx_hp_pTiles);

  REG_BG0CNT = BG_CBB(0) | BG_SBB(31);

  for (ii = 0; ii < PLAYER_MAX_HP; ii++)
    se_plot(&se_mem[31][0], ii + 1, 0, SE_ID(30) | SE_PALBANK(2));
}

void updateHud(Player p) {
  int ii;
  u32 dist;

  if (p.dead) {
    se_fill(&se_mem[31][0], SE_ID(0));

    if (p.spr)
      tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));

    return;
  }

  if (p.hp != PLAYER_MAX_HP) {
    dist = clamp(PLAYER_MAX_HP - p.hp, 0, PLAYER_MAX_HP + 1);

    for (ii = 0; ii < dist; ii++)
      se_plot(&se_mem[31][0], 8 - (ii == dist ? 1 : ii), 0, SE_ID(31) | SE_PALBANK(2));

  }

}
