#ifndef PDFUNCERTAINTY_H
#define PDFUNCERTAINTY_H

#include "LHAPDF/LHAPDF.h"
#include "NTupleReader.h"
#include <vector>

class PDFUncertainty
{
 public:
  PDFUncertainty();
  virtual ~PDFUncertainty();
  void getPDFUncertainty(NTupleReader& tr);
  void operator()(NTupleReader& tr);

 protected:
 private:
  std::vector<LHAPDF::PDF*> pdf1;
  std::vector<LHAPDF::PDF*> pdf2;
  std::vector<LHAPDF::PDF*> pdf3;
  std::string scaleWeight_, x1_, x2_, Q_, id1_, id2_;

  void setTreeNames(const NTupleReader& tr)
  {
      if(tr.checkBranch("met"))
      {
          scaleWeight_ = "ScaleWeightsMiniAOD";
          x1_ = "x1";
          x2_ = "x2";
          Q_ = "q";
          id1_ = "id1";
          id2_ = "id2";
      }
      else if(tr.checkBranch("MET"))
      {
          scaleWeight_ = "";
          x1_ = "";
          x2_ = "";
          Q_ = "";
          id1_ = "";
          id2_ = "";
      }
  }
};

#endif // PDFUNCERTAINTY_H
