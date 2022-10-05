#include <dragon/dragon.hpp>

VkInstance dgEngine::vkInstance;

DGAPI VKAPI_ATTR VkBool32 VKAPI_CALL dgVkValidationLayerDebugCalback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData
) {
	printf("Vulkan Debug Message\n--------------------\n\tMessage Type: ");
	
	switch(messageType) {
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			printf("General ");
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			printf("Specification-Violating ");
			break;
		default:
			printf("Non-Optimal ");
	};

	switch(messageSeverity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			printf("Verbose Message");
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			printf("Informational Message");
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			printf("Warning Message");
			break;
		default:
			printf("Error Message");
	};
	printf("\n\tMessage: %s\n", callbackData->pMessage);
	printf("\tMessage Influenced %i objects\n", callbackData->objectCount);

	return DG_FALSE;
}

DGAPI VkResult dgCreateDebugUtilsMessengerEXT(
	VkInstance instance, 
	const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
	const VkAllocationCallbacks* allocator,
	VkDebugUtilsMessengerEXT* debugMessenger
) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(dgEngine::vkInstance, "vkCreateDebugUtilsMessengerEXT"));
	if(func != nullptr) {
		return func(instance, createInfo, allocator, debugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

DGAPI void dgDestroyDebugUtilsMessengerEXT(
	VkInstance instance, 
	VkDebugUtilsMessengerEXT debugMessenger, 
	const VkAllocationCallbacks* allocator
) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(dgEngine::vkInstance, "vkDestroyDebugUtilsMessengerEXT"));
	if(func != nullptr) {
		func(instance, debugMessenger, allocator);
	}
}

DGAPI std::string dgConvertVkResultToString(VkResult result) {
	switch(result) {
		case VK_SUCCESS: 												return "VK_SUCCESS";
		case VK_NOT_READY: 												return "VK_NOT_READY";
		case VK_TIMEOUT: 												return "VK_TIMEOUT";
		case VK_EVENT_SET: 												return "VK_EVENT_SET";
		case VK_EVENT_RESET: 											return "VK_EVENT_RESET";
		case VK_INCOMPLETE: 											return "VK_INCOMPLETE";
		case VK_ERROR_OUT_OF_HOST_MEMORY: 								return "VK_ERROR_OUT_OF_HOST_MEMORY";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: 							return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		case VK_ERROR_INITIALIZATION_FAILED: 							return "VK_ERROR_INITIALIZATION_FAILED";
		case VK_ERROR_DEVICE_LOST: 										return "VK_ERROR_DEVICE_LOST";
		case VK_ERROR_MEMORY_MAP_FAILED: 								return "VK_ERROR_MEMORY_MAP_FAILED";
		case VK_ERROR_LAYER_NOT_PRESENT: 								return "VK_ERROR_LAYER_NOT_PRESENT";
		case VK_ERROR_EXTENSION_NOT_PRESENT: 							return "VK_ERROR_EXTENSION_NOT_PRESENT";
		case VK_ERROR_FEATURE_NOT_PRESENT: 								return "VK_ERROR_FEATURE_NOT_PRESENT";
		case VK_ERROR_INCOMPATIBLE_DRIVER:								return "VK_ERROR_INCOMPATIBLE_DRIVER";
		case VK_ERROR_TOO_MANY_OBJECTS:									return "VK_ERROR_TOO_MANY_OBJECTS";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:								return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		case VK_ERROR_FRAGMENTED_POOL:									return "VK_ERROR_FRAGMENTED_POOL";
		#ifdef VK_VERSION_1_1
			case VK_ERROR_OUT_OF_POOL_MEMORY:							return "VK_ERROR_OUT_OF_POOL_MEMORY";
			case VK_ERROR_INVALID_EXTERNAL_HANDLE:						return "VK_ERROR_INVALID_EXTERNAL_HANDLE";

			#ifdef VK_VERSION_1_2
				case VK_ERROR_FRAGMENTATION: 							return "VK_ERROR_FRAGMENTATION";
				case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:			return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";

				#ifdef VK_VERSION_1_3
					case VK_PIPELINE_COMPILE_REQUIRED:					return "VK_PIPELINE_COMPILE_REQUIRED";
				#endif
			#endif
		#endif

		#ifdef VK_ENABLE_BETA_EXTENSIONS
			case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:				return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
			case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:		return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
			case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:	return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
			case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:		return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
			case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:		return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
			case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:			return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
		#endif
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:		return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
		//case VK_ERROR_NOT_PERMITTED_KHR:								return "VK_ERROR_NOT_PERMITTED_KHR";
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:				return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
		case VK_THREAD_IDLE_KHR:										return "VK_THREAD_IDLE_KHR";
		case VK_THREAD_DONE_KHR:										return "VK_THREAD_DONE_KHR";
		case VK_OPERATION_DEFERRED_KHR:									return "VK_OPERATION_DEFERRED_KHR";
		case VK_OPERATION_NOT_DEFERRED_KHR:								return "VK_OPERATION_NOT_DEFERRED_KHR";
		default:														return "VK_ERROR_UNKNOWN";
	};
}