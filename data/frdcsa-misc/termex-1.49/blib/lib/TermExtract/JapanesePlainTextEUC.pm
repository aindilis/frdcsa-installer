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
# （専門用語とその頻度を得るサブルーチン）
#
#  Over-write TermExtract::Calc_Imp::get_noun_frq
#
# ========================================================================

sub get_noun_frq {
    my $self = shift;
    my $data = shift;           # 入力データ
    my $mode = shift || 0;      # 入力データがファイルか、変数かの識別用フラグ
    my %cmp_noun_list = ();     # 複合語と頻度情報を入れたハッシュ（関数の戻り値）
    my @terms = ();
    my @StopWords = ();

    $self->IsAgglutinativeLang; # 膠着言語指定（単語間１字空けなし）

    # 入力がファイルの場合
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

# カタカナ及び漢字を抽出するサブルーチン
sub get_katakana_kanji {
    my $word = shift;
    my @terms = ();
    my $iPos = 0; my $iLen = 0;
    my $pool = "";
    my $status = "0";  # 0 → 「漢字」、1 → 「英字及び１バイト記号、2 → 「カタカナ」
    my @cmp_noun = ();
    LOOP:
    for($iPos = 0;$word ne ""; $word = substr($word, $iLen)) {
        my $noun = "";
        # 1バイト文字
        if ($word =~ /^([\x00-\x7E])/) {
            $iLen = 1;
            $noun = $1;
            if ($status == 2) { push @cmp_noun, $pool; $pool = ""; }
            $pool .= $noun;
            $status = 1;
        }
        # ２バイト英字
        elsif ($word =~ /^(\xA3[\xC1-\xFA])/) {
            $iLen = 2;
            $noun = $1;
            if ($status == 2) { push @cmp_noun, $pool; $pool = ""; }
            $pool .= $noun;
            $status = 1;
        }
        # ２バイト記号及びカタカナ
        elsif ($word =~ /^([\xA1-\xA8][\xA0-\xFE])/) {
            $iLen = 2;
            $noun = $1;
            # 長音及びカタカナ
            if ($noun eq "\xA1\xBC" | $noun =~ /^\xA5[\xA0-\xFE]/) {
                if ($status == 1) { push @cmp_noun, $pool; $pool = ""; }
                $pool .= $noun;
                $status = 2;
            }
            # 記号
            else {
                if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
                $status = 0;
                add_cmp_noun (\@terms, \@cmp_noun);
            }
        }
        # ２バイト文字
        elsif ($word =~ /^([\xB0-\xFE][\xA0-\xFE])/) {
            $iLen = 2;
            $noun = $1;
            if ($status != 0) { push @cmp_noun, $pool; $pool = ""; }
            $status = 0;
            push @cmp_noun, $noun;
        }
        # ３バイト文字
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
    # 最後の一語のみ処理
    push @cmp_noun, $pool if $status != 0;
    add_cmp_noun (\@terms, \@cmp_noun) if @cmp_noun;;
    return \@terms;
}

# 複合語を登録
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
     -- 専門用語自動抽出モジュール（和文「カタカナ・漢字抽出方式」EUC版）

=head1 SYNOPSIS

    use TermExtract::JapanesePlainTextEUC;

=head1 DESCRIPTION

    日本語のテキストデータ（EUC)からそのまま専門用語を抽出するプログラム。

    当モジュールの使用法については、親クラス（TermExtract::Calc_Imp)か、
  以下のサンプルスクリプトを参照のこと。

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_JPTE.pl
 #
 #  標準出力に専門用語とその重要度を返すプログラム
 #  和文「カタカナ・漢字抽出方式」EUC版
 #
 #   version 0.05
 #
 #
 
 use TermExtract::JapanesePlainTextEUC;
 #use strict;
 my $data = new TermExtract::JapanesePlainTextEUC;
 my $InputFile = "JPTE_out.txt";    # 入力ファイル
 
 # プロセスの異常終了時処理
 # (ロックディレクトリを使用した場合のみ）
 $SIG{INT} = $SIG{QUIT} = $SIG{TERM} = 'sigexit';
 
 # 出力モードを指定
 # 1 → 専門用語＋重要度、2 → 専門用語のみ
 # 3 → カンマ区切り
 my $output_mode = 1;
 
 #
 # 重要度計算で、連接語の"延べ数"、"異なり数"、"パープレキシティ"のい
 # ずれをとるか選択。パープレキシティは「学習機能」を使えない
 # また、"連接語の情報を使わない"選択もあり、この場合は用語出現回数
 # (と設定されていればIDFの組み合わせ）で重要度計算を行う
 # （デフォルトは"延べ数"をとる $obj->use_total)
 #
 #$data->use_total;      # 延べ数をとる
 #$data->use_uniq;       # 異なり数をとる
 #$data->use_Perplexity; # パープレキシティをとる(TermExtract 3.04 以上)
 #$data->no_LR;          # 隣接情報を使わない (TermExtract 4.02 以上)
 
 #
 # 重要度計算で、連接情報に掛け合わせる用語出現頻度情報を選択する
 # $data->no_LR; との組み合わせで用語出現頻度のみの重要度も算出可能
 # （デフォルトは "Frequency" $data->use_frq)
 # TFはある用語が他の用語の一部に使われていた場合にもカウント
 # Frequency は用語が他の用語の一部に使われていた場合にカウントしない
 #
 #$data->use_TF;   # TF (Term Frequency) (TermExtract 4.02 以上)
 #$data->use_frq;  # Frequencyによる用語頻度
 #$data->no_frq;   # 頻度情報を使わない
 
 #
 # 重要度計算で、学習機能を使うかどうか選択
 # （デフォルトは、使用しない $obj->no_stat)
 #
 #$data->use_stat; # 学習機能を使う
 #$data->no_stat;  # 学習機能を使わない
 
 #
 # 重要度計算で、「ドキュメント中の用語の頻度」と「連接語の重要度」
 # のどちらに比重をおくかを設定する。
 # デフォルト値は１
 # 値が大きいほど「ドキュメント中の用語の頻度」の比重が高まる
 #
 #$data->average_rate(0.5);
 
 #
 # 学習機能用DBにデータを蓄積するかどうか選択
 # 重要度計算で、学習機能を使うときは、セットしておいたほうが
 # 無難。処理対象に学習機能用DBに登録されていない語が含まれる
 # と正しく動作しない。
 # （デフォルトは、蓄積しない $obj->no_storage）
 #
 #$data->use_storage; # 蓄積する
 #$data->no_storage;  # 蓄積しない
 
 #
 # 学習機能用DBに使用するDBMをSDBM_Fileに指定
 # （デフォルトは、DB_FileのBTREEモード）
 #
 #$data->use_SDBM;
 
 #
 # 過去のドキュメントの累積統計を使う場合のデータベースの
 # ファイル名をセット
 # （デフォルトは "stat.db"と"comb.db"）
 #
 #$data->stat_db("statUC.db");
 #$data->comb_db("combUC.db");
 
 #
 # データベースの排他ロックのための一時ディレクトリを指定
 # ディレクトリ名が空文字列（デフォルト）の場合はロックしない
 #
 #$data->lock_dir("lock_dir");
 
 #
 # データを読み込み
 # 専門用語リストを配列に返す
 # （累積統計DB使用、ドキュメント中の頻度使用にセット）
 #
 #my @noun_list = $data->get_imp_word($str, 'var');     # 入力が変数
 my @noun_list = $data->get_imp_word($InputFile); # 入力がファイル
 
 #
 # 前回読み込んだテキストファイルを元に
 # モードを変えて、専門用語リストを配列に返す
 #$data->use_stat->no_frq;
 #my @noun_list2 = $data->get_imp_word();
 # また、その結果を別のモードによる結果と掛け合わせる
 #@noun_list = $data->result_filter (\@noun_list, \@noun_list2, 30, 1000);
 
 #
 #  専門用語リストと計算した重要度を標準出力に出す
 #
 foreach (@noun_list) {
    # 日付・時刻は表示しない
    next if $_->[0] =~ /^(昭和)*(平成)*(\d+年)*(\d+月)*(\d+日)*(午前)*(午後)*(\d+時)*(\d+分)*(\d+秒)*$/;
 
    # 数値のみは表示しない
    next if $_->[0] =~ /^\d+$/;
 
    # 結果表示（$output_modeに応じて、出力様式を変更
    printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
    printf "%s\n",           $_->[0]          if $output_mode == 2;
    printf "%s,",            $_->[0]          if $output_mode == 3;
 }
 
 
