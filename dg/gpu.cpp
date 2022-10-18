#include <dragon/dragon.hpp>

uint32_t Dragon::engine::gpuCount;

DGAPI std::vector<Dragon::GPU> Dragon::getGPUs() {
	vkEnumeratePhysicalDevices(Dragon::engine::vkInstance, &Dragon::engine::gpuCount, nullptr);
	if(Dragon::engine::gpuCount == 0) {
		throw Dragon::VulkanNoPhysicalDevicesFoundException() << Dragon::ExceptionInfo("No VkPhysicalDevices Found");
	}
	std::vector<VkPhysicalDevice> devices(Dragon::engine::gpuCount);
	vkEnumeratePhysicalDevices(Dragon::engine::vkInstance, &Dragon::engine::gpuCount, devices.data());
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

DGAPI int dgRateGPU(Dragon::GPU gpu, Dragon::GPURequirements reqs) {
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