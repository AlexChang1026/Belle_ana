#include <iostream>
#include <fstream>
using namespace std;

double smear_sys(double nsig_input, double llh_list[], double nsig_list[])
{
    double sum_llh = 0.;
    
    for(int i=0;i<6000;i++)
    {
        double llh_smear = 0.;
        double delta_n = 2.95;
        double n_interval = 0.1;
        double factor = 1/(sqrt(2.0*3.14)*delta_n);
        
        llh_smear = n_interval*factor*llh_list[i]*TMath::Exp(-0.5*TMath::Power(((nsig_input-nsig_list[i])/delta_n),2));
        //cout<<"llh_smear:"<<llh_smear<<" "<<"llh:"<<llh_list[i]<<" "<<"nsig:"<<""<<nsig_list[i]<<" "<<TMath::Exp(-0.5*TMath::Power(((nsig_input-nsig_list[i])/delta_n),2))<<endl;
        
        sum_llh = sum_llh + llh_smear;
        

    }
    
    return sum_llh;
    
}

void style()
{
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(0.0);
    gStyle->SetErrorX(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
   
    
}


void data_smear()
{
    style();
    
    ifstream of;
    
    of.open("llh_list_plambdapi0.txt");
    
    double llh_nsig[12000];
    
    for(int i =0;i<12000;i++)
    {
      of>>llh_nsig[i] ;
    }
    
    of.close();
    
    double llh_log[6000];
    double llh[6000];
    double llh_smear[6000];
    double nsig[6000];

    for(int i = 0;i<6000;i++)
    {

        llh[i] = llh_nsig[2*i];
        nsig[i] = llh_nsig[2*i+1];
       
    }
    
    double llh_max_smear = smear_sys(215.45,llh,nsig);
    
    for(int i=0;i<6000;i++)
    {
        
        
        llh_log[i] = -2*TMath::Log(llh[i]);
        llh_smear[i] = -2*TMath::Log(smear_sys(nsig[i],llh,nsig)/llh_max_smear);

        
       if(i==0)
       {
           cout<<"Significance: "<<sqrt(llh_log[i])<<" "<<"Significance with sys error:"<<sqrt(llh_smear[i])<<endl;
       }
    }
    
    
    TCanvas *c1 = new TCanvas("c1","",500,500);
    c1->SetGrid();
    
    TGraph *llh_scan = new TGraph(6000,nsig,llh_log);
    TGraph *llh_smear_scan = new TGraph(6000,nsig,llh_smear);
   
    
    
    llh_smear_scan->SetLineColor(kBlue);
    llh_smear_scan->SetLineWidth(4);
    llh_scan->SetLineColor(kRed);
    llh_scan->SetLineWidth(4);
    llh_scan->GetXaxis()->SetTitle("N_{sig}");
    llh_scan->GetYaxis()->SetTitle("-2log(L/L_{max})");
    llh_scan->SetTitle("");
    
    
    llh_scan->Draw();
    llh_smear_scan->Draw("same");
    
    TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
     
    legend -> SetBorderSize(0);
    legend -> SetFillStyle(0);
    
    legend->AddEntry(llh_scan,"Stats Error");
    legend->AddEntry(llh_smear_scan,"Stats + Sys Error");
    legend->Draw();
}
