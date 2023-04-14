#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "engine/gba.h"
#include "bg.h"

#define SPAWN_MOB_LEFT 0x00
#define SPAWN_MOB_CENTER 0x01
#define SPAWN_MOB_RIGHT 0x02

enum LevelsIds {
  LEVEL_01,
  LEVELS_TOTAL
};

typedef struct {
  u32 amount;
  u8 id;
  u16 pos;
  u16 move_type;
} LevelSpawn;

typedef struct {
  SCR_ENTRY *map[2];
  TILE *tiles[2];
  COLOR *pal[2];

  u32 tiles_len[2];
  u16 tid[2], pb[2];
  u8 total;
  LevelSpawn *spawn;
} Level;

extern const Level g_levels[LEVELS_TOTAL];
extern FIXED g_bg_speed;
extern POINT32 g_bg_pos[2] EWRAM_BSS;
extern u8 g_level_spawn_count;

void L_initLevel(const Level *l);
void L_updateLevel(const Level *l);

#endif // !__LEVEL_H__
