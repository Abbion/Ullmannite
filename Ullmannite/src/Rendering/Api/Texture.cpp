#include "Ullpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "OpenGL/TextureOpenGL.h"
#include "Logger/Logger.h"

using namespace Ull;

Texture2D* Texture2D::Create()
{
	switch (Renderer::GetInstance()->GetApi())
	{
		case Renderer::API::OPEN_GL:
			return new Texture2DOpenGL();
		break;
	}

	ULOGE("Current API didn't implement Texture2D");
	return nullptr;
}