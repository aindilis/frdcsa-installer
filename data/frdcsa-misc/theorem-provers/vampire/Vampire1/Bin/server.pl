#!/usr/bin/perl

use Socket;
use IPC::Open2;
use Time::HiRes qw (usleep);
use POSIX qw(:sys_wait_h);


my $port = 5001;
my $kifVampire = "/var/lib/myfrdcsa/codebases/internal/freekbs2/data/theorem-provers/vampire/Vampire1/Bin/kif";
#my $sumoKB = "/opt/KIF/Merge.txt";
my $sumoKB = "/var/lib/myfrdcsa/codebases/internal/freekbs2/data/theorem-provers/vampire/Vampire1/Bin/Merge1.txt";
my $maxCLients = 5;

# additional function to reap dead children
sub reaper {
  1 until (-1 == waitpid(-1,WNOHANG));
  $SIG{CHLD} = \&reaper;
}				# reaper

$SIG{CHLD} = \&reaper;

# current client number
my $clientNumber = 0;

# make the socket
socket (SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp'))
  or die "No Socket!\n";

#so we can restart the server quickly
setsockopt (SERVER, SOL_SOCKET, SO_REUSEADDR, 1);

# build up my socket address
my $myAddress = sockaddr_in ($port, INADDR_ANY);
bind (SERVER, $myAddress)
  or die "Cannot bind to port $port: $!\n";

# estabich a queue for incoming connections
listen (SERVER, $maxClients)
  or die "Cannot listen to port $port: $!\n";

#disable buffering for debugging
$| = 1;

# main loop
while (1) {
  while (my $client = accept(CLIENT,SERVER)) {
    $clientNumber++;
    if ($pid = fork) {		# parent
      close CLIENT;
      next;
    }

    die "fork problem: $!\n" unless defined $pid;
    open2(*KIF, *VAMPIRE, "$kifVampire $sumoKB");

    if (my $pid = fork) {
      close SERVER;

      # this process will redirect from CLIENT to VAMPIRE
      while (my $line = <CLIENT>) {
	print VAMPIRE $line;
	last if ($line =~ m/\A\<bye\/\>/);
      }
      close CLIENT;
      close VAMPIRE;
      exit;
    }

    close VAMPIRE;		# no use to this process

    # child process
    die "fork problem: $!\n" unless defined $pid;
    close SERVER;		# no use to child

    print "New client: $clientNumber\n";
    # open $kifVampire who reads from CLIENT and writes into KIF
    while (my $line = <KIF>) {
      # replace the next print statement by
      print $line;
      # to make the server output to stdout instead of to the client
      # one can also output both to the client and to stdout
      send (CLIENT, $line, 0)
	or die("Could not send: $!\n");
    }
    close CLIENT;
    exit;			# child must exit
  }
  print "WAITING\n";
  usleep(10000);
}
print "CLOSING\n";

close SERVER;
