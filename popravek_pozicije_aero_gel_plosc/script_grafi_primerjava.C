void script_grafi_primerjava(){
    
    ifstream myfile("parametri.txt");
    string str;
    
    int l=0;
    int j=0;
    int m=0;
    int m1=0;
    int m2=0;
    int n=0;
    int o=0;
    int p=0;
    int p1=0;
    int p2=0;
    int r=0;
    float amp_exp;
    float amp_sim;
    float mean_exp;
    float mean_sim;
    float sigma_exp;
    float sigma_sim;
    
    TGraph *grax = new TGraph();
    TGraph *gras = new TGraph();
    
    TGraph *grmx1 = new TGraph();
    TGraph *grms1 = new TGraph();
    TGraph *grmx2 = new TGraph();
    TGraph *grms2 = new TGraph();
    TGraph *grmx3 = new TGraph();
    TGraph *grms3 = new TGraph();
    
    TGraph *grsx = new TGraph();
    TGraph *grss = new TGraph();
    
    
    
    while (getline(myfile, str)){
        if(l==0){
            l++;
            continue;
        }
        
        string delimiter = " ";
        
        size_t pos = 0;
        string token;
        
        int k=0;
        while ((pos = str.find(delimiter)) != string::npos){
            if(k==0){
                token = str.substr(0, pos);
                cout << token << endl;
                amp_exp = stof(token);
                grax->SetPoint(o, o, amp_exp);
                o++;
            }
            else if(k==1){
                token = str.substr(0, pos);
                cout << token << endl;
                amp_sim = stof(token);
                gras->SetPoint(j, j, amp_sim);
                j++;
            }
            
            if(r<50){
                if(k==2){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_exp = stof(token);
                    grmx1->SetPoint(p, p, mean_exp);
                    p++;
                }
                else if(k==3){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_sim = stof(token);
                    grms1->SetPoint(m, m, mean_sim);
                    m++;
                }
            }
            
            if(r<100 && r>50){
                if(k==2){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_exp = stof(token);
                    grmx2->SetPoint(p1, p1, mean_exp);
                    p1++;
                }
                else if(k==3){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_sim = stof(token);
                    grms2->SetPoint(m1, m1, mean_sim);
                    m1++;
                }
            }
            
            if(r<150 && r>100){
                if(k==2){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_exp = stof(token);
                    grmx3->SetPoint(p2, p2, mean_exp);
                    p2++;
                }
                else if(k==3){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_sim = stof(token);
                    grms3->SetPoint(m2, m2, mean_sim);
                    m2++;
                }
            }

            else if(k==4){
                token = str.substr(0, pos);
                cout << token << endl;
                sigma_exp = stof(token);
                grsx->SetPoint(n, n, sigma_exp);
                n++;
            }
                    

            str.erase(0, pos + delimiter.length());
            k++;
        }
            cout << str << "###" << r << endl;
            sigma_sim = stof(str);
            grss->SetPoint(r, r, sigma_sim);
            r++;
    }
    TCanvas *c1 = new TCanvas();
    c1->Divide(2,3);
    
    c1->cd(1);
    grax->SetMarkerStyle(20);
    grax->SetMarkerSize(0.4);
    
    gras->SetLineColor(2);
    gras->SetMarkerStyle(20);
    gras->SetMarkerSize(0.4);
    gras->SetMarkerColor(2);
    
    TMultiGraph *mgr1 = new TMultiGraph();
    mgr1->Add(grax, "lp");
    mgr1->Add(gras, "lp");
    mgr1->Draw("a");
    mgr1->SetTitle("Amplitude;;");
    TLegend *leg1 = new TLegend(0.86,0.75,0.99,0.85);
    leg1->AddEntry(grax, "experiment");
    leg1->AddEntry(gras, "simulation");
    leg1->Draw();
    
    
    c1->cd(3);
    grmx1->SetMarkerStyle(20);
    grmx1->SetMarkerSize(0.4);
    
    grms1->SetLineColor(2);
    grms1->SetMarkerStyle(20);
    grms1->SetMarkerSize(0.4);
    grms1->SetMarkerColor(2);
    
    TMultiGraph *mgr2 = new TMultiGraph();
    mgr2->Add(grmx1, "lp");
    mgr2->Add(grms1, "lp");
    mgr2->Draw("a");
    mgr2->SetTitle("Mean 0-50;;");
    TLegend *leg2 = new TLegend(0.86,0.75,0.99,0.85);
    leg2->AddEntry(grmx1, "experiment");
    leg2->AddEntry(grms1, "simulation");
    leg2->Draw();
    
    c1->cd(4);
    grmx2->SetMarkerStyle(20);
    grmx2->SetMarkerSize(0.4);
    
    grms2->SetLineColor(2);
    grms2->SetMarkerStyle(20);
    grms2->SetMarkerSize(0.4);
    grms2->SetMarkerColor(2);
    
    TMultiGraph *mgr4 = new TMultiGraph();
    mgr4->Add(grmx2, "lp");
    mgr4->Add(grms2, "lp");
    mgr4->Draw("a");
    mgr4->SetTitle("Mean 50-100;;");
    TLegend *leg4 = new TLegend(0.86,0.75,0.99,0.85);
    leg4->AddEntry(grmx2, "experiment");
    leg4->AddEntry(grms2, "simulation");
    leg4->Draw();
    
    c1->cd(5);
    grmx3->SetMarkerStyle(20);
    grmx3->SetMarkerSize(0.4);
    
    grms3->SetLineColor(2);
    grms3->SetMarkerStyle(20);
    grms3->SetMarkerSize(0.4);
    grms3->SetMarkerColor(2);
    
    TMultiGraph *mgr5 = new TMultiGraph();
    mgr5->Add(grmx3, "lp");
    mgr5->Add(grms3, "lp");
    mgr5->Draw("a");
    mgr5->SetTitle("Mean 100-150;;");
    TLegend *leg5 = new TLegend(0.86,0.75,0.99,0.85);
    leg5->AddEntry(grmx3, "experiment");
    leg5->AddEntry(grms3, "simulation");
    leg5->Draw();
    
    c1->cd(2);
    grsx->SetMarkerStyle(20);
    grsx->SetMarkerSize(0.4);
    
    grss->SetLineColor(2);
    grss->SetMarkerStyle(20);
    grss->SetMarkerSize(0.4);
    grss->SetMarkerColor(2);
    
    TMultiGraph *mgr3 = new TMultiGraph();
    mgr3->Add(grsx, "lp");
    mgr3->Add(grss, "lp");
    mgr3->Draw("a");
    mgr3->SetTitle("Sigma;;");
    TLegend *leg3 = new TLegend(0.86,0.75,0.99,0.85);
    leg3->AddEntry(grsx, "experiment");
    leg3->AddEntry(grss, "simulation");
    leg3->Draw();
    
    
}
