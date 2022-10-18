#include <dragon/dragon.hpp>

DGAPI Dragon::setOption(const uint32_t option, DgBool32 value) {
	switch(option) {
		case DRAGON_IRONBREATH_ENABLED: 
			Dragon::Options::ExtensionOptions::ironEnabled = value;
			break;
		case DRAGON_LIGHTBREATH_ENABLED:
			Dragon::Options::ExtensionOptions::lightEnabled = value;
			break;
		case DRAGON_STREAMBREATH_ENABLED:
			Dragon::Options::ExtensionOptions::streamEnabled = value;
			break;
		case DRAGON_THUNDERBREATH_ENABLED:
			Dragon::Options::ExtensionOptions::thunderEnabled = value;
			break;
		default:
			return;
	}
}

DGAPI DgBool32 Dragon::getOption(const uint32_t option) {
	switch(option) {
		case DRAGON_IRONBREATH_ENABLED: 
			return Dragon::Options::ExtensionOptions::ironEnabled;
		case DRAGON_LIGHTBREATH_ENABLED:
			return Dragon::Options::ExtensionOptions::lightEnabled;
		case DRAGON_STREAMBREATH_ENABLED:
			return Dragon::Options::ExtensionOptions::streamEnabled;
		case DRAGON_THUNDERBREATH_ENABLED:
			return Dragon::Options::ExtensionOptions::thunderEnabled;
		default:
			return DG_FALSE;
	}
}