//clomun.h helper class for table
//Christian Autermann, Universität Hamburg
#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>


namespace Table {

  ///virtual column base class
  class TColumnBase
  {
   public:
    TColumnBase():_header(""),precision_(1){};
    TColumnBase(std::string header):_header(header),precision_(1){};
    TColumnBase(std::string header, unsigned p):_header(header),precision_(p){};
    virtual ~TColumnBase(){};

    std::string         GetHeader() const {return _header;};
    void                SetHeader(const std::string& header){_header=header;};
    void                SetPrecision(unsigned p){precision_=p;};
    unsigned            GetPrecision() const {return precision_;};

    virtual void        SetMinumumWidth(unsigned p)=0;
    virtual void        Add(const void * )=0;
    virtual const void* Get(unsigned idx)=0;
    virtual unsigned    Size(){return 0;};
    virtual std::string Str(const unsigned i){return "";};
    virtual unsigned    Width(){return 0;};
    virtual unsigned    GetCurrentWidth(){return 0;};

   private:
    std::string _header;
    unsigned precision_;
  };


  ///A templated class for each column of class table
  template<typename T> 
  class TColumn : public TColumnBase {
   public:
    TColumn():TColumnBase(),width_(0){};
    TColumn(std::string header):TColumnBase(header),width_(0){};
    TColumn(std::string header, unsigned p):TColumnBase(header,p),width_(0){};
    virtual ~TColumn() {
      for (typename std::vector<const T*>::iterator it=field_.begin(); it!=field_.end(); ++it) delete *it;
      field_.clear();}

    virtual void        Add(const void * f){ field_.push_back( static_cast<const T*>(f));};
    virtual const void* Get(unsigned idx){return field_[idx];};
    virtual unsigned    Size(){return field_.size();};
    virtual void        SetMinumumWidth(unsigned p){width_=p;};
    std::string         Str(const unsigned i){
      if (i<0||i>=field_.size()) return "";
      std::stringstream ss;
      ss<<std::fixed << std::setprecision(GetPrecision())<< *field_[i];
      return ss.str();
    };
    virtual unsigned    Width(){
      std::stringstream ss;
      ss << GetHeader();
      width_=(ss.str().size()>width_?ss.str().size():width_);
      for (typename std::vector<const T*>::const_iterator it=field_.begin();it!=field_.end();++it){
	std::stringstream ss;
	ss <<std::fixed << std::setprecision(GetPrecision()) << **it;
	if (ss.str().size()>width_) width_=ss.str().size();
      }
      return width_; 
    }
    virtual unsigned    GetCurrentWidth(){return (width_>0?width_:Width());};

    void Set(unsigned i, T f){*field_.at(i)=f;};
    T    At(const unsigned i){return field_.at(i);};
   private:
    std::vector<const T*> field_;
    unsigned width_;
  };


}

#endif
