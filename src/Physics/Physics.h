#include "Renderer/Scene/GameObject.h"
#include <memory>

struct RaycastHit {
    bool hit = false;
    float distance = -1.0f;
    std::shared_ptr<GameObject> object = nullptr;
};
