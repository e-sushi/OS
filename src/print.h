#ifndef KERNEL_PRINT_H
#define KERNEL_PRINT_H

#include "common.h"
#include "memory.h"
#include "font.h"


namespace fmt {

struct Hex {
    u64 n;
    b32 prefix;
    Hex(u64 n, b32 prefix = true) : n(n), prefix(prefix) {};
};

// formats a number of bytes to a human readable format, eg.
// 5120 bytes -> 5kb
struct MemorySize {
    u64 bytes;
    MemorySize(u64 bytes) : bytes(bytes) {}
};

} // namespace fmt

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

u32 str_count(fmt::Hex x) {
    u32 n = x.n;
    u32 count = 1;
    while(1) {
        if(!n) break;
        n /= 16;
        count++;
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

u32 itos(u8* buffer, s64 x) {
    s64 temp = x;
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
u32 itos(u8* buffer, fmt::Hex x) {
    u64 temp = x.n;

    u8* p = buffer;
    
    while(1) {
        u64 remainder = temp % 16;
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
        memory::move(buffer + 2, buffer, count);
        buffer[0] = '0';
        buffer[1] = 'x';
        count += 2;
    }

    return count;
}

u32 itos(u8* buffer, fmt::MemorySize ms) {
    if(ms.bytes < Kilobytes(1)) {
        u32 count = itos(buffer, ms.bytes);
        return count + to_str(buffer+count, "b");
    } else if(ms.bytes < Megabytes(1)) {
        u32 count = itos(buffer, ms.bytes / Kilobytes(1));
        return count + to_str(buffer+count, "kb");
    } else if(ms.bytes < Gigabytes(1)) {
        u32 count = itos(buffer, ms.bytes / Megabytes(1));
        return count + to_str(buffer+count, "mb");
    } else if(ms.bytes < Terabytes(1)) {
        u32 count = itos(buffer, ms.bytes / Gigabytes(1));
        return count + to_str(buffer+count, "gb");
    } else {
        u32 count = itos(buffer, ms.bytes / Terabytes(1));
        return count + to_str(buffer+count, "tb");
    }
}

// this sucks 

// u32 to_str(u8* buffer, u8 x)            { return itos(buffer, x); }
// u32 to_str(u8* buffer, u16 x)           { return itos(buffer, x); }
// u32 to_str(u8* buffer, u32 x)           { return itos(buffer, x); }
// s32 to_str(u8* buffer, unsigned long x) { return itos(buffer, x); }
// u32 to_str(u8* buffer, u64 x)           { return itos(buffer, x); }
// u32 to_str(u8* buffer, s8 x)            { return itos(buffer, x); }
// u32 to_str(u8* buffer, s16 x)           { return itos(buffer, x); }
// u32 to_str(u8* buffer, s32 x)           { return itos(buffer, x); }
// u32 to_str(u8* buffer, s64 x)           { return itos(buffer, x); }
// u32 to_str(u8* buffer, Hex x)           { return itos(buffer, x); }
// u32 to_str(u8* buffer, MemorySize x)    { return itos(buffer, x); }

// this will probably break
template<typename T>
u32 to_str(u8* buffer, T x) { return itos(buffer, x); }

template<typename... T> u32 
to_str(u8* buffer, T... args) {
    u32 out = 0;
    ((out += to_str(buffer + out, args)), ...);
    return out;
}


struct Printer {
    u8* buffer;
    u32 x, y;

    template<typename... T>
    u32 print(u8* buffer, graphics::fcolor col, T... args);
    
    template<typename... T>
    u32 print(T... args);

    template<typename... T>
    u32 print(graphics::fcolor col, T... args);

    template<typename... T>
    u32 error(u8* buffer, T... args);

    template<typename... T>
    u32 error(T... args);

    // clears it all 
    void
    clear_screen();

    Printer() {}
    Printer(u8* addr) : buffer(addr), x(0), y(0) {}
};


#endif // KERNEL_PRINT_H