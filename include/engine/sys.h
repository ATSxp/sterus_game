#ifndef __ENGINE_SYS__
#define __ENGINE_SYS__

#include <string.h>

#include <tonc.h>

#include "sprites.h"
#include "posprintf.h"

// ==============================
// ========== Macros ============
// ==============================

// ==============================
// ======= Prototypes ===========
// ==============================

INLINE void T_init();
INLINE void T_update();
INLINE void T_setMode(u32 mode);
INLINE void T_enableBg(u32 bg);
INLINE void T_disableBg(u32 bg);
INLINE void T_enableObjs();
INLINE void T_disableObjs();

// ==============================
// ======= Functions ============
// ==============================

INLINE void T_init() {
  irq_init(NULL);
  irq_add(II_VBLANK, NULL);
  irq_enable(II_VBLANK);
}

INLINE void T_update() {
  VBlankIntrWait();
  key_poll();
}

INLINE void T_setMode(u32 mode)
{ REG_DISPCNT = mode; }

INLINE void T_enableBg(u32 bg) {
  REG_DISPCNT |= (((bg + 1) << 8) << (bg > 1 ? 1 : 0));
}

INLINE void T_disableBg(u32 bg) {
  REG_DISPCNT &= ~(((bg + 1) << 8) << (bg > 1 ? 1 : 0));
}

INLINE void T_enableObjs() {
  REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;
}

INLINE void T_disableObjs() {
  REG_DISPCNT &= ~(DCNT_OBJ);
  REG_DISPCNT &= ~(DCNT_OBJ_1D);
}

#endif // !__ENGINE_SYS__
