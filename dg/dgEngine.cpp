#include <dragon/dragon.hpp>

DGAPI DG_BOOL dgCreateNewWindow(DgWindowCreateParams params) {
	GLFWwindow* window;
	
	if(DgWindowCreateParams::shareResources && dgEngine::windows.size() > 0)
		window = glfwCreateWindow(
			params.width, params.height,
			dgEngine::appName.c_str(), 
			NULL,
			dgEngine::windows.at(0)
		);
	else 
		window = glfwCreateWindow(
			params.width, params.height,
			dgEngine::appName.c_str(),
			NULL,
			NULL
		);
	
	if(window == NULL)
		return DG_FALSE;
	
	dgEngine::windows.push_back(window);
	return DG_TRUE;
}