#include <dragon/dragon.hpp>

using namespace Dragon;

DGAPI DgBool32 createEngine(Engine* engine, CreateInfo* createInfo) {
	// Message Init
	if(DRAGON_MESSAGE_ENABLED) {
		engine->message.callback = &createInfo->pCallback;
	}

	engine->name = createInfo->name;
	// Graphics Init
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = VK_HEADER_VERSION_COMPLETE;
	appInfo.pApplicationName = engine->name.c_str();
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
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &engine->graphics.vkInstance);
	if(result != VK_SUCCESS) {
		Message::Message message{};
		message.code = 0xFF11000000000000 + result;
		message.message = "vkCreateInstance failed with result " + Message::VkResultAsString(result);
		Message::sendMessage(&message);
	}


	return DG_TRUE;
}

DGAPI void updateEngine(Engine* engine) {

}

DGAPI DgBool32 canEngineBeTerminated(Engine* engine) {
	if(!engine->graphics.windows.empty()) {
		return DG_FALSE;
	}
	return DG_TRUE;
}

DGAPI void terminateEngine(Engine* engine) {
	vkDestroyInstance(engine->graphics.vkInstance, nullptr);
	glfwTerminate();
}