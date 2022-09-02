#include "Ullpch.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"

using namespace Ull;

Renderer::~Renderer()
{
    ULOGD("Renderer terminated");
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
    m_api = api;
}

void Renderer::Init()
{
    if (m_api == API::OPEN_GL)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw InitializationException("Can't initialize GLAD");
    }
}

void Renderer::SetViewPort(const glm::uvec2& position, const glm::uvec2& size)
{
    if(m_api == API::OPEN_GL)
    {
        glViewport(position.x, position.y, size.x, size.y);
    }
}

void Renderer::SetClearColor(glm::vec4 color)
{
    if (m_api == API::OPEN_GL)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }
}

void Renderer::Clear(ClearBits clearBits)
{
    if (m_api == API::OPEN_GL)
    {
        UINT bitSum = (clearBits & ClearBits::COLOR) ? GL_COLOR_BUFFER_BIT : 0;
        bitSum |= (clearBits & ClearBits::DEPTH) ? GL_DEPTH_BUFFER_BIT : 0;
        bitSum |= (clearBits & ClearBits::SETNCIL) ? GL_STENCIL_BUFFER_BIT : 0;
        glClear(bitSum);
    }
}

void Renderer::DrawElements(GraphicsRenderPrimitives primitive, unsigned int count, GraphicsDataType type, unsigned int skip)
{
    if (m_api == API::OPEN_GL)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }
}

void Renderer::FlushContext()
{
    if (m_api == API::OPEN_GL)
    {
        glFlush();
    }
}
