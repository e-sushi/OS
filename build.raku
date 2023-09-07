my $osname = "sushos";

my $echo-commands = False;

my $bdir = "./build/";
my $as = "i686-elf-as";

my $boot-asm = "boot.s";
my $boot-out = $bdir~"boot.o";

my $gcc = "i686-elf-g++";

my @gcc-flags = [
    "-ffreestanding", # this program cannot use any standard libraries
    "-fno-exceptions",
    "-fno-rtti",
    "-fpermissive",
    "-fdiagnostics-plain-output",
    "-fdiagnostics-color=always",
    "-w",
    "-O0",            # we gotta be quick 
    "-g",
];

my $kernel-src = "kernel.cpp";
my $kernel-out = $bdir~"kernel.o";

my $linker-script = "linker.ld";

my @linker-flags = [
    "-ffreestanding",
    "-O0",
    "-nostdlib",
    "-static-libgcc",
    
];

my $os-out = "$osname.bin";



# ---- assemble 

my @assemble-cmd = $as, $boot-asm, '-o', $boot-out;
"assembling $boot-asm with command '{@assemble-cmd}'".say if $echo-commands;

my $assemble-proc = run @assemble-cmd, :out, :merge;
my $assemble-out = $assemble-proc.out.slurp(:close);
unless $assemble-proc {
    "assembly failed with messages: \n$assemble-out".say and die;
}

"assembly succeeded".say;


# ---- compile source

my @compile-cmd = [$gcc, '-c', $kernel-src, '-o', $kernel-out].append: @gcc-flags;
"compiling $kernel-src with command '{@compile-cmd}'".say if $echo-commands;

my $compile-proc = run @compile-cmd, :out, :merge;
my $compile-out = $compile-proc.out.slurp(:close);
unless $compile-proc {
    "compiling failed with messages: \n$compile-out".say and die;
}

"compiling succeeded".say;


# ---- linking kernel

my @linker-cmd = [$gcc, '-T', $linker-script, '-o', $os-out].append: @linker-flags.append: [$boot-out, $kernel-out, "-lgcc"];
"linking kernel with command '{@linker-cmd}'".say if $echo-commands;

my $link-proc = run @linker-cmd, :out, :merge;
my $link-out = $link-proc.out.slurp(:close);
unless $link-proc {
    "linking failed with messages: \n$link-out".say and die;
}

"linking succeeded".say;


# ---- verify multiboot

my @cmd = ['grub-file', '--is-x86-multiboot2', $os-out];
"verifying multiboot with command {@cmd}".say if $echo-commands;

my $grub-proc = run @cmd, :out, :merge;
unless $grub-proc {
    "the binary is not multiboot!".say and die;
}

"successfully confirmed multiboot".say;


# ---- build cdrom

mkdir 'iso/boot/grub';

"iso/boot/grub/grub.cfg".IO.spurt: qq:to/END/;
    set timeout=0
    set default=0
    menuentry "$osname" \{
        multiboot2 /boot/$os-out
    \}
    END

copy $os-out, 'iso/boot/' ~ $os-out;

my $iso-proc = run 'grub-mkrescue', '-o', "$osname.iso", 'iso', :out, :merge;
my $iso-out = $iso-proc.out.slurp: :close;
unless $iso-proc {
    "creating iso image failed with messages: \n$iso-out".say and die;
}

"iso image created succesfully!".say;