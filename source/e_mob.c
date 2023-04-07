#include "e_mob.h"
#include "e_player.h"

// Sprites sheets
#include "gfx_enemy_n.h"
#include "gfx_explo.h"
// Tmp
#include "gfx_bull_p01.h"

#define MOB_TID_BASE 16
#define MOB_SPEED_BASE 0x0200

void E_initCommon(Mob *m);
void E_updateCommon(Mob *m);

void E_mobVsBullet(Mob *m, Bullet *b);
void E_updateMove(Mob *m);
void E_updateBullet(Mob *m);

const struct MobTemplate g_mob_template[MOB_TOTAL] = {
  /*
    Tile id, Palette Bank, Object Size
    Width, Height
    Gfx
    Max Bullets, Can Shoot
    Init Callback, Update Callback
  */
  {
    MOB_TID_BASE, 0, OBJ_16X16, 
    16, 16, 
    SET_GFX_OBJ(FALSE, gfx_enemy_n), 
    5, TRUE,
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
  m->init_bullets = FALSE;
  m->hp = 1;
  m->id = id;
  m->w = t->w;
  m->h = t->h;
  m->move_type = MOB_MOVE_NORMAL;
  m->pos.x = x << 8;
  m->pos.y = y << 8;
  m->speed = 0x080;

  if (t->shoot) {
    m->max_bullets = t->max_bullet;
    m->bullet_max_timer = 0x0900;
    m->bullet_timer = m->bullet_max_timer;
  } else {
    m->max_bullets = 0;
  }

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

  // Tmp
  GRIT_CPY(&tile_mem[4][64], gfx_exploTiles);

  A_initAnim(&m->anims[MOB_STATE_DEAD], GET_ANIM(ANIM_DEATH), 14, 0x0480, FALSE, 0);

}

void E_updateMob(Mob *m) {
  int ii;
  POINT32 pt = {m->pos.x >> 8, m->pos.y >> 8};

  E_updateBullet(m);

  if ((pt.x < - (int)m->w || pt.x > SCREEN_WIDTH || 
      pt.y > SCREEN_HEIGHT))
    m->hp = 0;

  if (m->hp <= 0)
    E_removeMob(m);

  if (m->spr)
    A_updateAnim(&m->anims[m->state], m->spr);

  if (m->dead) {
    if (m->anims[MOB_STATE_DEAD].end && m->spr) {
      T_removeObj(m->spr);
      m->spr = NULL;
    }
    return;
  }

  if (m->dx != 0x00 || m->dy != 0x00)
    m->state = MOB_STATE_WALK;
  else
    m->state = MOB_STATE_IDLE;

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    E_mobVsBullet(m, &g_player.b[ii]);

  if (g_mob_template[m->id].run) 
    g_mob_template[m->id].run(m);

  E_updateMove(m);

  for (ii = 0; ii < 3; ii++)
    if (ii != m->state)
      A_resetAnim(&m->anims[ii]);

  m->pos.x += m->dx;
  m->pos.y += m->dy;

  m->spr->x = pt.x;
  m->spr->y = pt.y;
}

void E_updateBullet(Mob *m) {
  int ii;
  POINT32 pt = {m->pos.x >> 8, m->pos.y >> 8};

  if (m->max_bullets <= 0) return;

  if (!m->init_bullets) {
    m->init_bullets = TRUE;

    for (ii = 0; ii < m->max_bullets; ii++)
      m->bull[ii].dead = TRUE;
  }

  if (!m->dead) {
    m->bullet_timer -= 0x020;

    if (m->bullet_timer <= 0x00) {
      if (m->bull[m->bullet_count].dead) {
        Bullet *_b = &m->bull[m->bullet_count];

        initBullet(_b, pt.x, pt.y, 8, 8, 0x00, MOB_SPEED_BASE);

        // Tmp
        initGfxBullet(_b, OBJ_16X16, 32, 0, 1, SET_GFX_OBJ(FALSE, gfx_bull_p01));
        _b = NULL;
      }

      m->bullet_count = (m->bullet_count + 1) % m->max_bullets;
      m->bullet_timer = m->bullet_max_timer;
    }

  }

  for (ii = 0; ii < m->max_bullets; ii++)
    updateBullet(&m->bull[ii]);

}

void E_mobVsBullet(Mob *m, Bullet *b) {
  if (m->dead || b->dead) return;

  if (T_objVsObj(m->spr, b->spr) && m->hp > 0) {
    m->hp--;
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
