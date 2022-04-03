#!/usr/bin/perl

use Socket;

my $port = 242424;
my $remoteHost = '127.0.0.1';

# additional function to reap dead children
sub reaper {
  1 until (-1 == waitpid(-1,WNOHANG));
  $SIG{CHLD} = \&reaper;
} # reaper

$SIG{CHLD} = \&reaper;

# make the socket
socket (TO_SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp'))
  or die("Could not make a socket!\n");

$| = 1;

# disable buffering when writing to client
my $oldFileHandle = select (TO_SERVER);
$| = 1;
select ($oldFileHandle);

# address of the remote machine
my $internetAddr = inet_aton ($remoteHost)
  or die "Cannot convert $remoteHost to an internet address: $!\n";
my $paddr = sockaddr_in ($port,$internetAddr);

# connect
connect (TO_SERVER, $paddr)
  or die "Cannot connect to $remoteHost:$port: $!\n";

if (my $pid = fork) { # parent
  while (my $line = <TO_SERVER>) {
    print $line;
  }

  close TO_SERVER;
  exit;
}

while (my $line = <>) {
  send(TO_SERVER, $line, 0);
}

close TO_SERVER;


