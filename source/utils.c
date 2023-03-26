#include "utils.h"

#define NULL_SCENE {NULL, NULL, NULL}
#define FUNC_IS_NULL(fn) if ((fn)) (fn)()

static BOOL change_scene = FALSE;

static Scene cur_scene = NULL_SCENE, new_scene = NULL_SCENE;

void setScene(Scene next_scene) {
  change_scene = TRUE;
  new_scene = next_scene;
}

void updateScene() {
  if (change_scene) {
    change_scene = FALSE;
    FUNC_IS_NULL(cur_scene.end);
    cur_scene = new_scene;
    FUNC_IS_NULL(cur_scene.init);
  }

  FUNC_IS_NULL(cur_scene.update);
}
