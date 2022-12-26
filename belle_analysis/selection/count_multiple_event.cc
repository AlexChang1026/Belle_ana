void count_multiple_event()
{
    TFile *fin = new TFile("plambdapi_data_final_m_pl<2.8.root");
    TTree *B =(TTree *)fin->Get("h1");
    
    TCanvas *c1 = new TCanvas("c1");
    TH1D *count = new TH1D("count","",21,0,10);
    
    float evtcount ;
    int count1 ;
    int n;
    
    B->SetBranchAddress("evtcount",&evtcount);
    
    n = B->GetEntries();
    
    int *evt;
    evt = new int[n];
    
    for(int i = 0;i<B->GetEntries();i++)
    {
        B ->GetEntry(i);
     
        evt[i] = evtcount ;
        
    }
    
    for(int i = 0;i<B->GetEntries();i++)
    {
        B->GetEntry(i);
        
        if(i==0)
        {
            count1 = 1;
        }
        
        if(evt[i]==evt[i+1])
        {
            count1 = count1 + 1.;
            
        }
        else
        {
         count ->Fill(count1);
           
         count1 = 1;
       
        }
    }
    
    count ->SetFillColor(kBlue);
    count ->SetTitle("Candidate Count");
    count ->GetXaxis()->SetTitle("candidate/event");
   
    c1->SetGrid();
    count->Draw("bar1");
    
}

