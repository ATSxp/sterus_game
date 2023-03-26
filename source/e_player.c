#include "e_player.h"
#include "e_bullet.h"

#include "gfx_player.h"
#include "gfx_bull_p01.h"

#define PLAYER_INIT_X 0x07000
#define PLAYER_INIT_Y 0x04800
#define PLAYER_SPEED  0x0180
#define PLAYER_MAX_HP 8

#define SHOOT_MAX 8
#define TIMER_SHOOT_MAX 0x0300
#define SHOOT_SPEED 0x0340
#define SHOOT_TID 24

Player g_player;

FIXED t_shoot;
u8 bullet_count;

void updatePlayerBullet(Player *p);
void initPlayerBullet(Player *p);

void E_initPlayer(Player *p) {
  p->hp = PLAYER_MAX_HP;
  p->dead = FALSE;
  p->pos.x = PLAYER_INIT_X;
  p->pos.y = PLAYER_INIT_Y;
  p->w = p->h = 16;

  p->spr = T_addObj(
      PLAYER_INIT_X >> 8, PLAYER_INIT_Y >> 8, 
      OBJ_16X16, 0, 0, 1, 
      SET_GFX_OBJ(FALSE, gfx_player));

  t_shoot = TIMER_SHOOT_MAX;
  bullet_count = 0;

  initPlayerBullet(p);
}

void E_updatePlayer(Player *p) {
  POINT32 pt = {p->pos.x >> 8, p->pos.y >> 8};

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

  p->pos.x += p->dx;
  p->pos.y += p->dy;

  p->spr->x = pt.x;
  p->spr->y = pt.y;
}

void initPlayerBullet(Player *p) {
  u32 ii;
  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++)
    p->b[ii].dead = TRUE;
}

void updatePlayerBullet(Player *p) {
  u32 ii;
  Bullet *b = p->b;

  t_shoot -= 0x080;

  if (key_is_down(KEY_A) && t_shoot < 0) {
    if (p->b[bullet_count].dead) {
      initBullet(
          &p->b[bullet_count], 
          p->pos.x >> 8, p->pos.y >> 8, 
          16, 16, 
          0x00, -SHOOT_SPEED
        );

      initGfxBullet(&p->b[bullet_count], 
                    OBJ_16X16, SHOOT_TID, 0, 1, SET_GFX_OBJ(FALSE, gfx_bull_p01));

      bullet_count = (bullet_count + 1) & (SHOOT_MAX - 1);
    }

    t_shoot = TIMER_SHOOT_MAX;
  }

  for (ii = 0; ii < PLAYER_MAX_BULLET; ii++) {
    updateBullet(b);
    b++;
  }

}
