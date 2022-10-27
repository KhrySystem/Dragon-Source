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
			Dragon::Error::ErrorType type = Dragon::Error::ErrorType::FATAL;
			Dragon::Error::ErrorSeverity severity = Dragon::Error::ErrorSeverity::ERR;
			Dragon::Error::System system = Dragon::Error::System::GRAPHICS;
			Dragon::Error::ErrorInfo info{};
			info.code = 33000; // Fatal Error in graphics, initialization, first call to throwError
			info.message = "GLFWwindow failed to be initialized.";
			info.severity = &severity;
			info.system = &system;
			info.type = &type;

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