#include <dragon/dragon.hpp>

uint32_t Dragon::Engine::gpuCount;

DGAPI std::vector<Dragon::GPU> Dragon::getGPUs() {
	vkEnumeratePhysicalDevices(Dragon::Engine::vkInstance, &Dragon::Engine::gpuCount, nullptr);
	if(Dragon::Engine::gpuCount == 0) {
		if(Dragon::getOption(DRAGON_STREAMBREATH_ENABLED)) {
			Dragon::Error::ErrorInfo info{};
			info.code = "3.3.0.1";
			info.message = "No valid VkPhysicalDevice found on this system";
			Dragon::Stream::throwError(info);
		}
		return std::vector<Dragon::GPU>();
	}
	std::vector<VkPhysicalDevice> devices(Dragon::Engine::gpuCount);
	vkEnumeratePhysicalDevices(Dragon::Engine::vkInstance, &Dragon::Engine::gpuCount, devices.data());
	std::vector<Dragon::GPU> gpus;
	for(VkPhysicalDevice handle : devices) {
		Dragon::GPU gpu;
		gpu.handle = handle;
		vkGetPhysicalDeviceFeatures(handle, &gpu.features);
		vkGetPhysicalDeviceMemoryProperties(handle, &gpu.memoryProperties);
		vkGetPhysicalDeviceProperties(handle, &gpu.properties);
		gpus.push_back(gpu);
	}
	return gpus;
}

DGAPI int dgRateGPU(Dragon::GPU gpu) {
	int score = 1000;

	switch(gpu.properties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER: break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score -= 250;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: score += 1000;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: score -= 500;
		default: return 0;
	};

	return score;
}