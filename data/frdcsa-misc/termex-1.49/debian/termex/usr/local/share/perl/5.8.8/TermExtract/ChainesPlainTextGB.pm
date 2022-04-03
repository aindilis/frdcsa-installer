package TermExtract::ChainesPlainTextGB;
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

    $self->IsAgglutinativeLang; # 膠着言語指定（単語間１字空けなし）
    $self->IgnoreWords("和","与","的");  # 重要度計算外の語を指定

    # 入力がファイルの場合
    if ($mode ne 'var') {
        local($/) = undef;
        open (IN, $data) || die "Can not open input file. $!";
        $data = <IN>;
        close IN;
    }

    foreach my $morph ((split /\n/, $data)) {
        chomp $morph;
        $morph = CutStopWords($morph);
        LOOP:
        foreach my $word ((split /\s+/, $morph)) {
            next if $word eq "";
            my $terms = cut_GB($word);
            $cmp_noun_list{ join ' ', @$terms }++ if $$terms[0];
        }
    }
    return \%cmp_noun_list;
}

# ストップワード処理
sub CutStopWords {
    my $word = shift;
    my $noun = "";
    my $term = "";
    my $match = 0;

    LOOP:
    while($word ne "") {
    	
    	# ストップワードを指定
        if    ($word =~ s/^ //)  { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\t//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^　//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^//)  { $noun = " "; $match = 1; }
        elsif ($word =~ s/^、//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^。//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^”//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^“//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^，//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^《//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^》//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^：//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\(//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^（//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^）//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^；//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\)//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^〈//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^〉//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^「//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^」//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^『//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^』//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^【//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^】//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^〔//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^〕//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^？//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\?//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^！//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ー//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\-//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^ー//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^…//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^‘//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^’//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^／//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\.//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^—//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^;//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^,//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^\///) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^!//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^0//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^1//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^2//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^3//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^4//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^5//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^6//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^7//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^8//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^9//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^０//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^１//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^２//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^３//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^４//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^５//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^６//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^７//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^８//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^９//) { $noun = " "; $match = 1; }

        elsif ($word =~ s/^立方厘米//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^差点儿没//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^好不容易//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^无论如何//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^噼哩啪啦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘁嘁查查//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^丝丝拉拉//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哇哩哇啦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^唏哩哗啦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^进一步//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^差不多//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^差点儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一下子//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^狠很的//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^为什么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那会儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不得不//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一会儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一点点//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那么样//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^平方米//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^十二分//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^用不着//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^有时候//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^有一天//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^语气词//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^怎么样//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这会儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这么样//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一块儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^说不定//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^好容易//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哪会儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不由得//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^并且//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不必//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不大//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不但//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不管//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不仅//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不禁//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不料//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不论//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不如//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不下//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不要//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不用//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不知//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^常常//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^除非//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^除了//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^匆匆//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^匆忙//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^从来//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^凑巧//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^大概//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^大家//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^大约//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^但是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^到处//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^到底//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^的话//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^等于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^东边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^东面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^对于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^顿时//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^多亏//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^多么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^多少//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^俄顷//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^而且//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^而已//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^凡是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^反而//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^反正//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^非常//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^分外//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^否则//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^干脆//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^赶紧//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^赶快//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^敢于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^刚刚//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^格外//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^根本//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^根据//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^更加//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^公尺//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^公分//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^公斤//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^公升//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^故意//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^固然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^好久//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^好在//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^何况//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^何如//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^何以//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^轰隆//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^后边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^后面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^后天//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^后头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^忽然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^胡乱//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^互相//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^或是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^或者//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^极端//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^极了//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^极其//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^即使//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^几乎//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^几时//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^既然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^假如//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^假使//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^简直//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^渐渐//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^将近//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^将要//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^今天//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^仅仅//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^尽管//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^尽量//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^经常//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^竟然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^究竟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^就是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^就要//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^居然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^据说//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^看来//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^可能//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^可是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^可以//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^肯定//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^恳切//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^恐怕//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^况且//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^来说//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^厘米//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^里头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^历来//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^例如//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^立即//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^立刻//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^连连//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^连忙//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^连夜//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^另外//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^陆续//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^略微//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^马上//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^没有//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^猛然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^明明//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^明天//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哪儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哪里//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哪怕//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那里//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那些//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那样//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^奈何//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^南边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^南面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^难道//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^内边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^内面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^能够//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^你们//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^年年//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^宁可//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^偶尔//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^偶然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^旁边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^偏偏//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^偏巧//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^片刻//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^恰恰//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^千万//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^前边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^前面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^前头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^亲身//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^亲手//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^亲眼//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^轻易//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^全部//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^然而//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^然后//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^人家//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^仍旧//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^日日//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^如此//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^如果//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^若何//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^上边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^上面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^上头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^上周//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^稍微//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^甚么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^十分//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^十足//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^时常//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^时而//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^时时//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^什么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^使得//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^始终//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^事先//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^是故//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^是以//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^首先//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^谁都//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^顺便//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^顺着//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^虽然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^随后//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^随时//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^随手//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^索性//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^所以//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^他们//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^他日//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^它们//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^她们//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^倘若//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^特别//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^特地//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^特意//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^天天//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^通过//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^统统//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^偷偷//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^突然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^外边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^外面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^外头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^万分//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^万一//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^往往//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^微微//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^为着//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^未必//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^我们//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^呜呼//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^西边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^西面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^下边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^下面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^下头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^先后//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^相当//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^向来//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^向往//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^幸亏//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^须臾//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^沿着//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^眼看//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^要不//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^要么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^也许//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一旦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一定//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一共//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一起//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一日//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一头//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一向//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一再//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一直//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^依然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^已经//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以北//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以后//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以及//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以南//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以内//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以前//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以上//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以外//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以西//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以下//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以右//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以左//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^异常//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^因此//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^因为//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^应当//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^应该//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^永远//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^啊呀//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^尤其//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^由于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^有点//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^有时//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^啊哟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^有些//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^按照//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^右边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^右面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^于是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^与其//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^暗暗//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^预先//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^愿意//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^再三//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咱们//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^暂时//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^早就//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^早已//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^怎么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^罢了//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^怎样//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^曾经//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^北边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这里//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这么//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^北面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这些//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这样//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^正好//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^正巧//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^正在//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之北//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之后//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之间//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之南//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之内//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之前//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之上//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之外//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之西//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之下//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之右//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之中//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之左//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^只好//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^只是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^只要//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^只有//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^至多//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^至今//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^至少//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^至于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^终于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^逐步//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^逐渐//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^转眼//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^自从//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^自己//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^总共//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^总算//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^纵然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^昨天//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^左面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^阿嚏//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘣嘣//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^潺潺//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哧溜//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^脆生//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^滴答//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^丁当//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘎巴//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咯吱//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咕咚//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咕嘟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咕噜//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^呱呱//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哈哈//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哼哈//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^呼噜//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哗啦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^叽叽//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘎嘎//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^喳喳//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^本月//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^乒乓//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^扑通//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^比较//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^毕竟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^必定//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^必然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘻嘻//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^点儿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^要是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^一面//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^也要//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^也不//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^别看//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^别说//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^何必//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哎呀//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^我国//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^起来//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^来着//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^所谓//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^会得//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^方今//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^方得//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^啊//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^按//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^吧//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^把//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^被//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^比//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^彼//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^必//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^边//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^便//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^别//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^并//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^不//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^才//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^除//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^次//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^从//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^打//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^但//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^当//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^倒//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^到//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^得//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^等//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^点//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^顶//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^都//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^对//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^吨//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^多//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^俄//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^而//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^耳//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^尔//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^凡//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^个//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^跟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^更//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^故//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^顾//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^过//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^何//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^很//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哼//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^后//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^乎//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^还//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^或//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^极//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^及//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^即//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^几//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^既//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^间//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^将//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^叫//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^今//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^竟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^净//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^久//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^就//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咯//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^可//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^况//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^啦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^里//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^连//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^两//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^了//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^略//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘛//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^吗//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^没//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^末//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^莫//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哪//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^那//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^乃//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^呢//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^能//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^你//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^您//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^宁//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^旁//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^颇//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^其//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^起//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^岂//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^且//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^去//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^却//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^群//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^然//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^让//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^若//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^尚//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^少//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^甚//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^是//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^受//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^双//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^谁//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^他//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^它//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^她//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^太//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^趟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^条//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^挺//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^惟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^为//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^未//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^我//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^吾//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^勿//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^昔//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^先//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^笑//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^休//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^呀//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^焉//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^也//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^页//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^已//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^矣//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^以//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^亦//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^因//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^用//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^由//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^有//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^又//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^于//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^余//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^予//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哉//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^再//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^在//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^暂//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^遭//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^则//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^这//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^之//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^只//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^住//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^准//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^着//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^走//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^足//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^喽//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嘞//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^噢//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^噫//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^敢//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^肯//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^些//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^离//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^嗬//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^唉//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^咳//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^欸//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^哟//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^喂//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^此//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^夫//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^来//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^上//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^下//) { $noun = " "; $match = 1; }
        elsif ($word =~ s/^方//) { $noun = " "; $match = 1; }

        unless ($match) {
            if ($word =~ s/^([\x00-\x7F])//) {} 
            elsif ($word =~ s/^([\x81-\xFE][\x40-\xFE])//) {}
            elsif ($word =~ s/^([\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39])//) {}
            else {}
            $noun = $1;
        }
        $term .= $noun;
        $noun = "";
        $match = 0;
    }
    $term =~ s/^ //;
    return $term;
}

# GBコードの文字列を１文字づつ切り出す
sub cut_GB {
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
        elsif ($word =~ /^([\x81-\xFE][\x40-\xFE])/) {
            $iLen = 2;
            $noun = $1;
        }
        elsif ($word =~ /^([\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39])/) {
            $iLen = 4;
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

    TermExtract::ChainesPlainTextGB
                -- 専門用語抽出モジュール（中国語GB版)

=head1 SYNOPSIS

    use TermExtract::ChainesPlainTextGB;

=head1 DESCRIPTION

    中国語のテキストデータをからそのまま専門用語を抽出するプログラム。

    当モジュールの使用法については、親クラス（TermExtract::Calc_Imp)か、
  以下のサンプルスクリプトを参照のこと。

=head2 Sample Script

 #!/usr/local/bin/perl -w
 
 #
 #  ex_CPT_GB.pl
 #
 #  
 #  標準出力に専門用語とその重要度を返すプログラム
 #
 #   version 0.11
 #
 #
 
 use TermExtract::ChainesPlainTextGB;
 #use strict;
 my $data = new TermExtract::ChainesPlainTextGB;
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
 $data->stat_db("statGB.db");
 $data->comb_db("combGB.db");
 
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
    # 1文字(GB)のみは表示しない
    next if $_->[0] =~ /^[\x00-\x7F]$/;
    next if $_->[0] =~ /^[\x81-\xFE][\x40-\xFE]$/;
    next if $_->[0] =~ /^[\x81-\xEF][\x30-\x39][\x81-\xEF][\x30-\x39]$/;
 
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

    中国語文を次のルールにより複合語に生成する。第１引数は、処理対象のデ
  ータ、第２引数は第１引数の種別である。デフォルトでは、第１引数は、中国
  語文のテキストファイルとなる。第２引数に文字列'var'がセットされたときに
  は、第一引数を中国語文のテキストデータが入ったスカラー変数と解釈する。

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
      〔 〕 ？ ? ！ ー - ー … ‘ ’ ／ . ; , \/ !  —
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
    TermExtract::ChainesPlainTextUC
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
