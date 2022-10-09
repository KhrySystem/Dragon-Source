#include <dragon/dragon.hpp>

std::string dgEngine::appName;
std::vector<GLFWwindow*> dgEngine::windows;

DGAPI DG_BOOL dgCreateNewWindow(DgWindowCreateParams params) {
	GLFWwindow* window;
	if(dgEngine::windows.size() > 0) {
		window = glfwCreateWindow(
			params.width, params.height,
			dgEngine::appName.c_str(),
			NULL,
			dgEngine::windows.at(0)
		);
	} else {
		window = glfwCreateWindow(
			params.width, params.height,
			dgEngine::appName.c_str(),
			NULL, NULL
		);
	}

	if(window == NULL)
		throw dgGLFWWindowCreationFailedException() << DgExceptionInfo("GLFW window handle was NULL.");
	
	dgEngine::windows.push_back(window);
	return DG_TRUE;
}

DGAPI DG_BOOL dgShouldWindowClose(int index) {
	return glfwWindowShouldClose(dgEngine::windows.at(index));
}

DGAPI void dgUpdate() {
	glfwPollEvents();
}

DGAPI void dgCloseWindow(int index) {
	glfwDestroyWindow(dgEngine::windows.at(index));
}