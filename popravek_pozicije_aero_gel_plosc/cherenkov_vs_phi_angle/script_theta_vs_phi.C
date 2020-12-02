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
        

void script_theta_vs_phi(){

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
    
    int ntrkx=0;
    int ntrks=0;        
    
    TH2F *h2x = new TH2F("h2x", "Experiment", 100, -3.14, 3.14, 100, 0, 0.6);
    TH2F *h2s = new TH2F("h2s", "Experiment", 100, -3.14, 3.14, 100, 0, 0.6);
    TH1D *pyx;
    TGraph *grx = new TGraph();
    TGraph *sigx = new TGraph();
    TH1D *pys;
    TGraph *grs = new TGraph();
    TGraph *sigs = new TGraph();
    TGraph *igx = new TGraph();
    TGraph *igs = new TGraph();
    TGraph *pgx = new TGraph();
    TGraph *pgs = new TGraph();
    
    for(int j=0; j<nentriesx; j++){
        trx->GetEntry(j);   
            if(recHitx.p < 6) continue;
            
                if(65 < sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y) && 100 > sqrt(recHitx.x * recHitx.x + recHitx.y * recHitx.y)){
                    ntrkx++;
                    for(auto &photon : *photonsx){
                        if(photon.getMirror() != 0) continue;
                        h2x->Fill(photon.getPhiCer(), photon.getThetaCer());
                    }
                }
    }
    h2x->Scale(1./ntrkx);
    
    
    for(int j=1; j<100; j++){
    
        pyx = h2x->ProjectionY("pyx", j, j + 1);
        
        TF1 *f1 = new TF1("f1","gaus(0) + pol1(3)", 0.22, 0.44);
        f1->SetParameters(0.021,0.31,0.013,0.0007,0.002);
        pyx->Fit("f1","R");
            
        grx->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, f1->GetParameter(1));
        sigx->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, TMath::Abs(f1->GetParameter(2)));
        
        TF1 *f2 = new TF1("f2","gaus(0)", 0.22, 0.44);
        f2->SetParameters(f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2));
        
        TF1 *f3 = new TF1("f3","pol1(0)", 0.22, 0.44);
        f3->SetParameters(f1->GetParameter(3), f1->GetParameter(4));
        

        
        auto integralg = f2->Integral(0.22, 0.44);
        igx->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, integralg);
        
        auto integralp = f3->Integral(0.22,0.44);
        pgx->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, integralp);
        
    }
    
    
    
    
    for(int j=0; j<nentriesx; j++){
        trs->GetEntry(j);   
            if(recHits.p < 6) continue;
            
                if(65 < sqrt(recHits.x * recHits.x + recHits.y * recHits.y) && 100 > sqrt(recHits.x * recHits.x + recHits.y * recHits.y)){
                    ntrks++;
                    for(auto &photon : *photonss){
                        if(photon.getMirror() != 0) continue;
                        h2s->Fill(photon.getPhiCer(), photon.getThetaCer());
                    }
                }
    }
    h2s->Scale(1./ntrks);
    
    for(int j=1; j<100; j++){
    
        pys = h2s->ProjectionY("pys", j, j + 1);
        
        TF1 *f1 = new TF1("f1","gaus(0) + pol1(3)", 0.22, 0.44);
        f1->SetParameters(0.021,0.31,0.013,0.0007,0.0002);
        pys->Fit("f1","R");
        
        grs->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, f1->GetParameter(1));
        sigs->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, TMath::Abs(f1->GetParameter(2)));
        
        TF1 *f2 = new TF1("f2","gaus(0)", 0.22, 0.44);
        f2->SetParameters(f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2));
        
        TF1 *f3 = new TF1("f3","pol1(0)", 0.22, 0.44);
        f3->SetParameters(f1->GetParameter(3), f1->GetParameter(4));
        
        auto integralg = f2->Integral(0.22, 0.44);
        igs->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, integralg);
        
        auto integralp = f3->Integral(0.22,0.44);
        pgs->SetPoint(j-1, (j - 0.5) / 100 * 6.28 - 3.14, integralp);
    }
    
      
    TCanvas *c1 = new TCanvas("c1");
    c1->Divide(2,3);
    
    c1->cd(1);    
    h2x->Draw("colz");
    h2x->SetStats(kFALSE);
    h2x->SetTitle("Experiment");
    h2x->SetXTitle("Phi");
    h2x->SetYTitle("ThetaCer");
    
    c1->cd(2);    
    h2s->Draw("colz");
    h2s->SetStats(kFALSE);
    h2s->SetTitle("Simulation");
    h2s->SetXTitle("Phi");
    h2s->SetYTitle("ThetaCer");
    
        
    c1->cd(3);
    grx->SetMarkerStyle(20);
    grx->SetMarkerSize(0.4);
    
    grs->SetLineColor(2);
    grs->SetMarkerStyle(20);
    grs->SetMarkerSize(0.4);
    grs->SetMarkerColor(2);
    
    TMultiGraph *mgr1 = new TMultiGraph();
    mgr1->Add(grx, "lp");
    mgr1->Add(grs, "lp");
    mgr1->Draw("a");
    mgr1->SetTitle("Mean ThetaCer; Phi; ThetaCer");
    TLegend *leg1 = new TLegend(0.84,0.15,0.98,0.25);
    leg1->AddEntry(grx, "experiment");
    leg1->AddEntry(grs, "simulation");
    leg1->Draw();
   
    
    c1->cd(4);
    sigx->SetMarkerStyle(20);
    sigx->SetMarkerSize(0.4);
    
    sigs->SetLineColor(2);
    sigs->SetMarkerStyle(20);
    sigs->SetMarkerSize(0.4);
    sigs->SetMarkerColor(2);
    
    TMultiGraph *mgr2 = new TMultiGraph();
    mgr2->Add(sigx, "lp");
    mgr2->Add(sigs, "lp");
    mgr2->Draw("a");
    mgr2->SetTitle("Sigma ThetaCer; Phi; ThetaCer");
    TLegend *leg2 = new TLegend(0.84,0.15,0.98,0.25);
    leg2->AddEntry(sigx, "experiment");
    leg2->AddEntry(sigs, "simulation");
    leg2->Draw();
    
    
    c1->cd(5);
    igx->SetMarkerStyle(20);
    igx->SetMarkerSize(0.4);
    
    igs->SetLineColor(2);
    igs->SetMarkerStyle(20);
    igs->SetMarkerSize(0.4);
    igs->SetMarkerColor(2);
    
    TMultiGraph *mgr3 = new TMultiGraph();
    mgr3->Add(igx, "lp");
    mgr3->Add(igs, "lp");
    mgr3->Draw("a");
    mgr3->SetTitle("Gauss Integral; Phi; Accu");
    TLegend *leg3 = new TLegend(0.84,0.15,0.98,0.25);
    leg3->AddEntry(igx, "experiment");
    leg3->AddEntry(igs, "simulation");
    leg3->Draw();
    
    c1->cd(6);
    pgx->SetMarkerStyle(20);
    pgx->SetMarkerSize(0.4);
    
    pgs->SetLineColor(2);
    pgs->SetMarkerStyle(20);
    pgs->SetMarkerSize(0.4);
    pgs->SetMarkerColor(2);
    
    TMultiGraph *mgr4 = new TMultiGraph();
    mgr4->Add(pgx, "lp");
    mgr4->Add(pgs, "lp");
    mgr4->Draw("a");
    mgr4->SetTitle("Pol1 Integral; Phi; Accu");
    TLegend *leg4 = new TLegend(0.84,0.15,0.98,0.25);
    leg4->AddEntry(pgx, "experiment");
    leg4->AddEntry(pgs, "simulation");
    leg4->Draw();
    



}

