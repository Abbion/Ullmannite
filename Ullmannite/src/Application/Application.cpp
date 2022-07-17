#include "Ullpch.h"
#include "Application.h"
#include "Logger/Logger.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

using namespace Ull;

Application::Application()
{
    InitLog();
}

Application::~Application()
{
	
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Application::Run()
{
	GLFWwindow* window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	UASSERT(status, "Failed to init glad");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}