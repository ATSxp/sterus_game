#include "level.h"
#include "e_mob.h"

// Maps
#include "map_space_01_bg1.h"
#include "map_space_01_bg2.h"

#define M(map)  {(SCR_ENTRY*)map##1Map, (SCR_ENTRY*)map##2Map}
#define T(map)  {(TILE*)map##1Tiles, (TILE*)map##2Tiles}
#define TL(map) {map##1TilesLen, map##2TilesLen}
#define P(map)  {(COLOR*)map##1Pal, (COLOR*)map##2Pal}

FIXED g_bg_speed = 0x0100;
POINT32 g_bg_pos[2] = {{0x00, 0x00}, {0x00, 0x00}};
u8 g_level_spawn_count;
FIXED level_t;

void L_initLevel(const Level *l) {
  int ii;

  level_t = 0x03000;
  g_level_spawn_count = 0;

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

void L_updateLevel(const Level *l) {
  int ii, mul = 3, rnd;
  POINT32 pt = {0, 0};
  LevelSpawn *s = NULL;

  setPosBg(1, g_bg_pos[0].x >> 8, (g_bg_pos[0].y -= g_bg_speed) >> 8);
  setPosBg(2, g_bg_pos[1].x >> 8, (g_bg_pos[1].y -= g_bg_speed >> 2) >> 8);

  if (g_level_spawn_count >= l->total) return;

  level_t -= 0x040;

  if (l->spawn && level_t <= 0x00) {
    s = &l->spawn[g_level_spawn_count];

    rnd = qran_range(16, SCREEN_WIDTH - 32);

    for (ii = 0; ii < l->spawn->amount; ii++) {
      if (s->pos == SPAWN_MOB_CENTER) {
        pt.x = rnd;
        pt.y = -32;
        mul = 0;
      } else if (s->pos == SPAWN_MOB_LEFT) {
        pt.x = -(ii * 16);
        pt.y = 0;
      } else if (s->pos == SPAWN_MOB_RIGHT) {
        pt.x = SCREEN_WIDTH + (ii * 16);
        pt.y = 0;
      }

      E_initMob(
          s->id, 
          (s->pos != SPAWN_MOB_CENTER ? 
          (s->pos == SPAWN_MOB_LEFT ? (ii << mul) : 
          -(ii << mul)) : 0) + pt.x, 
          (ii << 4) + pt.y, s->move_type
        );
    }

    g_level_spawn_count++;
    level_t = 0x03000;
  }
}

const LevelSpawn level_01_spawn[3] = {
  {
    5, MOB_ID_COMMON, 
    SPAWN_MOB_CENTER, 
    MOB_MOVE_NORMAL, 
  }, 
  {
    4, MOB_ID_COMMON, 
    SPAWN_MOB_LEFT, 
    MOB_MOVE_GO
  }, 
  {
    4, MOB_ID_COMMON, 
    SPAWN_MOB_RIGHT, 
    MOB_MOVE_GO
  }

};

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
    {0, 9}, {0, 1}, 
    3, (LevelSpawn*)level_01_spawn
  },

};
