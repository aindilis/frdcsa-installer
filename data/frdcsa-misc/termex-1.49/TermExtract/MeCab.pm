package TermExtract::MeCab;
use TermExtract::Calc_Imp;

use strict;
use Exporter ();
use vars qw(@ISA $VERSION @EXPORT);

@ISA = qw(TermExtract::Calc_Imp Exporter);
@EXPORT = qw();
$VERSION = "2.15";

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
    my @input = ();             # �����ǲ��Ϸ�̤�����
    my $must  = 0;              # ���θ줬̾��Ǥʤ���Фʤ�ʤ����Ͽ�
    my @terms = ();             # ʣ���ꥹ�Ⱥ����Ѥκ��������
    my @unknown = ();           # ��m��������Ѻ���ѿ�

    $self->IsAgglutinativeLang; # ����������ñ��֣��������ʤ���

    # �����Ѹ�ꥹ�Ȥء����������ɲä��륵�֥롼����
    my $add = sub {
        my $terms         = shift;
        my $cmp_noun_list = shift;

        # ��Ƭ�����פʸ�κ��
        if (defined $terms->[0]) {
            shift @$terms if $terms->[0] eq '��';
        }
        # ������;ʬ�ʸ�κ��
        if (defined $terms->[0]) {
            my $end = $terms->[$#$terms];
            if ( $end eq '�ʤ�'  || $end eq '��'   || $end eq '��'       || 
                 $end eq '��'    || $end eq '��'   || $end eq '��'       ||
                 $end eq '��'    || $end eq '��'   || $end eq '��'       ||
                 $end =~ /^\s+$/ || $must) 
                { pop @$terms }
        }
        $cmp_noun_list->{ join ' ', @$terms }++ if defined $terms->[0];
        @$terms = ();
    };

    # ���Ϥ��ե�����ξ��
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    # ñ̾���Ϣ�����
    foreach my $morph ((split "\n", $data)) {
        chomp $morph;
	my ($noun, $values) = (split /\t/, $morph);
        my $part_of_speach = "";
        my $cl_1           = "";
        my $cl_2           = "";

        if (defined $values) {
            ($part_of_speach, $cl_1, $cl_2) = (split /,/, $values)[0,1,2];
        }

        # ���桦���ͤǶ��ڤ�줿��m��פϡ����ĤΤޤȤޤ�ˤ��Ƥ������
        #     ����ե��٥å�  �� \x41-\x5A, \x61-\x7A
        if ($part_of_speach eq 'm��' & $noun !~ /^[\(\)\[\]\<\>|\"\'\;\,]/) {
            if (@unknown) {
                # ��m��פ����桦���ͤǷ�ӤĤ��ʤ�
                unless ($unknown[$#unknown] =~ /[\x41-\x5A|\x61-\x7A]$/ & 
                        $noun =~ /^[\x41-\x5A|\x61-\x7A]/) {
                    push @unknown, $noun;  # ��m��פ�ҤȤޤȤ�ˤ���
                    next;
                }
            }
            else {
                push @unknown, $noun;
                next;
            }
        }
        # �ҤȤޤȤ�ˤ�����m��פ����Ѥ��롣
        while (@unknown) {
            if ($unknown[$#unknown] =~ /^[\x21-\x2F]|[{|}:\;\<\>\[\]]$/) {
                pop @unknown;
            }
            else {
               last;
            }
        }
        push @terms, join "", @unknown  if @unknown;
        @unknown = ();

        if( $part_of_speach eq '̾��' & $cl_1 eq '����'                       ||
            $part_of_speach eq '̾��' & $cl_1 eq '������³'                   ||
            $part_of_speach eq '̾��' & $cl_1 eq '����' & $cl_2 eq '����'     ||
            $part_of_speach eq '̾��' & $cl_1 eq '����' & $cl_2 eq '������³' ||
            $part_of_speach eq '̾��' & $cl_1 eq '��ͭ̾��'                   ||
            $part_of_speach eq '����' & $cl_1 eq '����ե��٥å�'             ||
            $part_of_speach eq 'm��'  & 
                               $noun !~ /^[\x21-\x2F]|[{|}:\;\<\>\[\]]$/
          ){
            push @terms, $noun;
            $must = 0; next;
        }
        elsif(($part_of_speach eq '̾��' & $cl_1 eq '����ư��촴'   | 
               $part_of_speach eq '̾��' & $cl_1 eq '�ʥ����ƻ�촴')
           ){
            push @terms, $noun;
            $must = 1; next;
        }
        elsif($part_of_speach eq '̾��' & $cl_1 eq '����' & $cl_2 eq '����ư��촴'){
            push @terms, $noun;
            $must = 1; next;
        }
        elsif($part_of_speach eq 'ư��'){
            @terms  = ();
        }
        else {
            &$add(\@terms, \%cmp_noun_list) unless $must;
        }
        @terms = () if $must;
        $must = 0;
    }
    close IN;

    return \%cmp_noun_list;
}

1;

__END__

=head1 NAME

    TermExtract::MeCab -- �����Ѹ���Х⥸�塼��ʡ����������)

=head1 SYNOPSIS

    use TermExtract::MeCab;

=head1 DESCRIPTION

    ���ϥƥ����Ȥ򡢡�������ס�������ü��ؤǺ������Ƥ�������
    ������ǲ��ϥץ����ˤˤ��������η�̤��Ȥ����ϥƥ���
    �Ȥ��������Ѹ����Ф���ץ���ࡣ
      �ʤ�����������פν��Ϥϥǥե���ȤΥե����ޥåȻ������
    ��Ȥ��Ƥ��롣
      ����ˡ�ˤĤ��Ƥϡ��ƥ��饹��TermExtract::Calc_Imp)�����ʲ���
    ����ץ륹����ץȤ򻲾ȤΤ��ȡ�

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_mecab.pl
 #
 #���ե����뤫���������פη����ǲ��ϺѤߤΥǡ������ɤ߼��
 #  ɸ����Ϥ������Ѹ�Ȥ��ν����٤��֤��ץ����
 #
 #   version 0.32
 #
 #   maeda@lib.u-tokyo.ac.jp
 
 use TermExtract::MeCab;
 #use strict;
 my $data = new TermExtract::MeCab;
 my $InputFile = "mecab_out.txt";    # ���ϥե�����
 
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
 
 # ���Υɥ�����Ȥ��������פ�Ȥ����Υǡ����١�����
 # �ե�����̾�򥻥å�
 # �ʥǥե���Ȥ� "stat.db"��"comb.db"��
 #
 #$data->stat_db("stat.db");
 #$data->comb_db("comb.db");
 
 #
 # �ǡ����١�������¾��å��Τ���ΰ���ǥ��쥯�ȥ�����
 # �ǥ��쥯�ȥ�̾����ʸ����ʥǥե���ȡˤξ��ϥ�å����ʤ�
 #
 #$data->lock_dir("lock_dir");
 
 #
 # �ַ����ǲ��ϡ׺ѤߤΥƥ����ȥե����뤫�顢�ǡ������ɤ߹���
 #  �����Ѹ�ꥹ�Ȥ�������֤�
 #  ����������DB���ѡ��ɥ������������ٻ��Ѥ˥��åȡ�
 #
 #my @noun_list = $data->get_imp_word($str,'var');      # ���Ϥ��ѿ�
 my @noun_list = $data->get_imp_word($InputFile);  # ���Ϥ��ե�����
 
 # �����ɤ߹�����ַ����ǲ��ϡ׺Ѥߥƥ����ȥե�����򸵤�
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
 
    # ���ɽ��
    printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
    printf "%s\n",           $_->[0]          if $output_mode == 2;
    printf "%s,",            $_->[0]          if $output_mode == 3;
 }
 
 # �ץ����ΰ۾ｪλ����DB�Υ�å�����
 # (��å��ǥ��쥯�ȥ����Ѥ������Τߡ�
 sub sigexit {
    $data->unlock_db;
 }

