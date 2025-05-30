#pragma once
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Shader/shaderClass.h"

namespace Render {
    class Balls;

    void drawMinimap(Mesh& tableMesh, Balls& balls, Shader* shader, int windowWidth, int windowHeight);
}