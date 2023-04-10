#include "bg.h"

void initBg(u16 bgnr, u16 cbb, u16 sbb, u16 prio, const void *map, int len) {
  REG_BGCNT[bgnr] = BG_CBB(cbb) | BG_SBB(sbb) | BG_REG_32x64 | BG_PRIO(prio);
  REG_BG_OFS[bgnr].x = REG_BG_OFS[bgnr].y = 0;

  tonccpy(&se_mem[sbb][cbb], map, len);
}
