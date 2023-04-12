#include "e_item.h"
#include "e_player.h"

#include "gfx_item_obj.h"
#include "gfx_items.h"

#define ITEM_TID_BASE 128

Item g_items[ITEM_MAX] EWRAM_BSS;

INLINE void I_getGun();
INLINE void I_getHealth();

INLINE void I_useHealth();

INLINE void I_ItemVsPlayer(Item *i, Player *p);

const ItemTemplate g_items_template[ITEM_TOTAL] = {
  {
    ITEM_TID_BASE, 0, OBJ_16X16, 
    16, 16, 
    SET_GFX_OBJ(FALSE, gfx_item_obj), 
    I_getGun
  }, 
  {
    ITEM_TID_BASE + 4, 0, OBJ_16X16, 
    16, 16, 
    NULL, 
    I_getHealth, 
    I_useHealth
  }, 

};

void I_initItem(u8 id, int x, int y) {
  int ii;
  const ItemTemplate *t = &g_items_template[id];
  Item *i = NULL;

  for (ii = 0; ii < ITEM_MAX; ii++) {
    if (!g_items[ii].spr) {
      i = &g_items[ii];
      break;
    }

  }

  GRIT_CPY(pal_obj_mem, gfx_itemsPal);
  GRIT_CPY(&tile_mem[4][ITEM_TID_BASE + 4], gfx_itemsTiles);

  i->id = id;
  i->dead = FALSE;
  i->pos.x = x << 8;
  i->pos.y = y << 8;
  i->w = t->w;
  i->h = t->h;
  i->spr = T_addObj(x, y, t->size, t->tid, t->pal, 1, t->gfx);
  i->dy = 0x020;

  i = NULL;
}

void I_updateItems(Item *i) {
  if (!i->spr) return;

  POINT32 pt = {i->pos.x >> 8, i->pos.y >> 8};

  if (pt.x < 0   || pt.x > SCREEN_WIDTH ||
      pt.y < -16 || pt.y > SCREEN_HEIGHT) {
    i->dead = TRUE;
    REM_SPR(i->spr);
  }

  I_ItemVsPlayer(i, &g_player);

  i->pos.x += i->dx;
  i->pos.y += i->dy;

  i->spr->x = pt.x;
  i->spr->y = pt.y;
}

INLINE void I_ItemVsPlayer(Item *i, Player *p) {
  if (T_objVsObj(i->spr, p->spr)) {
    if (g_items_template[i->id].get)
      g_items_template[i->id].get();

    i->dead = TRUE;
    REM_SPR(i->spr);
  }

}

INLINE void I_getGun() {}

INLINE void I_getHealth() 
{ g_player.i_health++; }

INLINE void I_useHealth() {
  g_player.i_health--;
  E_damagePlayer(&g_player, 4);
}
