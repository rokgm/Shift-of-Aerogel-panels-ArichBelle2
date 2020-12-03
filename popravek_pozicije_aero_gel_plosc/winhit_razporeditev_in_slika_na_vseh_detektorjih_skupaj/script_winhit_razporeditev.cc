 
struct TrackHit {
    Int_t PDG;
    Float_t x;
    Float_t y;
    Float_t z;
    Float_t p;
    Float_t theta;
    Float_t phi;
};
    
struct TrackWin {
    Float_t x;
    Float_t y;
};
        
    
void winhit_razporeditev(){
    
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
    
    TrackWin winHitx;
    TrackWin winHits;
    trx->SetBranchAddress("winHit",&winHitx);
    trs->SetBranchAddress("winHit",&winHits);
    

    Int_t nentriesx = (Int_t)trx->GetEntries();
    Int_t nentriess = (Int_t)trs->GetEntries();
    
    TCanvas * c1 = new TCanvas("c1");
    c1->Divide(2,2);

    TH2F *h2x = new TH2F("h2x", "Experimenet: All", 200,-0.3,0.3,200,-0.3,0.3);
    TH2F *h2s = new TH2F("h2s", "Simulation: All", 200,-0.3,0.3,200,-0.3,0.3);
    TH1D *h1x = new TH1D("h1x", "Experiment", 100, 50, 115);
    TH1D *h1s = new TH1D("h1s", "Simulation", 100, 50, 115);
    
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        if(recHitx.p > 6){
            if(65 < sqrt(winHitx.x * winHitx.x + winHitx.y * winHitx.y) || 100 > sqrt(winHitx.x * winHitx.x + winHitx.y * winHitx.y)){
                h1x->Fill(sqrt(winHitx.x*winHitx.x + winHitx.y*winHitx.y));
                for(auto &photon : *photonsx){
                    if(photon.getMirror() != 0) continue;
                    h2x->Fill((photon.getThetaCer()*TMath::Cos(photon.getPhiCerTrk())), (photon.getThetaCer()*TMath::Sin(photon.getPhiCerTrk())));
                }
            }
        }
    }
    
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        if(recHits.p > 6){
            if(65 < sqrt(winHits.x * winHits.x + winHits.y * winHits.y) || 100 > sqrt(winHits.x * winHits.x + winHits.y * winHits.y)){
                h1s->Fill(sqrt(winHits.x*winHits.x + winHits.y*winHits.y));
                for(auto &photon : *photonss){
                    if(photon.getMirror() != 0) continue;
                    h2s->Fill((photon.getThetaCer()*TMath::Cos(photon.getPhiCerTrk())), (photon.getThetaCer()*TMath::Sin(photon.getPhiCerTrk())));
                }
            }
        }
    }
    
    c1->cd(1);
    h2x->Draw("colz");
    h2x->SetStats(kFALSE);
    
    c1->cd(2);
    h2s->Draw("colz");
    h2s->SetStats(kFALSE);
    
    c1->cd(3);
    h1x->Draw();
    
    c1->cd(4);
    h1s->Draw();

}
