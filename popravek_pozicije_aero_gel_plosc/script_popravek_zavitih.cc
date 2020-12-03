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
        

void script_fit_spranj_zavitih(){

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
    

    long int nentriesx = (Int_t)trx->GetEntries();
    long int nentriess = (Int_t)trs->GetEntries();
    
    
    TProfile2D *hx[4];
    for(int i=0; i<4; i++){
        char namex[15];
        char titlex[15];
        sprintf(namex, "hx%d", i+1);
        sprintf(titlex, "Experiment %d", i+1);
        hx[i] = new TProfile2D(namex, titlex, 400, -3.142, 3.142, 400, 40, 120);
    }

    
    TH1D * px[200];
    TF1 *fx[200];
    
    for(int i=0; i<nentriesx; i++){
        trx->GetEntry(i);
        if(recHitx.p < 6) continue;
        if(54 < sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) && sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) < 66){
            hx[0]->Fill(recHitx.phi, sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y), detPhotx);
        }
        else if(74 < sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) && sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) < 86){
            hx[1]->Fill(recHitx.phi, sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y), detPhotx);
        }
        else if(90 < sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) && sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) < 102){
            hx[2]->Fill(recHitx.phi, sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y), detPhotx);
        }
        else if(105 < sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) && sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y) < 117){
            hx[3]->Fill(recHitx.phi, sqrt(recHitx.x*recHitx.x + recHitx.y*recHitx.y), detPhotx);
        }
    }
    
    
    float spranje[4] = {61.5, 78.7, 96.17, 111.5};
    
    TCanvas *cx[4];
    int l=0;
    for(int j=0; j<3; j++){
        char cnamex[10];
        sprintf(cnamex, "cx%d", j);
        cx[j] = new TCanvas(cnamex);
        cx[j]->Divide(4,4);
        cout << cnamex << endl;
        
        int k=1;
        for(int i=0; i<50; i++){
            cx[j]->cd(i+1);
            
            char pnamex[15];
            sprintf(pnamex, "px%d%d", j+1, i+1);
            char fnamex[15];
            sprintf(fnamex, "fx%d%d", j+1, i+1);
            fx[i + l] = new TF1("fnamex","-gaus(0) + pol0(3)", spranje[j] - 1.7, spranje[j] + 1.7);
            fx[i + l]->SetParameters(105,spranje[j],0.4,150);
            fx[i + l]->SetParLimits(0,0,300);
            fx[i + l]->SetParLimits(1, spranje[j] - 3, spranje[j] + 3);
            fx[i + l]->SetParLimits(2,0,5);
            fx[i + l]->SetParLimits(3,0,300);
            
            px[i + l] = hx[j]->ProjectionY(pnamex, k, k + 7);
            px[i + l]->Fit(fx[i + l], "R");
            
            px[i + l]->GetXaxis()->SetRangeUser(spranje[j] - 7, spranje[j] + 7);
            px[i + l]->SetMinimum(0);
            px[i + l]->Draw("hist");
            fx[i + l]->Draw("same");
            char naslov_grafax[15];
            sprintf(naslov_grafax, "Experiment %d %d;;", j+1, i+1);
            px[i + l]->SetTitle(naslov_grafax);
            
            
            k = k + 8;
        }
        l = l + 50;
    }
    
    
    TProfile2D *hs[4];
    for(int i=0; i<4; i++){
        char names[15];
        char titles[15];
        sprintf(names, "hx%d", i+1);
        sprintf(titles, "Experiment %d", i+1);
        hs[i] = new TProfile2D(names, titles, 400, -3.142, 3.142, 400, 40, 120);
    }

    
    TH1D * ps[200];
    TF1 *fs[200];
    
    for(int i=0; i<nentriess; i++){
        trs->GetEntry(i);
        if(recHits.p < 6) continue;
        if(54 < sqrt(recHits.x*recHits.x + recHits.y*recHits.y) && sqrt(recHits.x*recHits.x + recHits.y*recHits.y) < 66){
            hs[0]->Fill(recHits.phi, sqrt(recHits.x*recHits.x + recHits.y*recHits.y), detPhots);
        }
        else if(74 < sqrt(recHits.x*recHits.x + recHits.y*recHits.y) && sqrt(recHits.x*recHits.x + recHits.y*recHits.y) < 86){
            hs[1]->Fill(recHits.phi, sqrt(recHits.x*recHits.x + recHits.y*recHits.y), detPhots);
        }
        else if(90 < sqrt(recHits.x*recHits.x + recHits.y*recHits.y) && sqrt(recHits.x*recHits.x + recHits.y*recHits.y) < 102){
            hs[2]->Fill(recHits.phi, sqrt(recHits.x*recHits.x + recHits.y*recHits.y), detPhots);
        }
        else if(105 < sqrt(recHits.x*recHits.x + recHits.y*recHits.y) && sqrt(recHits.x*recHits.x + recHits.y*recHits.y) < 117){
            hs[3]->Fill(recHits.phi, sqrt(recHits.x*recHits.x + recHits.y*recHits.y), detPhots);
        }
    }
    
    

    
    TCanvas *cs[4];
    l=0;
    for(int j=0; j<3; j++){
        char cnames[10];
        sprintf(cnames, "cs%d", j);
        cs[j] = new TCanvas(cnames);
        cs[j]->Divide(4,4);
        cout << cnames << endl;
        
        int k=1;
        for(int i=0; i<50; i++){
            cs[j]->cd(i+1);
            
            char pnames[15];
            sprintf(pnames, "ps%d%d", j+1, i+1);
            char fnames[15];
            sprintf(fnames, "fs%d%d", j+1, i+1);
            fs[i + l] = new TF1("fnames","-gaus(0) + pol0(3)", spranje[j] - 1.7, spranje[j] + 1.7);
            fs[i + l]->SetParameters(105,spranje[j],0.4,150);
            fs[i + l]->SetParLimits(0,0,300);
            fs[i + l]->SetParLimits(1, spranje[j] - 3, spranje[j] + 3);
            fs[i + l]->SetParLimits(2,0,5);
            fs[i + l]->SetParLimits(3,0,300);
            
            ps[i + l] = hs[j]->ProjectionY(pnames, k, k + 7);
            ps[i + l]->Fit(fs[i + l], "R");
            
            ps[i + l]->GetXaxis()->SetRangeUser(spranje[j] - 7, spranje[j] + 7);
            ps[i + l]->SetMinimum(0);
            ps[i + l]->Draw("hist");
            fs[i + l]->Draw("same");
            char naslov_grafas[15];
            sprintf(naslov_grafas, "Simulation %d %d;;", j+1, i+1);
            ps[i + l]->SetTitle(naslov_grafas);
            
            
            k = k + 8;
        }
        l = l + 50;
    }
    
    
    TGraph *gax = new TGraph();
    TGraph *gmx = new TGraph();
    TGraph *gsx = new TGraph();
    TGraph *gas = new TGraph();
    TGraph *gms = new TGraph();
    TGraph *gss = new TGraph();
    
    ofstream myfile;
    myfile.open ("test###parametri.txt");
    myfile << "amplitude_exp amplitude_sim mean_exp mean_sim sigma_exp sigma_sim\n";

    
    for(int i=0; i<150; i++){
        gax->SetPoint(i, i, fx[i]->GetParameter(0));
        gmx->SetPoint(i, i, fx[i]->GetParameter(1));
        gsx->SetPoint(i, i, fx[i]->GetParameter(2));
        gas->SetPoint(i, i, fs[i]->GetParameter(0));
        gms->SetPoint(i, i, fs[i]->GetParameter(1));
        gss->SetPoint(i, i, fs[i]->GetParameter(2));
        myfile << fx[i]->GetParameter(0) << " " << fs[i]->GetParameter(0) << " " << fx[i]->GetParameter(1) << " " << fs[i]->GetParameter(1) << " " << fx[i]->GetParameter(2) << " " << fs[i]->GetParameter(2) <<"\n";
    }
    myfile.close();
    
    
    TCanvas *cpar = new TCanvas();
    cpar->Divide(2,3);
    
    cpar->cd(1);
    gax->Draw("ap");
    gax->SetMarkerStyle(22);
    gax->SetMarkerSize(0.8);
    gax->SetTitle("Amplitude Experiment");
    
    cpar->cd(2);
    gas->Draw("ap");
    gas->SetMarkerStyle(22);
    gas->SetMarkerSize(0.8);
    gas->SetTitle("Amplitude Simulation");
    
    cpar->cd(3);
    gmx->Draw("ap");
    gmx->SetMarkerStyle(22);
    gmx->SetMarkerSize(0.8);
    gmx->SetTitle("Mean Experiment");
    
    cpar->cd(4);
    gms->Draw("ap");
    gms->SetMarkerStyle(22);
    gms->SetMarkerSize(0.8);
    gms->SetTitle("Mean Simulation");
    
    cpar->cd(5);
    gsx->Draw("ap");
    gsx->SetMarkerStyle(22);
    gsx->SetMarkerSize(0.8);
    gsx->SetTitle("Sigma Experiment");
    
    cpar->cd(6);
    gss->Draw("ap");
    gss->SetMarkerStyle(22);
    gss->SetMarkerSize(0.8);
    gss->SetTitle("Sigma Simulation");
    

    
    
    
}
