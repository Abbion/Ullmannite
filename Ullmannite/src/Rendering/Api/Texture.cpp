#include "Ullpch.h"
#include "Texture.h"
#include "Logger/Logger.h"
#include "OpenGL/TextureOpenGL.h"
#include "Renderer.h"


using namespace Ull;

Texture2D* Texture2D::Create()
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new Texture2DOpenGL();
        break;
    }
    GL_RGB16;
    ULOGE("Current API didn't implement Texture2D");
    return nullptr;
}

Texture3D* Texture3D::Create()
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new Texture3DOpenGL();
        break;
    }

    ULOGE("Current API didn't implement Texture3D");
    return nullptr;
}