#pragma once
#include <string>

struct WindowSettings {
    std::string Title = "civee Engine";
    int Width = 1280;
    int Height = 720;
    bool Fullscreen = false;
    bool VSync = true;
};

struct RenderSettings {
    float ClearColor[4] = {0.1f, 0.1f, 0.12f, 1.0f};
    bool DepthTest = true;
};

class Config {
public:
    static Config& get() { static Config instance; return instance; }

    WindowSettings Window;
    RenderSettings Render;

private:
    Config() = default;
};
