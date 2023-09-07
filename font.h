#include "common.h"

#define W 0xff
const u8 o = 0x00;


struct Char {
    u8* array;
    u32 count;
};

u8* get_char(u8 c) {
    switch(c) {
        case ' ': {

        } break;
        case 'a': {
            u8 pixels[] = {
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, W, W, W, o,
                o, W, o, o, o, W,
                o, W, o, o, o, W,
                o, W, o, o, o, W,
                o, W, o, o, o, W,
                o, W, o, o, W, W,
                o, o, W, W, o, W,
            }; return pixels;
        } break;
        case 'b': {
            u8 pixels[] = {
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
                o, o, o, o, o, o,
            }; return pixels;
        } break;
    }
}