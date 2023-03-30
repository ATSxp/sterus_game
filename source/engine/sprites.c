#include "engine/sprites.h"

#define SPR_MAX 128

u32 spr_count = 0;

TSprite spr_buffer[128];

u32 unussed_ids_count = 0;
u8 unussed_ids[128];

int sort_keys[128];
u8 sort_ids[128];

IWRAM_CODE void idSortShell(int keys[], u8 ids[], int count) {
  u32 ii, inc;

  for (inc = 1; inc <= count; inc++)
    inc *= 3;

  do {
    inc /= 3;

    for (ii = inc; ii < count; ii++) {
      u32 j, id0 = ids[ii];
      int key0 = keys[id0];

      for (j = ii; j >= inc && keys[ids[j - inc]] > key0; j -= inc)
        ids[j] = ids[j - inc];

      ids[j] = id0;

    }

  } while(inc > 1);

}

INLINE u16 _getShapeFromSize(u16 size);
void _setGfxObj(TSprite *spr, TGfx *gfx, u16 tid);

void T_initObjs() {
  u32 ii;
  for (ii = 0; ii < SPR_MAX; ii++) {
    spr_buffer[ii].id = sort_ids[ii] = ii;
    obj_hide(&spr_buffer[ii].obj);
  }

}

TSprite *T_addObj(int x, int y, u16 size, u16 tid, u16 pb, u16 prio, TGfx *gfx) {
  TSprite *spr;
  OBJ_ATTR *obj;
  u8 un_id = unussed_ids[unussed_ids_count - 1];
  u8 id = unussed_ids_count > 0 ? un_id : spr_count;

  if (spr_count >= SPR_MAX)
    return NULL;

  spr = &spr_buffer[id];
  obj = &spr_buffer[id].obj;

  if (unussed_ids_count > 0) unussed_ids_count--;

  spr_count++;

  spr->active = TRUE;

  _setGfxObj(spr, gfx, tid);

  obj->attr0 = _getShapeFromSize(size);
  obj->attr1 = (size << ATTR1_SIZE_SHIFT);
  obj->attr2 = ATTR2_BUILD(tid, pb, prio);

  BFN_SET2(obj->attr1, spr->x = x, ATTR1_X);
  BFN_SET2(obj->attr0, spr->y = y, ATTR0_Y);

  return spr;
}

void T_removeObj(TSprite *spr) {
  if (!spr->active) return;

  unussed_ids[unussed_ids_count++] = spr->id;

  spr_buffer[spr->id].obj = (OBJ_ATTR){0, 0, 0};
  obj_hide(&spr_buffer[spr->id].obj);
  spr->active = FALSE;

  spr_count--;
}

void T_updateObjs(BOOL sort) {
  u32 ii;
  int *keys = sort_keys;
  TSprite *spr = spr_buffer;
  OBJ_ATTR *obj;

  for (ii = 0; ii < SPR_MAX; ii++) {
    obj = &spr_buffer[spr->id].obj;

    if (spr->active)
      T_setPosObj(spr, spr->x, spr->y);

    if (sort) {
      if ((obj->attr0 & ATTR0_MODE_MASK) != ATTR0_HIDE)
        *keys++ = spr->y + BFN_GET(obj->attr2, ATTR2_PRIO);
      else
        *keys++ = 0x7FFFFFFF;
    }

    spr++;
  }

  if (sort) {
    idSortShell(sort_keys, sort_ids, SPR_MAX);

    for (ii = 0; ii < SPR_MAX; ii++)
      oam_copy(&oam_mem[ii], &spr_buffer[sort_ids[ii]].obj, 1);
  } else {
    for (ii = 0; ii < SPR_MAX; ii++)
      oam_copy(&oam_mem[ii], &spr_buffer[ii].obj, 1);
  }
}

BOOL T_objVsObj(TSprite *s, TSprite *s2) {
  int sw = obj_get_width(&s->obj), sh = obj_get_height(&s->obj);
  int sw2 = obj_get_width(&s2->obj), sh2 = obj_get_height(&s2->obj);

  return (s->x < s2->x + sw2 && s->y < s2->y + sh2 && s2->x < s->x + sw && s2->y < s->y + sh);
}

INLINE u16 _getShapeFromSize(u16 size) {
  if (size > 3) return ATTR0_TALL;
  else if(size > 7) return ATTR0_WIDE;

  return ATTR0_SQUARE;
}

void _setGfxObj(TSprite *spr, TGfx *gfx, u16 tid) {
  TILE *tile_dst;
  COLOR *pal_dst;

  if (gfx) {
    spr->gfx.bpp = gfx->bpp;
    spr->gfx.tiles = gfx->tiles;
    spr->gfx.pal = gfx->pal;

    spr->gfx.tiles_len = gfx->tiles_len;
    spr->gfx.pal_len = gfx->pal_len;

    pal_dst = pal_obj_bank[BFN_GET(spr->obj.attr2, ATTR2_PALBANK)];

    if (!gfx->bpp) tile_dst = &tile_mem[4][tid];
    else tile_dst = &tile_mem[4][tid * 2];

    memcpy32(tile_dst, gfx->tiles, gfx->tiles_len / 4);
    memcpy32(pal_dst, gfx->pal, gfx->pal_len / 4);
  }

}
