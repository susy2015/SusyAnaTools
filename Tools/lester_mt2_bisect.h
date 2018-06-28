
/*
 *  Copyright 2014, Christopher Lester, Univeristy of Cambridge
 *
 *  version 5: arXiv:1411.4312v5
 *    * made more portable by removal of use of __FILE__ and __LINE__ macros in debug statement
 *    * made fewer demands on poor C++ compilers (ROOT5/CINT) by removal of certain inline statements
 *    * added this changelog!
 * 
 *  version 4: arXiv:1411.4312v4
 *    * added copyright information
 *
 *  version 3: arXiv:1411.4312v3
 *    * added option to turn on/off deci-sectioning
 *    * made code slightly slower for readability gain
 *
 *  version 2: arXiv:1411.4312v2
 *    * no changes w.r.t. version 1
 *
 *  version 1: arXiv:1411.4312v1
 *    * initial public release
 *
 *  This file will let you calculate MT2 or Asymmetric MT2 relatively easily.
 *  An example showing how to do so, may be found below this copyright message.
 *
 *  (Note that this is a low-level library.  Various wrappers exist around
 *   it to allow easier interfacing to ROOT or ATLAS code.)
 *
 *   If you use this implementation, please cite:
 *
 *   http://arxiv.org/abs/1411.4312
 *
 *   as the paper documenting this particular implementation.
 *
 *   You might also need to cite:
 *
 *   http://arxiv.org/abs/hep-ph/9906349 
 *   Journal reference: Phys.Lett.B463:99-103,1999
 *   DOI: 10.1016/S0370-2693(99)00945-4
 *
 *   as the paper defining MT2. 
 *
 * Here is an example of it's use:
 

float mVisA = 10; // mass of visible object on side A.  Must be >=0.
float pxA = 20; // x momentum of visible object on side A.
float pyA = 30; // y momentum of visible object on side A.

float mVisB = 10; // mass of visible object on side B.  Must be >=0.
float pxB = -20; // x momentum of visible object on side B.
float pyB = -30; // y momentum of visible object on side B.

float pxMiss = -5; // x component of missing transverse momentum.
float pyMiss = -5; // y component of missing transverse momentum.

float chiA = 4; // hypothesised mass of invisible on side A.  Must be >=0.
float chiB = 7; // hypothesised mass of invisible on side B.  Must be >=0.

float desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.

// asymm_mt2_lester_bisect::disableCopyrightMessage();

float MT2 =  asymm_mt2_lester_bisect::get_mT2(
           mVisA, pxA, pyA,
           mVisB, pxB, pyB,
           pxMiss, pyMiss,
           chiA, chiB,
           desiredPrecisionOnMt2);

 */


#ifndef LESTER_TESTWHETHERELLIPSESAREDISJOINT_H
#define LESTER_TESTWHETHERELLIPSESAREDISJOINT_H

#include <cmath> // for fabs( ... )

