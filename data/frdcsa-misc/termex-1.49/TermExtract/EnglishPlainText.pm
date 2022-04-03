package TermExtract::EnglishPlainText;
use TermExtract::Calc_Imp;

use strict;
use Exporter ();
use vars qw(@ISA $VERSION @EXPORT);
use locale;

@ISA = qw(TermExtract::Calc_Imp Exporter);
@EXPORT = qw();
$VERSION = "2.11";


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

    $self->IgnoreWords('of', 'Of', 'OF');  # of は重要度計算外とする

    # ストップワードを小文字で指定
    my @StopWordsLC = qw(
        a          about      above     across     after
        again      against    all       almost     alone
        along      already    also      although   always
        amoung     an         and       another    any
        anybody    anyone     anything  anywhere   are
        area       areas      around    as         ask
        asked      asking     asks      at         away
        b          back       backed    backing    backs
        be         because    became    become     becomes
        been       before     began     behind     being
        begins     best       better    between    big
        both       but        by        c          came
        can        cannot     case      cases      certain
        certainly  clear      clearly   come       could
        d          did        differ    different  differently
        do         dose       done      down       downed
        downing    downs      during    e          each
        early      either     end       ended      ending
        every      everydody  everyone  everything everywhere
        f          face       faces     fact       facts
        far        felt       few       find       finds
        first      for        four      from       full
        fully      further    furthered furthering furthers
        g          gave       general   generally  get
        gets       give       given     gives      go
        going      good       goods     got        great
        greater    greatest   group     grouped    grouping
        groups     h          had       has        have
        having     he         her       herself    here
        high       higher     highest   him        himself
        his        how        however   i          if
        important  in         interest  interested interesting
        interests  into       is        it         its
        itself     j          just      k          keep
        keeps      kind       knew      know       known
        knows      l          large     largely    last
        lather     lastest    least     less       let
        lets       like       likely    long       longer
        longest    m          made      make       making
        man        many       may       me         member
        members    mem        might     more       most
        mostly     mr         mrs       much       must
        my         myself     n         necessary  need
        needed     needing    needs     never      new
        newer      newest     next      no         non
        not        nodody     noone     nothing    now
        nowther    number     numbered  numbering  numbers
        o                     off       often      old
        older      oldest     on        once       one
        only       open       opened    opening    opens
        or         order      ordered   ordering   orders
        other      others     our       out        over
        p          part       parted    parting    parts
        per        perhaps    place     places     point
        pointed    pointing   points    possible   present
        presented  presenting presents  problem    problems
        put        puts       q         quite      r
        rather     really     right     room       rooms
        s          said       same      saw        say
        says       second     seconds   see        seem
        seemed     seeming    seems     sees       several
        shall      she        should    show       showed
        showing    shows      side      sides      since
        small      smaller    smallest  so         some
        somebody   someone    something somewhere  state
        states     still      such      sure       t
        take       taken      than      that       the
        their      them       then      there      therefore
        these      they       thing     things     think
        thoughts   three      those     through    thought
        today      together   too       took       toward
        turn       turned     turning   turns      two
        u          under      untill    up         upon
        us         use        uses      used       v
        very       w          want      wanted     wanting
        well       wells      went      were       what
        when       where      whether   which      while
        who        whole      whose     why        will
        with       within     without   work       worked
        working    works      would     x          y
        year       years      yet       you        young
        younger    youngest   your      yours      z
        to         this       it's      that's     what's
        who's      I'm        am        was        largest
        we         can't      couldn't  wouldn't   you're
        you'd      you've     you'll    wasn't     isn't
        aren't     don't      didn't    dosen't    he's
        he'll      she's      she'll    we're      weren't
        I'll       I'd        we'll      I've      it'll
        who's      where's    haven't    hasn't    hadn't
        won't      there's    whoever    whichever whatever
     );

    # 小文字のストップワードを追加
    @StopWords = @StopWordsLC;
    # 先頭１文字が大文字のストップワードを追加（１文字を除く）
    push @StopWords, (map { !/^[a-z]$/ ? ucfirst : ()} @StopWordsLC);
    # 大文字のストップワードを追加
    push @StopWords, (map { uc } @StopWordsLC);

    # 入力がファイルの場合
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    foreach my $morph ((split /\n/, $data)) {
        chomp $morph;
        LOOP:
        foreach my $word ((split /\s+/, $morph)) {
            my $is_end = 0;

            $word =~ s/\(//g;
            $word =~ s/\)//g;
            $word =~ s/\[//g;
            $word =~ s/\]//g;
            $word =~ s/\{//g;
            $word =~ s/\}//g;
            $word =~ s/\<//g;
            $word =~ s/\>//g;
            $word =~ s/\"//g;
            next if $word eq "";

            if (($word =~ s/[\.\,\?\!:\;]+$//) & ($word !~ /^[A-Z]\.$/)) {
               $is_end = 1;
            }

            foreach my $stop (@StopWords) {
                if ($word eq $stop || $word =~ /^[\-\.\=\#\/\|\&\*\+]+/) {
                    shift @terms if (defined $terms[0] && $terms[0]  =~ /of/i);
                    pop   @terms if (defined $terms[0] && $terms[-1] =~ /of/i);
                    $cmp_noun_list{ join ' ', @terms }++ if defined $terms[0];
                    @terms  = ();
                    next LOOP;
                }
            }
            push @terms, $word;

            if ($is_end) {
                shift @terms if (defined $terms[0] && $terms[0]  =~ /of/i);
                pop   @terms if (defined $terms[0] && $terms[-1] =~ /of/i);
                $cmp_noun_list{ join ' ', @terms }++ if defined $terms[0];
                @terms  = ();
                next LOOP;
            }
        }
        shift @terms if (defined $terms[0] && $terms[0]  =~ /of/i);
        pop   @terms if (defined $terms[0] && $terms[-1] =~ /of/i);
        $cmp_noun_list{ join ' ', @terms }++ if defined $terms[0];
        @terms = ();
    }
    return \%cmp_noun_list;
}

1;

__END__

=head1 NAME

    TermExtract::EnglishPlainText
                -- 専門用語抽出モジュール（英文版)

