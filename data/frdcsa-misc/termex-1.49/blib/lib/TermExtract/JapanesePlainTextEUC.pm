package TermExtract::JapanesePlainTextEUC;
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
# �������Ѹ�Ȥ������٤����륵�֥롼�����
#
#  Over-write TermExtract::Calc_Imp::get_noun_frq
#
# ========================================================================

sub get_noun_frq {
    my $self = shift;
    my $data = shift;           # ���ϥǡ���
    my $mode = shift || 0;      # ���ϥǡ������ե����뤫���ѿ����μ����ѥե饰
    my %cmp_noun_list = ();     # ʣ�������پ�������줿�ϥå���ʴؿ�������͡�
    my @terms = ();
    my @StopWords = ();

    $self->IsAgglutinativeLang; # ����������ñ��֣��������ʤ���

    # ���Ϥ��ե�����ξ��
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    foreach my $morph ((split /\n/, $data)) {
        chomp $morph;
        next if $morph eq "";
        my $terms = get_katakana_kanji($morph);
        foreach my $cmp_noun (@$terms) {
            next if @$cmp_noun < 2;
            $cmp_noun_list{ join ' ', @$cmp_noun }++ if $$cmp_noun[0];
        }
    }

    return \%cmp_noun_list;
}

# �������ʵڤӴ�������Ф��륵�֥롼����
sub get_katakana_kanji {
    my $word = shift;
    my @terms = ();
    my $iPos = 0; my $iLen = 0;
    my $pool = "";
    my $status = "0";  # 0 �� �ִ����ס�1 �� �ֱѻ��ڤӣ��Х��ȵ��桢2 �� �֥������ʡ�
    my @cmp_noun = ();
    LOOP:
    for($iPos = 0;$word ne ""; $word = substr($word, $iLen)) {
        my $noun = "";
        # 1�Х���ʸ��
        if ($word =~ /^([\x00-\x7E])/) {
            $iLen = 1;
            $noun = $1;
            if ($status == 2) { push @cmp_noun, $pool; $pool = ""; }
            $pool .= $noun;
            $status = 1;
        }
        # ���Х��ȱѻ�
        elsif ($word =~ /^(\xA3[\xC1-\xFA])/) {
            $iLen = 2;
            $noun = $1;
            if ($status == 2) { push @cmp_noun, $pool; $pool = ""; }
            $pool .= $noun;
            $status = 1;
        }
        # ���Х��ȵ���ڤӥ�������
        elsif ($word =~ /^([\xA1-\xA8][\xA0-\xFE])/) {
            $iLen = 2;
            $noun = $1;
            # Ĺ���ڤӥ�������
            if ($noun eq "\xA1\xBC" | $noun =~ /^\xA5[\xA0-\xFE]/) {
                if ($status == 1) { push @cmp_noun, $pool; $pool = ""; }
                $pool .= $noun;
                $status = 2;
            }
            # ����
            else {
                if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
                $status = 0;
                add_cmp_noun (\@terms, \@cmp_noun);
            }
        }
        # ���Х���ʸ��
        elsif ($word =~ /^([\xB0-\xFE][\xA0-\xFE])/) {
            $iLen = 2;
            $noun = $1;
            if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
            $status = 0;
            push @cmp_noun, $noun;
        }
        # ���Х���ʸ��
        elsif ($word =~ /^([\x8F-\xEF][\xA0-\xFE][\xA0-\xFE])/) {
            $iLen = 3;
            $noun = $1;
            if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
            $status = 0;
            push @cmp_noun, $noun;
        }
        else {
            $iLen = 1;
        }
    }
    # �Ǹ�ΰ��Τ߽���
    push @cmp_noun, $pool if $status != 0;
    add_cmp_noun (\@terms, \@cmp_noun) if @cmp_noun;;
    return \@terms;
}

# ʣ������Ͽ
sub add_cmp_noun {
    my $terms =    shift;
    my $cmp_noun = shift;
    my @work     = @$cmp_noun;
    push @$terms, \@work;
    @$cmp_noun    = ();
}

1;

__END__

=head1 NAME

    TermExtract::JapanesePlainTextEUC
     -- �����Ѹ켫ư��Х⥸�塼�����ʸ�֥������ʡ��������������EUC�ǡ�

=head1 SYNOPSIS

    use TermExtract::JapanesePlainTextEUC;