/*
 * The
 *
 *             bool ellipsesAreDisjoint(const EllipseParams & e1, const EllipseParams & e2);
 *
 * function determines whether two ellipses (not both singular) are disjoint.
 * Ellipses are assumed to be solid objects with a filled interior.
 * They are disjoint it no part of their interiors overlap.
 * Singular (in this context) is defined below.
 *
 * It uses the method of:

Computer Aided Geometric Design 23 (2006) 324–350
A new approach to characterizing the relative position of two ellipses depending on one parameter
Fernando Etayo 1,3, Laureano Gonzalez-Vega ∗,2,3, Natalia del Rio 3
Departamento de Matematicas, Estadistica y Computacion, Universidad de Cantabria, Spain
Received 15 September 2004; received in revised form 2 November 2005; accepted 10 January 2006 Available online 28 February 2006

pointed out to me by Gary B. Huges and Mohcine Chraibi authors of

 Comput Visual Sci (2012) 15:291–301 DOI 10.1007/s00791-013-0214-3
 Calculating ellipse overlap areas Gary B. Hughes · Mohcine Chraibi

 * Note:
 *
 * Though the paper above talks only about ellipses, from playing with some test cases, I (CGL) have conjectured that the algorithm actually works well even if the conics are parabolas provided that the axx>0&&ayy>0 test is reduced to axx>=0&&ayy>=0&&axx*ayy!=0 ... which is true is good news for the similicity of the MT2 calculator ... as the MT2 calculator will not need to distinguish these two possibilities.  In a private communication between me (CGL) and the  authors of Computer Aided Geometric Design 23 (2006) 324–350, the authors have indicated that it is not unreasonable to believe that the code does indeed work on the parabolica cases too.  This algorithm relies on that generalisation, which may be the subject of a paper (to appear) from Etayo and Gonzalez-Vega.
 *
 *
 * Definition: an ellipse is defined with respect to cartesian co-ordinates (x,y) by an equation of the form;
 *
 * xVec^T A xVec = 0                 (1)
 *
 * where xVec is a columnar three vec containing (x,y,1) and where A is a symmetric matrix having elements:
 *
 *       [ axx axy ax  ]
 *   A = [ axy ayy ay  ]
 *       [ ax  ay  a   ].
 *
 * Therfore the ellipse equation would look like:
 *
 * axx x^2 + 2 axy x y + ayy y^2 + 2 ax x + 2 ay y + a = 0.
 *
 * Note that this parametrisation has one parameter too many ... the "A"-matrix can be multiplied by a non-zero constant, and the ellipse is not changed.
 * Etayo et al's implementation REQUIRES that axx and ayy be strictly positive.
 * The implementation herein doesn't quite enforce that. The implementation herein allows axx or ayy to be non-negative .... and it is left to the user to ensure that axx and ayy are not exactly zero.
 * Note also that (1) is general enough to contain all conic sections, so it is left to the user to ensure that only values of A consistent
 * with (non-singluar) ellipses are fed into the program below.  For our purposes, an ellipse is "singular" iff coeffLamPow3 (see below) is zero.
 */

namespace Lester {

struct EllipseParams {


