#include <dragon/dragon.hpp>

DGAPI DG_BOOL dgInit(std::string appName) {
    dgEngine::appName = appName;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    dgEngine::appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    dgEngine::appInfo.pApplicationName = dgEngine::appName.c_str();

    return DG_TRUE;
}

DGAPI void dgTerminate() {

}