=head1 DESCRIPTION

    ���ܸ�Υƥ����ȥǡ�����EUC)���餽�Τޤ������Ѹ����Ф���ץ���ࡣ

    ���⥸�塼��λ���ˡ�ˤĤ��Ƥϡ��ƥ��饹��TermExtract::Calc_Imp)����
  �ʲ��Υ���ץ륹����ץȤ򻲾ȤΤ��ȡ�

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_JPTE.pl
 #
 #  ɸ����Ϥ������Ѹ�Ȥ��ν����٤��֤��ץ����
 #  ��ʸ�֥������ʡ��������������EUC��
 #
 #   version 0.05
 #
 #
 
 use TermExtract::JapanesePlainTextEUC;
 #use strict;
 my $data = new TermExtract::JapanesePlainTextEUC;
 my $InputFile = "JPTE_out.txt";    # ���ϥե�����
 
 # �ץ����ΰ۾ｪλ������
 # (��å��ǥ��쥯�ȥ����Ѥ������Τߡ�
 $SIG{INT} = $SIG{QUIT} = $SIG{TERM} = 'sigexit';
 
 # ���ϥ⡼�ɤ����
 # 1 �� �����Ѹ�ܽ����١�2 �� �����Ѹ�Τ�
 # 3 �� ����޶��ڤ�
 my $output_mode = 1;
 
 #
 # �����ٷ׻��ǡ�Ϣ�ܸ��"��ٿ�"��"�ۤʤ��"��"�ѡ��ץ쥭���ƥ�"�Τ�
 # �����Ȥ뤫���򡣥ѡ��ץ쥭���ƥ��ϡֳؽ���ǽ�פ�Ȥ��ʤ�
 # �ޤ���"Ϣ�ܸ�ξ����Ȥ�ʤ�"����⤢�ꡢ���ξ����Ѹ�и����
 # (�����ꤵ��Ƥ����IDF���Ȥ߹�碌�ˤǽ����ٷ׻���Ԥ�
 # �ʥǥե���Ȥ�"��ٿ�"��Ȥ� $obj->use_total)
 #
 #$data->use_total;      # ��ٿ���Ȥ�
 #$data->use_uniq;       # �ۤʤ����Ȥ�
 #$data->use_Perplexity; # �ѡ��ץ쥭���ƥ���Ȥ�(TermExtract 3.04 �ʾ�)
 #$data->no_LR;          # ���ܾ����Ȥ�ʤ� (TermExtract 4.02 �ʾ�)
 
 #
 # �����ٷ׻��ǡ�Ϣ�ܾ���˳ݤ���碌���Ѹ�и����پ�������򤹤�
 # $data->no_LR; �Ȥ��Ȥ߹�碌���Ѹ�и����٤Τߤν����٤⻻�в�ǽ
 # �ʥǥե���Ȥ� "Frequency" $data->use_frq)
 # TF�Ϥ����Ѹ줬¾���Ѹ�ΰ����˻Ȥ��Ƥ������ˤ⥫�����
 # Frequency ���Ѹ줬¾���Ѹ�ΰ����˻Ȥ��Ƥ������˥�����Ȥ��ʤ�
 #
 #$data->use_TF;   # TF (Term Frequency) (TermExtract 4.02 �ʾ�)
 #$data->use_frq;  # Frequency�ˤ���Ѹ�����
 #$data->no_frq;   # ���پ����Ȥ�ʤ�
 
 #
 # �����ٷ׻��ǡ��ؽ���ǽ��Ȥ����ɤ�������
 # �ʥǥե���Ȥϡ����Ѥ��ʤ� $obj->no_stat)
 #
 #$data->use_stat; # �ؽ���ǽ��Ȥ�
 #$data->no_stat;  # �ؽ���ǽ��Ȥ�ʤ�
 
 #
 # �����ٷ׻��ǡ��֥ɥ����������Ѹ�����١פȡ�Ϣ�ܸ�ν����١�
 # �Τɤ������Ť򤪤��������ꤹ�롣
 # �ǥե�����ͤϣ�
 # �ͤ��礭���ۤɡ֥ɥ����������Ѹ�����١פ���Ť���ޤ�
 #
 #$data->average_rate(0.5);
 
 #
 # �ؽ���ǽ��DB�˥ǡ��������Ѥ��뤫�ɤ�������
 # �����ٷ׻��ǡ��ؽ���ǽ��Ȥ��Ȥ��ϡ����åȤ��Ƥ������ۤ���
 # ̵�񡣽����оݤ˳ؽ���ǽ��DB����Ͽ����Ƥ��ʤ��줬�ޤޤ��
 # ��������ư��ʤ���
 # �ʥǥե���Ȥϡ����Ѥ��ʤ� $obj->no_storage��
 #
 #$data->use_storage; # ���Ѥ���
 #$data->no_storage;  # ���Ѥ��ʤ�
 
 #
 # �ؽ���ǽ��DB�˻��Ѥ���DBM��SDBM_File�˻���
 # �ʥǥե���Ȥϡ�DB_File��BTREE�⡼�ɡ�
 #
 #$data->use_SDBM;
 
 #
 # ���Υɥ�����Ȥ��������פ�Ȥ����Υǡ����١�����
 # �ե�����̾�򥻥å�
 # �ʥǥե���Ȥ� "stat.db"��"comb.db"��
 #
 #$data->stat_db("statUC.db");
 #$data->comb_db("combUC.db");
 
 #
 # �ǡ����١�������¾��å��Τ���ΰ���ǥ��쥯�ȥ�����
 # �ǥ��쥯�ȥ�̾����ʸ����ʥǥե���ȡˤξ��ϥ�å����ʤ�
 #
 #$data->lock_dir("lock_dir");
 
 #
 # �ǡ������ɤ߹���
 # �����Ѹ�ꥹ�Ȥ�������֤�
 # ����������DB���ѡ��ɥ������������ٻ��Ѥ˥��åȡ�
 #
 #my @noun_list = $data->get_imp_word($str, 'var');     # ���Ϥ��ѿ�
 my @noun_list = $data->get_imp_word($InputFile); # ���Ϥ��ե�����
 
 #
 # �����ɤ߹�����ƥ����ȥե�����򸵤�
 # �⡼�ɤ��Ѥ��ơ������Ѹ�ꥹ�Ȥ�������֤�
 #$data->use_stat->no_frq;
 #my @noun_list2 = $data->get_imp_word();
 # �ޤ������η�̤��̤Υ⡼�ɤˤ���̤ȳݤ���碌��
 #@noun_list = $data->result_filter (\@noun_list, \@noun_list2, 30, 1000);
 
 #
 #  �����Ѹ�ꥹ�Ȥȷ׻����������٤�ɸ����Ϥ˽Ф�
 #
 foreach (@noun_list) {
    # ���ա������ɽ�����ʤ�
    next if $_->[0] =~ /^(����)*(ʿ��)*(\d+ǯ)*(\d+��)*(\d+��)*(����)*(���)*(\d+��)*(\d+ʬ)*(\d+��)*$/;
 
    # ���ͤΤߤ�ɽ�����ʤ�
    next if $_->[0] =~ /^\d+$/;
 
    # ���ɽ����$output_mode�˱����ơ������ͼ����ѹ�
    printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
    printf "%s\n",           $_->[0]          if $output_mode == 2;
    printf "%s,",            $_->[0]          if $output_mode == 3;
 }
 
 
