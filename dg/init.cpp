#include <dragon/dragon.hpp>

VkApplicationInfo dgEngine::appInfo;
VkInstanceCreateInfo dgEngine::createInfo;
unsigned int dgEngine::glfwExtensionCount;
std::vector<const char*> dgEngine::extensions;
std::vector<VkExtensionProperties> dgEngine::supportedExtensions;
std::vector<const char*> dgEngine::requestedValidationLayers;
unsigned int dgEngine::layerCount;
std::vector<VkLayerProperties> dgEngine::availableLayers;
VkDebugUtilsMessengerEXT dgEngine::debugMessenger;
VkDebugUtilsMessengerCreateInfoEXT dgEngine::debugMessengerCreateInfo;
std::string concancate(std::string s1, std::string s2);

DGAPI void dgInit(std::string appName) {
    // Assorted Dragon Setup methods
    dgEngine::appName = appName;
    #ifndef DRAGON_FULL_POWER
        dgEngine::requestedValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
    #endif

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
    #ifndef DRAGON_FULL_POWER
        if(!dgAddValidationLayerToEngine("VK_LAYER_KHRONOS_validation"))
            throw dgVulkanValidationLayerNotFoundException() << DgExceptionInfo("Vulkan Validation Layer \"VK_LAYER_KHRONOS_validation\" not found");
        
        if(!dgAddExtensionLayerToEngine(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
            throw dgVulkanExtensionLayerNotFoundException() << DgExceptionInfo(
                concancate(
                    concancate(
                        "Vulkan Extension Layer \"", 
                        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
                    ).c_str(), 
                    "\"not found"
                ).c_str()
            );
    #endif
    VkResult result;

    // Vulkan Instance Creation
    result = vkCreateInstance(&dgEngine::createInfo, NULL, &dgEngine::vkInstance);
    if(result != VK_SUCCESS)
        throw dgVulkanInitializationFailedException() << DgExceptionInfo("Vulkan Instance Creation Failed");

    #ifndef DRAGON_FULL_POWER
        // Vulkan Validation Layer Setup
        vkEnumerateInstanceLayerProperties(&dgEngine::layerCount, nullptr);

        dgEngine::availableLayers.resize(dgEngine::layerCount);
        vkEnumerateInstanceLayerProperties(&dgEngine::layerCount, dgEngine::availableLayers.data());

        dgEngine::debugMessengerCreateInfo.flags = NULL;
        dgEngine::debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        dgEngine::debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        dgEngine::debugMessengerCreateInfo.pfnUserCallback = dgVkValidationLayerDebugCalback;
        dgEngine::debugMessengerCreateInfo.pNext = NULL;
        dgEngine::debugMessengerCreateInfo.pUserData = NULL;
        dgEngine::debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        result = dgCreateDebugUtilsMessengerEXT(dgEngine::vkInstance, &dgEngine::debugMessengerCreateInfo, NULL, &dgEngine::debugMessenger);
        if(result != VK_SUCCESS) {
            throw dgVulkanDebugUtilsMessengerEXTCreationFailedException() << DgExceptionInfo(("Vulkan DebugUtilsMessengerEXT Creation Failed with " + dgConvertVkResultToString(result)).c_str());
        }
    #endif
}

DGAPI DG_BOOL dgAddExtensionLayerToEngine(std::string layerName) {
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

    dgEngine::createInfo.ppEnabledLayerNames = dgEngine::requestedValidationLayers.data();
    return DG_TRUE;
}

DGAPI DG_BOOL dgAddValidationLayerToEngine(std::string layerName) {
    dgEngine::requestedValidationLayers.emplace_back(layerName.c_str());

    for (const char* layerName : dgEngine::requestedValidationLayers) {
        bool layerFound = false;

        for (VkLayerProperties layerProperties : dgEngine::availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

DGAPI void dgTerminate() {
    #ifndef DRAGON_FULL_POWER
        dgDestroyDebugUtilsMessengerEXT(dgEngine::vkInstance, dgEngine::debugMessenger, nullptr);
    #endif

    vkDestroyInstance(dgEngine::vkInstance, nullptr);
    glfwTerminate();
}
