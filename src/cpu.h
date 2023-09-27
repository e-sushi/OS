#ifndef KERNEL_CPU_H
#define KERNEL_CPU_H

#include "common.h"

extern "C" u64 get_cpu_features(void);
extern "C" void enable_sse(void);

namespace cpu_feat{
enum class ecx {
    sse3         = 1 << 0, 
    pclmul       = 1 << 1,
    dtes64       = 1 << 2,
    monitor      = 1 << 3,  
    ds_cpl       = 1 << 4,  
    vmx          = 1 << 5,  
    smx          = 1 << 6,  
    est          = 1 << 7,  
    tm2          = 1 << 8,  
    ssse3        = 1 << 9,  
    cid          = 1 << 10,
    sdbg         = 1 << 11,
    fma          = 1 << 12,
    cx16         = 1 << 13, 
    xtpr         = 1 << 14, 
    pdcm         = 1 << 15, 
    pcid         = 1 << 17, 
    dca          = 1 << 18, 
    sse4_1       = 1 << 19, 
    sse4_2       = 1 << 20, 
    x2apic       = 1 << 21, 
    movbe        = 1 << 22, 
    popcnt       = 1 << 23, 
    tsc          = 1 << 24, 
    aes          = 1 << 25, 
    xsave        = 1 << 26, 
    osxsave      = 1 << 27, 
    avx          = 1 << 28,
    f16c         = 1 << 29,
    rdrand       = 1 << 30,
    hypervisor   = 1 << 31,
};
enum class edx {
    fpu          = 1 << 0,  
    vme          = 1 << 1,  
    de           = 1 << 2,  
    pse          = 1 << 3,  
    tsc          = 1 << 4,  
    msr          = 1 << 5,  
    pae          = 1 << 6,  
    mce          = 1 << 7,  
    cx8          = 1 << 8,  
    apic         = 1 << 9,  
    sep          = 1 << 11, 
    mtrr         = 1 << 12, 
    pge          = 1 << 13, 
    mca          = 1 << 14, 
    cmov         = 1 << 15, 
    pat          = 1 << 16, 
    pse36        = 1 << 17, 
    psn          = 1 << 18, 
    clflush      = 1 << 19, 
    ds           = 1 << 21, 
    acpi         = 1 << 22, 
    mmx          = 1 << 23, 
    fxsr         = 1 << 24, 
    sse          = 1 << 25, 
    sse2         = 1 << 26, 
    ss           = 1 << 27, 
    htt          = 1 << 28, 
    tm           = 1 << 29, 
    ia64         = 1 << 30,
    pbe          = 1 << 31
};
} // namespace cpu_feat



void
list_cpu_features();

#endif // KERNEL_CPU_H
