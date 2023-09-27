#define STRINGIZE_(a) #a
#define STRINGIZE(a) STRINGIZE_(a)
#define supportedx(feat) if(edx & (u32)cpu_feat::edx::feat) { kernel.serial.say(STRINGIZE(feat) " supported"); } else { kernel.serial.say(STRINGIZE(feat) " not supported"); }



void
list_cpu_features() {
    // u64 f = get_cpu_features();
    // u32 ecx = f >> 32;
    // u32 edx = f;

    // supportedx(fpu);
    // supportedx(vme);
    // supportedx(de);
    // supportedx(pse);
    // supportedx(tsc);
    // supportedx(msr);
    // supportedx(pae);
    // supportedx(mce);
    // supportedx(cx8);
    // supportedx(apic);
    // supportedx(sep);
    // supportedx(mtrr);
    // supportedx(pge);
    // supportedx(mca);
    // supportedx(cmov);
    // supportedx(pat);
    // supportedx(pse36);
    // supportedx(psn);
    // supportedx(clflush);
    // supportedx(ds);
    // supportedx(acpi);
    // supportedx(mmx);
    // supportedx(fxsr);
    // supportedx(sse);
    // supportedx(sse2);
    // supportedx(ss);
    // supportedx(htt);
    // supportedx(tm);
    // supportedx(ia64);
    // supportedx(pbe);

}