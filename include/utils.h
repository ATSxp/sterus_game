#ifndef __UTILS_H__
#define __UTILS_H__

#include "engine/gba.h"

typedef struct {
  fnptr init, update, end;
} Scene;

INLINE u8 getTile(u32 bgnr, int x, int y);
INLINE BOOL isSolid(u32 bgnr, int x, int y, const u8 ids);

void setScene(Scene next_scene);
void updateScene();

INLINE u8 getTile(u32 bgnr, int x, int y) {
  SCR_ENTRY *dst = (SCR_ENTRY *)se_mem[BFN_GET(REG_BGCNT[bgnr], BG_SBB)];
  return dst[y * 32 + x];
}

INLINE BOOL isSolid(u32 bgnr, int x, int y, const u8 ids) {
  return getTile(bgnr, x >> 3, y >> 3) & ids;
}

#endif // !__UTILS_H__
