#include "Ullpch.h"
#include "Renderer.h"

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