  // Constructor for non-degenerate ellipses:
  /*
   * Ellipse is represented algebraically by:
   * c_xx x^2 + 2 c_xy x y + c_yy y^2 + 2 c_x x + 2 c_y y + c = 0.
   */
  EllipseParams(
    const float c_xx2,
    const float c_yy2,
    const float c_xy2,
    const float c_x2,
    const float c_y2,
    const float c2) :
    c_xx(c_xx2),
    c_yy(c_yy2),
    c_xy(c_xy2),
    c_x(c_x2),
    c_y(c_y2),
    c(c2) {
     //Etayo et al REQUIRE that c_xx and c_yy are non-negative, so:
     if (c_xx<0 || c_yy<0) {
       throw "precondition violation";
     }
    setDet();
  }
  EllipseParams() {
  }
  void setDet() {
    det = (2.0*c_x*c_xy*c_y + c*c_xx*c_yy - c_yy*c_x*c_x - c*c_xy*c_xy - c_xx*c_y*c_y) ;
  }
  // Consstructor for degenerate ellipse (i.e. a "dot" at (x0,y0) ).
  EllipseParams(
    const float x0,
    const float y0) :
    c_xx(1),
    c_yy(1),
    c_xy(0),
    c_x(-x0),
    c_y(-y0),
    c(x0*x0 + y0*y0),
    det(0) {
  }
  float lesterFactor(const EllipseParams & e2) const {
    const EllipseParams & e1 = *this;
    const float ans  = e1.c_xx*e1.c_yy*e2.c + 2.0*e1.c_xy*e1.c_y*e2.c_x - 2.0*e1.c_x*e1.c_yy*e2.c_x + e1.c*e1.c_yy*e2.c_xx - 2.0*e1.c*e1.c_xy*e2.c_xy + 2.0*e1.c_x*e1.c_y*e2.c_xy + 2.0*e1.c_x*e1.c_xy*e2.c_y - 2.0*e1.c_xx*e1.c_y*e2.c_y + e1.c*e1.c_xx*e2.c_yy - e2.c_yy*(e1.c_x*e1.c_x) - e2.c*(e1.c_xy*e1.c_xy) - e2.c_xx*(e1.c_y*e1.c_y);
    return ans;
  }
  bool operator==(const EllipseParams & other) const {
    return
      c_xx == other.c_xx &&
      c_yy == other.c_yy &&
      c_xy == other.c_xy &&
      c_x == other.c_x &&
      c_y == other.c_y &&
      c == other.c;
  }
 public:
  // Data
  float c_xx;
  float c_yy;
  float c_xy; // note factor of 2 above
  float c_x;  // note factor of 2 above
  float c_y;  // note factor of 2 above
  float c;
  float det; // The determinant of the 3x3 conic matrix
};

// This is the interface: users should call this function:
bool ellipsesAreDisjoint(const EllipseParams & e1, const EllipseParams & e2);

// This is an implementation thing: users should not call it:
bool __private_ellipsesAreDisjoint(const float coeffLamPow3, const float coeffLamPow2, const float coeffLamPow1, const float coeffLamPow0);

bool ellipsesAreDisjoint(const EllipseParams & e1, const EllipseParams & e2) {
  /* We want to construct the polynomial "Det(lamdba A + B)" where A and B are the 3x3 matrices associated with e1 and e2, and we want to get that
  polynomial in the form lambda^3 + a lambda^2 + b lambda + c.


  Note that by default we will not have unity as the coefficient of the lambda^3 term, however the redundancy in the parametrisation of A and B allows us to scale the whole ply until the first term does have a unit coefficient.
  */

  if (e1==e2) {
    return false; // Probably won't catch many cases, but may as well have it here.
  }

  // first get unscaled terms:
  const float coeffLamPow3 = e1.det; // Note that this is the determinant of the symmetric matrix associated with e1.
  const float coeffLamPow2 = e1.lesterFactor(e2);
  const float coeffLamPow1 = e2.lesterFactor(e1);
  const float coeffLamPow0 = e2.det; // Note that this is the determinant of the symmetric matrix associated with e2.

  // Since question is "symmetric" and since we need to dovide by coeffLamPow3 ... do this the way round that involves dividing by the largest number:

  if (fabs(coeffLamPow3) >= fabs(coeffLamPow0)) {
    return __private_ellipsesAreDisjoint(coeffLamPow3, coeffLamPow2, coeffLamPow1, coeffLamPow0); // normal order
  } else {
    return __private_ellipsesAreDisjoint(coeffLamPow0, coeffLamPow1, coeffLamPow2, coeffLamPow3); // reversed order
  }
}
bool __private_ellipsesAreDisjoint(const float coeffLamPow3, const float coeffLamPow2, const float coeffLamPow1, const float coeffLamPow0) {

  // precondition of being called:
  //assert(fabs(coeffLamPow3)>=fabs(coeffLamPow0));

  if(coeffLamPow3==0) {
    // The ellipses were singular in some way.
    // Cannot determine whether they are overlapping or not.
    throw 1;
  }

  // now scale terms to monomial form:
  const float a = coeffLamPow2 / coeffLamPow3;
  const float b = coeffLamPow1 / coeffLamPow3;
  const float c = coeffLamPow0 / coeffLamPow3;

#ifdef LESTER_DEEP_FIDDLE
  {
    const float thing1 = -3.0*b + a*a;
    const float thing2 = -27.0*c*c + 18.0*c*a*b + a*a*b*b - 4.0*a*a*a*c - 4.0*b*b*b;
    std::cout 
      << (thing1>0) << " && " << (thing2>0) << " && [[ " << (a>=0) << " " << (3.0*a*c + b*a*a - 4.0*b*b<0)  << " ] or "
      << "[ " << (a< 0)   << " ] =("<< ((a >= 0 /*&& thing1 > 0*/ && 3.0*a*c + b*a*a - 4.0*b*b< 0 /*&& thing2 > 0*/) ||
                                 (a <  0 /*&& thing1 > 0*/                                 /*&& thing2 > 0*/)) << ")] " << (
          ( (a >= 0 && thing1 > 0 && 3.0*a*c + b*a*a - 4.0*b*b< 0 && thing2 > 0) ||
                                 (a <  0 && thing1 > 0                                 && thing2 > 0))
          
          ) << std::endl;
  }
#endif

  // Use the main result of the above paper:
  const float thing1 = -3.0*b + a*a;
  if (thing1<=0) return false;
  const float thing2 = -27.0*c*c + 18.0*c*a*b + a*a*b*b - 4.0*a*a*a*c - 4.0*b*b*b;
  if (thing2<=0) return false;

  // ans true means ellipses are disjoint:
  const bool ans = ( (a >= 0 /*&& thing1 > 0*/ && 3.0*a*c + b*a*a - 4.0*b*b< 0 /*&& thing2 > 0*/) ||
                     (a <  0 /*&& thing1 > 0*/                                 /*&& thing2 > 0*/));
  return ans;

}

}

