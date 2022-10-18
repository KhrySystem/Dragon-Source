#include <dragon/dragon.hpp>

std::string Dragon::engine::appName;
std::vector<GLFWwindow*> Dragon::engine::windows;

DGAPI DgBool32 Dragon::createNewWindow(Dragon::WindowCreateParams params) {
	GLFWwindow* window;
	if(Dragon::engine::windows.size() > 0) {
		window = glfwCreateWindow(
			params.width, params.height,
			Dragon::engine::appName.c_str(),
			NULL,
			Dragon::engine::windows.at(0)
		);
	} else {
		window = glfwCreateWindow(
			params.width, params.height,
			Dragon::engine::appName.c_str(),
			NULL, NULL
		);
	}

	if(window == NULL)
		throw Dragon::GLFWWindowCreationFailedException() << Dragon::ExceptionInfo("GLFW window handle was NULL.");
	
	Dragon::engine::windows.push_back(window);
	return DG_TRUE;
}

DGAPI DgBool32 Dragon::shouldWindowClose(int index) {
	return glfwWindowShouldClose(Dragon::engine::windows.at(index));
}

DGAPI void Dragon::update() {
	glfwPollEvents();
}

DGAPI void Dragon::closeWindow(int index) {
	glfwDestroyWindow(Dragon::engine::windows.at(index));
}