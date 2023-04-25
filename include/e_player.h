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
  int hp;
  TSprite *spr;

  Bullet b[PLAYER_MAX_BULLET];
  Anim anim;

  u8 state, i_health;
  FIXED imortal_t;
  BOOL dead, damaged;
} Player;

extern Player g_player;

INLINE void E_damagePlayer(Player *p, int dmg);

void E_initPlayer(Player *p);
void E_updatePlayer(Player *p);

INLINE void E_damagePlayer(Player *p, int dmg) {
  if (!p->damaged) {
    p->damaged = TRUE;

    p->hp += dmg;
    
    if (dmg < 0)
      p->imortal_t = 0x0400;
  }

}

#endif // !__E_PLAYER__
