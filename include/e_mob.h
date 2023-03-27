#ifndef __E_MOB__
#define __E_MOB__

#include "engine/gba.h"
#include "e_bullet.h"

#define MOB_MAX 50

#define MOB_MOVE_NORMAL 0x00
#define MOB_MOVE_ZIGZAG 0x01
#define MOB_MOVE_GO     0x02

#define MOB_IDLE 0x00

enum MobIds {
  MOB_ID_COMMON,
  MOB_TOTAL
};

typedef struct {
  u8 id, state, move_type;
  POINT32 pos;
  FIXED dx, dy, speed;
  u32 w, h;
  BOOL dead;
  TSprite *spr;
} Mob;

typedef void (* fnptrMob)(Mob *);

struct MobTemplate {
  u16 tid, pal, size;
  u32 w, h;
  TGfx *gfx;
  fnptrMob run;
};

extern const struct MobTemplate g_mob_template[MOB_TOTAL];
extern Mob g_mobs[MOB_MAX];
extern u8 g_mob_count;

INLINE void E_initAllMobs();
INLINE void E_updateAllMobs();
INLINE void E_removeMob(Mob *m);

void E_initMob(enum MobIds id, int x, int y);
void E_updateMob(Mob *m);
void E_updateAllMobs();

INLINE void E_initAllMobs() {
  int ii;
  g_mob_count = 0;

  for (ii = 0; ii < MOB_MAX; ii++)
    g_mobs[ii].dead = TRUE;
}

INLINE void E_updateAllMobs() {
  int ii;
  for (ii = 0; ii < MOB_MAX; ii++) {
    E_updateMob(&g_mobs[ii]);
  }

}

INLINE void E_removeMob(Mob *m) {
  if (!m->dead) {
    m->dead = TRUE;
    T_removeObj(m->spr);
    g_mob_count--;
  }

}

#endif // !__E_MOB__
