struct TrackHit {
    Int_t PDG;
    Float_t x;
    Float_t y;
    Float_t z;
    Float_t p;
    Float_t theta;
    Float_t phi;
};
        
    
void script_primerjava_filtered(){
    
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
    
    
    TH2F *xyphx = new TH2F("xyphx", "xy_photons_exp_pos", 1000, -120, 120, 1000, -120, 120);
    TH2F *divxyphx = new TH2F("divxyphx", "xy_photons_exp_pos", 1000, -120, 120, 1000, -120, 120);
    
    for (Int_t i=0; i<nentriesx; i++) {
        trx->GetEntry(i);
        int j=0;
        if(recHitx.p < 6) continue;
        for(auto &photon : *photonsx){
            if(photon.getMirror() != 0) continue;
            if(photon.getThetaCer() < 0.25 || photon.getThetaCer() > 0.4) continue;
            j++;
        }
        if(j!=0){
            xyphx->Fill(recHitx.x, recHitx.y, j);  
            divxyphx->Fill(recHitx.x, recHitx.y);
        }      
    }
    
    TH2F *xyphs = new TH2F("xyphs", "xy_photons_sim_pos", 1000, -120, 120, 1000, -120, 120);
    TH2F *divxyphs = new TH2F("divxyphs", "xy_photons_exp_pos", 1000, -120, 120, 1000, -120, 120);
    
    for (Int_t i=0; i<nentriess; i++) {
        trs->GetEntry(i);
        int j=0;
        if(recHits.p < 6) continue;
        for(auto &photon : *photonss){
            if(photon.getMirror() != 0) continue;
            if(photon.getThetaCer() < 0.25 || photon.getThetaCer() > 0.4) continue;
            j++;
        }
        if(j!=0){
            xyphs->Fill(recHits.x, recHits.y, j);  
            divxyphs->Fill(recHits.x, recHits.y);
        }
    }
    

    
    xyphx->Divide(divxyphx);
    xyphs->Divide(divxyphs);
    
    
    TCanvas * c1 = new TCanvas("c1");
    c1->Divide(2,1);

    c1->cd(1);
    xyphx->SetStats(kFALSE);
    xyphx->Draw("colz");
    
    c1->cd(2);
    xyphs->SetStats(kFALSE);
    xyphs->Draw("colz");
}

