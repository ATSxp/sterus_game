#include "anim.h"
#include "engine/sprites.h"

void A_updateAnim(Anim *a, TSprite *spr) {
  u16 tids;

  if (!a || !spr) return;

  if (a->cur_frame >= (a->len - 1)) {
    a->end = TRUE;
    if (!a->loop) return;
  } else
    a->end = FALSE;

  a->size = spr->size;
  a->tick += a->speed;

  switch (a->size) {
  case OBJ_16X16:
    tids = 4;
    break;
  default:
    tids = 1;
    break;
  }

  if (a->tick >= 0x0800) {
    a->tick = 0x00;
    if (a->loop)
      a->cur_frame = (a->cur_frame + 1) % a->len;
    else
      a->cur_frame = clamp(a->cur_frame + 1, 0, a->len);
  }

  BFN_SET(spr->obj.attr2, a->offset + (tids * a->cur_frame), ATTR2_ID);
}

void A_setAnim(Anim *a, u16 offset_tid, u16 len, BOOL loop, TGfx *gfx, u16 pb) {
  if (!a) return;

  a->offset = offset_tid;
  a->len = len;
  a->loop = loop;

  if (gfx) {
    tonccpy(pal_obj_bank[pb], gfx->pal, gfx->pal_len);
    tonccpy(&tile_mem[4][offset_tid], gfx->tiles, gfx->tiles_len);
  }

}
