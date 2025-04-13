# TerrainEngine

## Description

A modern real-time 3D terrain rendering engine built with OpenGL 4.6, GLFW, and Dear ImGui. Features dynamic terrain generation, real-time editing, and interactive visualization tools.

## Features

- **Procedural Terrain Generation**
  - Diamond-Square algorithm implementation
  - GPU-accelerated heightmap generation
  - Customizable terrain parameters (roughness, elevation)
  
- **Advanced Rendering**
  - Multi-textured terrain blending
  - Dynamic day/night cycle (TODO)
  - Phong lighting model with specular highlights
  - Fog effects with distance-based attenuation (TODO)
  - Skybox with dynamic color transitions

- **Real-Time Controls**
  - Camera system (FPS-style movement)
  - Interactive parameter tuning via ImGui
  - Dynamic LOD (Level of Detail) adjustments
  - Terrain constraints for camera movement

- **Tools & UI**
  - Real-time performance metrics
  - Color preset system for environment
  - Framebuffer visualization tools
  - Customizable post-processing effects

## Technologies Used

- **Core Graphics**: OpenGL 4.6
- **Windowing**: GLFW 3.3
- **UI Framework**: Dear ImGui 1.92
- **Loader**: GLAD
- **Mathematics**: Custom linear algebra library
- **Build System**: isual Studio 2022

## Screenshots

![Terrain View](screenshots/terrain.png)
![UI Controls](screenshots/ui.png)

## Installation

### Dependencies
- Visual Studio 2022
- Conan 2.0+ (package manager)
- GLFW 3.3
- GLM 0.9.9+
- OpenGL 4.6 compatible GPU

### Build Instructions

```bash
# Clone repository
git clone https://github.com/El-sharqawy/TerrainEngine.git

Enter TerrainEngine, and Open TerrainEngine.sln in Visual Studio

