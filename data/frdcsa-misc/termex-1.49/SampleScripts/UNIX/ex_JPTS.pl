#!/usr/local/bin/perl -w

#
#  ex_JPTS.pl
#
#  �W���o�͂ɐ��p��Ƃ��̏d�v�x��Ԃ��v���O����
#  �a���u�J�^�J�i�E�������o�����vShif-JIS��
#
#   version 0.04
#
#

use TermExtract::JapanesePlainTextSJIS;
#use strict;
my $data = new TermExtract::JapanesePlainTextSJIS;
my $InputFile = "JPTS_out.txt";    # ���̓t�@�C��

# �v���Z�X�ُ̈�I��������
# (���b�N�f�B���N�g�����g�p�����ꍇ�̂݁j
$SIG{INT} = $SIG{QUIT} = $SIG{TERM} = 'sigexit';

# �o�̓��[�h���w��
# 1 �� ���p��{�d�v�x�A2 �� ���p��̂�
# 3 �� �J���}��؂�
my $output_mode = 1;

#
# �d�v�x�v�Z�ŁA�A�ڌ��"���א�"�A"�قȂ萔"�A"�p�[�v���L�V�e�B"�̂�
# ������Ƃ邩�I���B�p�[�v���L�V�e�B�́u�w�K�@�\�v���g���Ȃ�
# �܂��A"�A�ڌ�̏����g��Ȃ�"�I��������A���̏ꍇ�͗p��o����
# (�Ɛݒ肳��Ă����IDF�̑g�ݍ��킹�j�ŏd�v�x�v�Z���s��
# �i�f�t�H���g��"���א�"���Ƃ� $obj->use_total)
#
#$data->use_total;      # ���א����Ƃ�
#$data->use_uniq;       # �قȂ萔���Ƃ�
#$data->use_Perplexity; # �p�[�v���L�V�e�B���Ƃ�(TermExtract 3.04 �ȏ�)
#$data->no_LR;          # �אڏ����g��Ȃ� (TermExtract 4.02 �ȏ�) 

#
# �d�v�x�v�Z�ŁA�A�ڏ��Ɋ|�����킹��p��o���p�x����I������
# $data->no_LR; �Ƃ̑g�ݍ��킹�ŗp��o���p�x�݂̂̏d�v�x���Z�o�\
# �i�f�t�H���g�� "Frequency" $data->use_frq)
# TF�͂���p�ꂪ���̗p��̈ꕔ�Ɏg���Ă����ꍇ�ɂ��J�E���g
# Frequency �͗p�ꂪ���̗p��̈ꕔ�Ɏg���Ă����ꍇ�ɃJ�E���g���Ȃ�
#
#$data->use_TF;   # TF (Term Frequency) (TermExtract 4.02 �ȏ�)
#$data->use_frq;  # Frequency�ɂ��p��p�x
#$data->no_frq;   # �p�x�����g��Ȃ� 
#
# �d�v�x�v�Z�ŁA�w�K�@�\���g�����ǂ����I��
# �i�f�t�H���g�́A�g�p���Ȃ� $obj->no_stat)
#
#$data->use_stat; # �w�K�@�\���g��
#$data->no_stat;  # �w�K�@�\���g��Ȃ�

#
# �d�v�x�v�Z�ŁA�u�h�L�������g���̗p��̕p�x�v�Ɓu�A�ڌ�̏d�v�x�v
# �̂ǂ���ɔ�d����������ݒ肷��B
# �f�t�H���g�l�͂P
# �l���傫���قǁu�h�L�������g���̗p��̕p�x�v�̔�d�����܂�
#
#$data->average_rate(0.5);

#
# �w�K�@�\�pDB�Ƀf�[�^��~�ς��邩�ǂ����I��
# �d�v�x�v�Z�ŁA�w�K�@�\���g���Ƃ��́A�Z�b�g���Ă������ق���
# ����B�����ΏۂɊw�K�@�\�pDB�ɓo�^����Ă��Ȃ��ꂪ�܂܂��
# �Ɛ��������삵�Ȃ��B
# �i�f�t�H���g�́A�~�ς��Ȃ� $obj->no_storage�j
#
#$data->use_storage; # �~�ς���
#$data->no_storage;  # �~�ς��Ȃ�

#
# �w�K�@�\�pDB�Ɏg�p����DBM��SDBM_File�Ɏw��
# �i�f�t�H���g�́ADB_File��BTREE���[�h�j
#
#$data->use_SDBM;

#
# �ߋ��̃h�L�������g�̗ݐϓ��v���g���ꍇ�̃f�[�^�x�[�X��
# �t�@�C�������Z�b�g
# �i�f�t�H���g�� "stat.db"��"comb.db"�j
#
#$data->stat_db("statUC.db");
#$data->comb_db("combUC.db");

#
# �f�[�^�x�[�X�̔r�����b�N�̂��߂̈ꎞ�f�B���N�g�����w��
# �f�B���N�g�������󕶎���i�f�t�H���g�j�̏ꍇ�̓��b�N���Ȃ�
#
#$data->lock_dir("lock_dir");

#
# �f�[�^��ǂݍ���
# ���p�ꃊ�X�g��z��ɕԂ�
# �i�ݐϓ��vDB�g�p�A�h�L�������g���̕p�x�g�p�ɃZ�b�g�j
#
#my @noun_list = $data->get_imp_word($str, 'var');     # ���͂��ϐ�
my @noun_list = $data->get_imp_word($InputFile); # ���͂��t�@�C��

#
# �O��ǂݍ��񂾃e�L�X�g�t�@�C��������
# ���[�h��ς��āA���p�ꃊ�X�g��z��ɕԂ�
#$data->use_stat->no_frq;
#my @noun_list2 = $data->get_imp_word();
# �܂��A���̌��ʂ�ʂ̃��[�h�ɂ�錋�ʂƊ|�����킹��
#@noun_list = $data->result_filter (\@noun_list, \@noun_list2, 30, 1000);

#
#  ���p�ꃊ�X�g�ƌv�Z�����d�v�x��W���o�͂ɏo��
#
foreach (@noun_list) {
   # ���t�E�����͕\�����Ȃ�
   next if $_->[0] =~ /^(���a)*(����)*(\d+�N)*(\d+��)*(\d+��)*(�ߑO)*(�ߌ�)*(\d+��)*(\d+��)*(\d+�b)*$/;

   # ���l�݂͕̂\�����Ȃ�
   next if $_->[0] =~ /^\d+$/;

   # ���ʕ\���i$output_mode�ɉ����āA�o�͗l����ύX
   printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
   printf "%s\n",           $_->[0]          if $output_mode == 2;
   printf "%s,",            $_->[0]          if $output_mode == 3;
}
