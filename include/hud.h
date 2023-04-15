#ifndef __HUD_H__
#define __HUD_H__

#include "engine/gba.h"
#include "e_player.h"

#define MAX_INVENTORY 3

extern u8 g_inv_cursor;
extern s8 g_inv_p[MAX_INVENTORY];

void initHud(Player p);
void updateHud(Player p);

#endif // !__HUD_H__
