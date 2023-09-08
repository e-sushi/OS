my $kernel = "kershi";
my $kernel-bin = "$kernel.elf";
my $kernel-iso = "$kernel.iso";

my $echo-commands = False;

my $bdir = "./build/";
my $as-cmd = "nasm boot.s -felf64 -g -o $bdir/boot.o";

my $boot-asm = "boot.s";
my $boot-out = $bdir~"boot.o";

my $gcc = "x86_64-elf-g++";
my $ld = "x86_64-elf-ld";

my @user-nasm-flags = [
    "-F", "dwarf", # output debug info in dwarf format
    "-g", # output debug info
];

my @user-gcc-flags = [
    "-g",
    "-O0",
];

my @user-link-flags = [

];

sub prefix:<`>(*@a) {
    with run @a, :out, :merge {
        unless .so {
            ("failed with messages: \n" ~
                .out.slurp(:close)).say and die;
        }
        # say the output of the program if there's any non-whitespace text in it 
        .say if /\S/ given "\n" ~ .out.slurp(:close).indent(4);
    } 
}

# the rest of the flags should generally be left alone 

my @gcc-flags = [
    "-w", # disable if something is going wrong idk man gcc sucks and gives too many warnings but ill probably eat these words later when i spend like 5 hours trying to figure out some bug that gcc was warning me about the whole time
    "-ffreestanding", 
    "-fno-stack-protector",
    "-fno-stack-check",
    "-fno-lto",
    "-fPIE",
    "-m64",
    "-march=x86-64",
    "-mno-80387",
    "-mno-mmx",
    "-mno-sse",
    "-mno-sse2",
    "-mno-red-zone",
    "-fno-exceptions",
    "-fno-rtti",
    "-fpermissive",
    "-fdiagnostics-plain-output",
    "-fdiagnostics-color=always",
    "-std=c++20",
];

my @cpp-flags = [
    "-I.",
    "-MMD",
    "-MP",
];

my @linker-flags = [
    "-m", "elf_x86_64",
    "-nostdlib",
    "-static",
    "-pie",
    "--no-dynamic-linker",
    "-z", "text",
    "-z", "max-page-size=0x1000",
    "-T", "linker.ld",
    "-g",
];

my @nasm-flags = [
    "-Wall",
    "-f", "elf64"
];

my $as-files = [];
my $nasm-files = [];

my $obj = ".o";

"compiling...".print;
`($gcc, @gcc-flags, @user-gcc-flags, @cpp-flags, '-c', 'kernel.cpp', '-o', 'kernel.o');
"done".say;

"linking...".print;
`($ld, 'kernel.o', @linker-flags, @user-link-flags, '-o', $kernel-bin);
"done".say;

"limine.cfg".IO.spurt: qq:to/END/;
    TIMEOUT=0
    :myOS (kershi)
        PROTOCOL=limine
        KERNEL_PATH=boot:///$kernel-bin
    END

# if the limine directory doesn't exist we grab its stuff from git
# and build it 
unless "limine".IO.e {
    "gathering limine binary files from git...".print;
    `<git clone https://github.com/limine-bootloader/limine.git --branch=v5.x-branch-binary --depth=1>;
    "done".say;

    "builing limine...".print;
    `<make -C limine>;
    "done".say;
}

"creating ISO root and copying relevant files...".print;
`<mkdir -p iso>; # so fancy
`qq:w[
    cp -v $kernel-bin limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso/
];
"done".say;

"creating EFI boot tree and copying limine's efi exe over...".print;
`<mkdir -p iso/EFI/BOOT>;
`<cp -v limine/BOOTX64.EFI limine/BOOTIA32.EFI iso/EFI/BOOT>;
"done".say;

"creating the bootable ISO...".print;
`qq:w<xorriso -as mkisofs -b limine-bios-cd.bin 
          -no-emul-boot -boot-load-size 4 -boot-info-table
          --efi-boot limine-uefi-cd.bin
          -efi-boot-part --efi-boot-image --protective-msdos-label
          iso -o $kernel-iso>;
"done".say;

"installing limine stage 1 and 2 for legacy BIOS boot...".print;
`qq:w<limine/limine bios-install $kernel-iso>;
"done!".say;