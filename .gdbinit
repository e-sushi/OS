define refsrc
    file kershi.elf
    dir
end

define connect
    target remote localhost:1234
end

connect
refsrc
