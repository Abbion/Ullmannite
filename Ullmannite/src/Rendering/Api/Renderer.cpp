#include "Ullpch.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace Ull;

Renderer::~Renderer()
{
    //TODO Destruction message
}

Renderer* Renderer::GetInstance()
{
    if(m_rendererInstance == nullptr)
        m_rendererInstance = new Renderer();
    return m_rendererInstance;
}

void Renderer::SetApi(API api)
{
    //TODO check if can change api
    m_api = m_api;
}

void Renderer::init()
{
    if (m_api == API::OPEN_GL)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw InitializationException("Can't initialize GLAD");
    }
}

void Renderer::DrawElements(GraphicsRenderPrimitives primitive, unsigned int count, GraphicsDataType type, unsigned int skip)
{
    if (m_api == API::OPEN_GL)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }
}
