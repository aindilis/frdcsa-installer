//
//  file strategy.h (strategies)
//  07/06/2001 Manchester
//


#ifndef __strategy__
#define __strategy__


class Options;
class Prepro;
class Property;
class ostream;


#include "list.h"


class Strategy {
 public:

  enum Pre {
    NONE,
    A, C, D, E, F,
    AD, AF, CD, CF, DE, DF, EF,
  };

  enum Strat {
    UNDEFINED,

    NEQ,
    NNE,
    HEQ,
    PEQ,
    HNE,
    UEQ,
    EPR,

    dis_1,
    dis_2,
    dis_4,
    dis_5,
    dis_n3,
    dis_n4,
    dis_n5,
    dis_1002,
    dis_1003,
    dis_1004,

    lrs_1,
    lrs_1_lcn,
    lrs_1_kinky,
    lrs_2,
    lrs_2_kinky,
    lrs_3,
    lrs_3_kinky,
    lrs_4,
    lrs_4_lcn,
    lrs_4_kinky,
    lrs_5,
    lrs_5_lcn,
    lrs_5_kinky,
    lrs_7_lcn,
    lrs_1002,
    lrs_1003,
    lrs_1004,
    lrs_1005,
    lrs_1006,
    lrs_1007,
    lrs_1008,
    lrs_n2,
    lrs_n2_kinky,
    lrs_n4,
    lrs_n4_kinky,
    lrs_n4_kinky_awr10,
    lrs_n5,
    lrs_n5_kinky,

    ott_1,
    ott_n4,

    unit_dis_5,
    unit_dis_10,
    unit_dis_50,
    unit_lrs_3,
    unit_lrs_5,
    unit_lrs_26,
    unit_ott_1,
    unit_ott_3,

    CAT004_3,
    CIV003_1,
    COL006_3,
    COL006_4,
    COL076_1,
    COL077_1,
    FLD054_4,
    GEO026_3,
    GEO027_2,
    GEO029_2,
    GEO046_2,
    LAT005_4,
    LAT015_1,
    LCL054_1,
    LCL084_1,
    LCL099_1,
    LCL222_1,
    LCL225_3,
    LCL243_3,
    LCL330_3,
    LCL374_1,
    LCL384_1,
    LCL377_1,
    LCL391_1,
    LRU007_1,
    LRU1815_1,
    NUM049_1,
    NUM098_1,
    PUZ034_1004,
    SET019_3,
    SET661_p3,
    SYN067_2,
    SYN551_p1,
    ROB014_2,

    slicing_01,
    slicing_02,
    slicing_03,
    slicing_04,
    slicing_05,
    slicing_06,
    slicing_07,
    slicing_08,
    slicing_09,
  };

  // constructor
  explicit Strategy (Property&);

  // query/change structure
  //PreList* pre () const { return _pre; }
  //OptList* options () const { return _options; }

  // output
  void execute (Prepro&,ostream& options, ostream& problem, int timeLimit,int memLimit,char* tabFile, ostream& tab);

 private:
  Property& _property;
}; // class Strategy


#endif // __strategy__
