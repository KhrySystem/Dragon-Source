#include <dragon/dragon.hpp>

VkApplicationInfo dgEngine::appInfo;
std::string concancate(std::string s1, std::string s2);
VkInstanceCreateInfo dgEngine::createInfo;
std::vector<const char*> dgEngine::extensions;
unsigned int dgEngine::glfwExtensionCount;
std::vector<GPU> dgEngine::gpus;
std::vector<VkExtensionProperties> dgEngine::supportedExtensions;
VkInstance dgEngine::vkInstance;

DGAPI void dgInit(std::string appName) {
    // Assorted Dragon Setup methods
    dgEngine::appName = appName;

    // GLFW Setup
    if(!glfwInit())
        throw dgGLFWInitializationFailedException() << DgExceptionInfo("GLFW Initialization Failed.");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if(!glfwVulkanSupported())
        throw dgGLFWVulkanNotSupportedException() << DgExceptionInfo("GLFW didn't find any Vulkan Support.");

    //Vulkan Setup
    dgEngine::appInfo.apiVersion = VK_API_VERSION_1_2;
    dgEngine::appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    dgEngine::appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    dgEngine::appInfo.pApplicationName = appName.c_str();
    dgEngine::appInfo.pEngineName = "Dragon Engine";
    dgEngine::appInfo.pNext = NULL;
    dgEngine::appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    dgEngine::createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    dgEngine::createInfo.pApplicationInfo = &dgEngine::appInfo;

    //Vulkan Extension Layer Setup
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&dgEngine::glfwExtensionCount);
    for(unsigned int i = 0; i < dgEngine::glfwExtensionCount; i++) {
        dgEngine::extensions.emplace_back(glfwExtensions[i]);
    }

    #ifdef DG_PLAT_MACOSX
        dgEngine::extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        dgEngine::createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    VkResult result;
    // Vulkan Instance Creation
    result = vkCreateInstance(&dgEngine::createInfo, NULL, &dgEngine::vkInstance);
    if(result != VK_SUCCESS)
        if(dgIsStreambreathEnabled())
            throw dgVulkanInitializationFailedException() << DgExceptionInfo(("Vulkan Instance Creation Failed with " + dgConvertVkResultToString(result)).c_str());
        else
            throw dgVulkanInitializationFailedException() << DgExceptionInfo("Vulkan Instance Creation Failed");

    dgEngine::gpus = dgGetGPUs();
}

DGAPI DG_BOOL dgIsExtensionLayerSupported(std::string layerName) {
    dgEngine::extensions.emplace_back(layerName.c_str());

    for(const char* extensionName : dgEngine::extensions) {
        bool layerFound = DG_FALSE;

        for(VkExtensionProperties properties : dgEngine::supportedExtensions) {
            if(strcmp(extensionName, properties.extensionName) == 0) {
                layerFound = DG_TRUE;
                break;
            }
        }

        if(!layerFound) {
            return DG_FALSE;
        }
    }
    if(dgIsStreambreathEnabled())
        dgEngine::createInfo.ppEnabledLayerNames = dgStreamEngine::availibleLayerNames.data();
    else
        dgEngine::createInfo.ppEnabledExtensionNames = NULL;
    return DG_TRUE;
}

DGAPI void dgTerminate() {
    vkDestroyInstance(dgEngine::vkInstance, nullptr);
    glfwTerminate();
}
