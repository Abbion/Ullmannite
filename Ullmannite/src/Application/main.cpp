#include "Ullpch.h"
#include "Application.h"
#include "Rendering/Api/Renderer.h"

int main() 
{
	Ull::Application* app = new Ull::Application();

	if(!app->FailedToInitialize())
		app->Run();

	delete app;

	return 0;
}