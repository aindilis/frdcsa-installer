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
# ���ǥե���Ȥϡ�DB_File��BTREE?�`�ɣ�
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
   next if $_->[0] =~ /^[\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39]$/;    # �Y����ʾ
   printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
   printf "%s\n",           $_->[0]          if $output_mode == 2;
   printf "%s,",            $_->[0]          if $output_mode == 3;
}