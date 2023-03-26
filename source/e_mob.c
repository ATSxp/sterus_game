#include "e_mob.h"
#include "e_player.h"

// Sprites sheets
#include "gfx_enemy_n.h"

#define MOB_TID_BASE 16

void E_common(Mob *m);
void E_mobVsBullet(Mob *m, Bullet *b);

const struct MobTemplate g_mob_template[MOB_TOTAL] = {
  /*
    Tile id, Palette Bank, Object Size, 
    Width, Height, 
    Gfx, 
    Callback
  */
  {
    MOB_TID_BASE, 0, OBJ_16X16, 
    16, 16, 
    SET_GFX_OBJ(FALSE, gfx_enemy_n), 
    E_common
  },

};

Mob g_mobs[MOB_MAX];
u8 g_mob_count;

void E_initMob(enum MobIds id, int x, int y) {
  const struct MobTemplate *t = &g_mob_template[id];
  int ii;
  Mob *m = NULL;

  for (ii = 0; ii < MOB_MAX; ii++) {
    if (g_mobs[ii].dead) {
      m = &g_mobs[ii];
      g_mob_count++;
      break;
    }

  }

  m->dead = FALSE;
  m->id = id;
  m->w = t->w;
  m->h = t->h;
  m->pos.x = x << 8;
  m->pos.y = y << 8;
  m->spr = T_addObj(x, y, t->size, t->tid, t->pal, 1, t->gfx);
}

void E_updateMob(Mob *m) {
  int ii;
  POINT32 pt = {m->pos.x >> 8, m->pos.y >> 8};

  if (m->dead) return;

  if (pt.x < - (int)m->w || pt.x > SCREEN_WIDTH || 
      pt.y > SCREEN_HEIGHT) {
    E_removeMob(m);
  }

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    E_mobVsBullet(m, &g_player.b[ii]);

  if (g_mob_template[m->id].run) 
    g_mob_template[m->id].run(m);

  m->pos.x += m->dx;
  m->pos.y += m->dy;

  m->spr->x = pt.x;
  m->spr->y = pt.y;
}

void E_mobVsBullet(Mob *m, Bullet *b) {
  if (m->dead || b->dead) return;

  if (T_objVsObj(m->spr, b->spr)) {
    E_removeMob(m);
    destroyBullet(b);
  }

}

/*
 Enemys updaters
*/

void E_common(Mob *m) {
  m->dy = 0x080;
}
