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

struct TrackExp {
    Float_t e;
    Float_t mu;
    Float_t pi;
    Float_t K;
    Float_t p;
    Float_t d;
};
        

void script_fit_spranj_ravnih(){

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
    
    TrackExp expPhotx;
    TrackExp expPhots;
    trx->SetBranchAddress("expPhot",&expPhotx);
    trs->SetBranchAddress("expPhot",&expPhots);
    
    Int_t detPhotx, detPhots;
    
    trx->SetBranchAddress("detPhot",&detPhotx);
    trs->SetBranchAddress("detPhot",&detPhots);
    

    long int nentriesx = 10000;//(Int_t)trx->GetEntries();
    long int nentriess = 10000;//(Int_t)trs->GetEntries();
    
    float spranje[5] = {45.0, 61.5, 78.94, 96.18, 111.5};
    float meje1[22];
    
    for(int i=0; i<22; i++){
        meje1[i] = -3.14159 + i * 0.285599;
        cout << meje1[i] << endl;
    }
    
    TProfile2D *hx[4];
    for(int i=0; i<4; i++){
        char namex[15];
        char titlex[15];
        sprintf(namex, "hx%d", i+1);
        sprintf(titlex, "Experiment %d", i+1);
        hx[i] = new TProfile2D(namex, titlex, 400, -3.142, 3.142, 100, spranje[i], spranje[i+1]);
    }
    
    TProfile2D *hs[4];
    for(int i=0; i<4; i++){
        char names[15];
        char titles[15];
        sprintf(names, "hs%d", i+1);
        sprintf(titles, "Simulation %d", i+1);
        hs[i] = new TProfile2D(names, titles, 400, -3.142, 3.142, 100, spranje[i], spranje[i+1]);
    }

    /*TH1D * px[200];
    TF1 *fx[200];
    TH1D * ps[200];
    TF1 *fs[200];
    */
    
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        if(recHitx.p < 6) continue;
        for(int j=0; j<4; j++){
            if(spranje[j] < sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) && sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) < spranje[j+1]){
                hx[j]->Fill(recHitx.phi, sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y), detPhotx);
                continue;
            }
        }
    }
    
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        if(recHits.p < 6) continue;
        for(int j=0; j<4; j++){
            if(spranje[j] < sqrt(recHits.x*recHits.x + recHits.y*recHits.y) && sqrt(recHits.x*recHits.x + recHits.y*recHits.y) < spranje[j+1]){
                hs[j]->Fill(recHits.phi, sqrt(recHits.x*recHits.x + recHits.y*recHits.y), detPhots);
                continue;
            }
        }
    }
    
    TH1D *px1;
    TF1 *fx1;

    TCutG *cutg = new TCutG("mycut",5);
    cutg->SetVarX("y");
    cutg->SetVarY("x");
    cutg->SetPoint(0,meje1[0] + 0.142799,40);
    cutg->SetPoint(1,meje1[1] + 0.142799,40);
    cutg->SetPoint(2,meje1[1] + 0.142799,120);
    cutg->SetPoint(3,meje1[0] + 0.142799,120);
    cutg->SetPoint(4,meje1[0] + 0.142799,40);
    
    px1 = hx[2]->ProjectionX("px1", 0, 50);
    
    fx1 = new TF1("fnamex","-gaus(0) + pol0(3)", meje1[0] + 0.142799, meje1[1] + 0.142799);
    fx1->SetParameters(105,meje1[1],0.4,150);
    fx1->SetParLimits(0,0,300);
    fx1->SetParLimits(1, meje1[0] + 0.142799, meje1[1] + 0.142799);
    fx1->SetParLimits(2,0,5);
    fx1->SetParLimits(3,0,300);
    
    px1->Fit(fx1, "R");
    
    TCanvas *chx = new TCanvas();
    chx->Divide(2,2);
    
    for(int i; i<4; i++){
        chx->cd(i+1);
        hx[i]->Draw("colz");
    }
    
    TCanvas *chs = new TCanvas();
    chs->Divide(2,2);
    
    for(int i; i<4; i++){
        chs->cd(i+1);
        hs[i]->Draw("colz");
    }
    
    
    TCanvas *cpx = new TCanvas();
        cpx->Divide(2,2);
        px1->Draw("hist");
        
    

    
    
    
}

