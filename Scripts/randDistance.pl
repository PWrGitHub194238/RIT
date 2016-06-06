$minRange = int($ARGV[0]);
$maxRange = int($ARGV[1]);
$range = $maxRange - $minRange;
open(INPUT, "<".$ARGV[2]) or die $!;
open(OUTPUT, ">".$ARGV[3]) or die $!;
$seed = $ARGV[4];

print "Changing 9th DIMACS Implementation Challenge params...\nChange distance labels between nodes to random numbers...\n\nNew range:\t[$minRange\t:\t$maxRange]\nSource file:\t$ARGV[2]\nTarget file:\t$ARGV[3]\nSeed value:\t$seed\n\n\nStart processing...\n";

while($line = <INPUT>) {
	if ($line =~ /^(a (\d+) (\d+) (\d+))/) {	#tylko liczby całkowite, ignoruje część ułamkową kosztów
		if (rand() >= $seed) {
			$newCost = $4+int($minRange + rand($range));
			$out = "a ".$2." ".$3." ".$newCost."\n";
			print "[".$2."\t->\t".$3."]\tcost changed:\t".$4."\t=>\t".$newCost.".\n";
			print OUTPUT $out
		} else {
			print OUTPUT $line
		}

	} else {
		print OUTPUT $line
	}
}
print "Complete.\n";
close INPUT;
close OUTPUT;
