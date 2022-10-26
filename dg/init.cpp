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

DGAPI int splitString(const char *str, char c, char ***arr)
{
    int *tklen, *t, count = 1;
    char **arr, *p = (char *) txt;

    while (*p != '\0') if (*p++ == delim) count += 1;
    t = tklen = calloc (count, sizeof (int));
    for (p = (char *) txt; *p != '\0'; p++) *p == delim ? *t++ : (*t)++;
    *tokens = arr = malloc (count * sizeof (char *));
    t = tklen;
    p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
    while (*txt != '\0')
    {
        if (*txt == delim)
        {
            p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
            txt++;
        }
        else *p++ = *txt++;
    }
    free (tklen);
    return count;
}

DGAPI void setupGLFW() {
    if(!glfwInit())
        throw Dragon::GLFWInitializationFailedException() << Dragon::ExceptionInfo("GLFW Initialization Failed.");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if(!glfwVulkanSupported())
        throw Dragon::GLFWVulkanNotSupportedException() << Dragon::ExceptionInfo("GLFW didn't find any Vulkan Support.");
}
DGAPI void setupVulkan(VkCreateInfo* pCreateInfo) {
    VkApplicationInfo appInfo{};
    #if defined(VK_VERSION_1_3)
        appInfo.apiVersion = VK_API_VERSION_1_3;
    #elif defined(VK_VERSION_1_2)
        appInfo.apiVersion = VK_API_VERSION_1_2;
    #elif defined(VK_VERSION_1_1)
        appInfo.apiVersion = VK_API_VERSION_1_1;
    #else
        appInfo.apiVersion = VK_API_VERSION_1_0;
    #endif
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pApplicationName = Dragon::Engine::appName.c_str();
    appInfo.pEngineName = "Dragon Engine";
    appInfo.pNext = NULL;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    pCreateInfo->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    pCreateInfo->pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for(unsigned int i = 0; i < glfwExtensionCount; i++) {
        Dragon::Engine::extensions.emplace_back(glfwExtensions[i]);
    }

    #ifdef DG_PLAT_MACOS
        Dragon::Engine::extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        Dragon::Engine::createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        #pragma message "Enabling VK_KHR_PORTABILITY_ENUMERATION_EXTENSION for MacOS"
    #endif
}
DGAPI void setupExtensions() {
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
}
DGAPI void createVulkanInstance(VkCreateInfo* pCreateInfo) {
    VkResult result = vkCreateInstance(pCreateInfo, NULL, &Dragon::Engine::vkInstance);
    if(result != VK_SUCCESS)
        if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED))
            throw Dragon::VulkanInitializationFailedException() << Dragon::ExceptionInfo(("Vulkan Instance Creation Failed with " + dgConvertVkResultToString(result)).c_str());
        else
            throw Dragon::VulkanInitializationFailedException() << Dragon::ExceptionInfo("Vulkan Instance Creation Failed");
}
DGAPI void setupOpenAL() {
    ALString devListString = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    char ** devListNames = NULL;

    int devCount = splitString(devListString, NULL, &devListNames);
    for (i = 0; i < count; i++) {
        
    };

    Dragon::Engine::alcContext = alcCreateContext(Dragon::Engine::activeDevice, NULL);
    alcMakeContextCurrent(Dragon::Engine::alcContext);

    if(alcIsExtensionPresent(Dragon::Engine::activeDevice, "ALC_ENUMERATION_EXT")) {
    }

    if((error = alGetError()) != AL_NO_ERROR) {
        if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED))
            throw Dragon::ALInitializationError() << Dragon::ExceptionInfo("OpenAL initialization generated error" + dgConvertAlErrorToString(error).c_str() + " before buffer generation.");
        else 
            throw Dragon::ALInitializationError() << Dragon::ExceptionInfo("OpenAL initialization generated an error before buffer generation.");
    }

    alGenBuffers(NUM_BUFFERS, g_Buffers);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED))
            throw Dragon::ALInitializationError() << Dragon::ExceptionInfo("OpenAL initialization generated error" + dgConvertAlErrorToString(error).c_str() + " when generating buffers.");
        else 
            throw Dragon::ALInitializationError() << Dragon::ExceptionInfo("OpenAL initialization generated an error when generating buffers.");
    }
}


/// @brief Initializes DragonEngine fully. Uses hints set from before this function is called.
/// @param appName The name of the window for GLFW, and the app for Vulkan.
DGAPI void Dragon::init(std::string appName) {
    // Assorted Dragon Setup methods
    Dragon::Engine::appName = appName;
    VkApplicationInfo appInfo{};
    VkCreateInfo createInfo{};
    setupGLFW();
    setupVulkan(&createInfo);
    setupExtensions();

    Dragon::Engine::gpus = Dragon::getGPUs();

    setupOpenAL();
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
