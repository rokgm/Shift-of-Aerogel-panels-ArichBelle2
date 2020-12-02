void script_razlika_spranj(){
    
    ifstream myfile("parametri.txt");
    string str;
    
    int l=0;
    int j=0;
    int m=0;
    int n=0;
    float amp_exp;
    float amp_sim;
    float mean_exp;
    float mean_sim;
    float sigma_exp;
    float sigma_sim;
    
    TGraph *gra = new TGraph();
    TGraph *grm = new TGraph();
    TGraph *grs = new TGraph();
    
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
            if(k==0 || k==1){
                if(k==0){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    amp_exp = stof(token);
                }
                if(k==1){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    amp_sim = stof(token);
                gra->SetPoint(j, j, amp_exp - amp_sim);
                }
                j++;
            }
            if(k==2 || k==3){
                if(k==2){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_exp = stof(token);
                }
                if(k==3){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    mean_sim = stof(token);
                grm->SetPoint(m, m, mean_exp - mean_sim);
                }
                m++;
            }
            if(k==4 || k==5){
                if(k==4){
                    token = str.substr(0, pos);
                    cout << token << endl;
                    sigma_exp = stof(token);
                }
                n++;
            }           
            
            str.erase(0, pos + delimiter.length());
            k++;
        }
        cout << str << "###" << endl;
        sigma_sim = stof(str);
        grs->SetPoint(n, n, sigma_exp - sigma_sim);
        n++;        
    }
    TCanvas *c1 = new TCanvas();
    c1->Divide(2,2);
    
    c1->cd(1);
    gra->Draw("ap");
    gra->SetMarkerStyle(22);
    gra->SetMarkerSize(0.6);
    gra->SetTitle("Amplitude exp - sim");
    
    c1->cd(2);
    grm->Draw("ap");
    grm->SetMarkerStyle(22);
    grm->SetMarkerSize(0.6);
    grm->SetTitle("Mean exp - sim");
    
    c1->cd(3);
    grs->Draw("ap");
    grs->SetMarkerStyle(22);
    grs->SetMarkerSize(0.6);
    grs->SetTitle("Sigma exp - sim");
    
}
