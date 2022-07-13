#include <iostream>
#include "Application.h"

int main() 
{
	Ull::Application* app = new Ull::Application();
	app->Run();
	delete app;

	return 0;
}