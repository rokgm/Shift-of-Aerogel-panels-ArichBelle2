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
        
    
void script_primerjavaphi(){
    
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
    

    Int_t nentriesx = (Int_t)10000;//trx->GetEntries();
    Int_t nentriess = (Int_t)10000;//trs->GetEntries();
    
    TCanvas * c1 = new TCanvas("c1");
    c1->Divide(4,4);
    TCanvas * c2 = new TCanvas("c2");
    c2->Divide(3,3);

    
    int k=1;
    int l=2;
    float whc[] = {57.5, 65.9, 74.4, 82.7, 91.3, 99.6, 108.4};
    TH2F *hx[7];
    TH1D *px[7];
    TH2F *hs[7];
    TH1D *ps[7];
    TH2F *dhx[7];
    TH2F *dhs[7];
    int ntrkx[7] = {0};
    int ntrks[7] = {0};
    
    for(int i=0; i < 7; i++){
            char namex[15];
            char dnamex[15];
            char titlex[15];
            sprintf(namex, "hx%d", i);
            sprintf(dnamex, "dhx%d", i);
            sprintf(titlex, "Experiment %d", i);
            hx[i] = new TH2F(namex, titlex, 400,-0.6,0.6,400,-0.6,0.6);
            dhx[i] = new TH2F(dnamex, titlex, 400,-0.6,0.6,400,-0.6,0.6);
    } 
    
    for(int j=0; j<nentriesx; j++){
        trx->GetEntry(j);   
        for(int i=0; i < 7; i++){
            if(recHitx.p < 6) continue;
                
                if(whc[i] - 2 < sqrt(winHitx.x * winHitx.x + winHitx.y * winHitx.y) && whc[i] + 2 > sqrt(winHitx.x * winHitx.x + winHitx.y * winHitx.y)){
                    ntrkx[i]++;
                    for(auto &photon : *photonsx){
                        if(photon.getMirror() != 0) continue;
                        hx[i]->Fill((photon.getThetaCer()*TMath::Cos(photon.getPhiCerTrk())), (photon.getThetaCer()*TMath::Sin(photon.getPhiCerTrk())));
                    }
                }
        }        
    }
    TLegend* leg[7];
    for(int i=0; i < 7; i++){
        char pnamex[15];
        sprintf(pnamex, "px%d", i);
        hx[i]->Scale(1.0/ntrkx[i]);
        px[i] = hx[i]->ProjectionX(pnamex, 190, 212);
        
        c1->cd(k);
        hx[i]->Draw("colz");
        gPad->SetLogz();
        hx[i]->SetStats(kFALSE);
        
        c2->cd(i + 1);
        leg[i] = new TLegend(0.78,0.65,0.98,0.75);
        leg[i]->AddEntry(px[i], "experiment");
        leg[i]->Draw();
        px[i]->Draw("hist");
        
        k = k + 2;
    }
        
        
        
    for(int i=0; i < 7; i++){
            char names[15];
            char dnames[15];
            char titles[15];
            sprintf(names, "hs%d", i);
            sprintf(dnames, "dhs%d", i);
            sprintf(titles, "Simulation %d", i);
            hs[i] = new TH2F(dnames, titles, 400,-0.6,0.6,400,-0.6,0.6);
    }
            
    for(int j=0; j<nentriess; j++){
        trs->GetEntry(j);   
        for(int i=0; i < 7; i++){
            if(recHits.p < 6) continue;
            
                if(whc[i] - 2 < sqrt(winHits.x * winHits.x + winHits.y * winHits.y) && whc[i] + 2 > sqrt(winHits.x * winHits.x + winHits.y * winHits.y)){
                    ntrks[i]++;
                    for(auto &photon : *photonss){
                        if(photon.getMirror() != 0) continue;
                        hs[i]->Fill((photon.getThetaCer()*TMath::Cos(photon.getPhiCerTrk())), (photon.getThetaCer()*TMath::Sin(photon.getPhiCerTrk())));
                    }
                }
        }        
    }
    
    
    for(int i=0; i < 7; i++){
        char pnames[15];
        sprintf(pnames, "ps%d", i);
        hs[i]->Scale(1.0/ntrks[i]);
        ps[i] = hs[i]->ProjectionX(pnames, 190, 212);
        
        c1->cd(l);
        hs[i]->Draw("colz");
        gPad->SetLogz();
        hs[i]->SetStats(kFALSE);
        
        c2->cd(i + 1);
        
        ps[i]->SetLineColor(2);
        leg[i]->AddEntry(ps[i], "simulation");
        leg[i]->Draw();
        ps[i]->Draw("hist same");
        
        l = l + 2;
    }


}
