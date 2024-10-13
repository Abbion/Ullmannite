#include "Ullpch.h"

#ifdef GTEST
#include <iostream>
#include <gtest/gtest.h>
#include "allTests.h"

int main()
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}

#else
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
#endif