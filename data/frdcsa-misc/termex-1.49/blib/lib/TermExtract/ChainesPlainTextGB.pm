package TermExtract::ChainesPlainTextGB;
use TermExtract::Calc_Imp;

use strict;
use Exporter ();
use vars qw(@ISA $VERSION @EXPORT);

@ISA = qw(TermExtract::Calc_Imp Exporter);
@EXPORT = qw();
$VERSION = "0.22";


# ========================================================================
# get_noun_frq -- Get noun frequency.
#                 The values of the hash are frequency of the noun.
# �����T���Z�Ȥ����l�Ȥ�ä륵�֥�`����
#
#  Over-write TermExtract::Calc_Imp::get_noun_frq
#
# ========================================================================

sub get_noun_frq {
    my $self = shift;
    my $data = shift;           # �����ǩ`��
    my $mode = shift || 0;      # �����ǩ`�����ե����뤫�����������R�e�åե饰
    my %cmp_noun_list = ();     # �}���Z���l��������줿�ϥå��壨�v���Α��ꂎ��
    my @terms = ();

    $self->IsAgglutinativeLang; # �z�����Zָ�����g�Z�g���ֿդ��ʤ���
    $self->IgnoreWords("��","��","��");  # ��Ҫ��Ӌ������Z��ָ��

    # �������ե�����Έ���
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    foreach my $morph ((split /\n/, $data)) {
        chomp $morph;
        $morph = CutStopWords($morph);
        LOOP:
        foreach my $word ((split /\s+/, $morph)) {
            next if $word eq "";
            my $terms = cut_GB($word);
            $cmp_noun_list{ join ' ', @$terms }++ if $$terms[0];
        }
    }
    return \%cmp_noun_list;
}

