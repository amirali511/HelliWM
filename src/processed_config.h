#include "config.h"

/*
	Using the config file
*/
#ifdef BG
	#if (BG == DEFAULT)
		#undef BG
		#define BG 0xeb8650
	#endif
#endif

#ifdef MOD
	#if (MOD == DEFAULT)
		#undef MOD
		#define MOD XCB_MOD_MASK_4
	#endif
#endif

#ifdef QK
	#if (QK == DEFAULT)
		#undef QK
		#define QK XK_Q
	#endif
#endif

#ifdef LK
	#if (LK == DEFAULT)
		#undef LK
		#define LK XK_R
	#endif
#endif

#ifdef CK
	#if (CK == DEFAULT)
		#undef CK
		#define CK XK_C
	#endif
#endif

#ifdef BBG
  #if (BBG == DEFAULT)
    #undef BBG
    #define BBG 0x000000
  #endif
#endif

#ifdef TC
  #if (TC == DEFAULT)
    #undef TC
    #define TC 0xffffff
  #endif
#endif

#ifdef TBG
	#if (TBG == DEFAULT)
		#undef TBG
		#define TBG 0x000000
	#endif
#endif
