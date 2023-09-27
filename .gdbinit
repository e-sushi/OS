define refsrc
    file build/kershi.elf
    dir
end

define connect
    target remote localhost:1234
end

refsrc
connect
