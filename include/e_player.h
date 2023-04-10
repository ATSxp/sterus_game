#ifndef __E_PLAYER__
#define __E_PLAYER__

#include "engine/gba.h"
#include "e_bullet.h"
#include "anim.h"

#define PLAYER_MAX_BULLET 8
#define PLAYER_MAX_HP 8

#define PLAYER_STATE_IDLE 0x00
#define PLAYER_STATE_WALK 0x01
#define PLAYER_STATE_DEAD 0x02

typedef struct {
  POINT32 pos;
  FIXED dx, dy;
  u32 w, h;
  u32 hp;
  TSprite *spr;
  Bullet b[PLAYER_MAX_BULLET];
  Anim anims[3];
  u8 state;
  BOOL dead;
} Player;

extern Player g_player;

void E_initPlayer(Player *p);
void E_updatePlayer(Player *p);

#endif // !__E_PLAYER__
