#include "common.h"

u8* buffer = (u8*)0xffff;
u32 chars_printed = 0;

struct Str {
    u8* str;
    u32 count;
};

struct Hex {
    u32 n;
    Hex(u32 n) : n(n) {};
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
    u32 count = 0;
    while(1) {
        if(!x) break;
        x /= 10;
        count++;
    }

    return count;
}

Str to_str(mem* m, const char* c) {
    Str s = {m->current, 0};
    s.count = strlen(c);
    for(u32 i = 0; i < s.count; i++) {
        *(s.str + i) = c[i];
    }
    m->current += s.count;
    return s;
}

Str to_str(mem* m, s32 x) {
    Str s = {m->current, 0};
    s64 temp = x;
    u8* p = m->current;

    while(1) {
        s64 remainder = temp % 10;
        *p = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        p++;
        temp /= 10;
        if(!temp) break;
    }

    s.count = p - s.str;

    u8* p1 = s.str;
    u8* p2 = s.str + s.count - 1;
    while(p1 < p2) {
        u8 tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    
    }
    m->current += s.count;
    return s;
}

Str to_str(mem* m, Hex x) {
    Str s = {m->current, 0};
    s64 temp = x.n;
    u8* p = m->current;

    while(1) {
        s64 remainder = temp % 16;
        *p = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        p++;
        temp /= 16;
        if(!temp) break;
    }

    s.count = p - s.str;

    u8* p1 = s.str;
    u8* p2 = s.str + s.count - 1;
    while(p1 < p2) {
        u8 tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    
    }
    m->current += s.count;
    return s;
}

template<typename... T>
void print(mem* m, T... args) {
    (to_str(m, args), ...);
}