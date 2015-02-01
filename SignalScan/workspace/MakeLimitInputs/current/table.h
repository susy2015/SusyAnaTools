//class table for formatted output
//Christian Autermann, Universität Hamburg
#ifndef TABLE_H
#define TABLE_H

/* ----------------------------------------------------------------------------------------------
  Usage:
  ------

  using Table::TTable;

  TTable table("Particle mass table");
  table.AddColumn<string>("particle");
  table.AddColumn<double>("mass [GeV]");
  table.AddColumn<int>("number");
  
  table << "top" << 172. << 2;
  table << "W"   <<  80. << 13;
  
  cout << table << endl;
  ----------------------------------------------------------------------------------------------*/


#include <iostream>
#include <string>
#include <vector>

#include "column.h"

namespace Table {
 
  ///Different Styles, default is Plain
  enum TableStyle {Empty, Plain, TeX};

  class TTable {
   public:

    ///Constructor & Destructor allowing to directly set the table header, the delimiter between columns and the caption
    TTable():header_(""),delimiter_(" "),caption_(""),style_(Plain),idx_(0){};
    TTable(std::string h):header_(h),delimiter_(" "),caption_(""),style_(Plain),idx_(0){};
    TTable(std::string h, std::string d):header_(h),delimiter_(d),caption_(""),style_(Plain),idx_(0){};
    TTable(std::string h,std::string d,std::string c):header_(h),delimiter_(d),caption_(c),style_(Plain),idx_(0){};
    virtual ~TTable(){ for (std::vector<TColumnBase*>::iterator it=table_.begin();it!=table_.end();++it) delete *it; table_.clear(); };

    ///Method to specify the columns in this table. To be called once per column
    template <class T>      void AddColumn(const std::string& header){table_.push_back(new TColumn<T>(header));};

    ///Methods to set the contents of a cell in the table
    ///One argument: The active cell is set, and the next cell (left to right, top down) becomes active
    template <class T>      void Set(T value){table_.at(idx_++)->Add( new T(value) );if(idx_>=table_.size())idx_=0;};
    /*spezialize for char*/ void Set(const char* value){table_.at(idx_++)->Add( new std::string(value)); if(idx_>=table_.size())idx_=0;};

    ///Two arguments: New cells are appended at the bottom of the table; idx specifies the column. Column (idx+1) modulo n_columns is set active.
    template <class T>      void Set(unsigned idx,T value){table_.at(idx)->Add( new T(value) );idx_=(idx==table_.size()?0:idx+1);};
    /*spezialize for char*/ void Set(unsigned idx,const char* value){table_.at(idx)->Add( new std::string(value) );idx_=(idx==table_.size()?0:idx+1);};

    ///Three arguments: This method can be used to overwrite the contents of cell (x,y) with value.
    template <class T>      void Set(unsigned x, unsigned y, const T& value){static_cast<TColumn<T> >(table_.at(x))->Set(y,value);};

    ///Number of lines of this table; header and caption are not counted.
    unsigned Length() const {
      unsigned length=0;
      for (std::vector<TColumnBase*>::const_iterator it=table_.begin();it!=table_.end();++it)
	if ((*it)->Size()>length) length = (*it)->Size();
      return length;      
    };

    ///Methods to get and set Header, Delimiter between columns, caption, style of the table
    std::string GetHeader() const{return header_;};
    std::string GetDelimiter() const{return delimiter_;};
    std::string GetCaption() const{return caption_;};
    void SetHeader(const std::string& s){header_=s;};
    void SetDelimiter(const std::string& s){delimiter_=s;};
    void SetCaption(const std::string& s){caption_=s;};
    void SetStyle(TableStyle s){style_=s;};
    TableStyle GetStyle() const {return style_;};
    void SetMinumumWidth(unsigned w){table_.at(idx_)->SetMinumumWidth(w);}
    void SetMinumumWidth(unsigned w,unsigned idx){table_.at(idx)->SetMinumumWidth(w);}
    unsigned GetWidth(){return table_.at(idx_)->GetCurrentWidth();}
    unsigned GetWidth(unsigned idx){return table_.at(idx)->GetCurrentWidth();}

    ///Method to set the precision of column i. To be used e.g for <double> columns. Default is 1.
    void SetPrecision(unsigned i, unsigned p){table_[i]->SetPrecision(p);};

    ///Set all columns to one precision, default precision is 1
    void SetPrecision(unsigned p){ for (std::vector<TColumnBase*>::const_iterator it=table_.begin();it!=table_.end();++it) (*it)->SetPrecision(p);};

    const std::vector<TColumnBase*> * GetTable() const {return &table_;};

   private:
    std::string header_, delimiter_, caption_;
    std::vector<TColumnBase*> table_;
    TableStyle style_;
    unsigned idx_;
  };

  template <class T> TTable& operator<<(TTable& tab, T t){
    tab.Set(t);
    return tab;
  }

  ///Overloading the std::ostream operator for output.
  std::ostream& operator<<( std::ostream& os, const TTable& tab );

}

#endif
