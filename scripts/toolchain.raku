#
# A script for building and installing a custom tool chain for a given target
# 
# This only works on linux for now 
# TODO(sushi) some nice way to list what targets you can use instead of waiting for make binutils to fail if it's not valid
#
# TODO(sushi) finish this later... I want to work on the actual kernel rn.

# shorthand for executing a program 
# it kind of sucks I have to put this first but I suppose it makes sense from a compiling pov but whatever...
sub prefix:<`>(*@a) {
    # if $echo-commands {
    #     ("\n" ~ @a.join(" ")).indent(4).say;
    # }
    with run @a, :out, :merge {
        unless .so {
            ("\n {@a} failed with messages: \n" ~
                .out.slurp(:close)).say and exit;
        }
        # say the output of the program if there's any non-whitespace text in it 
        # if $echo-proc-output {
        #     .say if /\S/ given "\n" ~ .out.slurp(:close).indent(4);
        # }
    } 
}

sub MAIN(
    Str $target,          #= the target to build a tool chain for. A list of valid targets can be found in the binutils repo folder bfd 
    Str $toolchain-out,   #= the output location of the tools, typically a directory like ~/opt/cross
) {
    # first we check that we have all dependencies 
    "checking for missing dependencies...".print;
    my @missing-deps = look-for-deps();
    
    if @missing-deps {
        "\nthe following dependencies are missing: ".say;
        @missing-deps.join("\n").indent(4).say;
        exit;
    }

    "all there!".say;
    
    "starting toolchain build...".say;
    do-build($target, $toolchain-out);    
}

sub look-for-deps() { 
    my @missing-dependencies = [];

    unless qqx{ command -v gcc   } { @missing-dependencies.push: 'gcc'   }
    unless qqx{ command -v make  } { @missing-dependencies.push: 'make'  }
    unless qqx{ command -v bison } { @missing-dependencies.push: 'bison' }
    unless qqx{ command -v flex  } { @missing-dependencies.push: 'flex'  }
    unless qqx{ command -v tar   } { @missing-dependencies.push: 'tar'   }


    unless qqx{ ldconfig -p | grep gmp     } { @missing-dependencies.push: 'gmp' }
    unless qqx{ ldconfig -p | grep mpc     } { @missing-dependencies.push: 'mpc' }
    unless qqx{ ldconfig -p | grep mpfr    } { @missing-dependencies.push: 'mpfr' }
    
    # very poor man's check for this, I have no idea if this will actually work
    # in all cases
    unless "/usr/share/texinfo".IO.e { @missing-dependencies.push: 'texinfo' }

    return @missing-dependencies;
}

# we build the tool chain!
sub do-build($target, $toolchain-out) {

    'creating temp directory for stuff and moving into it...'.print; 
        `<mkdir -p tool-chain-temp>;
        chdir "tool-chain-temp";
    "done!".say;

    "retrieving binutils-2.41 (this may take a second)...".print;
        `<curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.xz>;
    "done".say;

    "extracting binutils...".print;
        `<tar -xf binutils-2.41.tar.xz>;
    "done".say;

    "moving into binutils...".print;
        chdir "binutils-2.41";
    "done".say;

    "creating build directory and running configure...".print;
        `<mkdir -p build>;
        chdir 'build';
        `<../configure>;



}






