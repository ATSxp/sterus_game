#ifndef __BG_H__
#define __BG_H__

#include "engine/gba.h"

#define LOAD_BG_DATA(data, tid, pal) \
  tonccpy(&tile_mem[0][(tid)], data##Tiles, data##TilesLen); \
  tonccpy(&pal_bg_bank[(pal)], data##Pal, data##PalLen);

INLINE void setPosBg(u16 bgnr, int x, int y);

void initBg(u16 bgnr, u16 cbb, u16 sbb, u16 prio, const void *map, int len);

INLINE void setPosBg(u16 bgnr, int x, int y) {
  REG_BG_OFS[bgnr].x = x;
  REG_BG_OFS[bgnr].y = y;
}

#endif // !__BG_H__
