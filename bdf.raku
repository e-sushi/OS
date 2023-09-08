$_ = "gohufont-11.bdf".IO.slurp; # load the file in as the topic

my $n_chars = m/<?after CHARS\s*>\d+/.Int;
my $font_bbx = (m/<?after FONTBOUNDINGBOX\s*> [(\-?\d+?)\s*?] ** 4 <?before \n>/).split(" ").map: *.Int;
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

my $bits_array = "static const u32 font[] = \{\n\t";

for @processed_chars -> $char {
    $bits_array ~= "// {$char.name} \n\t";
    my $bottom = $char.bbx[3];
    my $top = $bottom + $char.bbx[1];
    my $left = $char.bbx[2];
    my $right = $left + $char.bbx[0];
    $font_bbx.say;
    for $font_bbx[1]...0 -> $row {
        my $y = $row + $font_bbx[3];
        for 0..$font_bbx[0] -> $column {
            my $x = $column + $font_bbx[2];
            if $bottom <= $y < $top and $left <= $x < $right {
                say "{$char.name}";
                say "Y: $bottom < $y < $top row: $row";
                say "X: $left < $x < $$right col: $column";
                say ($char.bitmap[$char.bbx[1] - $y - 1 + $bottom] +& (1 +< (($x - $left)))).so;
                if $char.bitmap[$char.bbx[1] - $y - 1 + $bottom] +& (1 +< (7 - ($x - $left))) {
                    $bits_array ~= "0xffffff, ";
                } else {
                    $bits_array ~= "0x000000, ";
                }
            }  else {
                $bits_array ~= "0x000000, ";
            }
        }
        $bits_array ~= "\n\t";
    }
}

"out".IO.spurt: $bits_array;