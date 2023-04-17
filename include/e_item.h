#ifndef __E_ITEM_H__
#define __E_ITEM_H__

#include "engine/gba.h"
#include "e_player.h"

#define ITEM_MAX 20
#define ITEM_TID_BASE 192

enum ItemIds {
  ITEM_ID_GUN,
  ITEM_ID_HEALTH,
  ITEM_TOTAL
};

typedef struct {
  u8 id;
  POINT32 pos;
  FIXED dx, dy;
  u32 w, h;
  TSprite *spr;
  BOOL dead : TRUE;
} Item;

typedef struct {
  u16 tid, pal, size;
  u32 w, h;
  TGfx *gfx;
  fnptr get;
} ItemTemplate;

extern Item g_items[ITEM_MAX] EWRAM_BSS;
extern const ItemTemplate g_items_template[ITEM_TOTAL];

INLINE void E_updateAllItems();
INLINE void E_useHealth(Player *p);

void E_initItem(u8 id, int x, int y);
void E_updateItems(Item *i);

INLINE void E_updateAllItems() {
  int ii;
  for (ii = 0; ii < ITEM_MAX; ii++)
    E_updateItems(&g_items[ii]);
}

INLINE void E_useHealth(Player *p) {
  if (p->i_health <= 0) return;
  p->i_health--;
  E_damagePlayer(p, 4);
}

#endif // !__E_ITEM_H__
