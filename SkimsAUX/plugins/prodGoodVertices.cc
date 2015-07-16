#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

class prodGoodVertices : public edm::EDFilter {

  public:

    explicit prodGoodVertices(const edm::ParameterSet & iConfig);
    ~prodGoodVertices() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag vtxSrc_;
    StringCutObjectSelector<reco::Vertex,true> vtxCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
};


prodGoodVertices::prodGoodVertices(const edm::ParameterSet & iConfig) :
   vtxCut_(iConfig.existsAs<std::string>("vtxCut") ? iConfig.getParameter<std::string>("vtxCut") : "", true)
{
  produces<int>("");
  vtxSrc_ = iConfig.getParameter<edm::InputTag>("vtxSrc");
}


bool prodGoodVertices::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);

  int vtxCnt =0;
  for(unsigned int iv=0; iv<vertices->size(); iv++){
     const reco::Vertex & vtx = (*vertices)[iv];
     if( !vtxCut_(vtx) ) continue;
     vtxCnt++;
  }

  std::auto_ptr<int> prod(new int(vtxCnt));
  iEvent.put(prod);

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodGoodVertices);
