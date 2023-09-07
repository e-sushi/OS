my $n_chars = 0;

$_ = "gohufont-11.bdf".IO.slurp;



$n_chars = m/<?after CHARS\s*>\d+/.Int;

my @chars = m:g/<?after STARTCHAR\s*>.*?<?before ENDCHAR>/;

class Char {
    has $.name is rw;
    has $.encoding is rw;
    has $.swidth is rw;
    has $.dwidth is rw;
    has $.bbx is rw;
    has $.bitmap is rw;
}

my @processed_chars = [];

for @chars -> $char {
    my Char $c = Char.new;
    $c.name = ($char ~~ m/<?after \s*>.*?<?before \n>/).Str.trim;
    $c.encoding = ($char ~~ m/<?after ENCODING\s*>\d+<?before \n>/).Int;
    my $bbx = ($char ~~ m/<?after BBX\s*> [(\-?\d+?)\s*?] ** 4 <?before \n>/).list;
    $c.bbx = ($bbx[0][0].Int, $bbx[0][1].Int, $bbx[0][2].Int, $bbx[0][3].Int);
    my $bm = $char ~~ m/<?after BITMAP\s*>.*/;
    next unless $bm ~~ /\N/; 
    $c.bitmap = Blob.new($bm.Str.split("\n").map(*.trim).grep(* ~~ /\N/).map(("0x" ~ *).Int));
    @processed_chars.push: $c;
}

for @processed_chars -> $char {
    
}