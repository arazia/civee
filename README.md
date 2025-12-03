# A Custom-built Engine for building Isometric 2D Games
Planned to have a 2.5D effect, where we render a 3D world but utilise 2D sprite billboarding to create an isometric effect.
  - 3D World allows us to perform much faster calculations and is just better in general.

Currently Done:
- Rudimentary Engine
- VBO Pipeline
- Raycasting for Mouse 

TODO NOW:
- autotiling + uv tiling
- Material class
- change static functions to constructed objects (safer)
- Make sure we UNBIND FROM OUR VAOs PLEASE
- RAW POINTERS WEEE WOOO WEE WOO in Application
- Implement tiled/clustered forward rendering
- need to allow objects to be created manually for future editor
- physically-based lighting, shadows
- scene management
- post-fx pipeline with HDR tonemapping and maybe dof
- OpenGLShader should cache GLint locations instead of building strings
- multithreading, raytracing, streaming etc.

Required:
- Material system
-   // (In a real engine, you would move them to a separate "Static" list or just flag them)
- Keybind functionality
- More mesh functionality
- actual sprites and stuff lmao
- collision
- and more
- learn vulkan and implement it
- Separate Editor + ECS System

Future:
- Custom Physics Library
- Lighting

Dependencies:
- imgui is currently used for a basic gui for the engine

## Building
Create a `/build` folder in the root directory and run `cmake .. && make` within the `/build` directory to build the binary.
