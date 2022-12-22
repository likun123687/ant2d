#define STB_VORBIS_HEADER_ONLY
#include "third_party/stb_vorbis/stb_vorbis.c" /* Enables Vorbis decoding. */

#define MINIAUDIO_IMPLEMENTATION
// #define MA_NO_ENCODING
#include "third_party/miniaudio/miniaudio.h"

/* stb_vorbis implementation must come after the implementation of miniaudio. */
#undef STB_VORBIS_HEADER_ONLY
#include "third_party/stb_vorbis/stb_vorbis.c"