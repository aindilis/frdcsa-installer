#!/opt/perl/bin/perl

use IO::Socket;

my $port = 24;
my $kifVampire = "/home/voronkov/KIF/kif";

my $server = IO::Socket::INET::new ('LocalPort' => $port,
				    'Type' => SOCK_STREAM,
				    'Reuse' => 1,
				    'Listen' => 10)
             or die "Cannot be a TCP server on port $port";

while (my $client = $server->accept()) {
}

close $server;
