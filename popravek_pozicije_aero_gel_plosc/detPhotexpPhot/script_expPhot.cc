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

struct TrackExp{
    Float_t e;
    Float_t mu;
    Float_t pi;
    Float_t K;
    Float_t p;
    Float_t d;
};
        

void script_expPhot(){

    TChain *trx = new TChain("arich");
    trx->Add("/afs/f9.ijs.si/data/belle/data/arich/arich_mumu_proc11*.root");
    
    TChain *trs = new TChain("arich");
    trs->Add("/afs/f9.ijs.si/data/belle/data/arich/arich_mumu_mc13a*.root");
    
    /*std::vector<Belle2::ARICHPhoton>* photonsx = 0;
    trx->SetBranchAddress("photons",&photonsx);
    
    std::vector<Belle2::ARICHPhoton>* photonss = 0;
    trs->SetBranchAddress("photons",&photonss);*/
    
    TrackHit recHitx;    
    trx->SetBranchAddress("recHit",&recHitx);

    TrackHit recHits;    
    trs->SetBranchAddress("recHit",&recHits);
    
    /*TrackWin winHitx;
    TrackWin winHits;
    trx->SetBranchAddress("winHit",&winHitx);
    trs->SetBranchAddress("winHit",&winHits);*/
    
    TrackExp expPhotx;
    TrackExp expPhots;
    trx->SetBranchAddress("expPhot",&expPhotx);
    trs->SetBranchAddress("expPhot",&expPhots);
    
    Int_t detPhotx, detPhots;
    
    trx->SetBranchAddress("detPhot",&detPhotx);
    trs->SetBranchAddress("detPhot",&detPhots);
    

    long int nentriesx = (Int_t)trx->GetEntries();
    long int nentriess = (Int_t)trs->GetEntries();
    
    TProfile2D *h2x = new TProfile2D("h2x", "Experiment", 1000, -120, 120, 1000, -120, 120);
    TProfile2D *h2s = new TProfile2D("h2s", "Simulation", 1000, -120, 120, 1000, -120, 120);
    TProfile *rx = new TProfile("rx", "Experiment", 500, 40, 120);
    TProfile *rs = new TProfile("rs", "Experiment", 500, 40, 120);
    
    
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        if(recHitx.p < 6) continue;
        h2x->Fill(recHitx.x, recHitx.y, expPhotx.mu);
        rx->Fill(sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y), expPhotx.mu);
    }
    
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        if(recHits.p < 6) continue;
        h2s->Fill(recHits.x, recHits.y, expPhots.mu);
        rs->Fill(sqrt(recHits.x*recHits.x + recHits.y*recHits.y), expPhots.mu);
    }
      
    TCanvas *c1 = new TCanvas("c1");
    c1->Divide(2,2);
    
    c1->cd(1);    
    h2x->Draw("colz");
    h2x->SetStats(kFALSE);
    h2x->SetXTitle("X");
    h2x->SetYTitle("Y");
    
    c1->cd(2);    
    h2s->Draw("colz");
    h2s->SetStats(kFALSE);
    h2s->SetXTitle("X");
    h2s->SetYTitle("Y");
    
    c1->cd(3);
    rs->Draw("hist");
    rx->Draw("same hist");
    rs->GetYaxis()->SetRangeUser(0,30);
    rs->SetLineColor(2);
    rs->SetTitle("expPhot; Radius; NumPhotons");
    
    TLegend *leg = new TLegend(0.86,0.35,0.99,0.45);
    leg->AddEntry(rx, "experiment");
    leg->AddEntry(rs, "simulation");
    leg->Draw();
    
    
    
}


