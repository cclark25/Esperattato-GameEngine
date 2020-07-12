#ifndef ESPERATTO_XM
#define ESPERATTO_XM

#include <iostream>
#include <fstream>
#include <memory>
#include <allegro5/allegro_audio.h>
#include <vector>
#include <thread>
#include <limits>
extern "C"
{
#include "../../Dependencies/libxm/include/xm.h"
#include "../../Dependencies/libxm/src/xm_internal.h"
}
using namespace std;

namespace XM
{
xm_context_t *create_context_from_file(uint32_t rate, const char *filename);
ALLEGRO_SAMPLE *copyToBuffer(xm_context_t *xm);
}

#endif