#ifndef __SFMLIGHT_CONFIG_H__
#define __SFMLIGHT_CONFIG_H__

#if defined(_MSC_VER)
	#define SFMLIGHT_EXPORT __declspec(dllexport)
	#define SFMLIGHT_IMPORT __declspec(dllimport)
#elif __GNUC__ >= 4
	#define SFMLIGHT_EXPORT __attribute__ ((__visibility__ ("default")))
	#define SFMLIGHT_IMPORT __attribute__ ((__visibility__ ("default")))
#else
	#define SFMLIGHT_EXPORT
	#define SFMLIGHT_IMPORT
#endif


#endif //__SFMLIGHT_CONFIG_H__