# A Custom-built Engine for building Isometric 2D Games
Planned to have a 2.5D effect, where we render a 3D world but utilise 2D sprite billboarding to create an isometric effect.
  - 3D World allows us to perform much faster calculations and is just better in general.

Currently Done:
- Rudimentary Engine

Required:
- Shader
- Vertex Vector
- Buffers

Dependencies:
- imgui is currently used for a basic gui for the engine


## Building
Create a `/build` folder in the root directory and run `cmake .. && make` within the `/build` directory to build the binary.
