//output stream for table
//Christian Autermann, Universität Hamburg
#include "table.h"
#include <iomanip>
namespace Table {

  std::ostream& operator<<( std::ostream& os, const TTable& tab )
  {
    //determine current style
    if (tab.GetTable()->size()==0) return os;
    std::string delimiter = tab.GetDelimiter();
    bool tex   = (tab.GetStyle()==TeX);
    bool empty = (tab.GetStyle()==Empty);
    //header
    if (tab.GetHeader()!="") os<<tab.GetHeader()<<std::endl;
    if (tex) {
      delimiter = " & ";
      os << "\\begin{tabular}{";
      for (unsigned i=0; i<tab.GetTable()->size(); ++i) os << "c";
      os << "}" <<std::endl;
    }  
    //column-headers
    std::vector<TColumnBase*>::const_iterator it=tab.GetTable()->begin();
    if (!empty){
      os<<std::setw((*it)->Width())<<(*it)->GetHeader();
      ++it;
      for (;it!=tab.GetTable()->end();++it){
        os <<delimiter<<std::setw((*it)->Width())<<(*it)->GetHeader();
      }
      //draw a line below the header
      if (!tex) {
        it=tab.GetTable()->begin(); 
        os.fill('-');
        os<<std::endl<<std::setw((*it)->Width())<<"";
        ++it;
        for (;it!=tab.GetTable()->end();++it){
  	  os << "+" << std::setw((*it)->Width()) << "";
        }
        os.fill(' ');
      } else
        os<<"\\\\ \\hline";
      os<<std::endl;
    } 
    else  //empty style: define at least the column width's
      for (;it!=tab.GetTable()->end();++it) (*it)->Width();
    //all rows of the table
    for (unsigned l=0; l<tab.Length(); ++l){
      //all columns of current row
      it=tab.GetTable()->begin();
      os<<std::setw((*it)->GetCurrentWidth())<<std::left<<(*it)->Str(l);
      ++it;
      for (;it!=tab.GetTable()->end();++it){
	os<<delimiter<<std::setw((*it)->GetCurrentWidth()) <<(*it)->Str(l);
      }
      if (tex) os<<"\\\\";
      os<<std::endl;
    }
    //caption
    if (!tex && tab.GetCaption()!="")
       os<<tab.GetCaption()<<std::endl;
    if (tex) {
       os<<"\\label{tab:xyz}"<<std::endl;
       if (tab.GetCaption()!="") os<<"\\caption{"<<tab.GetCaption()<<"}"<<std::endl;
       os<<"\\end{tabular}"<<std::endl;
      }  
    return os;
  }
}

