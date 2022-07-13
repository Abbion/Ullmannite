#include "Application.h"
#include <iostream>

#include "../Logger/Logger.h";

using namespace Ull;

Application::Application()
{
    InitLog();
}

Application::~Application()
{
	
}

void Application::Run()
{
	ULOGD("Hello");
}