=head1 Methods

    ���Υ⥸�塼��Ǥϡ�get_imp_word �Τ߼�����������ʳ��Υ᥽�åɤϿ�
  �⥸�塼�� TermExtract::Calc_Imp �Ǽ�������Ƥ��롣
    get_imp_word �Ϸ����ǲ��Ϥ�Ԥ���Ф��줿ñ��򡢸ġ���ñ��θ���
  �ʻ����򸵤�ʣ�����������Ƥ��롣����ʳ��Υ᥽�åɤˤĤ��Ƥϡ�
  TermExtract::Calc_Imp ��POD�ɥ�����Ȥ򻲾Ȥ��뤳�ȡ�

=head2 get_imp_word

    �����ǲ��Ϸ�̤򼡤Υ롼��ˤ��ʣ�����������롣�裱�����ϡ�������
  �ݤΥǡ������裲�������裱�����μ��̤Ǥ��롣�ǥե���ȤǤϡ��裱������
  �������ǲ��ϺѤߤΥƥ����ȥե�����Ȥʤ롣�裲������ʸ���� 'var'������
  �Ȥ��줿�Ȥ��ˤϡ�������������ǲ��ϺѤΥƥ����ȥǡ��������ä�������
  ���ѿ��Ȳ�᤹�롣

    ���������ʻ����ñ̾�줬Ϣ³�Ǹ��줿�Ȥ��Ϸ�礹��
       ��̾��    ����
       ��̾��    ������³
       ��̾��    ����            ����
       ��̾��    ����            ������³
       ��̾��    ��ͭ̾��
       ������    ����ե��٥å�
       ��m��

        ����m��פξ��� . �ʥԥꥪ�ɡˤ� - �ʥϥ��ե�ˤʤɤǤ�줬
          ʬ�䤵��롣�����ǡ�ASCII�ε��椬�����줿�Ȥ��ϡ������
          ����礷�ƽ�������褦�ˤ��Ƥ��롣�����������ε���Ͻ�����
            ()[]<>|"';,

    ���������ʻ����ñ̾�줬���줿�Ȥ��ϡ�����³���줬�嵭����̾�줫��
      ����Ƚ�ꤷ���ۤʤ�Ȥ���ʣ���Ȥ��ư���ʤ�

       ��̾��    ����ư��촴
       ��̾��    �ʥ����ƻ�촴

    ���������ʻ����ñ̾�줬���줿�Ȥ��ϡ�����³���줬�嵭����̾�줫��
      ����Ƚ�ꤷ���ۤʤ�Ȥ���ʣ���Ȥ��ư���ʤ����ޤ���ʣ������Ƭ��
      ���������Ѵ����롣

        ��̾��    ����         ����ư��촴

    �����ʻ줬ư��ξ��ϡ�ʣ�����Ѵ�����

    �������Σ�ʸ���Ρ�̤�θ�פϸ�ζ��ڤ�Ȥ��롣�ޤ�����̤�θ�פ� , 
      �� �����Ȥ��ˤ��ζ��ڤ�Ȥ��롣

        !"#$%&'()*+,-./{|}:;<>[]

    ����ʣ����ʤ���Ƭ��ñ̾�줬���ܡפξ��ϡ����ܡפΤߺ�����롣

    ����ʣ����ʤ�ñ̾��Τ��������������θ�ξ��ϡ����������Τߺ��
      ���롣�ޤ�����������ξ��������롣

      "�ʤ�", "��", "��", "��", "��", "��", "��", "��" ,"��"

=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::BrillsTagger
    TermExtract::EnglishPlainText
    TermExtract::ChainesPlainTextUC
    TermExtract::ChainesPlainTextGB
    TermExtract::ICTCLAS
    TermExtract::JapanesePlainTextEUC
    TermExtract::JapanesePlainTextSJIS

=head1 COPYRIGHT

    ���Υץ����ϡ������ء�����͵�ֶ��������͹�Ω��ء���ä§������
  �����������������Ѹ켫ư��Х����ƥ�פ�termex.pl�򻲹ͤˡ������������
  �˺���������ΤǤ��롣
     ���κ�Ȥϡ������ء�����ϯ (maeda@lib.u-tokyo.ac.jp)���Ԥä���

    �ʤ����ܥץ����λ��Ѥˤ����������������ʤ��̤˴ؤ��Ƥ������Ǥ�
  ������Ǥ�����ʤ���

=cut
