#!/usr/bin/perl -w

use IO::File;
use Net::GitHub::V3;
use String::ShellQuote;

sub ConcatDir {
  my (@files) = (@_);
  my @new = @files;
  my $result;
  foreach my $file (@new) {
    $file =~ s/^\/+//;
    $file =~ s/\/+$//;
  }
  $result = join("/",@new);
  if ($files[0] =~ /^\//) {
    $result = "/$result";
  }
  if ($files[$#files] =~ /\/$/) {
    $result .= "/";
  }
  $result =~ s/.*\/\/+/\//;
  return $result;
}

my $password = `cat github-cred.txt`;
chomp $password;

my $gh = Net::GitHub::V3->new(login => 'aindilis', pass => $password);

my $name;
my $fh;

my $repos;
my $user;
my $searchresultuser;
my @userrepos;

my $search;
my $topic;
my $searchresulttopic;
my @topicrepos;

$user = 'aindilis';
$repos = $gh->repos;
$searchresultuser = $repos->list_user($user);

if (defined $user) {
  $name = $user;
  $name =~ s/\s+/-/sg;

  $fh = IO::File->new();
  if ($fh->open('>'.ConcatDir($datadir,"result-$name.dat"))) {
    print $fh Dumper($searchresultuser);
    $fh->close;
  }

  foreach my $entry (@$searchresultuser) {
    push @userrepos, $entry->{clone_url};
  }

  $fh = IO::File->new();
  if ($fh->open('>'.ConcatDir($datadir,"repos-$name.txt"))) {
    print $fh Dumper(\@userrepos);
    $fh->close;
  }

  DownloadRepos
    (
     Repos => \@userrepos,
     Dir => $name,
    );
}

sub DownloadRepos {
  my (%args) = @_;
  my @repos = @{$args{Repos}};

  my $dir = $args{Dir};
  my $gitrepodir = ConcatDir("./panoply-github",$dir);
  my $c = 'mkdir -p '.shell_quote($gitrepodir);
  print "$c\n";
  system $c;

  my $i = 0;
  foreach my $line (@repos) {
    ++$i;
    print "Checking $i) <$line>\n";
    if ($line =~ /\/([^\/]+)\/([^\/]+)$/) {
      my $account = $1;
      my $reponame = $2;
      $reponame =~ s/\.git$//sg;
      print "\tHas $account : $reponame\n";
      if (! -d "$gitrepodir/$account") {
	my $command = 'mkdir '.shell_quote("$gitrepodir/$account");
	print $command."\n";
	system $command;
      }
      if (-d "$gitrepodir/$account/$reponame") { 
	print "Already got: $account : $reponame\n";
	my $command = 'cd '.shell_quote("$gitrepodir/$account").' && git pull';
      } else {
	my $command = 'cd '.shell_quote("$gitrepodir/$account").' && git clone '.shell_quote($line);
	print $command."\n";
	system $command;
      } 
    } else {
      print "NO! $line\n";
    }
  }
}
