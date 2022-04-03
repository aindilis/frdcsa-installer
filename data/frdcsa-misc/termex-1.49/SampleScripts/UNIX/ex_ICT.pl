#!/usr/local/bin/perl -w

#
#  ex_ICT.pl
#
#　ファイルから ICTCLAS の処理結果を読み取り
#  標準出力に専門用語とその重要度を返すプログラム
#
#   version 0.07
#
#

use TermExtract::ICTCLAS;
#use strict;
my $data = new TermExtract::ICTCLAS;
my $InputFile = "ICT_out.txt";    # 入力ファイル指定

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
# （デフォルトは、DB_FileのBTREE?猢`ド）
#
#$data->use_SDBM;

#
# 過去のドキュメントの累積統計を使う場合のデータベースの
# ファイル名をセット
# （デフォルトは "stat.db"と"comb.db"）
#
$data->stat_db("statICT.db");
$data->comb_db("combICT.db");

#
# データベースの排他ロックのための一時ディレクトリを指定
# ディレクトリ名が空文字列（デフォルト）の場合はロックしない
#
#$data->lock_dir("lock_dir");

#
# 品詞タグ付け済みのテキストから、データを読み込み
# 専門用語リストを配列に返す
# （累積統計DB使用、ドキュメント中の頻度使用にセット）
#
#my @noun_list = $data->get_imp_word($str, 'var');     # 入力が変数
my @noun_list = $data->get_imp_word($InputFile); # 入力がファイル

#
# 前回読み込んだ品詞タグ付け済みテキストファイルを元に
# モードを変えて、専門用語リストを配列に返す
#$data->use_stat->no_frq;
#my @noun_list2 = $data->get_imp_word();
# また、その結果を別のモードによる結果と掛け合わせる
#@noun_list = $data->result_filter (\@noun_list, \@noun_list2, 30, 1000);

#
#  専門用語リストと計算した重要度を標準出力に出す
#
foreach (@noun_list) {
   # 数値のみは表示しない
   next if $_->[0] =~ /^\d+$/;
   # 1文字(GB)のみは表示しない
   next if $_->[0] =~ /^[\x00-\x7F]$/;
   next if $_->[0] =~ /^[\x81-\xFE][\x40-\xFE]$/;
   next if $_->[0] =~ /^[\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39]$/;    # 結果表示
   printf "%-60s %16.2f\n", $_->[0], $_->[1] if $output_mode == 1;
   printf "%s\n",           $_->[0]          if $output_mode == 2;
   printf "%s,",            $_->[0]          if $output_mode == 3;
}