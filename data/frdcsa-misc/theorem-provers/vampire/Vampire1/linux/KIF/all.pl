#!/usr/bin/perl

foreach $file (<*.xml>)
{
	print $file."/n";
	$fileres = $file . 'res';
	system("./client.pl < $file > $fileres");
}
