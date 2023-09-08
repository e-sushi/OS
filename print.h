#ifndef KERNEL_PRINT_H
#define KERNEL_PRINT_H

#include "common.h"
#include "memory.h"
#include "font.h"

struct Str {
    u8* str;
    u32 count;
};

struct Hex {
    u32 n;
    Hex(u32 n) : n(n) {};
};

template<typename T>
struct Stringify {
    T t;
    Stringify(T x) : t(x) {};
};

u32 strlen(const char* s) {
    u32 out = 0;
    char* scan = (char*)s;
    while(scan[out]) out++;
    return out;
}

u32 str_count(const char* c) {
    return strlen(c);
}

u32 str_count(s32 x) {
    u32 count = 1;
    while(1) {
        if(!x) break;
        x /= 10;
        count++;
    }
    return count;
}

u32 str_count(Hex x) {
    u32 n = x.n;
    u32 count = 1;
    while(1) {
        if(!n) break;
        n /= 16;
        count++;
    }
    return count;
}

u32 itos(u8* buffer, s32 x) {
    s32 temp = x;
    u8* p = buffer;

    while(1) {
        s32 remainder = temp % 10;
        *p = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        p++;
        temp /= 10;
        if(!temp) break;
    }

    u32 count = p - buffer;

    u8* p1 = buffer;
    u8* p2 = p - 1;
    while(p1 < p2) {
        u8 tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    
    }

    return count;
}

// converts a Hex formatted integer into a string 
// placed at the location pointed to by buffer
// which is assumed to have been preallocated.
// sets buffer->count to the amount of characters 
// printed
u32 itos(u8* buffer, Hex x) {
    s32 temp = x.n;
    u8* p = buffer;

    while(1) {
        s32 remainder = temp % 16;
        *p = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        p++;
        temp /= 16;
        if(!temp) break;
    }

    u32 count = p - buffer;

    u8* p1 = buffer;
    u8* p2 = p - 1;
    while(p1 < p2) {
        u8 tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    
    }

    return count;
}

u32 to_str(u8* buffer, const char* c) {
    u32 count = strlen(c);;
    memory::copy(buffer, (char*)c, count);
    return count;
}

u32 to_str(u8* buffer, char c) {
    memory::copy(buffer, &c, 1);
    return 1;
}

u32 to_str(u8* buffer, s32 x) { return itos(buffer, x); }
u32 to_str(u8* buffer, Hex x) { return itos(buffer, x); }


template<typename... T>
u32 print(u8* buffer, T... args) {
    u32 out = 0;
    ((out += to_str(buffer + out, args)), ...);

    multiboot::framebuffer_info* vid = multiboot::mi.addresses.framebuffer_info;

    static u32 x = 0, y = 0;

    for(u32 i = 0; i < out; i++) {
        graphics::draw_character(x % vid->width, y, font + (buffer[i] - 32) * 11 * 6, 6, 11);
        x += 6;
        if(x > vid->width) {
            x = 0;
            y += 11;
             if(y > vid->height) {
                y = 0;
            }
        }
    }

    return out;
}

template<typename... T>
u32 print(T... args) {
    (to_str(args), ...);
}

#endif // KERNEL_PRINT_H