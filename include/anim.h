#ifndef __ANIM_H__
#define __ANIM_H__

#include "engine/gba.h"

#define GET_ANIM(id) g_anims[id]

enum AnimId{
  ANIM_SHIP_IDLE,
  ANIM_SHIP_WALK,
  ANIM_E_COMMON_WALK,
  ANIM_TOTAL
};

typedef struct {
  cu16 *frames;
  FIXED speed;
  BOOL loop;
  u32 length;
  u16 offset_tid;
  int cur_frame;
  FIXED ticks;
  BOOL end;
} Anim;

extern cu16* g_anims[ANIM_TOTAL];

INLINE void A_initAnim(Anim *a, cu16 *frames, u32 length, FIXED speed, BOOL loop, u16 offset_tid);
INLINE void A_resetAnim(Anim *a);

void A_updateAnim(Anim *a, TSprite *spr);

INLINE void A_initAnim(Anim *a, cu16 *frames, u32 length, FIXED speed, BOOL loop, u16 offset_tid) {
  a->frames = frames;
  a->length = length;
  a->speed = speed;
  a->loop = loop;
  a->offset_tid = offset_tid;
}

INLINE void A_resetAnim(Anim *a) {
  a->cur_frame = 0;
  a->ticks = 0x00;
  a->end = FALSE;
}

#endif // !__ANIM_H__