=head1 SYNOPSIS

    use TermExtract::EnglishPlainText;

=head1 DESCRIPTION

    英文のテキストデータをからそのまま専門用語を抽出するプログラム。

    当モジュールの使用法については、親クラス（TermExtract::Calc_Imp)か、
  以下のサンプルスクリプトを参照のこと。

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_EPT.pl
 #
 #  
 #  標準出力に専門用語とその重要度を返すプログラム
 #
 #   version 0.09
 #
 #
 
 use TermExtract::EnglishPlainText;
 #use strict;
 my $data = new TermExtract::EnglishPlainText;
 my $InputFile = "EnglishPlainText_out.txt";    # 入力ファイル指定
 
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
 #$data->stat_db("stat.db");
 #$data->comb_db("comb.db");
 
 #
 # データベースの排他ロックのための一時ディレクトリを指定
 # ディレクトリ名が空文字列（デフォルト）の場合はロックしない
 #
 #$data->lock_dir("lock_dir");
 
 #
 # 英文そのものを読み込み
 # 専門用語リストを配列に返す
 # （累積統計DB使用、ドキュメント中の頻度使用にセット）
 #
 #my @noun_list = $data->get_imp_word($str, 'var');     # 入力が変数
 my @noun_list = $data->get_imp_word($InputFile); # 入力がファイル
 
 #
 # 前回読み込んだ英文テキストデータを元に
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

    英文を次のルールにより複合語に生成する。第１引数は、処理対象のデー
  タ、第２引数は第１引数の種別である。デフォルトでは、第１引数は、英文
  のテキストファイルとなる。第２引数に文字列'var'がセットされたときには、
  第一引数を英文のテキストデータが入ったスカラー変数と解釈する。

    １．英文を次の条件により、複合語に分割する
      １）改行があった場合は、そこで複合語の区切りとする

      ２）単語の末尾が次の文字で終わる場合は、単語の末尾の文字を削除
        し、その単語までを複合語とする。ただし、アルファベット大文字
        １文字＋.  （例 A.）の場合は例外とする
          .,?!:;

      ３）指定したストップワードが出現した場合は、そこで複合語の区切り
       とする。ストップワードは次の文献を元とし、それに調整を加えたも
       のである

         William B. Frakes, Ricardo Baeza-Yates. 1992. Information 
        retrieval : data structures & algorithms. Upper. Saddle 
        River, N.J. :  Prentice Hall PTR
         (ISBN 0-13-463837-9)

          削除した語
            of

          追加した語
              to         this       it's      that's     what's
              who's      I'm        am        was        largest
              we         can't      couldn't  wouldn't   you're
              you'd      you've     you'll    wasn't     isn't
              aren't     don't      didn't    dosen't    he's
              he'll      she's      she'll    we're      weren't
              I'll       I'd        we'll      I've      it'll
              who's      where's    haven't    hasn't    hadn't
              won't      there's    whoever    whichever whatever

      ４）次の記号や数値からなる文字列が出現した場合は、そこで複合語の
        区切りとする
          -.=#/|&*+

    ２．"of" が複合語の先頭もしくは末尾の単語になったときは削除する

    ３．単語に次のくくり文字が含まれている場合は削除する
      ()[]{}<>"

    ４．重要度計算において次の語は無視する
      of Of OF


=head1 SEE ALSO

    TermExtract::Calc_Imp
    TermExtract::Chasen
    TermExtract::MeCab
    TermExtract::BrillsTagger
    TermExtract::ChainesPlainTextUC
    TermExtract::ChainesPlainTextGB
    TermExtract::ICTCLAS
    TermExtract::JapanesePlainTextEUC
    TermExtract::JapanesePlainTextSJIS

=head1 COPYRIGHT

    このプログラムは、東京大学・中川裕志教授の英文専門用語抽出のアイデア
　を元に、東京大学・前田朗 (maeda@lib.u-tokyo.ac.jp)が作成したものである。

    なお、本プログラムの使用において生じたいかなる結果に関しても当方では
  一切責任を負わない。

=cut
