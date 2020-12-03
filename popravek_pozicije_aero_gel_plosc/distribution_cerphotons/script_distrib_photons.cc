struct TrackHit {
    Int_t PDG;
    Float_t x;
    Float_t y;
    Float_t z;
    Float_t p;
    Float_t theta;
    Float_t phi;
    };
    
void script_distrib_photons() {
    
    TChain *trx = new TChain("arich");
    trx->Add("/afs/f9.ijs.si/data/belle/data/arich/arich_mumu_proc11*.root");
    
    TChain *trs = new TChain("arich");
    trs->Add("/afs/f9.ijs.si/data/belle/data/arich/arich_mumu_mc13a*.root");
    
    std::vector<Belle2::ARICHPhoton>* photonsx = 0;
    trx->SetBranchAddress("photons",&photonsx);
    
    std::vector<Belle2::ARICHPhoton>* photonss = 0;
    trs->SetBranchAddress("photons",&photonss);
    
    TrackHit recHitx;    
    trx->SetBranchAddress("recHit",&recHitx);

    TrackHit recHits;    
    trs->SetBranchAddress("recHit",&recHits);
    
    Int_t nentriesx = (Int_t)trx->GetEntries();
    Int_t nentriess = (Int_t)trs->GetEntries();
    
    TH1D *distphx = new TH1D("distphx", "Distribution of CerPhotons", 66, -0.5, 65.5);
    TH1D *distphs = new TH1D("distphs", "Distribution of CerPhotons", 66, -0.5, 65.5);
    
    
    for(int j=0; j<nentriesx; j++){
        trx->GetEntry(j);  
        int i=0;
        if(recHitx.p < 6) continue;
        
            if(65 < sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y) && 100 > sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y)){
                for(auto &photon : *photonsx){
                    if(photon.getMirror() != 0) continue;
                    i++;
                }
            }
        if(i!=0){
            distphx->Fill(i);
        }
    }

    
    for(int j=0; j<nentriess; j++){
        trs->GetEntry(j);  
        int i=0;
        if(recHitx.p < 6) continue;
        
            if(65 < sqrt(recHits.x * recHits.x + recHits.y * recHits.y) && 100 > sqrt(recHits.x * recHits.x + recHits.y * recHits.y)){
                for(auto &photon : *photonss){
                    if(photon.getMirror() != 0) continue;
                    i++;
                }
            }
        if(i!=0){
            distphs->Fill(i);
        }
    }  

    
    distphs->Draw();
    distphx->Draw("same");
    distphs->SetLineColor(2);
    TLegend* leg = new TLegend(0.78,0.65,0.98,0.75);
    leg->AddEntry(distphx, "experiment");
    leg->AddEntry(distphs, "simulation");
    leg->Draw();
    
    
    
}
