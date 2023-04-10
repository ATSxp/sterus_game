#include <string.h>

#include "e_player.h"
#include "e_bullet.h"

#include "gfx_player.h"
#include "gfx_bull_p01.h"
#include "gfx_explo.h"

#define PLAYER_INIT_X 0x07000
#define PLAYER_INIT_Y 0x04800
#define PLAYER_SPEED  0x0180

#define SHOOT_MAX 8
#define TIMER_SHOOT_MAX 0x0300
#define SHOOT_SPEED 0x0340
#define SHOOT_TID 32

Player g_player;

FIXED t_shoot;
u8 bullet_count;

void updatePlayerBullet(Player *p);
void initPlayerBullet(Player *p);
void destroyPlayer(Player *p);

void E_initPlayer(Player *p) {
  COLOR p_pal[16];

  p->hp = PLAYER_MAX_HP;
  p->dead = FALSE;
  p->pos.x = PLAYER_INIT_X;
  p->pos.y = PLAYER_INIT_Y;
  p->w = p->h = 16;

  p->spr = T_addObj(
      PLAYER_INIT_X >> 8, PLAYER_INIT_Y >> 8, 
      OBJ_16X16, 0, 0, 1, 
      SET_GFX_OBJ(FALSE, gfx_player));

  initPlayerBullet(p);

  // Load explosion sprite-sheet on VRAM
  GRIT_CPY(&tile_mem[4][64], gfx_exploTiles);

  toncset16(p_pal, CLR_WHITE, 16);
  tonccpy(pal_obj_bank[14], p_pal, 32);

  A_initAnim(&p->anims[PLAYER_STATE_IDLE], GET_ANIM(ANIM_SHIP_IDLE), 2, 0x080,  TRUE,  0);
  A_initAnim(&p->anims[PLAYER_STATE_WALK], GET_ANIM(ANIM_SHIP_WALK), 2, 0x080,  TRUE,  0);
  A_initAnim(&p->anims[PLAYER_STATE_DEAD], GET_ANIM(ANIM_DEATH),    14, 0x0480, FALSE, 0);
}

void E_updatePlayer(Player *p) {
  POINT32 pt = {p->pos.x >> 8, p->pos.y >> 8};

  p->hp = clamp(p->hp, 0, PLAYER_MAX_HP + 1);

  if (p->hp <= 0) destroyPlayer(p);

  if (key_is_down(KEY_UP) && pt.y > 0) {
    p->dy = -PLAYER_SPEED;
  } else if (key_is_down(KEY_DOWN) && pt.y < (SCREEN_HEIGHT - p->h)) {
    p->dy = PLAYER_SPEED;
  } else {
    p->dy = 0;
  }

  if (key_is_down(KEY_LEFT) && pt.x > 1) {
    p->dx = -PLAYER_SPEED;
  } else if (key_is_down(KEY_RIGHT) && pt.x < (SCREEN_WIDTH - (p->w + 1))) {
    p->dx = PLAYER_SPEED;
  } else {
    p->dx = 0;
  }

  updatePlayerBullet(p);

  if (!p->spr) return;

  A_updateAnim(&p->anims[p->state], p->spr);

  if (p->dead) { 
    if (p->anims[PLAYER_STATE_DEAD].end) {
      REM_SPR(p->spr);
    }

    return;
  }

  T_flipObj(p->spr, p->dx < 0x00, FALSE);

  p->damaged = FALSE;

  if (p->dx != 0x00 || p->dy != 0x00)
    p->state = PLAYER_STATE_WALK;
  else
    p->state = PLAYER_STATE_IDLE;

  p->pos.x += p->dx;
  p->pos.y += p->dy;

  p->spr->x = pt.x;
  p->spr->y = pt.y;
}

void initPlayerBullet(Player *p) {
  int ii;
  t_shoot = TIMER_SHOOT_MAX;
  bullet_count = 0;

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    p->b[ii].dead = TRUE;
}

void updatePlayerBullet(Player *p) {
  int ii;
  t_shoot -= 0x080;

  if (key_is_down(KEY_A) && t_shoot < 0 && !p->dead) {
    if (p->b[bullet_count].dead) {
      initBullet(
          &p->b[bullet_count], 
          p->pos.x >> 8, p->pos.y >> 8, 
          16, 16, 
          0x00, -SHOOT_SPEED
        );

      initGfxBullet(&p->b[bullet_count], 
                    OBJ_16X16, SHOOT_TID, 0, 1, SET_GFX_OBJ(FALSE, gfx_bull_p01));
    }

    bullet_count = (bullet_count + 1) & (SHOOT_MAX - 1);
    t_shoot = TIMER_SHOOT_MAX;
  }

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    updateBullet(&p->b[ii]);

}

void destroyPlayer(Player *p) {
  if (!p->dead) {
    p->dead = TRUE;
    p->state = PLAYER_STATE_DEAD;
  }

}
