#ifndef __ANIM_H__
#define __ANIM_H__

#include "engine/gba.h"

typedef struct {
  FIXED tick, speed;
  BOOL loop, end;
  u16 cur_frame;
  u16 offset, len, size;
} Anim;

INLINE void A_setAnimSpeed(Anim *a, FIXED speed);
INLINE void A_resetAnim(Anim *a);

void A_updateAnim(Anim *a, TSprite *spr);
void A_setAnim(Anim *a, u16 offset_tid, u16 len, BOOL loop, TGfx *gfx, u16 pb);

INLINE void A_setAnimSpeed(Anim *a, FIXED speed) 
{ a->speed = speed; }

INLINE void A_resetAnim(Anim *a) {
  a->tick = 0x00;
  a->cur_frame = 0;
  a->end = FALSE;
}

#endif // !__ANIM_H__
