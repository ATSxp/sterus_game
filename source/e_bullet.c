#include "e_bullet.h"

void initBullet(Bullet *b, int x, int y, u32 w, u32 h, FIXED dx, FIXED dy) {
  b->dead = FALSE;
  b->pos.x = x << 8;
  b->pos.y = y << 8;
  b->dx = dx;
  b->dy = dy;
}

void updateBullet(Bullet *b) {
  POINT32 pt = {b->pos.x >> 8, b->pos.y >> 8};

  if (b->dead) return;

  if (pt.x < 0   || pt.x > SCREEN_WIDTH ||
      pt.y < -16 || pt.y > SCREEN_HEIGHT) {
    destroyBullet(b);
  }

  b->pos.x += b->dx;
  b->pos.y += b->dy;

  b->spr->x = pt.x;
  b->spr->y = pt.y;
}
