namespace graphics {

b32
initialize() {
    volatile static limine_framebuffer_request fbr = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
    };

    if(!fbr.response) {
        kernel.serial.error("limine framebuffer request failed");
        return false;
    }

    kernel.serial.say("limine framebuffer request responded with: ", fmt::Hex(fbr.response));

    auto response = fbr.response;

    kernel.serial.say("  there are ", response->framebuffer_count, " framebuffers");

    fori(response->framebuffer_count) {
        auto framebuffer = response->framebuffers[i];
        
        kernel.serial.say(
            "    ", i, ":\n ", 
            "         address: ", fmt::Hex(framebuffer->address), " \n", 
            "      resolution: ", framebuffer->width, "x", framebuffer->height, "\n",
            "           pitch: ", framebuffer->pitch, "bytes\n",
            "             bpp: ", framebuffer->bpp, "bits\n",
            "   ?memory model: ", framebuffer->memory_model, "\n",
            "     r mask size: ", fmt::Hex(framebuffer->red_mask_size), "\n",
            "    r mask shift: ", fmt::Hex(framebuffer->red_mask_shift), "\n",
            "     g mask size: ", fmt::Hex(framebuffer->green_mask_size), "\n",
            "    g mask shift: ", fmt::Hex(framebuffer->green_mask_shift), "\n",
            "     b mask size: ", fmt::Hex(framebuffer->blue_mask_size), "\n",
            "    b mask shift: ", fmt::Hex(framebuffer->blue_mask_shift), "\n"
            "   n video modes: ", framebuffer->mode_count, "\n"
        );

        kernel.graphics.framebuffer.address = framebuffer->address;
        kernel.graphics.framebuffer.width = framebuffer->width;
        kernel.graphics.framebuffer.height = framebuffer->height;
        kernel.graphics.framebuffer.pitch = framebuffer->pitch;
        kernel.graphics.framebuffer.bpp = framebuffer->bpp;
        kernel.graphics.framebuffer.memory_model = framebuffer->memory_model;
        kernel.graphics.framebuffer.red_mask_size = framebuffer->red_mask_size;
        kernel.graphics.framebuffer.red_mask_shift = framebuffer->red_mask_shift;
        kernel.graphics.framebuffer.green_mask_size = framebuffer->green_mask_size;
        kernel.graphics.framebuffer.green_mask_shift = framebuffer->green_mask_shift;
        kernel.graphics.framebuffer.blue_mask_size = framebuffer->blue_mask_size;
        kernel.graphics.framebuffer.blue_mask_shift = framebuffer->blue_mask_shift;
        break; // TODO(sushi) do something about multiple framebuffers
    }

    return true;
}


void
draw_character(u32 x, u32 y, u32* character_map, u32 width, u32 height, fcolor col) {
    auto fb = kernel.graphics.framebuffer;
    
    forx(yc, height) {
        for(u32 xc = 0; xc < width; xc += 4) {
            u32* px = (u32*)((u8*)fb.address + (y + yc) * fb.pitch + (x + xc) * fb.bpp / 8);
            u32* ch = character_map + xc + yc * width;
            asm volatile(
                "movups %1, %%xmm0\n"
                "movups %%xmm0, %0"
                :: "m" (*px), "m" (*ch)
            );
            *((u8*)px + 2) *= col.r;
            *((u8*)px + 1) *= col.g;
            *((u8*)px + 0) *= col.b;
        }
    }
}

} // namespace graphics 