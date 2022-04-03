#
# Run this script once and you are all set!
#
# modefied by maeda@lib.u-tokyo.ac.jp
#                          2003.08.19

use strict;
use File::Path;
use File::Basename;
require './jcode.pl';
# set file name for convert EUC to Shift-JIS
my @convert_kanji_code = qw(TermExtract/Chasen.pm TermExtract/MeCab.pm);

my $install_dir = "";

for my $INC (@INC){
    if ($INC =~ /site/io){
	$install_dir = $INC;
	last;
    }
}

unless ($install_dir) {
    die qq(Please create 'site/lib' folder in ActivePerl folder!\n);
}else{
    warn "Ok. I'll install files in $install_dir folder.\n";
}

my @files = qw(
	       );

# push all files in ExJPTerm directory;

require "find.pl";
chdir '../';
&find('TermExtract');

sub wanted {
    no strict "vars";
    my ($dev,$ino,$mode,$nlink,$uid,$gid) = lstat($_);
    return unless -f _;
    push @files, $name;
}


for my $file (@files) {
    my $src = $file;
    open SRC, $src or die "$src:$!";

    my $dst = $install_dir . "/" . $file;
    warn "Installing $src to $dst.\n";

    my $dir = dirname($dst);
    mkpath([$dir], 0755, 1) or die "$dir:$!" unless -d $dir;
    open DST, ">$dst" or die "$dst:$!";

    my $kanji_mode = 0;
    foreach my $file (@convert_kanji_code) {
        $kanji_mode = 1 if $file eq $src;
    }

    while(defined ($_ = <SRC>)){
        chomp $_;
        &jcode'convert(*_, 'sjis', 'euc') if $kanji_mode;
	print DST $_, "\n";
    }

    close SRC; close DST;
    # MacPerl::SetFileInfo('McPL', 'TEXT', $dst);

}

warn "All scripts are installed successfully\n";

__END__
