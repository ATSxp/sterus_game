#ifndef __E_PLAYER__
#define __E_PLAYER__

#include "engine/gba.h"
#include "e_bullet.h"

#define PLAYER_MAX_BULLET 8

typedef struct {
  POINT32 pos;
  FIXED dx, dy;
  u32 w, h;
  u32 hp;
  TSprite *spr;
  Bullet b[PLAYER_MAX_BULLET];
  BOOL dead;
} Player;

extern Player g_player;

void E_initPlayer(Player *p);
void E_updatePlayer(Player *p);

#endif // !__E_PLAYER__
