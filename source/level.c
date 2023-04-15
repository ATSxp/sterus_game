#include "level.h"
#include "e_mob.h"

// Maps
#include "map_space_01_bg1.h"
#include "map_space_01_bg2.h"

#define M(map)  {(SCR_ENTRY*)map##1Map, (SCR_ENTRY*)map##2Map}
#define T(map)  {(TILE*)map##1Tiles, (TILE*)map##2Tiles}
#define TL(map) {map##1TilesLen, map##2TilesLen}
#define P(map)  {(COLOR*)map##1Pal, (COLOR*)map##2Pal}

#define MAX_LEVEL_T 0x0900

#define LEVEL_01_MAX_SPAWN 5

POINT32 g_bg_pos[2] = {{0x00, 0x00}, {0x00, 0x00}};

u8 g_level_spawn_count, spawn_c;

FIXED g_bg_speed;
FIXED level_t;

void L_initLevel(const Level *l) {
  int ii;

  level_t = MAX_LEVEL_T;
  spawn_c = 0;

  g_bg_speed = 0x0100;
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
  LevelSpawn *s = NULL;

  setPosBg(1, g_bg_pos[0].x >> 8, (g_bg_pos[0].y -= g_bg_speed) >> 8);
  setPosBg(2, g_bg_pos[1].x >> 8, (g_bg_pos[1].y -= g_bg_speed >> 2) >> 8);

  if (g_level_spawn_count >= l->total) return;

  level_t -= 0x040;

  if (l->spawn[g_level_spawn_count].amount != 0) {
    if (level_t <= 0x00) {
      s = &l->spawn[g_level_spawn_count];

      E_initMob(s->id, s->pos.x, s->pos.y, s->move_type);
      spawn_c++;

      if (spawn_c >= s->amount) {
        g_level_spawn_count++;
        spawn_c = 0;
      }

      level_t = MAX_LEVEL_T;
    }

  } else {
    level_t = MAX_LEVEL_T << 3;
    g_level_spawn_count++;
  }

}

const LevelSpawn level_01_mobs[LEVEL_01_MAX_SPAWN] = {
  {
    5, MOB_ID_COMMON, 
    {0, -32}, 
    MOB_MOVE_NORMAL, 
  }, 
  {0}, 
  {
    4, MOB_ID_COMMON, 
    {-16, 16}, 
    MOB_MOVE_GO
  }, 
  {
    4, MOB_ID_COMMON, 
    {SCREEN_WIDTH, 16}, 
    MOB_MOVE_GO
  }, 
  {
    4, MOB_ID_COMMON, 
    {-16, 0}, 
    MOB_MOVE_ZIGZAG
  }, 

};

const Level g_levels[LEVELS_TOTAL] = {
  /*
    Map          {bg 1, bg 2}
    Tiles        
    Pal          
    Tiles Length 
    Tile Id, Palette Bank 
    Max spawn
    Mobs list
  */
  {
    M(map_space_01_bg),
    T(map_space_01_bg),
    P(map_space_01_bg),
    TL(map_space_01_bg),
    {0, 9}, {0, 1}, 
    LEVEL_01_MAX_SPAWN, 
    (LevelSpawn*)level_01_mobs
  },

};