#endif








#ifndef ASYMM_MT2_BISECT_H
#define ASYMM_MT2_BISECT_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>


class asymm_mt2_lester_bisect {
 public:

  static const int MT2_ERROR=-1;

  static float get_mT2( // returns asymmetric mT2 (which is >=0), or returns a negative number (such as MT2_ERROR) in the case of an error.
    const float mVis1, const float pxVis1, const float pyVis1,
    const float mVis2, const float pxVis2, const float pyVis2,
    const float pxMiss, const float pyMiss,
    const float mInvis1, const float mInvis2,
    const float desiredPrecisionOnMT2=0, // This must be non-negative.  If set to zero (default) MT2 will be calculated to the highest precision available on the machine (or as close to that as the algorithm permits).  If set to a positive value, MT2 (note that is MT2, not its square) will be calculated to within +- desiredPrecisionOnMT2. Note that by requesting precision of +- 0.01 GeV on an MT2 value of 100 GeV can result in speedups of a factor of ...
    const bool useDeciSectionsInitially=true // If true, interval is cut at the 10% point until first acceptance, which gives factor 3 increase in speed calculating kinematic min, but 3% slowdown for events in the bulk.  Is on (true) by default, but can be turned off by setting to false.
  ) {

    const float mT2_Sq = get_mT2_Sq(
                            mVis1, pxVis1, pyVis1,
                            mVis2, pxVis2, pyVis2,
                            pxMiss,pyMiss,
                            mInvis1, mInvis2,
                            desiredPrecisionOnMT2,
                            useDeciSectionsInitially);
    if (mT2_Sq==MT2_ERROR) {
      return MT2_ERROR;
    }
    return sqrt(mT2_Sq);
  }
  
  static void disableCopyrightMessage(const bool printIfFirst=false) {
    static bool first = true;
    if (first && printIfFirst) {
    std::cout 
      << "\n\n"
      << "#=========================================================\n"
      << "# To disable this message, place a call to \n"
      << "# \n"
      << "#     asymm_mt2_lester_bisect::disableCopyrightMessage();\n"
      << "# \n"
      << "# somewhere before you begin to calculate your MT2 values.\n"
      << "#=========================================================\n"
      << "# You are calculating symmetric or asymmetric MT2 using\n"
      << "# the implementation defined in:\n"
      << "# \n"
      << "#     http://arxiv.org/abs/1411.4312\n"
      << "# \n"
      << "# Please cite the paper above if you use the MT2 values\n"
      << "# for a scholarly purpose. For the variable MT2 itself,\n"
      << "# please also cite:\n"
      << "# \n"
      << "#     http://arxiv.org/abs/hep-ph/9906349\n"
      << "#=========================================================\n"
      << "\n\n" << std::flush;
    }
    first = false;
  }

