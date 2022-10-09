#include <dragon/dragon.hpp>

DGAPI DG_BOOL dgIsIronbreathEnabled() {
	#ifdef DRAGON_IRONBREATH_ENABLED
		return DG_TRUE;
	#else
		return DG_FALSE;
	#endif
}

DGAPI DG_BOOL dgIsLightbreathEnabled() {
	#ifdef DRAGON_LIGHTBREATH_ENABLED
		return DG_TRUE;
	#else
		return DG_FALSE;
	#endif
}

DGAPI DG_BOOL dgIsStreambreathEnabled() {
	#ifdef DRAGON_STREAMBREATH_ENABLED
		return DG_TRUE;
	#else
		return DG_FALSE;
	#endif
}