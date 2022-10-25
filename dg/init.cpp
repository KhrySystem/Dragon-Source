#include <dragon/dragon.hpp>

VkApplicationInfo Dragon::Engine::appInfo;
std::string concancate(std::string s1, std::string s2);
VkInstanceCreateInfo Dragon::Engine::createInfo;
std::vector<const char*> Dragon::Engine::extensions;
std::vector<Dragon::GPU> Dragon::Engine::gpus;
std::vector<VkExtensionProperties> Dragon::Engine::supportedExtensions;
VkInstance Dragon::Engine::vkInstance;
std::vector<const char*> Dragon::Stream::Engine::availibleLayerNames;
ALCdevice* Dragon::Engine::activeDevice;
ALCcontext* Dragon::Engine::alcContext;

DGAPI void Dragon::init(std::string appName) {
    // Assorted Dragon Setup methods
    Dragon::Engine::appName = appName;

    // GLFW Setup
    if(!glfwInit())
        throw Dragon::GLFWInitializationFailedException() << Dragon::ExceptionInfo("GLFW Initialization Failed.");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if(!glfwVulkanSupported())
        throw Dragon::GLFWVulkanNotSupportedException() << Dragon::ExceptionInfo("GLFW didn't find any Vulkan Support.");

    //Vulkan Setup
    Dragon::Engine::appInfo.apiVersion = VK_API_VERSION_1_2;
    Dragon::Engine::appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    Dragon::Engine::appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    Dragon::Engine::appInfo.pApplicationName = appName.c_str();
    Dragon::Engine::appInfo.pEngineName = "Dragon Engine";
    Dragon::Engine::appInfo.pNext = NULL;
    Dragon::Engine::appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    Dragon::Engine::createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    Dragon::Engine::createInfo.pApplicationInfo = &Dragon::Engine::appInfo;

    //Vulkan Extension Layer Setup
    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for(unsigned int i = 0; i < glfwExtensionCount; i++) {
        Dragon::Engine::extensions.emplace_back(glfwExtensions[i]);
    }

    #ifdef DG_PLAT_MACOSX
        Dragon::Engine::extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        Dragon::Engine::createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    
    if(Dragon::getOption(DRAGON_FIREBREATH_ENABLED))
        Dragon::Fire::init();
    
    if(Dragon::getOption(DRAGON_IRONBREATH_ENABLED))
        Dragon::Iron::init();
    
    if(Dragon::getOption(DRAGON_LIGHTBREATH_ENABLED))
        Dragon::Light::init();

    #ifdef DRAGON_OPENCL_FOUND
        if(Dragon::getOption(DRAGON_THUNDERBREATH_ENABLED))
            Dragon::Thunder::init();
    #endif

    VkResult result;
    // Vulkan Instance Creation
    result = vkCreateInstance(&Dragon::Engine::createInfo, NULL, &Dragon::Engine::vkInstance);
    if(result != VK_SUCCESS)
        if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED))
            throw Dragon::VulkanInitializationFailedException() << Dragon::ExceptionInfo(("Vulkan Instance Creation Failed with " + dgConvertVkResultToString(result)).c_str());
        else
            throw Dragon::VulkanInitializationFailedException() << Dragon::ExceptionInfo("Vulkan Instance Creation Failed");

    Dragon::Engine::gpus = Dragon::getGPUs();

    // OpenAL setup
    Dragon::Engine::activeDevice = alcOpenDevice(NULL);

    if (Dragon::Engine::activeDevice) {
        Dragon::Engine::alcContext = alcCreateContext(Dragon::Engine::activeDevice, NULL);
        alcMakeContextCurrent(Dragon::Engine::alcContext);
    }
}

DGAPI DgBool32 Dragon::isExtensionLayerSupported(std::string layerName) {
    for(VkExtensionProperties properties : Dragon::Engine::supportedExtensions) {
        if(strcmp(layerName.c_str(), properties.extensionName) == DG_TRUE) {
            return DG_TRUE;
        }
    }
    return DG_FALSE;
}

DGAPI void Dragon::terminate() {
    vkDestroyInstance(Dragon::Engine::vkInstance, nullptr);
    glfwTerminate();
    Dragon::Fire::terminate();
    Dragon::Iron::terminate();
    Dragon::Light::terminate();
    Dragon::Stream::terminate();
    #ifdef DRAGON_OPENCL_FOUND
        Dragon::Thunder::terminate();
    #endif
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Dragon::Engine::alcContext);
    alcCloseDevice(Dragon::Engine::activeDevice);
}
