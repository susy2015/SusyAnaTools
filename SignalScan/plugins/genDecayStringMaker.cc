/* class genDecayStringMaker
 *
 * \author Luca Lista, INFN
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/EDFilter.h"

class genDecayStringMaker : public edm::EDFilter{
public:
  genDecayStringMaker( const edm::ParameterSet & );
private:
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag src_;
  std::string decay( const reco::Candidate &, std::list<const reco::Candidate *> &, std::vector<int> &idxVec ) ;
  edm::ESHandle<ParticleDataTable> pdt_;
  /// print parameters
  bool printDecay_, printP4_, printPtEtaPhi_, printVertex_;

  std::vector<std::string> keyDecayStrs_;

  /// print 4 momenta
  std::string printP4( const reco::Candidate & ) const;
  /// accept candidate
  bool accept( const reco::Candidate &, const std::list<const reco::Candidate *> & ) const;
  /// select candidate
  bool select( const reco::Candidate & ) const;
  /// has valid daughters in the chain
  bool hasValidDaughters( const reco::Candidate & ) const;

  int countKeyStr(const std::string &bigStr, const std::string &subStr);

  edm::Handle<edm::View<reco::Candidate> > particles;

  int find_idx(const reco::Candidate & target);
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include <iostream>
#include <sstream> 
#include <algorithm>
using namespace std;
using namespace edm;
using namespace reco;

genDecayStringMaker::genDecayStringMaker( const ParameterSet & cfg ) :
  src_( cfg.getParameter<InputTag>( "src" ) ),
  printDecay_( cfg.getUntrackedParameter<bool>( "printDecay", false ) ),
  printP4_( cfg.getUntrackedParameter<bool>( "printP4", false ) ),
  printPtEtaPhi_( cfg.getUntrackedParameter<bool>( "printPtEtaPhi", false ) ),
  printVertex_( cfg.getUntrackedParameter<bool>( "printVertex", false ) ),
  keyDecayStrs_( cfg.getParameter<std::vector<std::string> >("keyDecayStrs") ) {
 
  produces<std::vector<std::string> >("decayStr");
  produces<std::vector<int> >("decayType");
  produces<std::vector<std::string> >("keyStr");

  produces<std::vector<int> >("decayChainPartIdxVec");
  produces<std::vector<std::string> >("decayChainPartStrVec");
}

bool genDecayStringMaker::accept( const reco::Candidate & c, const list<const Candidate *> & skip ) const {
  if( find( skip.begin(), skip.end(), & c ) != skip.end() ) return false;
  return select( c );
}

bool genDecayStringMaker::select( const reco::Candidate & c ) const {
  return c.status() == 3;
}

bool genDecayStringMaker::hasValidDaughters( const reco::Candidate & c ) const {
  size_t ndau = c.numberOfDaughters();
  for( size_t i = 0; i < ndau; ++ i )
    if ( select( * c.daughter( i ) ) )
      return true;
  return false;
}

bool genDecayStringMaker::filter(edm::Event & event, const edm::EventSetup & es) {

  std::unique_ptr<std::vector<std::string> > decayStrPtr (new std::vector<std::string> );
  std::unique_ptr<std::vector<std::string> > keyStrPtr (new std::vector<std::string> );
  std::unique_ptr<std::vector<int> > decayTypePtr (new std::vector<int> );

  std::unique_ptr<std::vector<int> > decayChainPartIdxPtr(new std::vector<int> );
  std::unique_ptr<std::vector<std::string> > decayChainPartStrPtr(new std::vector<std::string> );

  es.getData( pdt_ );
  event.getByLabel( src_, particles );
  list<const Candidate *> skip;
  vector<const Candidate *> nodes, moms;
  for( View<Candidate>::const_iterator p = particles->begin();
       p != particles->end(); ++ p ) {
    if( p->numberOfMothers() > 1 ) {
      if ( select( * p ) ) {
	skip.push_back( & * p );
	nodes.push_back( & * p );
	for( size_t j = 0; j < p->numberOfMothers(); ++ j ) {
	  const Candidate * mom = p->mother( j );
	  const Candidate * grandMom;
	  while ( ( grandMom = mom->mother() ) != 0 )
	    mom = grandMom;
	  if ( select( * mom ) ) {
	    moms.push_back( mom );
	  }
	}
      }
    }
  }
//  cout << "-- decay: --" << endl;
  std::stringstream buffer;
  std::vector<int> idxVec;
  if( moms.size() > 0 ) {
    if ( moms.size() > 1 ){
      for( size_t m = 0; m < moms.size(); ++ m ) {
        std::vector<int> perIdxVec;
	string dec = decay( * moms[ m ], skip, perIdxVec );
	if ( ! dec.empty() ){
	  buffer << "{ " << dec << " } ";
          for(size_t ii=0; ii<perIdxVec.size(); ii++){ idxVec.push_back(perIdxVec[ii]); }
        }
      }
    }else{
      std::vector<int> perIdxVec;
      buffer << decay( * moms[ 0 ], skip, perIdxVec );
      for(size_t ii=0; ii<perIdxVec.size(); ii++){ idxVec.push_back(perIdxVec[ii]); }
    }
  }
  if ( nodes.size() > 0 ) {
    buffer << "-> ";
    if ( nodes.size() > 1 ) {
      for( size_t n = 0; n < nodes.size(); ++ n ) {    
	skip.remove( nodes[ n ] );
        std::vector<int> perIdxVec;
	string dec = decay( * nodes[ n ], skip, perIdxVec );
	if ( ! dec.empty() ) {
          for(size_t ii=0; ii<perIdxVec.size(); ii++){ idxVec.push_back(perIdxVec[ii]); }
	  if ( dec.find( "->", 0 ) != string::npos ){
	    buffer << " ( " << dec << " )";
	  }else{
	    buffer << " " << dec;
          }
        }
      }
    } else {
      skip.remove( nodes[ 0 ] );
      std::vector<int> perIdxVec;
      buffer << decay( * nodes[ 0 ], skip, perIdxVec );
      for(size_t ii=0; ii<perIdxVec.size(); ii++){ idxVec.push_back(perIdxVec[ii]); }
    }
  }
  if( printDecay_ ){
     cout << buffer.str().c_str();
     cout << endl;
     for(size_t ii=0; ii<idxVec.size(); ii++){
        cout<<idxVec[ii]<<"  ";
     }
     cout<<endl;
  }

  for(size_t ii=0; ii<idxVec.size(); ii++){
     const reco::Candidate &c = particles->at(idxVec[ii]);
     int id = c.pdgId();
     const ParticleData * pd = pdt_->particle( id );
     assert( pd != 0 );
     decayChainPartIdxPtr->push_back(idxVec[ii]);
     decayChainPartStrPtr->push_back(pd->name()); 
  }

  std::string decayStr = buffer.str();

  for(unsigned int ik=0; ik<keyDecayStrs_.size(); ik++){
     int cnt =0;
     for(size_t ii=0; ii<decayChainPartStrPtr->size(); ii++){
        if( keyDecayStrs_[ik] == decayChainPartStrPtr->at(ii) ) cnt++;
     }
     decayTypePtr->push_back(cnt);
     keyStrPtr->push_back(keyDecayStrs_[ik]);
     if( printDecay_ ) cout<<"ik : "<<ik<<"  "<<keyDecayStrs_[ik].c_str()<<"  cnt : "<<cnt<<std::endl;
  }
 
  decayStrPtr->push_back(buffer.str());
  event.put(std::move(decayStrPtr), "decayStr");
  event.put(std::move(decayTypePtr), "decayType");
  event.put(std::move(keyStrPtr), "keyStr");
  
  event.put(std::move(decayChainPartIdxPtr), "decayChainPartIdxVec");
  event.put(std::move(decayChainPartStrPtr), "decayChainPartStrVec");

  return true;
}

string genDecayStringMaker::printP4( const Candidate & c ) const {
  ostringstream cout;
  if ( printP4_ ) cout << " (" << c.px() << ", " << c.py() << ", " << c.pz() << "; " << c.energy() << ")"; 
  if ( printPtEtaPhi_ ) cout << " [" << c.pt() << ": " << c.eta() << ", " << c.phi() << "]";
  if ( printVertex_ ) cout << " {" << c.vx() << ", " << c.vy() << ", " << c.vz() << "}";
  return cout.str();
}

string genDecayStringMaker::decay( const reco::Candidate & c, 
				   list<const reco::Candidate *> & skip, std::vector<int> & idxVec) {
  char tmpStr[100];
  string out; idxVec.clear();
  if ( find( skip.begin(), skip.end(), & c ) != skip.end() ) 
    return out;
  skip.push_back( & c );

  
  int id = c.pdgId();
  const ParticleData * pd = pdt_->particle( id );  
  assert( pd != 0 );
  out += ( pd->name() + printP4( c ) );

  int perIdx = find_idx(c);
  idxVec.push_back(perIdx);
  sprintf(tmpStr, " [%d]", perIdx);
  out += tmpStr;

  size_t validDau = 0, ndau = c.numberOfDaughters();
  for( size_t i = 0; i < ndau; ++ i )
    if ( accept( * c.daughter( i ), skip ) )
      ++ validDau;
  if ( validDau == 0 ) return out;

  out += " ->";
  
  for( size_t i = 0; i < ndau; ++ i ) {
    const Candidate * d = c.daughter( i );
    if ( accept( * d, skip ) ) {
      std::vector<int> perIdxVec;
      string dec = decay( * d, skip, perIdxVec );
      if ( dec.find( "->", 0 ) != string::npos ){
	out += ( " ( " + dec + " )" );
      }else{
	out += ( " " + dec );
      }
      for(size_t ii=0; ii<perIdxVec.size(); ii++){ idxVec.push_back(perIdxVec[ii]); }
    }
  }
  return out;
}

int genDecayStringMaker::countKeyStr(const std::string &bigStr, const std::string &subStr){
   const char *p=bigStr.c_str();
   int total=0;
   while ( (p=strstr(p,subStr.c_str())) != NULL ){
      total++;
      p++;
   }
   if( p ) delete p;
   return total;
}

int genDecayStringMaker::find_idx(const reco::Candidate & target){
   int pickedIdx = -1;
   for(size_t ig=0; ig<particles->size(); ig++){
      const reco::Candidate & gen = particles->at(ig);
      if( target.p4() == gen.p4() && target.vertex() == gen.vertex() && target.charge() == gen.charge() ){
         pickedIdx = (int)ig;
         break;
      }
   }
   return pickedIdx;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( genDecayStringMaker );