# ���ȥåץ�`�ɄI��
sub CutStopWords {
    my $word = shift;
    my $noun = "";
    my $term = "";
    my $match = 0;

    LOOP:
    while($word ne "") {
    	
    	# ���ȥåץ�`�ɤ�ָ��
        if    ($word =~ s/^ //)  { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\t//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//)  { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\(//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\)//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\?//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�`//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\-//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�`//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\.//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^;//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^,//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\///) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^!//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^0//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^1//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^2//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^3//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^4//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^5//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^6//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^7//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^8//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^9//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }

        elsif ($word =~ s/^��������//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����û//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ò�����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�������//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����ž��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���Ҳ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˿˿����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��������//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��������//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ݺܵ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ϊʲô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ǻ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���ò�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ƽ����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʮ����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ò���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʱ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^������//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˵����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^������//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ļ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���ɵ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ҫ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��֪//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ҵ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��æ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Լ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ļ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ǳ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ɴ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ͻ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ͽ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ո�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�þ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ο�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��¡//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʹ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʹ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ֱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ҫ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ҫ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��˵//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�϶�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��˵//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��æ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ҹ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^½��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��΢//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^û��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ķ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ƕ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Щ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�κ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ϱ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ѵ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ڱ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ܹ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ż��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^żȻ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ա�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ƫƫ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ƫ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ƭ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ǡǡ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ǧ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ǰ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ǰ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ǰͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ȫ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ȼ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ȼ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�˼�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ծ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ϱ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��΢//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʮ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʮ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʱ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʱ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʱʱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʲô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʹ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ʼ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ǹ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˭��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˳��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˳��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ر�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ص�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ͨ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ͳͳ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^͵͵//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ͻȻ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��һ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^΢΢//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ϊ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^δ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�غ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�±�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ⱥ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�൱//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ҿ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ۿ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ҫ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ҫô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ҳ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һͷ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һֱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ѿ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ա�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ժ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Լ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ǰ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�쳣//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ϊ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ӧ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ӧ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Զ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ѽ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�е�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ӵ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Щ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ұ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ԥ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ը��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ʱ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ô//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Щ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮ǰ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ֻ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ֻ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ֻҪ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ֻ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ת��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Դ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Լ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ܹ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�δ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�°�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��֨//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�߹�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ߴߴ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�¸�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ƹ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ͨ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ƚ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�Ͼ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ض�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ҫ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^һ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ҲҪ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ҳ��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��˵//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�α�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ѽ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�ҹ�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��ν//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^���//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^����//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^û//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ĩ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ī//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ȥ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ȴ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ⱥ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ȼ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˫//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^˭//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^̫//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ͦ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ω//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ϊ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^δ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ц//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ѽ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ҳ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ҳ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^֮//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ֻ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ס//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^׼//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Щ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^�G//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^Ӵ//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ι//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^��//) { $noun = " "; $match = 1; }

        unless ($match) {
            if ($word =~ s/^([\x00-\x7F])//) {} 
            elsif ($word =~ s/^([\x81-\xFE][\x40-\xFE])//) {}
            elsif ($word =~ s/^([\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39])//) {}
            else {}
            $noun = $1;
        }
        $term .= $noun;
        $noun = "";
        $match = 0;
    }
    $term =~ s/^ //;
    return $term;
}

# GB���`�ɤ������Ф����֤Ť��Ф����
sub cut_GB {
    my $word = shift;
    my @terms = ();
    my $iPos = 0; my $iLen = 0;
    my $ascii = "";
    my $was_ascii = 0;
    my $noun = "";
    for($iPos = 0;$word ne ""; $word = substr($word, $iLen)) {
        if ($word =~ /^([\x00-\x7F])/) {
            $iLen = 1;
            if ($was_ascii == 1) {
                $ascii .= $1;
            }
            else {
                $ascii = $1;
            }
            $was_ascii = 1;
            next;
        } 
        elsif ($word =~ /^([\x81-\xFE][\x40-\xFE])/) {
            $iLen = 2;
            $noun = $1;
        }
        elsif ($word =~ /^([\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39])/) {
            $iLen = 4;
            $noun = $1;
        }
        else {
            $iLen = 1;
            $was_ascii = 1;
            next;
        }
        push @terms, $ascii if $was_ascii == 1;
        push @terms, $noun;
        $ascii = ""; $noun = "";
        $was_ascii = 0;
    }
    return \@terms;
}

1;

__END__

=head1 NAME

    TermExtract::ChainesPlainTextGB
                -- ���T���Z����⥸��`�루�й��ZGB��)

=head1 SYNOPSIS

    use TermExtract::ChainesPlainTextGB;

=head1 DESCRIPTION

    �й��Z�Υƥ����ȥǩ`���򤫤餽�Τޤތ��T���Z��������ץ���ࡣ

    ���⥸��`���ʹ�÷��ˤĤ��Ƥϡ��H���饹��TermExtract::Calc_Imp)����
  ���¤Υ���ץ륹����ץȤ���դΤ��ȡ�

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_CPT_GB.pl
 #
 #  
 #  �˜ʳ����ˌ��T���Z�Ȥ�����Ҫ�Ȥ򷵤��ץ����
 #
 #   version 0.11
 #
 #
 
 use TermExtract::ChainesPlainTextGB;
 #use strict;
 my $data = new TermExtract::ChainesPlainTextGB;
 my $InputFile = "ChainesPlainText_out.txt";    # �����ե�����ָ��
 
 # �ץ����ή����K�˕r�I��
 # (��å��ǥ��쥯�ȥ��ʹ�ä������ϤΤߣ�
 $SIG{INT} = $SIG{QUIT} = $SIG{TERM} = 'sigexit';
 
 # ������`�ɤ�ָ��
 # 1 �� ���T���Z����Ҫ�ȡ�2 �� ���T���Z�Τ�
 # 3 �� ��������Ф�
 my $output_mode = 1;
 
 #
 # ��Ҫ��Ӌ��ǡ��B���Z��"�Ӥ���"��"���ʤ���"��"�ѩ`�ץ쥭���ƥ�"�Τ�
 # �����Ȥ뤫�x�k���ѩ`�ץ쥭���ƥ��ϡ�ѧ���C�ܡ���ʹ���ʤ�
 # �ޤ���"�B���Z������ʹ��ʤ�"�x�k�⤢�ꡢ���Έ��Ϥ����Z���F����
 # (���O������Ƥ����IDF�νM�ߺϤ碌������Ҫ��Ӌ����Ф�
 # ���ǥե���Ȥ�"�Ӥ���"��Ȥ� $obj->use_total)
 #
 #$data->use_total;      # �Ӥ�����Ȥ�
 #$data->use_uniq;       # ���ʤ�����Ȥ�
 #$data->use_Perplexity; # �ѩ`�ץ쥭���ƥ���Ȥ�(TermExtract 3.04 ����)
 #$data->no_LR;          # �O������ʹ��ʤ� (TermExtract 4.02 ����)
 
 #
 # ��Ҫ��Ӌ��ǡ��B�����˒줱�Ϥ碌�����Z���F�l�������x�k����
 # $data->no_LR; �ȤνM�ߺϤ碌�����Z���F�l�ȤΤߤ���Ҫ�Ȥ��������
 # ���ǥե���Ȥ� "Frequency" $data->use_frq)
 # TF�Ϥ������Z���������Z��һ����ʹ���Ƥ������Ϥˤ⥫�����
 # Frequency �����Z���������Z��һ����ʹ���Ƥ������Ϥ˥�����Ȥ��ʤ�
 #
 #$data->use_TF;   # TF (Term Frequency) (TermExtract 4.02 ����)
 #$data->use_frq;  # Frequency�ˤ�����Z�l��
 #$data->no_frq;   # �l������ʹ��ʤ�
 
 #
 # ��Ҫ��Ӌ��ǡ�ѧ���C�ܤ�ʹ�����ɤ����x�k
 # ���ǥե���Ȥϡ�ʹ�ä��ʤ� $obj->no_stat)
 #
 #$data->use_stat; # ѧ���C�ܤ�ʹ��
 #$data->no_stat;  # ѧ���C�ܤ�ʹ��ʤ�
 
 #
 # ��Ҫ��Ӌ��ǡ����ɥ�������Ф����Z���l�ȡ��ȡ��B���Z����Ҫ�ȡ�
 # �Τɤ���˱��ؤ򤪤������O�����롣
 # �ǥե���Ȃ��ϣ�
 # �����󤭤��ۤɡ��ɥ�������Ф����Z���l�ȡ��α��ؤ��ߤޤ�
 #
 #$data->average_rate(0.5);
 
 #
 # ѧ���C����DB�˥ǩ`������e���뤫�ɤ����x�k
 # ��Ҫ��Ӌ��ǡ�ѧ���C�ܤ�ʹ���Ȥ��ϡ����åȤ��Ƥ������ۤ���
 # �o�y���I�팝���ѧ���C����DB�˵��h����Ƥ��ʤ��Z�����ޤ��
 # ���������������ʤ���
 # ���ǥե���Ȥϡ���e���ʤ� $obj->no_storage��
 #
 #$data->use_storage; # ��e����
 #$data->no_storage;  # ��e���ʤ�
 
 #
 # ѧ���C����DB��ʹ�ä���DBM��SDBM_File��ָ��
 # ���ǥե���Ȥϡ�DB_File��BTREE��`�ɣ�
 #
 #$data->use_SDBM;
 
 #
 # �^ȥ�Υɥ�����Ȥ��۷e�yӋ��ʹ�����ϤΥǩ`���٩`����
 # �ե��������򥻥å�
 # ���ǥե���Ȥ� "stat.db"��"comb.db"��
 #
 $data->stat_db("statGB.db");
 $data->comb_db("combGB.db");
 
 #
 # �ǩ`���٩`����������å��Τ����һ�r�ǥ��쥯�ȥ��ָ��
 # �ǥ��쥯�ȥ������������У��ǥե���ȣ��Έ��Ϥϥ�å����ʤ�
 #
 #$data->lock_dir("lock_dir");
 
 #
 # �ǩ`�����i���z��
 # ���T���Z�ꥹ�Ȥ����Ф˷���
 # ���۷e�yӋDBʹ�á��ɥ�������Ф��l��ʹ�ä˥��åȣ�
 #
 #my @noun_list = $data->get_imp_word($str, 'var');     # ����������
 my @noun_list = $data->get_imp_word($InputFile); # �������ե�����
 
 #
 # ǰ���i���z����ƥ����ȥե������Ԫ��
 # ��`�ɤ�䤨�ơ����T���Z�ꥹ�Ȥ����Ф˷���
 #$data->use_stat->no_frq;
 #my @noun_list2 = $data->get_imp_word();
 # �ޤ������νY����e�Υ�`�ɤˤ��Y���Ȓ줱�Ϥ碌��
 #@noun_list = $data->result_filter (\@noun_list, \@noun_list2, 30, 1000);
 
 #
 #  ���T���Z�ꥹ�Ȥ�Ӌ�㤷����Ҫ�Ȥ�˜ʳ����˳���
 #
 foreach (@noun_list) {
    # �����Τߤϱ�ʾ���ʤ�
    next if $_->[0] =~ /^\d+$/;
    # 1����(GB)�Τߤϱ�ʾ���ʤ�
    next if $_->[0] =~ /^[\x00-\x7F]$/;
    next if $_->[0] =~ /^[\x81-\xFE][\x40-\xFE]$/;
    next if $_->[0] =~ /^[\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39]$/;
 
    # �Y����ʾ��$output_mode�ˏꤸ�ơ�������ʽ����
    printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
    printf "%s\n",           $_->[0]          if $output_mode == 2;
    printf "%s,",            $_->[0]          if $output_mode == 3;
 }

=head1 Methods

    ���Υ⥸��`��Ǥϡ�get_imp_word �Τߌgװ������������Υ᥽�åɤ��H
  �⥸��`�� TermExtract::Calc_Imp �ǌgװ����Ƥ��롣
    get_imp_word �ϥ��ȥåץ�`�ɤˤ�����¤��}���Z�΅gλ�ޤǤ˷ָ��
  ���롣��������Υ᥽�åɤˤĤ��Ƥϡ�TermExtract::Calc_Imp ��POD�ɥ���
  ���Ȥ���դ��뤳�ȡ�

=head2 get_imp_word

    �й��Z�Ĥ�ΤΥ�`��ˤ���}���Z�����ɤ��롣�ڣ������ϡ��I�팝��Υ�
  �`�����ڣ������ϵڣ������ηN�e�Ǥ��롣�ǥե���ȤǤϡ��ڣ������ϡ��й�
  �Z�ĤΥƥ����ȥե�����Ȥʤ롣�ڣ�������������'var'�����åȤ��줿�Ȥ���
  �ϡ���һ�������й��Z�ĤΥƥ����ȥǩ`������ä�������`�����Ƚ�ዤ��롣

    �����й��Z�Ĥ�Τ������ˤ�ꡢ�}���Z�˷ָ��
      �������Ф����ä����Ϥϡ��������}���Z�����Ф�Ȥ���

      ����ָ���������ȥåץ�`�ɤ����F�������Ϥϡ��������}���Z�����Ф�
       �Ȥ��롣���ȥåץ�`�ɤ����¤ΤȤ��ꡣ

       �������� ����û �ò����� ������� ����ž�� ���Ҳ�� ˿˿����
       �������� ��������
       ��һ�� ��� ���� һ���� �ݺܵ� Ϊʲô �ǻ�� ���ò� һ���
       һ��� ��ô�� ƽ���� ʮ���� �ò��� ��ʱ�� ��һ�� ������ ��ô��
       ���� ��ô�� һ��� ˵���� ������ �Ļ�� ���ɵ�
       ���� ���� ���� ���� ���� ���� ���� ���� ���� ��Ȼ ���� ���� ����
       ��Ҫ ���� ��֪ ���� ���� ���� �Ҵ� ��æ ���� ���� ��� ��� ��Լ
       ���� ���� ���� �Ļ� ���� ���� ���� ���� ��ʱ ��� ��ô ���� ����
       ���� ���� ���� ���� ���� �ǳ� ���� ���� �ɴ� �Ͻ� �Ͽ� ���� �ո�
       ���� ���� ���� ���� ���� ���� ���� ���� ���� ��Ȼ �þ� ���� �ο�
       ���� ���� ��¡ ��� ���� ���� ��ͷ ��Ȼ ���� ���� ���� ���� ����
       ���� ���� ��ʹ ���� ��ʱ ��Ȼ ���� ��ʹ ��ֱ ���� ���� ��Ҫ ����
       ���� ���� ���� ���� ��Ȼ ���� ���� ��Ҫ ��Ȼ ��˵ ���� ���� ����
       ���� �϶� ���� ���� ���� ��˵ ���� ��ͷ ���� ���� ���� ���� ����
       ��æ ��ҹ ���� ½�� ��΢ ���� û�� ��Ȼ ���� ���� �Ķ� ���� ����
       �Ƕ� ���� ��ô ��Щ ���� �κ� �ϱ� ���� �ѵ� �ڱ� ���� �ܹ� ����
       ���� ���� ż�� żȻ �Ա� ƫƫ ƫ�� Ƭ�� ǡǡ ǧ�� ǰ�� ǰ�� ǰͷ
       ���� ���� ���� ���� ȫ�� Ȼ�� Ȼ�� �˼� �Ծ� ���� ��� ��� ����
       �ϱ� ���� ��ͷ ���� ��΢ ��ô ʮ�� ʮ�� ʱ�� ʱ�� ʱʱ ʲô ʹ��
       ʼ�� ���� �ǹ� ���� ���� ˭�� ˳�� ˳�� ��Ȼ ��� ��ʱ ���� ����
       ���� ���� ���� ���� ���� ���� �ر� �ص� ���� ���� ͨ�� ͳͳ ͵͵
       ͻȻ ��� ���� ��ͷ ��� ��һ ���� ΢΢ Ϊ�� δ�� ���� �غ� ����
       ���� �±� ���� ��ͷ �Ⱥ� �൱ ���� ���� �ҿ� ���� ���� �ۿ� Ҫ��
       Ҫô Ҳ�� һ�� һ�� һ�� һ�� һ�� һͷ һ�� һ�� һֱ ��Ȼ �Ѿ�
       �Ա� �Ժ� �Լ� ���� ���� ��ǰ ���� ���� ���� ���� ���� ���� �쳣
       ��� ��Ϊ Ӧ�� Ӧ�� ��Զ ��ѽ ���� ���� �е� ��ʱ ��Ӵ ��Щ ����
       �ұ� ���� ���� ���� ���� Ԥ�� Ը�� ���� ���� ��ʱ ��� ���� ��ô
       ���� ���� ���� ��� ���� ���� ��ô ���� ��Щ ���� ���� ���� ����
       ֮�� ֮�� ֮�� ֮�� ֮�� ֮ǰ ֮�� ֮�� ֮�� ֮�� ֮�� ֮�� ֮��
       ֻ�� ֻ�� ֻҪ ֻ�� ���� ���� ���� ���� ���� �� �� ת�� �Դ�
       �Լ� �ܹ� ���� ��Ȼ ���� ���� ���� ���� ���� ���� ���� �δ� ����
       �°� ��֨ ���� ��� ���� ���� ���� �߹� ���� ���� ߴߴ �¸� ����
       ���� ƹ�� ��ͨ �Ƚ� �Ͼ� �ض� ��Ȼ ���� ��� Ҫ�� һ�� һ�� ҲҪ
       Ҳ�� �� ��˵ �α� ��ѽ �ҹ� ���� ���� ��ν ��� ���� ����
       �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
       �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
       �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
       �� �� �� �� �� �� �� û ĩ Ī �� �� �� �� �� �� �� �� �� �� �� ��
       �� �� ȥ ȴ Ⱥ Ȼ �� �� �� �� �� �� �� ˫ ˭ �� �� �� ̫ �� �� ͦ
       Ω Ϊ δ �� �� �� �� �� Ц �� ѽ �� Ҳ ҳ �� �� �� �� �� �� �� ��
       �� �� �� �� �� �� �� �� �� �� �� ֮ ֻ ס ׼ �� �� �� � �� �� ��
       �� �� Щ �� �� �� �� �G Ӵ ι �� �� �� �� �� ��
       �� �� �� �� �� ��  �� �� �� ( �� �� �� ) �� �� �� �� �� �� �� ��
      �� �� �� ? �� �` - �` �� �� �� �� . ; , \/ !  ��
       0 1 2 3 4 5 6 7 8 9 �� �� �� �� �� �� �� �� �� ��

    ������Ҫ�Ȥˤ������B�����΂���Ӌ��ϡ��h�֣����օgλ���Ф����ʤ���
      �B�A����1�Х������֣����Ӣ���ʤɣ��Έ��Ϥϡ��ޤȤ�ƣ��Z�Ȥ��ƒQ
      ����

    ������Ҫ��Ӌ��ˤ����ƴΤ��Z�ϟoҕ����
      �� �� ��


=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::MeCab
    TermExtract::BrillsTagger
    TermExtract::EnglishPlainText
    TermExtract::ChainesPlainTextUC
    TermExtract::ICTCLAS
    TermExtract::JapanesePlainTextEUC
    TermExtract::JapanesePlainTextSJIS

=head1 COPYRIGHT

    ���Υץ����ϡ��|����ѧ �д�ԣ־���ڤ����Č��T���Z����Υ����ǥ�
����Ԫ�ˡ��|����ѧ ǰ���� (maeda@lib.u-tokyo.ac.jp)�����ɤ�����ΤǤ��롣
    ���ȥåץ�`�ɤ��{���ϡ��|����ѧ С�u��֮(kojima@e.u-tokyo.ac.jp)��
  �Фä���

    �ʤ������ץ�����ʹ�äˤ����������������ʤ�Y�����v���Ƥ⵱���Ǥ�
  һ��؟�Τ�ؓ��ʤ���

=cut
