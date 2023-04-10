#include "level.h"

// Maps
#include "map_space_01_bg1.h"
#include "map_space_01_bg2.h"

#define M(map)  {(SCR_ENTRY*)map##1Map, (SCR_ENTRY*)map##2Map}
#define T(map)  {(TILE*)map##1Tiles, (TILE*)map##2Tiles}
#define TL(map) {map##1TilesLen, map##2TilesLen}
#define P(map)  {(COLOR*)map##1Pal, (COLOR*)map##2Pal}

FIXED g_bg_speed = 0x0100;
POINT32 g_bg_pos[2] EWRAM_BSS;

void L_initLevel(const Level *l) {
  int ii;
  for (ii = 0; ii < 2; ii++) {
    tonccpy(&pal_bg_bank[l->pb[ii]], l->pal[ii], 32);
    tonccpy(&tile_mem[0][l->tid[ii]], l->tiles[ii], l->tiles_len[ii]);

    g_bg_pos[ii].x = 0x0800;
    g_bg_pos[ii].y = 0x00;

    // 4069 = Length of map 32x64
    initBg(ii + 1, 0, 20 + (4 * (ii + 1)), ii + 1, l->map[ii], 4096);
    setPosBg(ii + 1, 8, 0);
  }

}

const Level g_levels[LEVELS_TOTAL] = {
  /*
    Map          {bg 1, bg 2}
    Tiles        
    Pal          
    Tiles Length 
    Tile Id, Palette Bank 
  */
  {
    M(map_space_01_bg),
    T(map_space_01_bg),
    P(map_space_01_bg),
    TL(map_space_01_bg),
    {0, 9}, {0, 1}
  },

};