=head1 Methods

    ���Υ⥸�塼��Ǥϡ�get_imp_word �Τ߼�����������ʳ��Υ᥽�åɤϿ�
  �⥸�塼�� TermExtract::Calc_Imp �Ǽ�������Ƥ��롣
    get_imp_word �ϥ��ȥåץ�ɤˤ��ʸ�Ϥ�ʣ����ñ�̤ޤǤ�ʬ�䤷��
  ���롣����ʳ��Υ᥽�åɤˤĤ��Ƥϡ�TermExtract::Calc_Imp ��POD�ɥ���
  ���Ȥ򻲾Ȥ��뤳�ȡ�

=head2 get_imp_word

    ���ܸ��ʸ�Ϥ��鼡�Υ롼��ˤ��ʣ������Ф��롣�裱�����ϡ�����
  �оݤΥǡ������裲�������裱�����μ��̤Ǥ��롣�ǥե���ȤǤϡ��裱��
  ���ϡ����ܸ��Υƥ����ȥե�����Ȥʤ롣�裲������ʸ����'var'�����å�
  ���줿�Ȥ��ˤϡ������������ܸ�Υƥ����ȥǡ��������ä������顼�ѿ�
  �Ȳ�᤹�롣

    �ʣ���Ϣ³�����ִ����ס��֥������ʡס��ֱѻ��ڤӣ��Х��ȵ���פ���
���������Ф��롣
     (���˲��Ԥ����ä����ϡ�������ʣ���ζ��ڤ�Ȥ���
    �ʣ��ˡִ����פϣ�ʸ��ñ�̡��֥������ʡ׵ڤӡֱѻ��ڤӣ��Х��ȵ���
���������ס��ϸ�ñ�̤ǤޤȤޤ���롣
    �ʣ��˾嵭���ˤ���ñ�̰ʾ�Ϣ³���Ƥ�����������Ѹ�Ȥ��롣
    �ʣ��˽����٤η׻��ϡ��嵭���ˤ�ñ�̤ǹԤ�

=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::MeCab
    TermExtract::BrillsTagger
    TermExtract::EnglishPlainText
    TermExtract::ChainesPlainTextUC
    TermExtract::ChainesPlainTextGB
    TermExtract::ICTCLAS
    TermExtract::JapanesePlainTextSJIS

=head1 COPYRIGHT

    ���Υץ����ϡ������� ����ϯ (maeda@lib.u-tokyo.ac.jp)��������
  ����ΤǤ��롣���������Ѹ���ФΥ����ǥ��������� ����͵�ֶ�������ʸ
  ���ȥåץ�������ˤ�������Ѹ�����ٷ׻��������ȡ�������ɽ���ȥƥ���
  �ȡ��ޥ��˥󥰡�(�����ھ��ԡ����ĵ�ɧ�� ���н�Ź 2003.10�ˤ˵��ܤ����
  �����������ʡ�������Фˤ�륭������ڤ�Ф��Υ����ǥ��򸵤ˤ��Ƥ��롣

    �ʤ����ܥץ����λ��Ѥˤ����������������ʤ��̤˴ؤ��Ƥ������Ǥ�
  ������Ǥ�����ʤ���

=cut