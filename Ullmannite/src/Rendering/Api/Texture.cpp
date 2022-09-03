#include "Ullpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "OpenGL/TextureOpenGL.h"

using namespace Ull;

Texture2D* Texture2D::Create()
{
	switch (Renderer::GetInstance()->GetApi())
	{
		case Renderer::API::OPEN_GL:
			return new Texture2DOpenGL();
		break;
	}

	//TODO: error message
	return nullptr;
}