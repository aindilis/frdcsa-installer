package TermExtract::ChainesPlainTextUC;
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
    $self->IgnoreWords("和","与","的");  # 重要度計算外の語を指定

    # ストップワードを指定
    @StopWords = qw(
       立方厘米 差点儿没 好不容易 无论如何 噼哩啪啦 嘁嘁查查 丝丝拉拉
       哇哩哇啦 唏哩哗啦
       进一步 差不多 差点儿 一下子 狠很的 为什么 那会儿 不得不 一会儿
       一点点 那么样 平方米 十二分 用不着 有时候 有一天 语气词 怎么样
       这会儿 这么样 一块儿 说不定 好容易 哪会儿 不由得
       并且 不必 不大 不但 不管 不仅 不禁 不料 不论 不然 不如 不是 不下
       不要 不用 不知 常常 除非 除了 匆匆 匆忙 从来 凑巧 大概 大家 大约
       但是 到处 到底 的话 等于 东边 东面 对于 顿时 多亏 多么 多少 俄顷
       而且 而已 凡是 反而 反正 非常 分外 否则 干脆 赶紧 赶快 敢于 刚刚
       格外 根本 根据 更加 公尺 公分 公斤 公升 故意 固然 好久 好在 何况
       何如 何以 轰隆 后边 后面 后天 后头 忽然 胡乱 互相 或是 或者 极端
       极了 极其 即使 几乎 几时 既然 假如 假使 简直 渐渐 将近 将要 今天
       仅仅 尽管 尽量 经常 竟然 究竟 就是 就要 居然 据说 看来 可能 可是
       可以 肯定 恳切 恐怕 况且 来说 厘米 里头 历来 例如 立即 立刻 连连
       连忙 连夜 另外 陆续 略微 马上 没有 猛然 明明 明天 哪儿 哪里 哪怕
       那儿 那里 那么 那些 那样 奈何 南边 南面 难道 内边 内面 能够 你们
       年年 宁可 偶尔 偶然 旁边 偏偏 偏巧 片刻 恰恰 千万 前边 前面 前头
       亲身 亲手 亲眼 轻易 全部 然而 然后 人家 仍旧 日日 如此 如果 若何
       上边 上面 上头 上周 稍微 甚么 十分 十足 时常 时而 时时 什么 使得
       始终 事先 是故 是以 首先 谁都 顺便 顺着 虽然 随后 随时 随手 索性
       所以 他们 他日 它们 她们 倘若 特别 特地 特意 天天 通过 统统 偷偷
       突然 外边 外面 外头 万分 万一 往往 微微 为着 未必 我们 呜呼 西边
       西面 下边 下面 下头 先后 相当 向来 向往 幸亏 须臾 沿着 眼看 要不
       要么 也许 一旦 一定 一共 一起 一日 一头 一向 一再 一直 依然 已经
       以北 以后 以及 以南 以内 以前 以上 以外 以西 以下 以右 以左 异常
       因此 因为 应当 应该 永远 啊呀 尤其 由于 有点 有时 啊哟 有些 按照
       右边 右面 于是 与其 暗暗 预先 愿意 再三 咱们 暂时 早就 早已 怎么
       罢了 怎样 曾经 这儿 北边 这里 这么 北面 这些 这样 正好 正巧 正在
       之北 之后 之间 之南 之内 之前 之上 之外 之西 之下 之右 之中 之左
       只好 只是 只要 只有 至多 至今 至少 至于 终于 逐步 逐渐 转眼 自从
       自己 总共 总算 纵然 昨天 左面 阿嚏 嘣嘣 潺潺 哧溜 脆生 滴答 丁当
       嘎巴 咯吱 咕咚 咕嘟 咕噜 呱呱 哈哈 哼哈 呼噜 哗啦 叽叽 嘎嘎 喳喳
       本月 乒乓 扑通 比较 毕竟 必定 必然 嘻嘻 点儿 要是 一边 一面 也要
       也不 别看 别说 何必 哎呀 我国 起来 来着 所谓 会得 方今 方得
       啊 按 吧 把 被 比 彼 必 边 便 别 并 不 才 除 次 从 打 但 当 倒
       到 得 等 点 顶 都 对 吨 多 俄 而 耳 尔 凡 个 跟 更 故 顾 过 何 很
       哼 后 乎 还 或 极 及 即 几 既 间 将 叫 今 竟 净 久 就 咯 可 况 啦
       里 连 两 了 略 嘛 吗 没 末 莫 哪 那 乃 呢 能 你 您 宁 旁 颇 其 起
       岂 且 去 却 群 然 让 若 尚 少 甚 是 受 双 谁 他 它 她 太 趟 条 挺
       惟 为 未 我 吾 勿 昔 先 笑 休 呀 焉 也 页 已 矣 以 亦 因 用 由 有
       又 于 余 予 哉 再 在 暂 遭 则 这 之 只 住 准 着 走 足 喽 嘞 噢 噫
       敢 肯 些 离 嗬 唉 咳 欸 哟 喂 此 夫 来 上 下 方
        、 。 ” “ ，  《 》 ：  （ ） ；  〈 〉 「 」 『 』 【 】
      〔 〕 ？  ！ ー - ー … ‘ ’ ／
       0 1 2 3 4 5 6 7 8 9 ０ １ ２ ３ ４ ５ ６ ７ ８ ９
     );

    # 入力がファイルの場合
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    foreach my $morph ((split /\n/, $data)) {
        chomp $morph;
        foreach my $stop (@StopWords) {
            $morph =~ s/$stop/ /g;
        }
        # 別枠処理
        $morph =~ s/\,/ /g; $morph =~ s/\./ /g; $morph =~ s/\(/ /g;
        $morph =~ s/\)/ /g; $morph =~ s/\;/ /g; $morph =~ s/\!/ /g;
        $morph =~ s/\)/ /g; $morph =~ s/\?/ /g; $morph =~ s/\// /g;
        LOOP:
        foreach my $word ((split /\s+/, $morph)) {
            next if $word eq "";
            my $terms = cut_utf8($word);
            $cmp_noun_list{ join ' ', @$terms }++ if $$terms[0];
        }
    }
    return \%cmp_noun_list;
}

