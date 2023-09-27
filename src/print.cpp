template<typename... T> u32 Printer::  
print(u8* buffer, graphics::fcolor col, T... args) {
    u32 out = 0;
    ((out += to_str(buffer + out, args)), ...);

    auto fb = kernel.graphics.framebuffer;

    for(u32 i = 0; i < out; i++) {
        if(buffer[i] == '\n') goto newline;
        graphics::draw_character(x, y, font + (buffer[i] - 32) * 11 * 6, 6, 11, col);
        if(x + 6 > fb.width) {
newline:
            x = 0;
            y += 11;
            if(y >= fb.height) {
                // we have to shift everything back a line 
                memory::copy(fb.address, fb.address + fb.pitch * 11, (fb.width * fb.height) * sizeof(u32) - fb.pitch*11);
                x = 0;
                y = fb.height - 11;
                memory::zero(fb.address + fb.pitch * (fb.height - 11), fb.pitch * 11 + fb.width * sizeof(u32));
            }
        } else x += 6;
    }

    return out;
}

template<typename... T> u32 Printer::
print(T... args) {
    return print(buffer, graphics::fcolor{1.0, 1.0, 1.0}, args...);
}

template<typename... T> u32 Printer::
print(graphics::fcolor col, T... args) {
    return print(buffer, col, args...);
}

template<typename... T> u32 Printer::
error(u8* buffer, T... args) {
    u32 out = print(buffer, graphics::fcolor{1.0, 0, 0}, "ERROR");
    out += print(": ", args...);
    return out;
}

template<typename... T> u32 Printer::
error(T... args) {
    return error(buffer, args...);
}

// clears it all 
void Printer::
clear_screen() {
    x = y = 0;
    graphics::clear_screen();
}
