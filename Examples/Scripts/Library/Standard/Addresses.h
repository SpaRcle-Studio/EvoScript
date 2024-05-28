#ifndef EVOSCRIPTLIB_ADDRESSES_H
#define EVOSCRIPTLIB_ADDRESSES_H

#ifdef __GNUC__
	#define EXTERN __attribute__((visibility("default")))
#else
	#define EXTERN extern "C" __declspec(dllexport)
#endif

void** g_methodPointers; 

EXTERN void Init(void** methodPointers) {
	g_methodPointers = methodPointers;
}

#endif
