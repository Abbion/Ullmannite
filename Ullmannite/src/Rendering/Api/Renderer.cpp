#include "Ullpch.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "OpenGL/DataConverterOpenGL.h"

using namespace Ull;

namespace {
    static void GLAPIENTRY MessageCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        if (type == GL_DEBUG_TYPE_ERROR)
        {
            ULOGE("OpenGL error: " << type << ", " << severity << ", " << message);
        }
    }

    unsigned int ConvertBarrierTypeOpenGL(Renderer::BarrierType barrier)
    {
        switch (barrier)
        {
        case Renderer::BarrierType::STORAGE_BUFFER_BARRIER:
            return GL_SHADER_STORAGE_BARRIER_BIT;
            break;

        case Renderer::BarrierType::ATOMIC_COUNTER_BARRIER:
            return GL_ATOMIC_COUNTER_BARRIER_BIT;
            break;

        case Renderer::BarrierType::IMAGE_BARRIER:
            return GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
            break;
        
        default:
            ULOGE("Can't conver this barrier format");
            return 0;
            break;
        }
    }
}

Renderer Renderer::m_rendererInstance;

Renderer::~Renderer()
{
}

Renderer& Renderer::GetInstance()
{
    return m_rendererInstance;
}

void Renderer::SetApi(API api)
{
    if (m_apiLocked)
    {
        UASSERT(false, "Can't change the API durring program execution");
    }
    else
    {
        m_apiLocked = true;
        m_api = api;
    }
}

void Renderer::Init()
{
    if (m_api == API::OPEN_GL)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw InitializationException("Can't initialize GLAD");
        
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, nullptr);
    }
}

void Renderer::Terminate()
{
    m_shaderManager.UnloadAllShaders();
}

void Renderer::SetViewPort(const glm::uvec2& position, const glm::uvec2& size)
{
    if(m_api == API::OPEN_GL)
    {
        glViewport(position.x, position.y, size.x, size.y);
    }
}

void Renderer::SetDepth(State state)
{
    if(m_api == API::OPEN_GL)
    {
        if(state == State::ENABLE)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::SetFaceCulling(FaceCulling culling)
{
    if(m_api == API::OPEN_GL)
    {
        if(culling == FaceCulling::NONE)
            glDisable(GL_CULL_FACE);
        else
        {
            glEnable(GL_CULL_FACE);

            if(culling == FaceCulling::FRONT)
                glCullFace(GL_FRONT);

            else if(culling == FaceCulling::BACK)
                glCullFace(GL_BACK);

            else if(culling == FaceCulling::FRONT_AND_BACK)
                glCullFace(GL_FRONT_AND_BACK);
        }
    }
}

void Renderer::SetFaceWinding(FaceWinding winding)
{
    if(m_api == API::OPEN_GL)
    {
        if(winding == FaceWinding::CLOCKWISE)
            glFrontFace(GL_CW);

        else
            glFrontFace(GL_CCW);
    }
}

void Renderer::SetPixelPackWidth(unsigned int width)
{
    glPixelStorei(GL_PACK_ALIGNMENT, width);
}

void Renderer::SetPixelUnpackWidth(unsigned int width)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, width);
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
        unsigned int bitSum = (clearBits & ClearBits::COLOR) ? GL_COLOR_BUFFER_BIT : 0;
        bitSum |= (clearBits & ClearBits::DEPTH) ? GL_DEPTH_BUFFER_BIT : 0;
        bitSum |= (clearBits & ClearBits::SETNCIL) ? GL_STENCIL_BUFFER_BIT : 0;
        glClear(bitSum);
    }
}

void Renderer::DrawElements(GraphicsRenderPrimitives primitive, unsigned int count, GraphicsDataType type, unsigned int skip)
{
    if (m_api == API::OPEN_GL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, ConvertFillType(primitive));
        glDrawElements(ConvertPrimitive(primitive), count, ConvetDataType(type), (void*)skip);
    }
}

void Renderer::DrawArrays(GraphicsRenderPrimitives primitive, unsigned int count,  unsigned int skip)
{
    if (m_api == API::OPEN_GL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, ConvertFillType(primitive));
        glDrawArrays(ConvertPrimitive(primitive), skip, count);
    }
}

void Renderer::DispatchComputeShader(unsigned int groupSizeX, unsigned int groupSizeY, unsigned int groupSizeZ)
{
    glDispatchCompute(groupSizeX, groupSizeY, groupSizeZ);
}

void Renderer::FlushContext()
{
    if (m_api == API::OPEN_GL)
    {
        glFlush();
    }
}

void Renderer::Barrier(BarrierType barrier)
{
    if (m_api == API::OPEN_GL)
    {
        glMemoryBarrier(ConvertBarrierTypeOpenGL(barrier));
    }
}