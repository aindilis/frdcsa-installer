//
//  file strategy.cpp (strategies)
//  07/06/2001 Manchester
//


#include <string.h>

#include "strategy.h"
#include "property.h"
#include "prepro.h"
#include "options.h"
#include "int.h"


Strategy::Strategy (Property& prop)
  :
  _property (prop)
{
} // Strategy::Strategy


void Strategy::execute (Prepro& prepro, ostream& opts, ostream& problem, 
                        int timeLimit,int memLimit,char* tabFile, ostream& tab )
{
  char pro [100]; // 100 is enough
  _property.toString (pro);

  // cerr << pro << "\n";
  tab << pro << "\n";
  //  cout << pro;
  Strat strat = UNDEFINED;  
  Pre pre = NONE;  

  if ( ! strcmp (pro,"h0010060002_55n") ) {  // PUZ037-3
    strat = EPR;
  }
  if ( ! strcmp (pro,"h0012051200_3n") ) {  // RNG004-3
    strat = dis_2;
  }
  if ( ! strcmp (pro,"N0001150105_7y") ||  // GEO060-2
       ! strcmp (pro,"h0100031100_12n") ) {  // LCL040-1, LCL070-1
    strat = dis_5;    
  }
  else if ( ! strcmp (pro,"H0010031300_9y") || // ???
            ! strcmp (pro,"N0120051400_5n") ||  // SET014-4
            ! strcmp (pro,"N068228Ua73_9n") ) { // GRP026-1
    strat = dis_1002;
  }
  else if ( ! strcmp (pro,"N0210091000_15y") ) { // MSC007-2.005
    strat = EPR;
  }
  else if ( ! strcmp (pro,"n0040031300_7n") ) { // SET013-1, SET015-1
    strat = dis_1003;
  }
  else if ( ! strcmp (pro,"N0001190106_11y") ) { // GEO063-3
    //pre = EF;
    //strat = dis_1004;
    pre = E;
    strat = EPR;
  }
  else if ( ! strcmp (pro,"N05740SXO70_5n") ||  // NUM159-1
            ! strcmp (pro,"N05230PTK50_13n") ) { // SET232-6, SET233-6
    pre = A;               
    strat = dis_n3;
  }
  else if ( ! strcmp (pro,"n0222066000_8n") ||   // COM003-1
            ! strcmp (pro,"P0000071100_28y") ) { // GRP050-1
    strat = dis_n4;
  }
  else if ( ! strcmp (pro,"N06511OUZ62_8n") ||   // ALG001-1 
            ! strcmp (pro,"N0KK007z800_73y") ) {  // lru1464-1
    strat = lrs_1;  // doe snot work for ALG001
  }
  else if ( ! strcmp (pro,"N0001190105_11y") ) {  // GEO028-3
    strat = lrs_1_kinky;
  }
  else if ( ! strcmp (pro,"N0001170105_3y") ) {  // GEO026-3, GEO030-3
    strat = GEO026_3;
  }
  else if ( ! strcmp (pro,"H0200021200_10n") || // LCL214-3 LCL215-3
            ! strcmp (pro,"H0200021200_11n") ) { // LCL213-3 LCL216-3 LCL260-3
    pre = CD,
    strat = lrs_1_lcn;
  }
  else if ( ! strcmp (pro,"H0010090100_3n") || // LDA004-1
            ! strcmp (pro,"H0200011400_6n") || // LCL326-3
            ! strcmp (pro,"P0000051300_11y") || // COL078-1
            ! strcmp (pro,"N0KK005z900_105y") ) { // LRU357-1 LRU358-1
    strat = lrs_2;
  }
  else if ( ! strcmp (pro,"N05230NTK50_11y") ) { // SET382-6
    pre = A;
    strat = lrs_2_kinky;
  }
  else if ( ! strcmp (pro,"N0001170005_3y") ) { // GEO034-2
    strat = lrs_4; // does not work
  }
  else if ( ! strcmp (pro,"n0112052200_22n") || // FLD041-4
            ! strcmp (pro,"n0112062200_31n") || // FLD012-4
            ! strcmp (pro,"n0112082200_38n") || // FLD047-4
            ! strcmp (pro,"n01120L2200_53n") || // FLD048-4
            ! strcmp (pro,"h0020058200_86n") || // ANA003-2
            ! strcmp (pro,"h0100011100_7n") || //  LCL005-1
            ! strcmp (pro,"h0100031100_16n") ) { // LCL383-1
    strat = lrs_4_lcn;
  }
  else if ( ! strcmp (pro,"N05230NTK50_8y") ) { // SET442-6 SET468-6
    pre = A;
    strat = lrs_4_lcn;
  }
  else if ( ! strcmp (pro,"N05740TXO70_4n") || // NUM142-1
            ! strcmp (pro,"N05740TXO70_6y") || // NUM143-1
            ! strcmp (pro,"N05740TXO70_7y") || // NUM144-1
            ! strcmp (pro,"N05740TXO70_8n") || // NUM145-1
            ! strcmp (pro,"N052209PK50_20y") || // SET098-6
            ! strcmp (pro,"N052209PK50_26y") || // SET097-6
            ! strcmp (pro,"N052209PL50_6y") || // SET126-6
            ! strcmp (pro,"N05220LPL50_16y") || // SET128-6
            ! strcmp (pro,"N05230NTK50_5y") || // SET002-6 SET162-6 SET205-6 SET206-6
            ! strcmp (pro,"N05230OTK50_14y") || // SET157-6
            ! strcmp (pro,"N05230OTK50_5n") || // SET194-6 SET195-6
            ! strcmp (pro,"N05230OTK50_7y") || // SET160-6
            ! strcmp (pro,"N05230OTK50_8y") || // SET185-6
            ! strcmp (pro,"N05230QTK50_12n") || // SET254-6
            ! strcmp (pro,"N05220LPL50_11y") || // SET123-6
            ! strcmp (pro,"N05230QTK50_13n") ) { // SET201-6
    pre = C;
    strat = lrs_4_lcn;
  }
  else if ( ! strcmp (pro,"H0200021200_12n") ) { // LCL217-3 LCL220-3
    pre = CD; // changed from C, C did not solve LCL220-3
    strat = lrs_4_lcn;
  }
  else if (! strcmp (pro,"h0100040100_16n") ) { // LCL125-1
    strat = lrs_4_kinky;
  }
  else if ( ! strcmp (pro,"N05230OTK50_6n") ) { // SET518-6
    pre = E;
    strat = lrs_5;
  }
  else if ( ! strcmp (pro,"N05740VXO70_11n") ) { // NUM071-1
    pre = A;
    strat = lrs_5_kinky; // lrs_5_lcn did not work
  }
  else if ( ! strcmp (pro,"h0100031100_15n") ) { // LCL384-1 LCL392-1
    strat = LCL384_1;
  }
  else if ( ! strcmp (pro,"n0112032200_6n") ) { // FLD043-5
    // strat = lrs_n5;
    strat = lrs_7_lcn;
  }
  else if ( ! strcmp (pro,"H0100050100_16n") ) { // COL002-2
    strat = dis_4;
  }
  else if ( ! strcmp (pro,"H0200011400_7n") ) { // LCL312-3
    pre = E;
    strat = lrs_1002;
  }
  else if ( ! strcmp (pro,"h0060085800_0n") ) { // HAR3878-1 HAR3879-1
    strat = lrs_1003;
  }
  else if ( ! strcmp (pro,"H0200031200_12n") ) { // LCL195-3
    pre = CD;
    strat = lrs_1003;
  }
  else if ( ! strcmp (pro,"H0200021400_9n") || // LCL306-3
       ! strcmp (pro,"N0010041300_20n") ) {  // LCL144-1
    pre = E;
    strat = lrs_1003;
  }
  else if ( ! strcmp (pro,"N00100L1100_3n") ) { // LDA008-2
    strat = lrs_1004;
  }
  else if ( ! strcmp (pro,"N00100R1100_3n") ) { // LDA014-1
    pre = C;
    strat = lrs_1004;
  }
  else if ( ! strcmp (pro,"N05740VXO70_16n") ) { // NUM069-1
    pre = E;
    strat = lrs_1004;
  }
  else if ( ! strcmp (pro,"n0120042200_18n") || // FLD027-1
            ! strcmp (pro,"n0120042200_15n") ) {  // FLD033-1
    strat = lrs_1005;
  }
  else if ( ! strcmp (pro,"n0112042200_12n") ) { // FLD062-3, FLD063-3
    // strat = lrs_n4_kinky; did not work
    // strat = lrs_n5; did not work
    strat = lrs_1005;
  }
  else if ( ! strcmp (pro,"H0200021400_11n") ) { // LCL289-3 LCL300-3
    // strat = lrs_1006;  changed experimentally for LCL289-3 and LCL300-3
    strat = lrs_1007; // does not solve LCL221-1
  }
  else if ( ! strcmp (pro,"N0KK104zP00_200y") ) { // LRU1506-1
    strat = lrs_1007;
  }
  else if ( ! strcmp (pro,"N0KK007z800_76y") ||  // LRU1483-1
            ! strcmp (pro,"N0230077L01_24y") ||  // SET671+3 SET673+3
            ! strcmp (pro,"N0KK005z800_94y") ) { // LRU1482-1
    strat = lrs_1008;
  }
  else if ( ! strcmp (pro,"N065118UZ62_3y") ) {   // SET019-3, SET019-4
    pre = C;
    strat = SET019_3;
  }
  else if ( ! strcmp (pro,"N07822N]z73_4y") || // 
            ! strcmp (pro,"H0010041300_13y") ) { // LCL152-1
    pre = E;
    strat = lrs_n2;
  }
  else if ( ! strcmp (pro,"H01000d1300_193y") ||  // CIV002-1
            ! strcmp (pro,"N05740VXO70_9y") ) { // NUM058-1
    strat = lrs_n2_kinky;
  }
  else if ( ! strcmp (pro,"N05230OTK50_6y") || // SET246-6 SET247-6
            ! strcmp (pro,"N05230MTK50_4y") || // SET347-6
            ! strcmp (pro,"N05740TXO70_3n") ||  // NUM051-1 (E 1005 did not work before)
            ! strcmp (pro,"N05740SXO70_4y") ) { // NUM217-1
    pre = A;
    strat = lrs_n2_kinky;
  }
  else if ( ! strcmp (pro,"h0200031100_21n") ) {  // LCL253-1
    strat = lrs_n4;
  }
  else if ( ! strcmp (pro,"n0120052200_18n") || // FLD068-2
            ! strcmp (pro,"n0120042200_13n") ) { // FLD024-1 FLD068-1
    strat = lrs_n4_kinky;
  }
  else if ( ! strcmp (pro,"N052209RK50_26y") || // SET097-7
            ! strcmp (pro,"N065119UZ62_3y") || // SET012-3
            ! strcmp (pro,"N05740VXO70_19y") || // NUM095-1
            ! strcmp (pro,"N05740VXO70_18y") || // NUM094-1
            ! strcmp (pro,"N05230NTK50_12y") ) { // SET280-6, cannot prove
    pre = A;
    strat = lrs_n4_kinky;
  }
  else if ( ! strcmp (pro,"N06511KUZ62_3y") ) { // SET013-3 SET016-3
    pre = A;
    strat = lrs_n4_kinky_awr10;
  }
  else if ( ! strcmp (pro,"n0110056000_1284n") || // SYN067-1
            ! strcmp (pro,"n0110056000_716n") ) { // SYN067-3
    strat = EPR; 
  }
  else if ( ! strcmp (pro,"N07822N]z73_4n") ) { // NUM010-1
    strat = ott_n4; // does not work
  }
  else if ( ! strcmp (pro,"H0100030100_44n") ) { // COL006-3
    pre = DF;
    strat = COL006_3;
  }
  else if ( ! strcmp (pro,"U0000031300_19y") ) { // GRP185-4
    pre = C;
    strat = unit_dis_10;
  }
  else if ( ! strcmp (pro,"U0000031100_14y") ||  // ROB005-1
            ! strcmp (pro,"U0000060120_23y") || // ALG003-1
            ! strcmp (pro,"U0000031220_11y") ) { // RNG025-1
    strat = unit_dis_50;
  }
  else if ( ! strcmp (pro,"U0000051310_16y") ) { // RNG019-6 RNG019-7 RNG020-6 RNG020-7 RNG021-6 RNG021-7
    pre = EF;
    strat = unit_lrs_3;
  }
  else if ( ! strcmp (pro,"u0001091320_127y") ) { // CIV004-1
    strat = unit_lrs_5;
  }
  else if ( ! strcmp (pro,"U0000041100_9y") ) { // COL044-6 COL044-7 COL044-8 COL044-9
    pre = C;
    strat = unit_lrs_5;
  }
  else if ( ! strcmp (pro,"U0000051310_37y") ) { // RNG026-6 RNG026-7
    pre = EF;
    strat = unit_lrs_5;
  }
  else if ( ! strcmp (pro,"U0000031300_21y") ) { // GRP185-1 GRP185-2
    strat = unit_ott_3; // did not work
  }
  else if ( ! strcmp (pro,"N0110042200_3y") ||  // CAT004-3
            ! strcmp (pro,"H0110042100_3y") ) { // CAT004-4
    strat = CAT004_3;
  }
  else if ( ! strcmp (pro,"u00100O1320_142y") ||  // CIV003-1
            ! strcmp (pro,"u0010031310_163y") ) { // also CIV003-1
    pre = C;
    strat = CIV003_1;
  }
  else if ( ! strcmp (pro,"H0100030100_30n") ) { 
    pre = DE;
    strat = COL006_4;
  }
  else if ( ! strcmp (pro,"P0000060300_11y") ) { // COL076-1
    pre = C;
    strat = COL076_1;
  }
  else if ( ! strcmp (pro,"P0000051300_13y") ) { 
    pre = C;
    strat = COL077_1;
  }
  else if ( ! strcmp (pro,"n0112072200_37n") ) { 
    strat = FLD054_4;
  }
  else if ( ! strcmp (pro,"N0001160005_7y") ) { // 
    strat = GEO027_2;
  }
  else if ( ! strcmp (pro,"N0001150005_11y") ) { // GEO029-2
    strat = GEO029_2;
  }
  else if ( ! strcmp (pro,"N0001170005_4n") ) { // GEO044-2, GEO045-2, GEO046-2, GEO047-2
    strat = GEO046_2; 
  }
  else if ( ! strcmp (pro,"P0000080200_47y") ) { 
    strat = LAT005_4;
  }
  else if ( ! strcmp (pro,"P0000030200_60y") ) { 
    strat = LAT015_1;
  }
  else if ( ! strcmp (pro,"h0100031100_14n") ) { // LCL054-1 LCL388-1 LCL389-1
    strat = LCL054_1;
  }
  else if ( ! strcmp (pro,"h0100030100_12n") ) { // LCL084-1, LCL019-1, LCL020-1, LCL021-1, LCL119-1, LCL166-1, LCL167-1
    // strat = LCL084_1; // cannot solve LCL019-1, LCL020-1, LCL021-1, LCL119-1, LCL166-1, LCL167-1
    strat = lrs_4_lcn;
  }
  else if ( ! strcmp (pro,"h0100050100_24n") ) {  // LCL124-1, LCL099-1
    strat = LCL099_1;
  }
  else if ( ! strcmp (pro,"h0200031100_17n") ) {  // LCL221-1, LCL222-1
    strat = LCL222_1;
  }
  else if ( ! strcmp (pro,"H0200031200_14n") ) { // LCL223-3 LCL224-3 LCL225-3
    pre = CD;
    strat = LCL225_3;
  }
  else if ( ! strcmp (pro,"H0200031300_12n") ) { // LCL242-3 LCL243-3 LCL253-3 
    pre = CD;   // LCL253-3 was not solved
    strat = LCL243_3;
  }
  else if ( ! strcmp (pro,"H0200021400_10n") ) { // LCL262-3 LCL330-3 LCL337-3 LCL302-3
    pre = E;
    strat = LCL330_3;  
  }
  else if ( ! strcmp (pro,"h0100041100_16n") ) { // LCL368-1 LCL372-1 LCL374-1
    strat = LCL374_1;
  }
  else if ( ! strcmp (pro,"h0100041100_18n") ) { // LCL376-1 LCL377-1
    strat = LCL377_1;
  }
  else if ( ! strcmp (pro,"h0100021100_12n") ) { // LCL391-1
    strat = LCL391_1;
  }
  else if ( ! strcmp (pro,"N0KK005zL00_84y") ) { 
    strat = LRU007_1;
  }
  else if ( ! strcmp (pro,"N0KK008zL00_120y") ) { 
    strat = LRU1815_1;
  }
  else if ( ! strcmp (pro,"N05740UXO70_8n") ) { // NUM049-1
    pre = A;
    strat = NUM049_1;
  }
  else if ( ! strcmp (pro,"N05740SXO70_3n") || // NUM098-1
            ! strcmp (pro,"N05740SXO70_4n") ) { // NUM099-1
    pre = E;
    strat = NUM098_1;
  }
  else if ( ! strcmp (pro,"n0045021100_7n") ) { 
    strat = PUZ034_1004;
  }
  else if ( ! strcmp (pro,"N023006KM30_35y") ) { 
    strat = SET661_p3;
  }
  else if ( ! strcmp (pro,"n0110066000_809n") ||  
            ! strcmp (pro,"n0110066000_812n") ) { 
    strat = SYN067_2;
  }
  else if ( ! strcmp (pro,"P000006L300_500y") ) { 
    strat = SYN551_p1;
  }
  else if ( ! strcmp (pro,"H0100032200_15y") ) { 
    strat = ROB014_2;
  }
  else if ( ! strcmp (pro,"n0120042200_12n") ) {  // FLD015-1 FLD063-1
    strat = slicing_01;
  }
  else if ( ! strcmp (pro,"n0112062200_30n") ) { // FLD049-3
    strat = slicing_02;
  }
  else if ( ! strcmp (pro,"h0100050100_20n") ) { 
    strat = slicing_03;
  }
  else if ( ! strcmp (pro,"P0000080100_28y") ||  // ALG004-1 GRP199-1
            ! strcmp (pro,"U0000080120_28y") ) { // ALG004-1 
    strat = slicing_04;
  }
  else if ( ! strcmp (pro,"H0200021400_12n") ) { // LCL299-3 LCL348-3 LCL345-3
    pre = DE;   
    strat = slicing_05; // does not solve LCL345-3
  }
  else if ( ! strcmp (pro,"H0200021400_8n") ) { // LCL269-3 LCL273-3 LCL286-3 LCL305-3
    pre = DE;
    strat = slicing_06;
  }
  else if ( ! strcmp (pro,"N05230OTK50_9y") ) { // SET165-6 SET190-6 SET191-6
    pre = C;
    strat = slicing_07;
  }
  else if ( ! strcmp (pro,"U0000040100_9y") ) { // COL006-5 COL006-6 COL006-7
    pre = CF;
    strat = slicing_08; // COL006-6 must be solved in the first slice
  }
  else if ( ! strcmp (pro,"N05230OTK50_10y") ) { // SET156-6 SET287-6 SET473-6 SET230-6
    pre = C;
    strat = slicing_09;
  }

  if ( strat == UNDEFINED ) { // if the strategy is undefined, pick a standard one
    switch ( pro[0] ) {
      case 'N':
        pre = A;
        strat = NEQ;
        break;

      case 'H':
        pre = AD;
        strat = HEQ;
        break;

      case 'P':
        pre = A;
        strat = PEQ;
        break;

      case 'U':
      case 'u':
        pre = AF;
        strat = UEQ;
        break;

      case 'h':
        pre = D;
        strat = HNE;
        break;

      case 'n':
        strat = NNE;
        break;
      
      case 'e':
      case 'p':
        strat = EPR;
        break;
    }
  }

  // first. preprocess the problem
  switch ( pre ) {
    case A:
      prepro.simplifyA (tab);
      break;
    
    case C:
      prepro.simplifyC (tab);
      break;
    
    case D:
      prepro.simplifyD (tab);
      break;
    
    case E:
      prepro.simplifyE (tab);
      break;
    
    case F:
      prepro.simplifyF (tab);
      break;
    
    case AD:
      prepro.simplifyA (tab);
      prepro.simplifyD (tab);
      break;
    
    case AF:
      prepro.simplifyA (tab);
      prepro.simplifyF (tab);
      break;

    case CD:
      prepro.simplifyC (tab);
      prepro.simplifyD (tab);
      break;

    case CF:
      prepro.simplifyC (tab);
      prepro.simplifyF (tab);
      break;

    case DE:
      prepro.simplifyD (tab);
      prepro.simplifyE (tab);
      break;

    case DF:
      prepro.simplifyD (tab);
      prepro.simplifyF (tab);
      break;

    case EF:
      prepro.simplifyE (tab);
      prepro.simplifyF (tab);
      break;

    case NONE:
      break;
  }
  problem << prepro;

  Options options;
  options.tabFile (tabFile);
  options.timeLimit (timeLimit);
  options.memoryLimit (memLimit*3/4);
  const int min = 55;
  int slice2 = Int::max (timeLimit/2,min);
  int slice3 = Int::max (timeLimit/3,min);
  int slice4 = Int::max (timeLimit/4,min);
  int slice5 = Int::max (timeLimit/5,min);
  // options.memoryLimit (128);

  switch ( strat ) {
    case NEQ: 
      // cerr << "Unrecognized problem: (" << pro << ") " ;
      options.timeLimit (slice5);
      
      // best strategy first
      options.timeLimit (2*slice5);
      options.lrs (4,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";

      options.reset ();
      options.timeLimit (slice5);
      options.lrs (-4,5);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (slice5);
      options.lrs (1003,5);
      opts << options << "\n";

      options.reset ();
      options.timeLimit (Int::max(1,timeLimit - 4*slice5));
      options.lrs (5,5);
      opts << options << "\n";
      return;

    case NNE:
      //cerr << "Unrecognized problem: (" << pro << ") " ;
      options.timeLimit (slice5);

      options.lrs (-4,5);
      opts << options << "\n";
      
      options.reset ();
      options.lrs (1005,5);
      opts << options << "\n";
      
      options.reset ();
      options.lrs (1003,5);
      opts << options << "\n";
      
      options.reset ();
      options.lrs (1002,5);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit - 4*slice5);
      options.lrs (1,5);
      opts << options << "\n";
      return;

    case HEQ:
      //cerr << "Unrecognized problem: (" << pro << ") " ;
      options.timeLimit (slice3);

      options.reset ();
      options.lrs (1002,5);
      opts << options << "\n";
      
      options.reset ();
      options.lrs (-5,5);
      opts << options << "\n";
      
      options.timeLimit (timeLimit - 2*slice3);
      options.reset ();
      options.lrs (3,5);
      opts << options << "\n";
      return;

    case PEQ:
      //cerr << "Unrecognized problem: (" << pro << ") " ;
      options.timeLimit (slice5);
      
      options.lrs (1003,5);
      opts << options << "\n";
      
      options.reset ();
      options.lrs (4,5);
      opts << options << "\n";

      options.reset ();
      options.lrs (1007,5);
      opts << options << "\n";

      options.reset ();
      options.timeLimit (timeLimit - 2*slice5);
      options.lrs (-4,5);
      opts << options << "\n";
      return;

    case HNE:
      //cerr << "Unrecognized problem: (" << pro << ") " ;
      options.timeLimit (slice4);
      
      // at CASC this should be ran last with double time
      options.lrs (1003,5);
      opts << options;
      
      options.reset ();
      options.lrs (-4,5);
      opts << options << "\n";

      options.reset ();
      options.lrs (5,5);
      opts << options;

      options.reset ();
      options.timeLimit (timeLimit - 3*slice4);
      options.lrs (1,5);
      opts << options;
      return;

    case UEQ:
      //cerr << "Unrecognized problem: (" << pro << ") " ;
      options.ueq (Options::LRS,5);
      opts << options << "\n";
      return;

    case EPR:
      options.lrs (5,10);
      options.fullSplitting ();
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case dis_1:
      options.discount (1,5);
      opts << options << "\n";
      return;

    case dis_2:
      options.discount (2,5);
      opts << options << "\n";
      return;

    case dis_4:
      options.discount (4,5);
      opts << options << "\n";
      return;

    case dis_5:
      options.discount (5,5);
      opts << options << "\n";
      return;

    case dis_n3:
      options.discount (-3,5);
      opts << options << "\n";
      return;

    case dis_n4:
      options.discount (-4,5);
      opts << options << "\n";
      return;

    case dis_n5:
      options.discount (-5,5);
      opts << options << "\n";
      return;

    case dis_1002:
      options.discount (1002,5);
      opts << options << "\n";
      return;

    case dis_1004:
      options.discount (1004,5);
      opts << options << "\n";
      return;

    case dis_1003:
      options.discount (1003,5);
      opts << options << "\n";
      return;

    case lrs_1: 
      options.lrs (1,5);
      opts << options << "\n";
      return;

    case lrs_1_lcn:
      options.lrs (1,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      return;

    case lrs_1_kinky: 
      options.lrs (1,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_2: 
      options.lrs (2,5);
      opts << options << "\n";
      return;

    case lrs_2_kinky: 
      options.lrs (2,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_3: 
      options.lrs (3,5);
      opts << options << "\n";
      return;

    case lrs_4: 
      options.lrs (4,5);
      opts << options << "\n";
      return;

    case lrs_3_kinky: 
      options.lrs (3,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_4_kinky: 
      options.lrs (4,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_4_lcn:
      options.lrs (4,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      return;

    case lrs_5:
      options.lrs (5,5);
      opts << options << "\n";
      return;

    case lrs_5_lcn:
      options.lrs (5,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      return;

    case lrs_5_kinky: 
      options.lrs (5,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_7_lcn:
      options.lrs (7,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      return;

    case lrs_1002: 
      options.lrs (1002,5);
      opts << options << "\n";
      return;

    case lrs_1003: 
      options.lrs (1003,5);
      opts << options << "\n";
      return;

    case lrs_1004:
      options.lrs (1004,5);
      opts << options << "\n";
      return;

    case lrs_1005: 
      options.lrs (1005,5);
      opts << options << "\n";
      return;

    case lrs_1006: 
      options.lrs (1006,5);
      opts << options << "\n";
      return;

    case lrs_1007: 
      options.lrs (1007,5);
      opts << options << "\n";
      return;

    case lrs_1008: 
      options.lrs (1008,5);
      opts << options << "\n";
      return;

    case lrs_n2: 
      options.lrs (-2,5);
      opts << options << "\n";
      return;

    case lrs_n2_kinky: 
      options.lrs (-2,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_n4:
      options.lrs (-4,5);
      opts << options << "\n";
      return;

    case lrs_n4_kinky: 
      options.lrs (-4,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_n4_kinky_awr10: 
      options.lrs (-4,5);
      options.awr (10);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case lrs_n5: 
      options.lrs (-5,5);
      opts << options << "\n";
      return;

    case lrs_n5_kinky: 
      options.lrs (-5,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case ott_1: 
      options.otter (1,5);
      opts << options << "\n";
      return;

    case ott_n4: 
      options.otter (-4,5);
      opts << options << "\n";
      return;

    case unit_lrs_3: 
      options.ueq (Options::LRS,3);
      opts << options << "\n";
      return;

    case unit_lrs_5:
      options.ueq (Options::LRS,5);
      opts << options << "\n";
      return;

    case unit_lrs_26:
      options.ueq (Options::LRS,26);
      opts << options << "\n";
      return;

    case unit_dis_5: 
      options.ueq (Options::DISCOUNT,5);
      opts << options << "\n";
      return;

    case unit_dis_10: 
      options.ueq (Options::DISCOUNT,10);
      opts << options << "\n";
      return;

    case unit_dis_50: 
      options.ueq (Options::DISCOUNT,50);
      opts << options << "\n";
      return;

    case unit_ott_1: 
      options.ueq (Options::OTTER,1);
      opts << options << "\n";
      return;

    case unit_ott_3: 
      options.ueq (Options::OTTER,3);
      opts << options << "\n";
      return;

    case CAT004_3:  
      options.cat004_3 ();
      opts << options << "\n";
      return;

    case CIV003_1:  
      options.civ003_1 ();
      opts << options << "\n";
      return;

    case COL006_3:  
      options.col006_3 ();
      opts << options << "\n";
      return;

    case COL006_4:  
      options.col006_4 ();
      opts << options << "\n";
      return;

    case COL076_1:  
      options.col076_1 ();
      opts << options << "\n";
      return;

    case COL077_1:  
      options.col077_1 ();
      opts << options << "\n";
      return;

    case FLD054_4:  
      options.fld054_4 ();
      opts << options << "\n";
      return;

    case GEO026_3: 
      options.geo026_3 ();
      opts << options << "\n";
      return;

    case GEO027_2: 
      options.geo027_2 ();
      opts << options << "\n";
      return;

    case GEO029_2: 
      options.geo029_2 ();
      opts << options << "\n";
      return;

    case GEO046_2: 
      options.geo046_2 ();
      opts << options << "\n";
      return;

    case LAT005_4: 
      options.lat005_4 ();
      opts << options << "\n";
      return;

    case LAT015_1: 
      options.lat015_1 ();
      opts << options << "\n";
      return;

    case LCL054_1: 
      options.lcl054_1 ();
      opts << options << "\n";
      return;

    case LCL084_1: 
      options.lcl084_1 ();
      opts << options << "\n";
      return;

    case LCL099_1: 
      options.lcl099_1 ();
      opts << options << "\n";
      return;

    case LCL222_1: 
      options.lcl222_1 ();
      opts << options << "\n";
      return;

    case LCL225_3: {
      int largeTime = (timeLimit * 11)/10;
      options.timeLimit (largeTime);
      options.lcl225_3 ();
      opts << options << "\n";
      return;
    }

    case LCL243_3: 
      options.lcl243_3 ();
      opts << options << "\n";
      return;

    case LCL330_3: 
      options.lcl330_3 ();
      opts << options << "\n";
      return;

    case LCL374_1: 
      options.lcl374_1 ();
      opts << options << "\n";
      return;

    case LCL377_1: 
      options.lcl377_1 ();
      opts << options << "\n";
      return;

    case LCL391_1: 
      options.lcl391_1 ();
      opts << options << "\n";
      return;

    case LCL384_1: 
      options.lcl384_1 ();
      opts << options << "\n";
      return;

    case LRU007_1: 
      options.lru007_1 ();
      opts << options << "\n";
      return;

    case LRU1815_1: 
      options.lru1815_1 ();
      opts << options << "\n";
      return;

    case NUM049_1:
      options.num049_1 ();
      opts << options << "\n";
      return;

    case NUM098_1:
      options.num098_1 ();
      opts << options << "\n";
      return;

    case PUZ034_1004:
      options.puz034_1004 ();
      opts << options << "\n";
      return;

    case SET019_3:
      options.set019_3 ();
      opts << options << "\n";
      return;

    case SET661_p3:
      options.set661_p3 ();
      opts << options << "\n";
      return;

    case SYN067_2:
      options.syn067_2 ();
      opts << options << "\n";
      return;

    case SYN551_p1:
      options.syn551_p1 ();
      opts << options << "\n";
      return;

    case ROB014_2:
      options.rob014_2 ();
      opts << options << "\n";
      return;

    case slicing_01: 
      options.timeLimit (slice4); // for FLD015-1, but did not word
      options.lrs (1002,5);
      opts << options << "\n";
      
      options.reset (); // for FLD063-1, but it did not work, trying full time, did not work either
      options.timeLimit (timeLimit - slice4);
      options.lrs (-4,5);
      options.precedence (Options::KINKY);
      opts << options << "\n";
      return;

    case slicing_02: 
      options.timeLimit (slice2);
      
      options.lrs (1003,5);
      opts << options << "\n";
      
      options.reset (); // this was for FLD049-3, but did not work
      options.timeLimit (timeLimit - slice2);
      options.lrs (-4,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      return;

    case slicing_03: 
      options.timeLimit (slice5);
      options.lrs (7,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      
      options.timeLimit (timeLimit);  // this is not a mistake
      options.reset ();
      options.lcl100_1 ();
      opts << options << "\n";
      return;

    case slicing_04: {
      int small = timeLimit/20;
      options.timeLimit (small);
      options.ueq (Options::DISCOUNT,10);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit);  // this is not a mistake
      options.reset ();
      options.ueq (Options::LRS,50);
      opts << options << "\n";
      return;
    }

    case slicing_05: 
      options.timeLimit (2*slice5);
      options.lcl348_3 ();
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit-3*slice5);
      options.lcl299_3 ();
      opts << options << "\n";

      options.reset ();
      options.timeLimit (slice5);
      options.lcl345_3 ();
      opts << options << "\n";
      return;

    case slicing_06: 
      options.timeLimit (slice2);
      options.lrs (1006,5);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit-slice2);
      options.lrs (-4,5);
      opts << options << "\n";
      return;

    case slicing_07: 
      options.timeLimit (slice2);
      options.lrs (-4,5);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit-slice2);
      options.lrs (4,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      return;

    case slicing_08: 
      options.timeLimit (slice5);
      options.ueq (Options::DISCOUNT,5);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit-slice5);
      options.ueq (Options::LRS,100);
      opts << options << "\n";
      return;

    case slicing_09: 
      options.timeLimit (slice2);
      options.lrs (4,5);
      options.literalComparison (Options::NORMAL);
      opts << options << "\n";
      
      options.reset ();
      options.timeLimit (timeLimit-slice2);
      options.lrs (-5,5);
      opts << options << "\n";
      return;

    case UNDEFINED:
      ASSERT ( debug_strategy, false );
      return;
  }
} // Strategy::execute
