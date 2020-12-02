struct TrackHit {
    Int_t PDG;
    Float_t x;
    Float_t y;
    Float_t z;
    Float_t p;
    Float_t theta;
    Float_t phi;
};
        
    
void script_primerjava(){
    
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
    

    Int_t nentriesx = (Int_t)1000;//trx->GetEntries();
    Int_t nentriess = (Int_t)1000;//trs->GetEntries();
    
    
    
    
    TH1D * px = new TH1D("pxz", "magnitude_exp", 100, 0, 10);
    for (Int_t i=0; i<nentriesx; i++) {
        trx->GetEntry(i);   
        px->Fill(recHitx.p);      
    }
    
    TH1D * ps = new TH1D("ps", "magnitude_sim", 100, 0, 10);
    for (Int_t i=0; i<nentriess; i++) {
        trs->GetEntry(i);      
        ps->Fill(recHits.p);      
    }
    
    
    
        
    TH1D * rx = new TH1D("rx", "radius_exp", 100, 0, 120);
    for (Int_t i=0; i<nentriesx; i++) {
        trx->GetEntry(i);     
        if(recHitx.p < 6) continue;
        rx->Fill(sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y));       
    }
    
    TH1D * rs = new TH1D("rs", "radius_sim", 100, 0, 120);
    for (Int_t i=0; i<nentriess; i++) {
        trs->GetEntry(i);  
        if(recHits.p < 6) continue;
        rs->Fill(sqrt(recHits.x * recHits.x + recHits.y * recHits.y));       
    }
        
    
    
    
    TH2F *xyx = new TH2F("xyx", "xy_exp_pos", 100, -120, 120, 100, -120, 120);
    for (Int_t i=0; i<nentriesx; i++) {
        trx->GetEntry(i);     
        xyx->Fill(recHitx.x, recHitx.y);       
    }
    
    TH2F *xys = new TH2F("xys", "xy_sim_pos", 100, -120, 120, 100, -120, 120);
    for (Int_t i=0; i<nentriess; i++) {
        trs->GetEntry(i);     
        xys->Fill(recHits.x, recHits.y);       
    }
    
    
    
    
    TH1D *distthetcerx = new TH1D("distthetcerx", "distthetcerx", 100, 0.2, 0.4);
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        for(auto &photon : *photonsx){
        distthetcerx->Fill(photon.getThetaCer());
      }
    }
    
    float meanthetcerx = distthetcerx->GetMean();
    float stdthetcerx = distthetcerx->GetStdDev();

    TH1D *distphx = new TH1D("distphx", "Distribution of NCerPhotons", 36, -0.5, 35.5);
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        int j=0;
        if(recHitx.p < 6) continue;
        if(65 > sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y) || 100 < sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y)) continue;
        for(auto &photon : *photonsx){
            if(photon.getMirror() != 0) continue;
            if((meanthetcerx - 2* stdthetcerx) < photon.getThetaCer() && photon.getThetaCer() < (meanthetcerx + 2* stdthetcerx)){
                j++;
            }
        }
        if(j !=0) distphx->Fill(j);
    }
    
    
    TH1D *distthetcers = new TH1D("distthetcers", "distthetcers", 100, 0.2, 0.4);
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        for(auto &photon : *photonss){
        distthetcers->Fill(photon.getThetaCer());
      }
    }
    
    float meanthetcers = distthetcers->GetMean();
    float stdthetcers = distthetcers->GetStdDev();
    
    TH1D *distphs = new TH1D("distphs", "Distribution of NCerPhotons", 36, -0.5, 35.5);
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        int j=0;
        if(recHits.p < 6) continue;
        if(65 > sqrt(recHits.x * recHits.x + recHits.y * recHits.y) || 100 < sqrt(recHits.x * recHits.x + recHits.y * recHits.y)) continue;
        for(auto &photon : *photonss){
            if(photon.getMirror() != 0) continue;
            if((meanthetcers - 2* stdthetcers) < photon.getThetaCer() && photon.getThetaCer() < (meanthetcers + 2* stdthetcers)){
                j++;
            }
        }
        if(j != 0) distphs->Fill(j);
    }
    
    
    
    
    TH1D *distcerx = new TH1D("distcerx", "Distribution of Filtered CerTheta", 100, 0, 0.6);
    int k1=0;
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        if(recHitx.p < 6) continue;
        if(65 > sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y) || 100 < sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y)) continue;
        k1++;
        for(auto &photon : *photonsx){
            if(photon.getMirror() != 0) continue;
            
            distcerx->Fill(photon.getThetaCer());
        }
    }
    
    
    TH1D *distcers = new TH1D("distcers", "Distribution of Filtered CerTheta", 100, 0, 0.6);
    int k2=0;
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        if(recHits.p < 6) continue;
        if(65 > sqrt(recHits.x * recHits.x + recHits.y * recHits.y) || 100 < sqrt(recHits.x * recHits.x + recHits.y * recHits.y)) continue;
        k2++;
        for(auto &photon : *photonss){
            if(photon.getMirror() != 0) continue;
            
            distcers->Fill(photon.getThetaCer());
        }
    }
    
    
    
    
    TH1D *distunfilcerx = new TH1D("distunfilcerx", "Distribution of Unfiltered CerTheta", 100, 0, 0.6);
    int ku1=0;
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        ku1++;
        for(auto &photon : *photonsx){
        distunfilcerx->Fill(photon.getThetaCer());
      }
    }
    
    TH1D *distunfilcers = new TH1D("distunfilcers", "Distribution of Unfiltered CerTheta", 100, 0, 0.6);
    int ku2=0;
    for(int i=0; i<nentriesx; i++){
        trs->GetEntry(i);
        ku2++;
        for(auto &photon : *photonss){
        distunfilcers->Fill(photon.getThetaCer());
      }
    }
    
    
    
    
    TH2F *xyphx = new TH2F("xyphx", "xy_photons_exp_pos", 1000, -120, 120, 1000, -120, 120);
    
    for (Int_t i=0; i<nentriesx; i++) {
        trx->GetEntry(i);
        int j=0;
        for(auto &photon : *photonsx) j++;
        xyphx->Fill(recHitx.x, recHitx.y, j);       
    }
    
    TH2F *xyphs = new TH2F("xyphs", "xy_photons_sim_pos", 1000, -120, 120, 1000, -120, 120);
    
    for (Int_t i=0; i<nentriess; i++) {
        trs->GetEntry(i);
        int j=0;
        for(auto &photon : *photonss) j++;
        xyphs->Fill(recHits.x, recHits.y, j);       
    }
    
    
    TH2F *divxyphx = new TH2F("divxyphx", "xy_photons_exp_pos", 1000, -120, 120, 1000, -120, 120);
    
    for (Int_t i=0; i<nentriesx; i++) {
        trx->GetEntry(i);
        int j=0;
        for(auto &photon : *photonsx) j++;
        divxyphx->Fill(recHitx.x, recHitx.y);       
    }
    
    TH2F *divxyphs = new TH2F("divxyphs", "xy_photons_exp_pos", 1000, -120, 120, 1000, -120, 120);
    
    for (Int_t i=0; i<nentriess; i++) {
        trs->GetEntry(i);
        int j=0;
        for(auto &photon : *photonss) j++;
        divxyphs->Fill(recHits.x, recHits.y);       
    }
    
    xyphx->Divide(divxyphx);
    xyphs->Divide(divxyphs);
    
    
    
    
    
    TCanvas * c1 = new TCanvas("c1");
    c1->Divide(3,3);
        
    c1->cd(1);
    px->DrawNormalized();
    ps->SetLineColor(2);
    ps->DrawNormalized("same");
    TLegend* leg1 = new TLegend(0.78,0.65,0.98,0.75);
    leg1->AddEntry(distphx, "experiment");
    leg1->AddEntry(distphs, "simulation");
    leg1->Draw();
    
    c1->cd(2);
    rx->DrawNormalized();
    rs->SetLineColor(2);
    rs->DrawNormalized("same");
    TLegend* leg2 = new TLegend(0.78,0.65,0.98,0.75);
    leg2->AddEntry(distphx, "experiment");
    leg2->AddEntry(distphs, "simulation");
    leg2->Draw();
    
    c1->cd(4);
    xyx->SetStats(kFALSE);
    xyx->Draw("colz");
        
    c1->cd(5);
    xys->SetStats(kFALSE);
    xys->Draw("colz");
    
    c1->cd(3);
    distphx->DrawNormalized();
    distphs->DrawNormalized("same");
    distphs->SetLineColor(2);
    TLegend* leg3 = new TLegend(0.78,0.65,0.98,0.75);
    leg3->AddEntry(distphx, "experiment");
    leg3->AddEntry(distphs, "simulation");
    leg3->Draw();
    
    c1->cd(6);
    distcerx->Scale(1.0/k1);
    distcers->Scale(1.0/k2);
    distcerx->Draw();
    distcers->SetLineColor(2);
    distcers->Draw("same");
    TLegend* leg6 = new TLegend(0.78,0.65,0.98,0.75);
    leg6->AddEntry(distphx, "experiment");
    leg6->AddEntry(distphs, "simulation");
    leg6->Draw();
    
    c1->cd(9);
    distunfilcerx->Draw();
    distunfilcers->SetLineColor(2);
    distunfilcers->Draw("same");
    distunfilcerx->Scale(1.0/ku1);
    distunfilcers->Scale(1.0/ku2);
    TLegend* leg9 = new TLegend(0.78,0.65,0.98,0.75);
    leg9->AddEntry(distunfilcerx, "experiment");
    leg9->AddEntry(distunfilcers, "simulation");
    leg9->Draw();
    
    c1->cd(7);
    xyphx->SetStats(kFALSE);
    xyphx->Draw("colz");
    
    c1->cd(8);
    xyphs->SetStats(kFALSE);
    xyphs->Draw("colz");
}