=head1 Methods

    このモジュールでは、get_imp_word のみ実装し、それ以外のメソッドは親
  モジュール TermExtract::Calc_Imp で実装されている。
    get_imp_word はストップワードにより文章を複合語の単位までに分割して
  いる。それ以外のメソッドについては、TermExtract::Calc_Imp のPODドキュ
  メントを参照すること。

=head2 get_imp_word

    日本語の文章から次のルールにより複合語を抽出する。第１引数は、処理
  対象のデータ、第２引数は第１引数の種別である。デフォルトでは、第１引
  数は、日本語語のテキストファイルとなる。第２引数に文字列'var'がセット
  されたときには、第一引数を日本語のテキストデータが入ったスカラー変数
  と解釈する。

    （１）連続した「漢字」、「カタカナ」、「英字及び１バイト記号」を抽
　　　　出する。
     (２）改行があった場合は、そこで複合語の区切りとする
    （３）「漢字」は１文字単位、「カタカナ」及び「英字及び１バイト記号
　　　　」、は語単位でまとまりを作る。
    （４）上記３）が２単位以上連続している場合に専門用語とする。
    （５）重要度の計算は、上記３）の単位で行う

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

    このプログラムは、東京大学 前田朗 (maeda@lib.u-tokyo.ac.jp)が作成し
  たものである。この専門用語抽出のアイデアは東京大学 中川裕志教授の中文
  ストップワード方式による専門用語重要度計算の理論と、「正規表現とテキス
  ト・マイニング」(佐良木昌編、新田義彦著 明石書店 2003.10）に記載されて
  いたカタカナ・漢字抽出によるキーワード切り出しのアイデアを元にしている。

    なお、本プログラムの使用において生じたいかなる結果に関しても当方では
  一切責任を負わない。

=cut