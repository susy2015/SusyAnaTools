//=======================================================
// indexSort.h
// Description : Template class to find indexSorts
//=======================================================
// Copyright 2012 - Hongxuan Liu 
// No warranty, implied or expressed, is included.
// Author is not liable for any type of loss through 
// the use of this source code. Use it at your own risk!
//=======================================================


#ifndef __INDEXSORT_H__
#define __INDEXSORT_H__

#include <algorithm>

namespace stdindexSort
{
   template <class RAIter, class Compare> 
   class PairComp {
      public:
      Compare comp ;
      PairComp(Compare comp_) : comp(comp_) {}
      bool operator() (const std::pair<size_t,RAIter>& a, const std::pair<size_t,RAIter>& b) const { return comp(*a.second, *b.second); }
   };

   template <class RAIter, class Compare>
   void argsort(RAIter iterBegin, RAIter iterEnd, Compare comp, std::vector<size_t>& indexes) {

      std::vector< std::pair<size_t,RAIter> > pv ;
      pv.reserve(iterEnd - iterBegin) ;

      RAIter iter ;
      size_t k ;
      for (iter = iterBegin, k = 0 ; iter != iterEnd ; iter++, k++) {
         pv.push_back( std::pair<int,RAIter>(k,iter) ) ;
      }

      std::sort(pv.begin(), pv.end(), PairComp<RAIter, Compare>(comp));
 
      indexes.resize(pv.size());
      for(size_t ii=0; ii<pv.size(); ii++){ indexes[ii] = pv[ii].first; }
   }
}

#endif

