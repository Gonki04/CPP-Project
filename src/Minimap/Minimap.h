#pragma once
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Shader/shaderClass.h"


namespace Render {

void drawMinimap(Mesh& tableMesh, Mesh& sphereMesh, Shader* shader, int windowWidth, int windowHeight);

}