#include "Core/Application.h"
#include "Game/CivLayer.h"

#include "Editor/EditorLayer.h"

class CivApp : public Application {
public:
  CivApp() {
    CivLayer *game_layer = new CivLayer();
    push_layer(game_layer);

    push_layer(new EditorLayer(game_layer));
  }
};

Application *create_application() { return new CivApp(); }

int main(int argc, char *argv[]) {
  auto app = create_application();
  app->run();
  delete app;
  return 0;
}
