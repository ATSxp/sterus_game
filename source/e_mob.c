#include "e_mob.h"
#include "e_player.h"

// Sprites sheets
#include "gfx_enemy_n.h"

#define MOB_TID_BASE 16

void E_initCommon(Mob *m);
void E_updateCommon(Mob *m);

void E_mobVsBullet(Mob *m, Bullet *b);
void E_updateMove(Mob *m);

const struct MobTemplate g_mob_template[MOB_TOTAL] = {
  /*
    Tile id, Palette Bank, Object Size
    Width, Height
    Gfx
    Max Bullets
    Can Shoot
    Init Callback, Update Callback
  */
  {
    MOB_TID_BASE, 0, OBJ_16X16, 
    16, 16, 
    SET_GFX_OBJ(FALSE, gfx_enemy_n), 
    5, 
    FALSE, 
    E_initCommon, E_updateCommon
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
  m->move_type = MOB_MOVE_NORMAL;//t->move_type;
  m->pos.x = x << 8;
  m->pos.y = y << 8;
  m->speed = 0x080;
  m->spr = T_addObj(x, y, t->size, t->tid, t->pal, 1, t->gfx);

  if (t->init) t->init(m);

  if (m->move_type == MOB_MOVE_NORMAL) {
    m->dy = m->speed;
  } else if (m->move_type == MOB_MOVE_ZIGZAG) {
    m->dx = m->speed + 0x0100;
    m->dy = m->speed - 0x020;
  } else if (m->move_type == MOB_MOVE_GO) {
    m->dx = m->speed + 0x0100;
    m->dy = m->speed - 0x030;
  }

}

void E_updateMob(Mob *m) {
  if (m->dead) return;

  int ii;
  POINT32 pt = {m->pos.x >> 8, m->pos.y >> 8};

  if (pt.x < - (int)m->w || pt.x > SCREEN_WIDTH || 
      pt.y > SCREEN_HEIGHT) {
    E_removeMob(m);
  }

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    E_mobVsBullet(m, &g_player.b[ii]);

  if (g_mob_template[m->id].run) 
    g_mob_template[m->id].run(m);

  if (m->dx != 0x00 || m->dy != 0x00)
    m->state = MOB_STATE_WALK;
  else
    m->state = MOB_STATE_IDLE;

  A_updateAnim(&m->anims[MOB_STATE_WALK], m->spr);
  E_updateMove(m);

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

void E_updateMove(Mob *m) {
  if (m->move_type == MOB_MOVE_ZIGZAG) {
    if ((m->spr->x < 0 && m->dx < 0) ||
        (m->spr->x > SCREEN_WIDTH - 16 && m->dx > 0))
      m->dx = -m->dx;
  }

}

/*
  Enemys Callback
*/

// Enemy Common
void E_initCommon(Mob *m) {
  A_initAnim(&m->anims[MOB_STATE_WALK], GET_ANIM(ANIM_E_COMMON_WALK), 2, 0x080, TRUE, MOB_TID_BASE);
}

void E_updateCommon(Mob *m) {}
