#include "limine/limine.h"

volatile static void* limreqs[] = {
    &limine_framebuffer_request,
    0,
};