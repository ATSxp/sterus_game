#ifndef __ENGINE_MAPS__
#define __ENGINE_MAPS__

#include <tonc.h>

typedef struct {
  int x, y;
  u16 cnt;
  u16 flag;
  u32 w, h;
  SCR_ENTRY *src;
} TMap;

#endif // !__ENGINE_MAPS__
