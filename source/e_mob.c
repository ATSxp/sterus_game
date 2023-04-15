#include "e_mob.h"
#include "e_player.h"
#include "e_item.h"

// Sprites sheets
#include "gfx_enemy_n.h"
#include "gfx_explo.h"
#include "gfx_bullet_enemy.h"

#define MOB_TID_BASE 16
#define MOB_BULLET_TID_BASE 36
#define MOB_SPEED_BASE 0x0300

void E_initCommon(Mob *m);
void E_updateCommon(Mob *m);

void E_mobVsBullet(Mob *m, Bullet *b);
void E_mobVsPlayer(Mob *m, Player *p);
void E_updateMove(Mob *m);
void E_updateBullet(Mob *m);

void playerVsBullet(Player *p, Bullet *b);

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
    0, FALSE,
    E_initCommon, E_updateCommon
  },

};

Mob g_mobs[MOB_MAX];
u8 g_mob_count, spawn_count = 0;

void E_initMob(enum MobIds id, int x, int y, u16 move_type) {
  const struct MobTemplate *t = &g_mob_template[id];
  Mob *m = NULL;

  while (!m) {
    if (!g_mobs[spawn_count].spr && g_mobs[spawn_count].bullet_total == 0) {
      m = &g_mobs[spawn_count];
      g_mob_count++;
    }

    spawn_count = (spawn_count + 1) % MOB_MAX;
  }

  m->dead = FALSE;
  m->init_bullets = FALSE;
  m->hp = 1;
  m->id = id;
  m->w = t->w;
  m->h = t->h;
  m->move_type = move_type;
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
    if (x < SCREEN_WIDTH >> 1)
      m->dx = m->speed + 0x0100;
    else
      m->dx = -(m->speed + 0x0100);

    m->dy = m->speed - 0x020;
  } else if (m->move_type == MOB_MOVE_GO) {
    if (x < SCREEN_WIDTH >> 1)
      m->dx = m->speed;
    else
      m->dx = -m->speed;

    m->dy = m->speed - 0x030;
  }

  // Load explosion sprite-sheet on VRAM
  tonccpy(&tile_mem[4][64], gfx_exploTiles, gfx_exploTilesLen);

  A_initAnim(&m->anims[MOB_STATE_DEAD], GET_ANIM(ANIM_DEATH), 14, 0x0480, FALSE, 0);
}

void E_updateMob(Mob *m) {
  int ii;
  u32 rnd = qran_range(ITEM_ID_GUN, ITEM_TOTAL);
  POINT32 pt = {m->pos.x >> 8, m->pos.y >> 8};

  E_updateBullet(m);

  if (!m->spr) return;

  if ((pt.x < - (int)m->w || pt.x > SCREEN_WIDTH || 
      pt.y > SCREEN_HEIGHT))
    m->hp = 0;

  if (m->hp <= 0)
    E_removeMob(m);

  A_updateAnim(&m->anims[m->state], m->spr);

  if (m->dead) {
    if (m->anims[MOB_STATE_DEAD].end) {
      REM_SPR(m->spr);

      if (rnd != ITEM_TOTAL)
        E_initItem(rnd, pt.x, pt.y);
    }
    return;
  }

  if (m->dx != 0x00 || m->dy != 0x00)
    m->state = MOB_STATE_WALK;
  else
    m->state = MOB_STATE_IDLE;

  E_mobVsPlayer(m, &g_player);

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    E_mobVsBullet(m, &g_player.b[ii]);

  for (ii = 0; ii < m->max_bullets; ii++)
    playerVsBullet(&g_player, &m->bull[ii]);

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

  if (!m->dead)
    m->bullet_timer -= 0x020;
  else
    m->bullet_timer = m->bullet_max_timer;

  if (m->bullet_timer <= 0x00) {
    if (m->bull[m->bullet_count].dead) {
      Bullet *_b = &m->bull[m->bullet_count];

      initBullet(
          _b, 
          pt.x + (m->w >> 1) - 3, pt.y, 
          8, 8, 0x00, MOB_SPEED_BASE
        );

      initGfxBullet(
          _b, OBJ_8X8, 
          MOB_BULLET_TID_BASE, 
          0, 1, 
          SET_GFX_OBJ(FALSE, gfx_bullet_enemy)
        );

        m->bullet_total++;
      _b = NULL;
    }

    m->bullet_count = (m->bullet_count + 1) % m->max_bullets;
    m->bullet_timer = m->bullet_max_timer;
  }

  for (ii = 0; ii < m->max_bullets; ii++) {
    updateBullet(&m->bull[ii]);

    if (m->bull[ii].dead && m->bull[ii].spr) m->bullet_total--;
  }

}

void E_mobVsBullet(Mob *m, Bullet *b) {
  if (m->dead || b->dead) return;

  if (T_objVsObj(m->spr, b->spr)) {
    m->hp--;
    destroyBullet(b);
  }

}

void E_mobVsPlayer(Mob *m, Player *p) {
  if (m->dead || p->dead) return;

  if (T_objVsObj(m->spr, p->spr)) {
    E_damagePlayer(p, -1);
    m->hp--;
  }

}

void playerVsBullet(Player *p, Bullet *b) {
  if (p->dead || b->dead) return;

  if (T_objVsObj(p->spr, b->spr)) {
    E_damagePlayer(p, -1);
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