  static float get_mT2_Sq( // returns square of asymmetric mT2 (which is >=0), or returns a negative number (such as MT2_ERROR) in the case of an error.
    const float mVis1, const float pxVis1, const float pyVis1,
    const float mVis2, const float pxVis2, const float pyVis2,
    const float pxMiss, const float pyMiss,
    const float mInvis1, const float mInvis2,
    const float desiredPrecisionOnMT2=0, // This must be non-negative.  If set to zero (default) MT2 will be calculated to the highest precision available on the machine (or as close to that as the algorithm permits).  If set to a positive value, MT2 (note that is MT2, not its square) will be calculated to within +- desiredPrecisionOnMT2. Note that by requesting precision of +- 0.01 GeV on an MT2 value of 100 GeV can resJult in speedups of a factor of ..
    const bool useDeciSectionsInitially=true // If true, interval is cut at the 10% point until first acceptance, which gives factor 3 increase in speed calculating kinematic min, but 3% slowdown for events in the bulk.  Is on (true) by default, but can be turned off by setting to false.
      ) {


    disableCopyrightMessage(true); // By supplying an argument to disable, we actually ask for the message to be printed, if printing is not already disabled.   This counterintuitive function naming is to avoid the need to introduce static variable initialisations ....

    const float m1Min = mVis1+mInvis1; // when parent has this mass, ellipse 1 has smallest physical size
    const float m2Min = mVis2+mInvis2; // when parent has this mass, ellipse 2 has smallest physical size

    if (m1Min>m2Min) {
      // swap 1 and 2
      return asymm_mt2_lester_bisect::get_mT2_Sq(
               mVis2, pxVis2, pyVis2,
               mVis1, pxVis1, pyVis1,
               pxMiss, pyMiss,
               mInvis2, mInvis1,
               desiredPrecisionOnMT2
             );
    }

    // By now, we can be sure that m1Min <= m2Min
    assert(m1Min<=m2Min);

    const float mMin = m2Min; // when parent has this mass, both ellipses are physical, and at least one has zero size.  Note that the name "min" expresses that it is the minimum potential parent mass we should consider, not that it is the min of m1Min and m2Min.  It is in fact the MAX of them!

    // TODO: What about rounding?  What about idiots who give us mVis values that have been computed from E^2-p^2 terms that are perilously close to zero, or perilously degenerate?

    const float msSq = mVis1*mVis1;
    const float sx = pxVis1;
    const float sy = pyVis1;
    const float mpSq = mInvis1*mInvis1;

    const float mtSq = mVis2*mVis2;
    const float tx = pxVis2;
    const float ty = pyVis2;
    const float mqSq = mInvis2*mInvis2;

    const float sSq = sx*sx + sy*sy;
    const float tSq = tx*tx + ty*ty;
    const float pMissSq = pxMiss*pxMiss + pyMiss*pyMiss;
    const float massSqSum = msSq + mtSq + mpSq + mqSq;
    const float scaleSq = (massSqSum + sSq + tSq + pMissSq)/8.0;

// #define LESTER_DBG 1

#ifdef LESTER_DBG
    std::cout <<"\nMOO ";
#endif
    // Check for an easy MT2 zero, not because we think it will speed up many cases, but because it will allow us to, ever after, assume that scaleSq>0.
    if (scaleSq==0) {
      return 0;
    }
    const float scale = sqrt(scaleSq);

    // disjoint at mMin.  So find an mUpper at which they are not disjoint:
    float mLower = mMin;
    float mUpper = mMin + scale; // since scaleSq is guaranteed to be >0 at this stage, the adition of scaleSq quarantees that mUpperSq is also >0, so it can be exponentially grown (later) by doubling.
    unsigned int attempts=0;
    const unsigned int maxAttempts=10000;
    while (true) {
      ++attempts;

      const float mUpperSq = mUpper*mUpper;
      const Lester::EllipseParams & side1=helper(mUpperSq, msSq, -sx, -sy, mpSq, 0,      0     ); // see side1Coeffs in mathematica notebook
      const Lester::EllipseParams & side2=helper(mUpperSq, mtSq, +tx, +ty, mqSq, pxMiss, pyMiss); // see side2Coeffs in mathematica notebook

      bool disjoint;
      try {
        disjoint = Lester::ellipsesAreDisjoint(side1, side2);
      } catch (...) {
        return MT2_ERROR;
      }

      if (!disjoint) {
        break;
      }

      if (attempts>=maxAttempts) {
        std::cerr << "MT2 algorithm failed to find upper bound to MT2" << std::endl;
        return MT2_ERROR;
      }

#ifdef LESTER_DBG
      std::cout << " - ";
#endif
      mUpper *= 2; // grow mUpper exponentially
    }

    //const float tol = relativeTolerance * sqrt(scaleSq);

    // Now begin the bisection:
    bool goLow = useDeciSectionsInitially;
    while(desiredPrecisionOnMT2<=0 || mUpper-mLower>desiredPrecisionOnMT2) {

      const float trialM = ( goLow ?
                              (mLower*15+mUpper)/16  // bias low until evidence this is not a special case
                              :
                              (mUpper + mLower)/2.0 // bisect
                            ); // worry about this not being between mUpperSq and mLowerSq! TODO

      if (trialM<=mLower || trialM>=mUpper) {
        // We reached a numerical precision limit:  the interval can no longer be bisected!
#ifdef LESTER_DBG
        std::cout << " MACHINE_PREC " << std::setprecision(10) << mLower << " " << trialM << " " << mUpper << " " << mUpper-mLower << " " << desiredPrecisionOnMT2 << std::endl;
#endif
        return trialM*trialM;
      }
      const float trialMSq = trialM * trialM;
      const Lester::EllipseParams & side1 = helper(trialMSq, msSq, -sx, -sy, mpSq, 0,      0     ); // see side1Coeffs in mathematica notebook
      const Lester::EllipseParams & side2 = helper(trialMSq, mtSq, +tx, +ty, mqSq, pxMiss, pyMiss); // see side2Coeffs in mathematica notebook

      try {
        const bool disjoint = Lester::ellipsesAreDisjoint(side1, side2);
        if (disjoint) {
          mLower = trialM;
          goLow = false;
#ifdef LESTER_DBG
          std::cout << "UP " ;
#endif
        } else {
          mUpper = trialM;
#ifdef LESTER_DBG
          std::cout << "== ";
#endif
        }
      } catch (...) {
        // The test for ellipses being disjoint failed ... this means the ellipses became degenerate, which can only happen right at the bottom of the MT2 search range (subject to numerical precision).  So:
#ifdef LESTER_DBG
        std::cout << " THROW " << std::endl;
#endif
        return mLower*mLower;
      }
    }

    const float mAns = (mLower+mUpper)/2.0;

#ifdef LESTER_DBG
    std::cout << " USER_PREC " << std::endl;
#endif
    return mAns*mAns;
  };
 private:
  static float lestermax(const float x, const float y) {
    return (x>y)?x:y;
  }
  static const Lester::EllipseParams helper(const float mSq, // The test parent-mass value (squared)
       const float mtSq, const float tx, const float ty, // The visible particle transverse momentum
       const float mqSq, // The mass of the invisible particle
       const float pxmiss, const float pymiss
             ) {
    const float txSq = tx*tx;
    const float tySq = ty*ty;
    const float pxmissSq = pxmiss*pxmiss;
    const float pymissSq = pymiss*pymiss;


    const float c_xx = +4.0* mtSq + 4.0* tySq;

    const float c_yy = +4.0* mtSq + 4.0* txSq;

    const float c_xy = -4.0* tx*ty;

    const float c_x  = -4.0* mtSq*pxmiss - 2.0* mqSq*tx + 2.0* mSq*tx - 2.0* mtSq*tx  +
               4.0* pymiss*tx*ty - 4.0* pxmiss*tySq;

    const float c_y  = -4.0* mtSq*pymiss - 4.0* pymiss*txSq - 2.0* mqSq*ty + 2.0* mSq*ty - 2.0* mtSq*ty +
               4.0* pxmiss*tx*ty;

    const float c =   - mqSq*mqSq + 2*mqSq*mSq - mSq*mSq + 2*mqSq*mtSq + 2*mSq*mtSq - mtSq*mtSq +
                4.0* mtSq*pxmissSq + 4.0* mtSq*pymissSq + 4.0* mqSq*pxmiss*tx -
                4.0* mSq*pxmiss*tx + 4.0* mtSq*pxmiss*tx + 4.0* mqSq*txSq +
                4.0* pymissSq*txSq + 4.0* mqSq*pymiss*ty - 4.0* mSq*pymiss*ty +
                4.0* mtSq*pymiss*ty - 8.0* pxmiss*pymiss*tx*ty + 4.0* mqSq*tySq +
                4.0* pxmissSq*tySq;

    return Lester::EllipseParams(c_xx, c_yy, c_xy, c_x, c_y, c);
  }
};

