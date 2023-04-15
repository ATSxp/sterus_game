#ifndef __E_MOB__
#define __E_MOB__

#include "engine/gba.h"
#include "e_bullet.h"
#include "anim.h"

#define MOB_MAX 50

#define MOB_MOVE_NORMAL 0x00
#define MOB_MOVE_ZIGZAG 0x01
#define MOB_MOVE_GO     0x02

#define MOB_STATE_IDLE 0x00
#define MOB_STATE_WALK 0x01
#define MOB_STATE_DEAD 0x02
#define MOB_STATE_DELETED 0x0FF

enum MobIds {
  MOB_ID_COMMON,
  MOB_TOTAL
};

typedef struct {
  u8 id, state;
  u16 move_type;
  POINT32 pos;
  FIXED dx, dy, speed;
  u32 w, h;
  int hp;
  BOOL dead : TRUE;

  TSprite *spr;
  Anim anims[3];

  // Bullets
  Bullet bull[10];
  u32 max_bullets;
  u16 bullet_count, bullet_total;
  FIXED bullet_max_timer, bullet_timer;
  BOOL init_bullets;
} Mob;

typedef void (* fnptrMob)(Mob *);

struct MobTemplate {
  u16 tid, pal, size;
  u32 w, h;
  TGfx *gfx;
  u32 max_bullet;
  BOOL shoot;
  fnptrMob init;
  fnptrMob run;
};

extern const struct MobTemplate g_mob_template[MOB_TOTAL];
extern Mob g_mobs[MOB_MAX];

INLINE void E_updateAllMobs();
INLINE void E_removeMob(Mob *m);
INLINE void E_clearAllMobs();

void E_initMob(enum MobIds id, int x, int y, u16 move_type);
void E_updateMob(Mob *m);
void E_updateAllMobs();

INLINE void E_updateAllMobs() {
  int ii;
  for (ii = 0; ii < MOB_MAX; ii++)
    E_updateMob(&g_mobs[ii]);

}

INLINE void E_clearAllMobs() {
  int ii;
  for (ii = 0; ii < MOB_MAX; ii++) {
    REM_SPR(g_mobs[ii].spr);
  }

}

INLINE void E_removeMob(Mob *m) {
  if (!m->dead) {
    m->dead = TRUE;
    m->state = MOB_STATE_DEAD;
  }

}

#endif // !__E_MOB__
