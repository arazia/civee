#include "Core/Application.h"
#include "Game/CivLayer.h"

class CivApp : public Application {
public:
    CivApp() {
        push_layer(new CivLayer());
    }
};

Application* create_application() {
    return new CivApp();
}

int main(int argc, char* argv[]) {
    auto app = create_application();
    app->run();
    delete app;
    return 0;
}