void myversion(){

  std::cout << "Version is : 2014_11_13" << std::endl;

}

float MT(float px1, float px2, float py1, float py2, float m1 , float m2){
  float E1 = sqrt(px1*px1+py1*py1+m1*m1);
  float E2 = sqrt(px2*px2+py2*py2+m2*m2);
  float Msq = (E1+E2)*(E1+E2)-(px1+px2)*(px1+px2)-(py1+py2)*(py1+py2);
  if (Msq < 0) Msq = 0;
  return sqrt(Msq);
}

std::pair <float,float>  ben_findsols(float MT2, float px, float py, float visM, float Ma, float pxb, float pyb, float metx, float mety, float visMb, float Mb){

  //Visible particle (px,py,visM)                                                                                                                  
  std::pair <float,float> sols;

  ///////                                                                                                                                          
  //Find the minimizing points given MT2                                                                                                           
  //////                      

  float Pt = sqrt(px*px+py*py);
  float E = sqrt(Pt*Pt+visM*visM);
  float M = MT2;
  float E2 = E*E;
  float M2 = M*M;
  float M4 = M2*M2;
  float Ma2 = Ma*Ma;
  float Ma4 = Ma2*Ma2;
  float px2 = px*px;
  float py2 = py*py;
  float px4 = px2*px2;
  float py4 = py2*py2;
  float py3 = py2*py;
  float E4 = E2*E2;
  float TermA = E2*px-M2*px+Ma2*px-px2*px-px*py2;
  float TermB = -2.*px*py;
  float TermSqy0 = E4*E2-2.*E4*M2-2.*E4*Ma2-2.*E4*px2-2.*E4*py2+E2*M4-2.*E2*M2*Ma2+2.*E2*M2*px2+2.*E2*M2*py2+E2*Ma4+2.*E2*Ma2*px2-2.*E2*Ma2*py2+E2*px4+2.*E2*px2*py2+E2*py4;
  float TermSqy1 = -4.*E4*py+4.*E2*M2*py-4.*E2*Ma2*py+4.*E2*px2*py+4.*E2*py3;
  float TermSqy2 = -4.*E4+4.*E2*px2+4.*E2*py2;

  //First, determine the range.                                                                                                                   
  float myx = 0.;
  float myy = 0.;
  if (TermSqy1*TermSqy1-4.*TermSqy0*TermSqy2 < 0){
    //unbalanced                                                                                                                                 
  }
  else{
    float sol1 = (-TermSqy1 - sqrt(TermSqy1*TermSqy1-4.*TermSqy0*TermSqy2))/(2.*TermSqy2);
    float sol2 = (-TermSqy1 + sqrt(TermSqy1*TermSqy1-4.*TermSqy0*TermSqy2))/(2.*TermSqy2);
    float low = sol1;
    float high = sol2;
    if (low > high){
      low = sol2;
      high = sol1;
    }

    float myclose = 99999999.;
    for (float metpy = low; metpy<=high; metpy+=(high-low)/10000.){
      float metpx = -(TermB*metpy+TermA-sqrt(TermSqy0+TermSqy1*metpy+TermSqy2*metpy*metpy))*0.5/(E2-px2);
      float metpx2 = -(TermB*metpy+TermA+sqrt(TermSqy0+TermSqy1*metpy+TermSqy2*metpy*metpy))*0.5/(E2-px2);
      float mt1a = MT(px,metpx,py,metpy,visM,Ma);
      float mt1b = MT(px,metpx2,py,metpy,visM,Ma);
      float metpxb = metx-metpx;
      float metpx2b = metx-metpx2;
      float mt2a = MT(pxb,metpxb,pyb,mety-metpy,visMb,Mb);
      float mt2b = MT(pxb,metpx2b,pyb,mety-metpy,visMb,Mb);
      if (fabs(mt1a-mt2a) < myclose){
	myclose = fabs(mt1a-mt2a);
	myy = metpy;
	myx = metpx;
      }
      if (fabs(mt1b-mt2b) < myclose){
	myclose = fabs(mt1b-mt2b);
	myy = metpy;
	myx = metpx2;
      }
    }
  }

  sols.first = myx;
  sols.second = myy;

  return sols;

}

#endif




























