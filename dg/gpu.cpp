#include <dragon/dragon.hpp>

uint32_t dgEngine::gpuCount;

DGAPI std::vector<GPU> dgGetGPUs() {
	vkEnumeratePhysicalDevices(dgEngine::vkInstance, &dgEngine::gpuCount, nullptr);
	if(dgEngine::gpuCount == 0) {
		throw dgVulkanNoPhysicalDevicesFoundException() << DgExceptionInfo("No VkPhysicalDevices Found");
	}
	std::vector<VkPhysicalDevice> devices(dgEngine::gpuCount);
	vkEnumeratePhysicalDevices(dgEngine::vkInstance, &dgEngine::gpuCount, devices.data());
	std::vector<GPU> gpus;
	for(VkPhysicalDevice handle : devices) {
		GPU gpu;
		gpu.handle = handle;
		vkGetPhysicalDeviceFeatures(handle, &gpu.features);
		vkGetPhysicalDeviceMemoryProperties(handle, &gpu.memoryProperties);
		vkGetPhysicalDeviceProperties(handle, &gpu.properties);
		gpus.push_back(gpu);
	}
	return gpus;
}

DGAPI int dgRateGPU(GPU gpu, DgGPURequirements reqs) {
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