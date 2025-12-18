#include "Ullpch.h"
#include "Texture.h"
#include "Logger/Logger.h"
#include "OpenGL/TextureOpenGL.h"
#include "Application/Application.h"


using namespace Ull;

Texture1D* Texture1D::Create()
{
    switch(Application::GetRenderer().GetApi())
    {
        case Renderer::API::OPEN_GL:
            return new Texture1DOpenGL();
        break;
    }

    ULOGE("Current API didn't implement Texture1D");
    return nullptr;
}

Texture2D* Texture2D::Create()
{
    switch (Application::GetRenderer().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new Texture2DOpenGL();
        break;
    }

    ULOGE("Current API didn't implement Texture2D");
    return nullptr;
}

Texture3D* Texture3D::Create()
{
    switch (Application::GetRenderer().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new Texture3DOpenGL();
        break;
    }

    ULOGE("Current API didn't implement Texture3D");
    return nullptr;
}