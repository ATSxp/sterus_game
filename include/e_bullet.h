#ifndef __E_BULLET__
#define __E_BULLET__

#include "engine/gba.h"

typedef struct {
  POINT32 pos;
  FIXED dx, dy;
  u32 w, h;
  TSprite *spr;
  BOOL dead;
} Bullet;

INLINE void initGfxBullet(Bullet *b, u16 size, u16 tid, u16 pal, u16 prio, TGfx *gfx);
INLINE void destroyBullet(Bullet *b);
INLINE void clearBullets(Bullet *buffer, u32 len);

void initBullet(Bullet *b, int x, int y, u32 w, u32 h, FIXED dx, FIXED dy);
void updateBullet(Bullet *b);

INLINE void initGfxBullet(Bullet *b, u16 size, u16 tid, u16 pal, u16 prio, TGfx *gfx) {
  b->spr = T_addObj(b->pos.x, b->pos.y, size, tid, pal, prio, gfx);
}

INLINE void destroyBullet(Bullet *b) {
  if (!b->dead) {
    b->dead = TRUE;
    T_removeObj(b->spr);
    b->spr = NULL;
  }

}

INLINE void clearBullets(Bullet *buffer, u32 len) {
  int ii;
  for (ii = 0; ii < len; ii++)
    destroyBullet(&buffer[ii]);
}

#endif // !__E_BULLET__
