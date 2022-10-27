#include <dragon/dragon.hpp>

std::string Dragon::Engine::appName;
std::vector<GLFWwindow*> Dragon::Engine::windows;

DGAPI DgBool32 Dragon::createNewWindow(Dragon::WindowCreateParams params) {
	GLFWwindow* window;
	if(Dragon::Engine::windows.size() > 0) {
		window = glfwCreateWindow(
			params.width, params.height,
			Dragon::Engine::appName.c_str(),
			NULL,
			Dragon::Engine::windows.at(0)
		);
	} else {
		window = glfwCreateWindow(
			params.width, params.height,
			Dragon::Engine::appName.c_str(),
			NULL, NULL
		);
	}

	if(window == NULL)
		if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED)) {
			info.code = "3.3.0.0"; // Fatal Error in graphics, initialization, first call to throwError
			info.message = "GLFW window failed to be created";
			Dragon::Stream::throwError(info);
		}
	
	Dragon::Engine::windows.push_back(window);
	return DG_TRUE;
}

DGAPI DgBool32 Dragon::shouldWindowClose(int index) {
	return glfwWindowShouldClose(Dragon::Engine::windows.at(index));
}

DGAPI void Dragon::update() {
	glfwPollEvents();
}

DGAPI void Dragon::closeWindow(int index) {
	glfwDestroyWindow(Dragon::Engine::windows.at(index));
}