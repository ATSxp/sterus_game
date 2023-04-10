#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "engine/gba.h"
#include "bg.h"

enum LevelsIds {
  LEVEL_01,
  LEVELS_TOTAL
};

typedef struct {
  SCR_ENTRY *map[2];
  TILE *tiles[2];
  COLOR *pal[2];

  u32 tiles_len[2];
  u16 tid[2], pb[2];
} Level;

extern const Level g_levels[LEVELS_TOTAL];
extern FIXED g_bg_speed;
extern POINT32 g_bg_pos[2] EWRAM_BSS;

INLINE void L_updateLevel();

void L_initLevel(const Level *l);

INLINE void L_updateLevel() {
  setPosBg(1, g_bg_pos[0].x >> 8, (g_bg_pos[0].y -= g_bg_speed) >> 8);
  setPosBg(2, g_bg_pos[1].x >> 8, (g_bg_pos[1].y -= g_bg_speed >> 2) >> 8);
}

#endif // !__LEVEL_H__
