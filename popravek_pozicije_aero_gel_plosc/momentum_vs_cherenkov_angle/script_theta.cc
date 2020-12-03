struct TrackHit {
    Int_t PDG;
    Float_t x;
    Float_t y;
    Float_t z;
    Float_t p;
    Float_t theta;
    Float_t phi;
    };
        
    
void theta(){
    TChain *tr = new TChain("mu");
    tr->Add("data/mumu_mc/*.root");
    
    std::vector<Belle2::ARICHPhoton>* photons = 0;
    tr->SetBranchAddress("arich_photons",&photons);
     
    TrackHit recHit;    
    tr->SetBranchAddress("arich_recHit",&recHit);
     
    TH2F *thetap = new TH2F("thetap","p vs thetaCer", 50,0,3, 50,0,0.6);
    
    Int_t nentries = (Int_t)tr->GetEntries();
    for (Int_t i=0; i<nentries; i++) {
      tr->GetEntry(i);
      for(auto &photon : *photons){
        thetap->Fill(recHit.p, photon.getThetaCer());
      }
    }
    
    TH1D *py = 0;
    TGraphErrors *gr = new TGraphErrors();
    
    for(int j=7; j<51; j++){
    
        py = thetap->ProjectionY("py", j, j + 1);
        
        TF1 *f1 = new TF1("f1","gaus(0) + pol1(3)", 0.15, 0.4);
        f1->SetParameters(2000,0.3,0.015,20,30);
        py->Fit("f1","R");
        
        gr->SetPoint(j-7, (j + 0.5) / 50 * 3, f1->GetParameter(1));
        gr->SetPointError(j-7, 0, TMath::Abs(f1->GetParError(1)));
    }
    
    TCanvas * c1 = new TCanvas("c1");
    c1->Divide(2,2);
     
    c1->cd(1);
    py->Draw();
    
    c1->cd(2);
    thetap->SetYTitle("Theta Cherenkov");
    thetap->SetXTitle("Magnitude");    
    thetap->Draw("colz");
    
    c1->cd(3);
    gr->SetMarkerStyle(22);
    gr->SetMarkerSize(0.8);
    gr->Draw("ap");
    
    TF1 *f2 = new TF1("f2", "TMath::ACos(1 / (1.055 * (TMath::Sqrt(x**2 / ((0.1057**2) + x**2)))))", 0.4, 3.5);
    //c1->cd(4);
    f2->Draw("same");
    
    
    
    
    
    
}
