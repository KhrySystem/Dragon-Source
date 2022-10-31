#include <dragon/dragon.hpp>

using namespace Dragon;

DGAPI DgBool32 createEngine(Engine* engine, std::string name) {
	engine.name = name;
	// Graphics Init
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = VK_HEADER_VERSION_COMPLETE;
	appInfo.pApplicationName = name.c_str();
	appInfo.pEngineName = "DragonEngine";
	appInfo.engineVersion = DRAGON_VERSION;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensionNames;
	glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	
	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.ppEnabledExtensionNames = glfwExtensionNames;
	instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;
	VkResult result = vkCreateInstance(&createInfo, nullptr, &engine.graphics.vkInstance);
	


	return engine;
}

DGAPI void updateEngine(Engine* engine) {

}

DGAPI void canEngineBeTerminated(Engine* engine) {
	if(!engine->graphics.windows.empty()) {
		return DG_FALSE;
	}
	return DG_TRUE;
}

DGAPI void terminateEngine(Engine* engine) {
	vkDestroyInstance(engine->graphics.vkInstance, nullptr);
	glfwTerminate();
}