#include "anim.h"

#define LEN_F(l) (cu16[l])

void A_updateAnim(Anim *a, TSprite *spr) {
  if (!a) {
    BFN_SET(spr->obj.attr2, a->offset_tid, ATTR2_ID);
    return;
  }

  if (a->cur_frame >= (a->length - 1)) {
    a->end = TRUE;
  } else {
    a->end = FALSE;
  }

  a->ticks += a->speed;

  if (a->ticks >= 0x0800) {
    a->ticks = 0x00;

    if (a->loop)
      a->cur_frame = (a->cur_frame + 1) % a->length;
    else
      a->cur_frame = clamp(a->cur_frame + 1, 0, a->length);
  }

  BFN_SET(spr->obj.attr2, a->offset_tid + a->frames[a->cur_frame], ATTR2_ID);
}

// All animations of Mobs/Items/etc
cu16 *g_anims[ANIM_TOTAL] = {
  LEN_F(2){0, 4},
  LEN_F(2){8, 12},
  LEN_F(2){0, 4},
  LEN_F(14){0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48},
};
