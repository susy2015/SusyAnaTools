#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

class prodNtupleVersionString : public edm::EDFilter {

  public:

    explicit prodNtupleVersionString(const edm::ParameterSet & iConfig);
    ~prodNtupleVersionString() {}

  private:
    std::vector<std::string> inputStr_;
    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
};


prodNtupleVersionString::prodNtupleVersionString(const edm::ParameterSet & iConfig){
   inputStr_ = iConfig.getParameter<std::vector<std::string> >("inputStr");
   produces<std::vector<std::string> >("");
}

bool prodNtupleVersionString::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects

  std::unique_ptr<std::vector<std::string> > prod(new std::vector<std::string>(inputStr_));
  iEvent.put(std::move(prod));

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodNtupleVersionString);
