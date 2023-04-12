#ifndef __E_ITEM_H__
#define __E_ITEM_H__

#include "engine/gba.h"

#define ITEM_MAX 10

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
  fnptr get, use;
} ItemTemplate;

extern Item g_items[ITEM_MAX] EWRAM_BSS;
extern const ItemTemplate g_items_template[ITEM_TOTAL];

INLINE void I_updateAllItems();

void I_initItem(u8 id, int x, int y);
void I_updateItems(Item *i);

INLINE void I_updateAllItems() {
  int ii;
  for (ii = 0; ii < ITEM_MAX; ii++)
    I_updateItems(&g_items[ii]);
}

#endif // !__E_ITEM_H__
