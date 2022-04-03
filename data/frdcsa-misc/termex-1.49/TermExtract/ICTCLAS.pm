package TermExtract::ICTCLAS;
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
    # ��Ҫ��Ӌ������Z��ָ��
    $self->IgnoreWords("��","��","��","֮","��","��","ʽ","��");  
    $self->IsAgglutinativeLang; # �z�����Zָ�����g�Z�g���ֿդ��ʤ�)

    # �������ե�����Έ���
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    foreach my $morph ((split /\n/, $data)) {
        chomp $morph;
        next if $morph =~ /^\s*$/;

        # $status = 1   ǰ�����~(ng n nr ns nt nz nz vn an i j)
        #           2   ǰ�������~(ag a)
        #           3   ǰ�����~(u), ��ӳɷ�(k)
        #           4   ǰ���B�~[�͡���](c)
        #           5   ǰ�����e�~(b)

        my $status = 0;

        my $rest   = 0;  # ���~������Z�����Z�B�A�������������
        my @seg    = (); # �}���Z�Υꥹ�ȣ����У�

        foreach my $term (split(/\s+/, $morph)) {
        	$term =~ s/^��+//;
        	# ���������Ф�ӛ�ŤΈ���
            if($term =~ /^[\s\+\-\%\&\$\*\#\^\|\/\>\<\;\:]/ || $term =~ /^[\d]+\//){
                _increase_frq(\%cmp_noun_list, \@seg, \$rest);
                next;
            }
        	
            # ���~������Z�������~�������~�ʤɤ򺬤ࣩ�Έ���
            if($term =~ /\/ng$/ || $term =~ /\/n$/  || $term =~ /\/nr$/ || $term =~ /\/ns$/ ||
               $term =~ /\/nt$/ || $term =~ /\/nz$/ || $term =~ /\/nx$/ || $term =~ /\/vn$/ ||
               $term =~ /\an$/  || $term =~ /\/i$/  || $term =~ /\/j$/){
                $status = 1;
                push(@seg, $term); $rest = 0;
            }
            # �����~�Έ���
            elsif($term =~ /\/ag$/ || $term =~ /\/a$/){
                #��ǰ���Z��"�ʤ�","�����~","���~����ӳɷ�","�B�~"�Έ��Ϥ��B�Y����
                if($status == 0 || $status == 2 || $status == 3 || $status == 4){
                    push(@seg, $term); $rest++;
                }
                else {
                    _increase_frq(\%cmp_noun_list, \@seg, \$rest);
                    @seg = ($term); $rest++;
                }
                $status = 2;
            }
            # ���~��������~�Έ���
            elsif($term =~ /\/u$/ || $term =~ /\/k$/){
                #��ǰ���Z��"���~","�����~"�Έ��Ϥ��B�Y����
                if($status == 1 || $status == 2){
                    push(@seg, $term); $rest++;
                }
                else {
                    _increase_frq(\%cmp_noun_list, \@seg, \$rest);
                    $rest = 0;
                }
                $status = 3;
            }
            # �B�~���͡��룩�Έ���
            elsif($term =~ /\/c$/ && ($term =~ /^��/ | $term =~ /^��/)){
                #��ǰ���Z��"���~"�Έ��Ϥ��B�Y����
                if($status == 1){
                    push(@seg, $term); $rest++;
                }
                else {
                    _increase_frq(\%cmp_noun_list, \@seg, \$rest);
                    $rest = 0;
                }
                $status = 4;
            }
            # ���e�~�Έ���
            elsif($term =~ /\/b$/){
                #��ǰ���Z��"�ʤ�","���~", "���~����ӳɷ�", "�B�~"�Έ��Ϥ��B�Y����
                if($status == 0 || $status == 1 || $status == 3 || $status == 4){
                    push(@seg, $term); $rest++;
                }
                else {
                    _increase_frq(\%cmp_noun_list, \@seg, \$rest);
                    $rest = 0;
                }
                $status = 5;
            }
            # ָ������Ʒ�~����Έ��Ϥϡ��������}���Z�����Ф�Ȥ���
            else{
                _increase_frq(\%cmp_noun_list, \@seg, \$rest);
                $status = 0;
            }
        }
        # ���Ф����ä����ϤϤ������}���Z�����Ф�Ȥ���
        _increase_frq(\%cmp_noun_list, \@seg, \$rest);
        $status = 0;
    }
    return \%cmp_noun_list;
}


# ---------------------------------------------------------------------------
#   _increase_frq  --  �l�ȉ��䤹
# 
#   usage : _increase_frq(frequency_of_compnoun, segment, rest_after_noun);
# ---------------------------------------------------------------------------

sub _increase_frq {
    my $frq_ref  = shift;
    my $seg      = shift;
    my $rest     = shift;
    my $allwords = "";

    # �}���Z��ĩβ�����~�Ȥ�������������Ф�ΤƤ�
    $#$seg -= $$rest if @$seg;
    $$rest = 0;

    if ($#$seg >= 0) {
        foreach my $word (@$seg) {
	        $word =~ s/\/[A-Z\$]+$//i; # ������ȡ�����
            if ($allwords eq "") { 
                $allwords = $word;
            }
            else {
                $allwords .= ' ' . $word;
            }
        }
        $$frq_ref{"$allwords"}++;
    }
    @$seg = ();
}

1;

__END__

