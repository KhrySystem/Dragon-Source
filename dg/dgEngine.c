#include <dragon/dragon.h>

DGAPI DG_BOOL dgCreateNewWindow(DgWindowCreateParams params) {
	if(params.shareResources) {
		GLFWwindow* share;
		for(GLFWwindow)
	}

	GLFWwindow* window = glfwCreateWindow(
		params.width, params.height,
		dgEngine.appName, 
	)
}