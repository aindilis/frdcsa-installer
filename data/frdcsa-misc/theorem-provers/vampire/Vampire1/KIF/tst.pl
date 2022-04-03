#!/opt/perl/bin/perl

# for my $file (</home/voronkov/tptp/*/*+*.p>) {
for my $file (</home/voronkov/tptp/*/*.p>) {
  my $name = substr ($file, 24);
  substr ($name, -2) = "";
  system "vampire --casc on $file";
#  print "\n";
  print " $name\n";
}

