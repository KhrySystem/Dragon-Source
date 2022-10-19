#include <dragon/dragon.hpp>

VkApplicationInfo Dragon::engine::appInfo;
std::string concancate(std::string s1, std::string s2);
VkInstanceCreateInfo Dragon::engine::createInfo;
std::vector<const char*> Dragon::engine::extensions;
std::vector<Dragon::GPU> Dragon::engine::gpus;
std::vector<VkExtensionProperties> Dragon::engine::supportedExtensions;
VkInstance Dragon::engine::vkInstance;
std::vector<const char*> Dragon::Stream::engine::availibleLayerNames;

DGAPI void Dragon::init(std::string appName) {
    // Assorted Dragon Setup methods
    Dragon::engine::appName = appName;

    // GLFW Setup
    if(!glfwInit())
        throw Dragon::GLFWInitializationFailedException() << Dragon::ExceptionInfo("GLFW Initialization Failed.");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if(!glfwVulkanSupported())
        throw Dragon::GLFWVulkanNotSupportedException() << Dragon::ExceptionInfo("GLFW didn't find any Vulkan Support.");

    //Vulkan Setup
    Dragon::engine::appInfo.apiVersion = VK_API_VERSION_1_2;
    Dragon::engine::appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    Dragon::engine::appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    Dragon::engine::appInfo.pApplicationName = appName.c_str();
    Dragon::engine::appInfo.pEngineName = "Dragon Engine";
    Dragon::engine::appInfo.pNext = NULL;
    Dragon::engine::appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    Dragon::engine::createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    Dragon::engine::createInfo.pApplicationInfo = &Dragon::engine::appInfo;

    //Vulkan Extension Layer Setup
    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for(unsigned int i = 0; i < glfwExtensionCount; i++) {
        Dragon::engine::extensions.emplace_back(glfwExtensions[i]);
    }

    #ifdef DG_PLAT_MACOSX
        Dragon::engine::extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        Dragon::engine::createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    
    VkResult result;
    // Vulkan Instance Creation
    result = vkCreateInstance(&Dragon::engine::createInfo, NULL, &Dragon::engine::vkInstance);
    if(result != VK_SUCCESS)
        if(Dragon::getOption( DRAGON_STREAMBREATH_ENABLED))
            throw Dragon::VulkanInitializationFailedException() << Dragon::ExceptionInfo(("Vulkan Instance Creation Failed with " + dgConvertVkResultToString(result)).c_str());
        else
            throw Dragon::VulkanInitializationFailedException() << Dragon::ExceptionInfo("Vulkan Instance Creation Failed");

    Dragon::engine::gpus = Dragon::getGPUs();
}

DGAPI DgBool32 DragonIsExtensionLayerSupported(std::string layerName) {
    Dragon::engine::extensions.emplace_back(layerName.c_str());

    for(const char* extensionName : Dragon::engine::extensions) {
        bool layerFound = DG_FALSE;

        for(VkExtensionProperties properties : Dragon::engine::supportedExtensions) {
            if(strcmp(extensionName, properties.extensionName) == 0) {
                layerFound = DG_TRUE;
                break;
            }
        }

        if(!layerFound) {
            return DG_FALSE;
        }
    }
    if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED)) {
        Dragon::Stream::init();
        Dragon::engine::createInfo.ppEnabledLayerNames = Dragon::Stream::engine::availibleLayerNames.data();
    } else {
        Dragon::engine::createInfo.ppEnabledExtensionNames = NULL;
    }
    return DG_TRUE;
}

DGAPI void dgTerminate() {
    vkDestroyInstance(Dragon::engine::vkInstance, nullptr);
    glfwTerminate();
}