sub cut_utf8 {
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
        elsif ($word =~ /^([\xC0-\xDF][\x80-\xBF])/) {
            $iLen = 2;
            $noun = $1;
        }
        elsif ($word =~ /^([\xE0-\xEF][\x80-\xBF][\x80-\xBF])/) {
            $iLen = 3;
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

    TermExtract::ChainesPlainTextUC
                -- 専門用語抽出モジュール（中国語UTF-8版)

=head1 SYNOPSIS

    use TermExtract::ChainesPlainTextUC;

=head1 DESCRIPTION

    中国語のテキストデータからそのまま専門用語を抽出するプログラム。

    当モジュールの使用法については、親クラス（TermExtract::Calc_Imp)か、
  以下のサンプルスクリプトを参照のこと。

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_CPT_UC.pl
 #
 #  
 #  標準出力に専門用語とその重要度を返すプログラム
 #  UNICODE(UTF-8)版
 #
 #   version 0.11
 #
 
 use TermExtract::ChainesPlainTextUC;
 #use strict;
 my $data = new TermExtract::ChainesPlainTextUC;
 my $InputFile = "ChainesPlainText_out.txt";    # 入力ファイル指定
 
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
 $data->stat_db("statUC.db");
 $data->comb_db("combUC.db");
 
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
    # 数値のみは表示しない
    next if $_->[0] =~ /^\d+$/;
    # 1文字(UTF-8)のみは表示しない
    next if $_->[0] =~ /^[\x00-\x7F]$/;
    next if $_->[0] =~ /^[\xC0-\xDF][\x80-\xBF]$/;
    next if $_->[0] =~ /^[\xE0-\xEF][\x80-\xBF][\x80-\xBF]$/;
 
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

    中国語を次のルールにより複合語に生成する。第１引数は、処理対象のデ
  ータ、第２引数は第１引数の種別である。デフォルトでは、第１引数は、中
  国語のテキストファイルとなる。第２引数に文字列'var'がセットされたと
  きには、第一引数を中国語のテキストデータが入ったスカラー変数と解釈す
  る。

    １．中国語文を次の条件により、複合語に分割する
      １）改行があった場合は、そこで複合語の区切りとする

      ２）指定したストップワードが出現した場合は、そこで複合語の区切り
       とする。ストップワードは以下のとおり。

       立方厘米 差点儿没 好不容易 无论如何 噼哩啪啦 嘁嘁查查 丝丝拉拉
       哇哩哇啦 唏哩哗啦
       进一步 差不多 差点儿 一下子 狠很的 为什么 那会儿 不得不 一会儿
       一点点 那么样 平方米 十二分 用不着 有时候 有一天 语气词 怎么样
       这会儿 这么样 一块儿 说不定 好容易 哪会儿 不由得
       并且 不必 不大 不但 不管 不仅 不禁 不料 不论 不然 不如 不是 不下
       不要 不用 不知 常常 除非 除了 匆匆 匆忙 从来 凑巧 大概 大家 大约
       但是 到处 到底 的话 等于 东边 东面 对于 顿时 多亏 多么 多少 俄顷
       而且 而已 凡是 反而 反正 非常 分外 否则 干脆 赶紧 赶快 敢于 刚刚
       格外 根本 根据 更加 公尺 公分 公斤 公升 故意 固然 好久 好在 何况
       何如 何以 轰隆 后边 后面 后天 后头 忽然 胡乱 互相 或是 或者 极端
       极了 极其 即使 几乎 几时 既然 假如 假使 简直 渐渐 将近 将要 今天
       仅仅 尽管 尽量 经常 竟然 究竟 就是 就要 居然 据说 看来 可能 可是
       可以 肯定 恳切 恐怕 况且 来说 厘米 里头 历来 例如 立即 立刻 连连
       连忙 连夜 另外 陆续 略微 马上 没有 猛然 明明 明天 哪儿 哪里 哪怕
       那儿 那里 那么 那些 那样 奈何 南边 南面 难道 内边 内面 能够 你们
       年年 宁可 偶尔 偶然 旁边 偏偏 偏巧 片刻 恰恰 千万 前边 前面 前头
       亲身 亲手 亲眼 轻易 全部 然而 然后 人家 仍旧 日日 如此 如果 若何
       上边 上面 上头 上周 稍微 甚么 十分 十足 时常 时而 时时 什么 使得
       始终 事先 是故 是以 首先 谁都 顺便 顺着 虽然 随后 随时 随手 索性
       所以 他们 他日 它们 她们 倘若 特别 特地 特意 天天 通过 统统 偷偷
       突然 外边 外面 外头 万分 万一 往往 微微 为着 未必 我们 呜呼 西边
       西面 下边 下面 下头 先后 相当 向来 向往 幸亏 须臾 沿着 眼看 要不
       要么 也许 一旦 一定 一共 一起 一日 一头 一向 一再 一直 依然 已经
       以北 以后 以及 以南 以内 以前 以上 以外 以西 以下 以右 以左 异常
       因此 因为 应当 应该 永远 啊呀 尤其 由于 有点 有时 啊哟 有些 按照
       右边 右面 于是 与其 暗暗 预先 愿意 再三 咱们 暂时 早就 早已 怎么
       罢了 怎样 曾经 这儿 北边 这里 这么 北面 这些 这样 正好 正巧 正在
       之北 之后 之间 之南 之内 之前 之上 之外 之西 之下 之右 之中 之左
       只好 只是 只要 只有 至多 至今 至少 至于 终于 逐步 逐渐 转眼 自从
       自己 总共 总算 纵然 昨天 左面 阿嚏 嘣嘣 潺潺 哧溜 脆生 滴答 丁当
       嘎巴 咯吱 咕咚 咕嘟 咕噜 呱呱 哈哈 哼哈 呼噜 哗啦 叽叽 嘎嘎 喳喳
       本月 乒乓 扑通 比较 毕竟 必定 必然 嘻嘻 点儿 要是 一边 一面 也要
       也不 别看 别说 何必 哎呀 我国 起来 来着 所谓 会得 方今 方得
       啊 按 吧 把 被 比 彼 必 边 便 别 并 不 才 除 次 从 打 但 当 倒
       到 得 等 点 顶 都 对 吨 多 俄 而 耳 尔 凡 个 跟 更 故 顾 过 何 很
       哼 后 乎 还 或 极 及 即 几 既 间 将 叫 今 竟 净 久 就 咯 可 况 啦
       里 连 两 了 略 嘛 吗 没 末 莫 哪 那 乃 呢 能 你 您 宁 旁 颇 其 起
       岂 且 去 却 群 然 让 若 尚 少 甚 是 受 双 谁 他 它 她 太 趟 条 挺
       惟 为 未 我 吾 勿 昔 先 笑 休 呀 焉 也 页 已 矣 以 亦 因 用 由 有
       又 于 余 予 哉 再 在 暂 遭 则 这 之 只 住 准 着 走 足 喽 嘞 噢 噫
       敢 肯 些 离 嗬 唉 咳 欸 哟 喂 此 夫 来 上 下 方
       　 、 。 ” “ ，  《 》 ： ( （ ） ； ) 〈 〉 「 」 『 』 【 】
      〔 〕 ？ ? ！ ー - ー … ‘ ’ ／ . ; , / !
       0 1 2 3 4 5 6 7 8 9 ０ １ ２ ３ ４ ５ ６ ７ ８ ９


    ２．重要度における連接情報の値の計算は、漢字１文字単位で行う。なお、
      連続した1バイト文字（半角英数など）の場合は、まとめて１語として扱
      う。

    ３．重要度計算において次の語は無視する
      和 与 的

=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::MeCab
    TermExtract::BrillsTagger
    TermExtract::EnglishPlainText
    TermExtract::ChainesPlainTextGB
    TermExtract::ICTCLAS
    TermExtract::JapanesePlainTextEUC
    TermExtract::JapanesePlainTextSJIS

=head1 COPYRIGHT

    このプログラムは、東京大学 中川裕志教授の中文専門用語抽出のアイデア
　を元に、東京大学 前田朗 (maeda@lib.u-tokyo.ac.jp)が作成したものである。
    ストップワードの調整は、東京大学 小島浩之(kojima@e.u-tokyo.ac.jp)が
  行った。

    なお、本プログラムの使用において生じたいかなる結果に関しても当方では
  一切責任を負わない。

=cut
