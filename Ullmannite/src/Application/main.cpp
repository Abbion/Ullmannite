#include "Ullpch.h"
#include "Application.h"
#include "Rendering/Api/Renderer.h"
#include "Resources/ResourceManager.h"

int main() 
{
	Ull::Application* app = new Ull::Application();

	if(!app->FailedToInitialize())
		app->Run();

	delete app;

	Ull::Renderer::GetInstance().Terminate();
	Ull::ResourceManager::GetInstance().Terminate();

	return 0;
}