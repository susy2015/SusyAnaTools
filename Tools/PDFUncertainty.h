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
};

#endif // PDFUNCERTAINTY_H
