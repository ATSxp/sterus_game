#include "e_item.h"

#include "gfx_item_obj.h"

Item g_items[ITEM_MAX] EWRAM_BSS;

void I_initItem(u8 id, int x, int y) {
  int ii;
  Item *i = NULL;

  for (ii = 0; ii < ITEM_MAX; ii++) {
    if (!g_items[ii].spr) {
      i = &g_items[ii];
      break;
    }

  }

  i->id = id;
  i->dead = FALSE;
  i->pos.x = x << 8;
  i->pos.y = y << 8;
  i->w = i->h = 16;
  i->spr = T_addObj(x, y, OBJ_16X16, 128, 0, 1, SET_GFX_OBJ(FALSE, gfx_item_obj));
  i->dy = 0x020;

  i = NULL;

}

void I_updateItems(Item *i) {
  if (!i->spr) return;

  POINT32 pt = {i->pos.x >> 8, i->pos.y >> 8};

  i->pos.x += i->dx;
  i->pos.y += i->dy;

  i->spr->x = pt.x;
  i->spr->y = pt.y;

}
