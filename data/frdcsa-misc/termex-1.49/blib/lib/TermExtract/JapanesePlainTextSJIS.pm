package TermExtract::JapanesePlainTextSJIS;
use TermExtract::Calc_Imp;

use strict;
use Exporter ();
use vars qw(@ISA $VERSION @EXPORT);

@ISA = qw(TermExtract::Calc_Imp Exporter);
@EXPORT = qw();
$VERSION = "0.23";


# ========================================================================
# get_noun_frq -- Get noun frequency.
#                 The values of the hash are frequency of the noun.
# �i���p��Ƃ��̕p�x�𓾂�T�u���[�`���j
#
#  Over-write TermExtract::Calc_Imp::get_noun_frq
#
# ========================================================================

sub get_noun_frq {
    my $self = shift;
    my $data = shift;           # ���̓f�[�^
    my $mode = shift || 0;      # ���̓f�[�^���t�@�C�����A�ϐ����̎��ʗp�t���O
    my %cmp_noun_list = ();     # ������ƕp�x������ꂽ�n�b�V���i�֐��̖߂�l�j
    my @terms = ();
    my @StopWords = ();

    $self->IsAgglutinativeLang; # �P������w��i�P��ԂP���󂯂Ȃ��j

    # ���͂��t�@�C���̏ꍇ
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

# �J�^�J�i�y�ъ����𒊏o����T�u���[�`��
sub get_katakana_kanji {
    my $word = shift;
    my @terms = ();
    my $iPos = 0; my $iLen = 0;
    my $pool = "";
    my $status = "0";  # 0 �� �u�����v�A1 �� �u�p���y�тP�o�C�g�L���A2 �� �u�J�^�J�i�v
    my @cmp_noun = ();
    for($iPos = 0;$word ne ""; $word = substr($word, $iLen)) {
        my $noun = "";
        # 1�o�C�g�����iASCII)
        if ($word =~ /^([\x00-\x7E])/) {
            $iLen = 1;
            $noun = $1;
            if ($status == 2) { push @cmp_noun, $pool; $pool = ""; }
            $pool .= $noun;
            $status = 1;
        } 
        # 1�o�C�g�����i���p�J�i�j
        elsif ($word =~ /^([\xA6-\xDF])/) {
            $iLen = 1;
            if ($status == 1) { push @cmp_noun, $pool; $pool = ""; }
            $pool .= $noun;
            $status = 2;
        }
        # �Q�o�C�g����
        elsif ($word =~ /^([\x81-\x9F|\xE0-\xEF][\x40-\xFC])/) {
            $iLen = 2;
            $noun = $1;
            # �Q�o�C�g�p��
            if ($noun =~ /^\x82[\x60-\x9A]/) {
                if ($status == 2) { push @cmp_noun, $pool; $pool = ""; }
                $pool .= $noun;
                $status = 1;
            } 
            # �J�^�J�i
            elsif ($noun eq "\x81\x5B" | $noun =~ /^\x83[\x3F-\x96]/) {
                if ($status == 1) { push @cmp_noun, $pool; $pool = ""; }
                $pool .= $noun;
                $status = 2;
            }
            # ����
            elsif ($noun =~ /^[\x88-\xEA]/) {
                if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
                $status = 0;
                push @cmp_noun, $noun;
            }
            # �L���Ȃ�
            else {
                if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
                $status = 0;
                add_cmp_noun (\@terms, \@cmp_noun);
            }
        }
        else {
            $iLen = 1;
        }
    }
    # �Ō�̈��̂ݏ���
    push @cmp_noun, $pool if $status != 0;
    add_cmp_noun (\@terms, \@cmp_noun) if @cmp_noun;
    return \@terms;
}

# �������o�^
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

    TermExtract::JapanesePlainTextSJIS
     -- ���p�ꎩ�����o���W���[���i�a���u�J�^�J�i�E�������o�����vSJIS�Łj

=head1 SYNOPSIS

    use TermExtract::JapanesePlainTextSJIS;

=head1 DESCRIPTION

    ���{��̃e�L�X�g�f�[�^�iSJIS)���炻�̂܂ܐ��p��𒊏o����v���O�����B

    �����W���[���̎g�p�@�ɂ��ẮA�e�N���X�iTermExtract::Calc_Imp)���A
  �ȉ��̃T���v���X�N���v�g���Q�Ƃ̂��ƁB

=head2 Sample Script

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
 
 
=head1 Methods

    ���̃��W���[���ł́Aget_imp_word �̂ݎ������A����ȊO�̃��\�b�h�͐e
  ���W���[�� TermExtract::Calc_Imp �Ŏ�������Ă���B
    get_imp_word �̓X�g�b�v���[�h�ɂ�蕶�͂𕡍���̒P�ʂ܂łɕ�������
  ����B����ȊO�̃��\�b�h�ɂ��ẮATermExtract::Calc_Imp ��POD�h�L��
  �����g���Q�Ƃ��邱�ƁB

=head2 get_imp_word

    ���{��̕��͂��玟�̃��[���ɂ�蕡����𒊏o����B��P�����́A����
  �Ώۂ̃f�[�^�A��Q�����͑�P�����̎�ʂł���B�f�t�H���g�ł́A��P��
  ���́A���{���̃e�L�X�g�t�@�C���ƂȂ�B��Q�����ɕ�����'var'���Z�b�g
  ���ꂽ�Ƃ��ɂ́A����������{��̃e�L�X�g�f�[�^���������X�J���[�ϐ�
  �Ɖ��߂���B

    �i�P�j�A�������u�����v�A�u�J�^�J�i�v�A�u�p���y�тP�o�C�g�L���v��
�@�@�@�@�o����B
     (�Q�j���s���������ꍇ�́A�����ŕ�����̋�؂�Ƃ���
    �i�R�j�u�����v�͂P�����P�ʁA�u�J�^�J�i�v�y�сu�p���y�тP�o�C�g�L��
�@�@�@�@�v�A�͌�P�ʂł܂Ƃ܂�����B
    �i�S�j��L�R�j���Q�P�ʈȏ�A�����Ă���ꍇ�ɐ��p��Ƃ���B
    �i�T�j�d�v�x�̌v�Z�́A��L�R�j�̒P�ʂōs��

=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::MeCab
    TermExtract::BrillsTagger
    TermExtract::EnglishPlainText
    TermExtract::ChainesPlainTextUC
    TermExtract::ChainesPlainTextGB
    TermExtract::ICTCLAS
    TermExtract::JapanesePlainTextEUC

=head1 COPYRIGHT

    ���̃v���O�����́A������w �O�c�N (maeda@lib.u-tokyo.ac.jp)���쐬��
  �����̂ł���B���̐��p�ꒊ�o�̃A�C�f�A�͓�����w ����T�u�����̒���
  �X�g�b�v���[�h�����ɂ����p��d�v�x�v�Z�̗��_�ƁA�u���K�\���ƃe�L�X
  �g�E�}�C�j���O�v(���ǖ؏��ҁA�V�c�`�F�� ���Ώ��X 2003.10�j�ɋL�ڂ����
  �����J�^�J�i�E�������o�ɂ��L�[���[�h�؂�o���̃A�C�f�A�����ɂ��Ă���B

    �Ȃ��A�{�v���O�����̎g�p�ɂ����Đ����������Ȃ錋�ʂɊւ��Ă������ł�
  ��ؐӔC�𕉂�Ȃ��B

=cut