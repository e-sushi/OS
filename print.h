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
    b32 prefix;
    Hex(u32 n, b32 prefix = true) : n(n), prefix(prefix) {};
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

    // TODO(sushi) do this better somehow idk 
    if(x.prefix) {
        memory::copy(buffer + 2, buffer, count);
        buffer[0] = '0';
        buffer[1] = 'x';
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

struct Printer {
    u8* buffer;
    u32 x, y;

    template<typename... T>
    u32 print(u8* buffer, graphics::fcolor col, T... args) {
        u32 out = 0;
        ((out += to_str(buffer + out, args)), ...);

        multiboot::framebuffer_info* vid = multiboot::mi.addresses.framebuffer_info;

        for(u32 i = 0; i < out; i++) {
            if(buffer[i] == '\n') goto newline;
            graphics::draw_character(x, y, font + (buffer[i] - 32) * 11 * 6, 6, 11, col);
            if(x + 6 > vid->width) {
newline:
                x = 0;
                y += 11;
                if(y > vid->height) {
                    // we have to shift everything back a line 
                    memory::copy(vid->addr, vid->addr + vid->pitch * 11, (vid->width * vid->height - 6*11) * sizeof(u32));
                    x = 0;
                    y = vid->height - 11;
                    memory::zero(vid->addr + vid->pitch * (vid->height - 11), vid->pitch * 11 + vid->width * sizeof(u32));
                }
            } else x += 6;
        }

        return out;
    }
    
    template<typename... T>
    u32 print(T... args) {
        return print(buffer, graphics::fcolor{1.0, 1.0, 1.0}, args...);
    }

    template<typename... T>
    u32 print(graphics::fcolor col, T... args) {
        return print(buffer, col, args...);
    }

    template<typename... T>
    u32 error(u8* buffer, T... args) {
        u32 out = print(buffer, graphics::fcolor{1.0, 0, 0}, "ERROR");
        out += print(": ", args...);
        return out;
    }

    template<typename... T>
    u32 error(T... args) {
        return error(buffer, args...);
    }

    // clears it all 
    void
    clear_screen() {
        x = y = 0;
        graphics::clear_screen();
    }

    Printer() {}
    Printer(u8* addr) : buffer(addr), x(0), y(0) {}
};


// template<typename... T>
// u32 print(T... args) {
//     (to_str(args), ...);
// }

#endif // KERNEL_PRINT_H