=head1 NAME

    TermExtract::ICTCLAS
                -- ���T���Z����⥸��`�루"ICTCLAS"��)

=head1 SYNOPSIS

    use TermExtract::ICTCLAS;

=head1 DESCRIPTION

    �����ƥ����Ȥ�"ICTCLAS"�����Ĥ�Ʒ�~��������ץ���ࣩ�ˤ�
  �������νY�����Ȥ������ƥ����Ȥ��錟�T���Z��������ץ���ࡣ

    ���⥸��`���ʹ�÷��ˤĤ��Ƥϡ��H���饹��TermExtract::Calc_Imp)����
  ���¤Υ���ץ륹����ץȤ���դΤ��ȡ�

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_ICT.pl
 #
 #���ե����뤫�� ICTCLAS �΄I��Y�����i��ȡ��
 #  �˜ʳ����ˌ��T���Z�Ȥ�����Ҫ�Ȥ򷵤��ץ����
 #
 #   version 0.07
 #
 #
 
 use TermExtract::ICTCLAS;
 #use strict;
 my $data = new TermExtract::ICTCLAS;
 my $InputFile = "ICT_out.txt";    # �����ե�����ָ��
 
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
 $data->stat_db("statICT.db");
 $data->comb_db("combICT.db");
 
 #
 # �ǩ`���٩`����������å��Τ����һ�r�ǥ��쥯�ȥ��ָ��
 # �ǥ��쥯�ȥ������������У��ǥե���ȣ��Έ��Ϥϥ�å����ʤ�
 #
 #$data->lock_dir("lock_dir");
 
 #
 # Ʒ�~���������g�ߤΥƥ����Ȥ��顢�ǩ`�����i���z��
 # ���T���Z�ꥹ�Ȥ����Ф˷���
 # ���۷e�yӋDBʹ�á��ɥ�������Ф��l��ʹ�ä˥��åȣ�
 #
 #my @noun_list = $data->get_imp_word($str, 'var');     # ����������
 my @noun_list = $data->get_imp_word($InputFile); # �������ե�����
 
 #
 # ǰ���i���z���Ʒ�~���������g�ߥƥ����ȥե������Ԫ��
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

    # �Y����ʾ
    printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
    printf "%s\n",           $_->[0]          if $output_mode == 2;
    printf "%s,",            $_->[0]          if $output_mode == 3;
 }

=head1 Methods

    ���Υ⥸��`��Ǥϡ�get_imp_word �Τߌgװ������������Υ᥽�åɤ��H
  �⥸��`�� TermExtract::Calc_Imp �ǌgװ����Ƥ��롣
    get_imp_word ��Ʒ�~����������Ф�������줿�g�Z�򡢂����΅g�Z���Z�
  ��Ʒ�~����Ԫ���}���Z�����ɤ��Ƥ��롣��������Υ᥽�åɤˤĤ��Ƥϡ�
  TermExtract::Calc_Imp ��POD�ɥ�����Ȥ���դ��뤳�ȡ�

=head2 get_imp_word

    ���Ĥ�Ʒ�~��������Y����ΤΥ�`��ˤ���}���Z�����ɤ��롣�ڣ������ϡ�
  �I�팝��Υǩ`�����ڣ������ϵڣ������ηN�e�Ǥ��롣�ǥե���ȤǤϡ��ڣ�
  �����ϡ�Ʒ�~���������g�ߤΥƥ����ȥե�����Ȥʤ롣�ڣ�������������
  'var'�����åȤ��줿�Ȥ��ˤϡ���һ������Ʒ�~���������g�Υƥ����ȥǩ`��
  ����ä�������`�����Ƚ�ዤ��롣

    ������Ʒ�~�ϴΤΤȤ���Y�Ϥ���
       ���������~������Z(ng n nr ns nt nz nx vn an i j) *���ᡸ���~��
           �������~�������~�����~����ӳɷ֡��B�~���͡��룩�˽Y�Ϥ��롣
             �}���Z�����^�ˤʤ롣
       �����������~(ag, a)
           ���������~�����~����ӳɷ֡��B�~���͡��룩�˽Y�Ϥ��롣�}���Z��
             ���^�ˤʤ�
       ���������~(u), ��ӳɷ�(k)
          �������~�������~�˽Y�Ϥ���
       �������B�~(c)
          ������,��Έ��ϤΤߡ����~�˽Y�Ϥ��롣
       ���������e�~(b)
           �������~, ���~���B�~���͡��룩�˽Y�Ϥ��롣�}���Z�����^�ˤʤ�

    �������Ф����ä����Ϥϡ��������}���Z�����Ф�Ȥ���

    �����Z�^��ȫ�ǥ��ک`����ȡ�����

  �������Τ�ӛ�Ť�������ʼ�ޤ��Z�Έ��Ϥϡ��������}���Z�����Ф�Ȥ���
        +-%\&\$*#^|<>;:

    �����}���Z�����~�ǽK����ΤȤ���������Ф�ΤƤ�

    ������Ҫ��Ӌ��ˤ����ƴΤ��Z�����~������~���B�~���ϟoҕ����
      �� �� �� ֮ �� �� ʽ ��

=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::MeCab
    TermExtract::BrillsTagger
    TermExtract::EnglishPlainText
    TermExtract::ChainesPlainTextUC
    TermExtract::ChainesPlainTextGB
    TermExtract::JapanesePlainTextEUC
    TermExtract::JapanesePlainTextSJIS

=head1 COPYRIGHT

    ���Υץ����ϡ��|����ѧ �д�ԣ־���ڤ����Č��T���Z����Υ����ǥ�
����Ԫ�ˡ��|����ѧ ǰ���� (maeda@lib.u-tokyo.ac.jp)�����ɤ�����ΤǤ��롣
�����˘��Υ����å��ϡ��|����ѧ С�u��֮(kojime@e.u-tokyo.ac.jp)���Фä���

    �ʤ������ץ�����ʹ�äˤ����������������ʤ�Y�����v���Ƥ⵱���Ǥ�
  һ��؟�Τ�ؓ��ʤ���

=cut
