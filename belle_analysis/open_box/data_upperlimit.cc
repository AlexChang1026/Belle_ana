#include<iostream>
#include<fstream>
using namespace std;

float smear_sys(double nsig_input, double llh_list[], double nsig_list[])
{
    double sum_llh = 0.;
    
    for(int i=0;i<4000;i++)
    {
        double llh_smear=0.;
        
        double factor = 1./(sqrt(2.0*3.14)*3.92);
        
        llh_smear = factor*llh_list[i]*TMath::Exp(-0.5*pow((nsig_input-nsig_list[i])/3.92,2));
        
        sum_llh = sum_llh + llh_smear;
        

    }
    
    return sum_llh;
    
}


void integral(double llh_list[], double nsig_list[] )
{
    double sum_llh_total = 0.;
    
    for(int i=0;i<4000;i++)
    {
        if(nsig_list[i]>0)
    {
        sum_llh_total = sum_llh_total+llh_list[i];
    }
    
    }
    double sum_llh = 0.;
    
    for(int i=0;i<4000;i++)
    {
        if(nsig_list[i]>0)
    {
        sum_llh = sum_llh + llh_list[i];
        
       printf("%f %f \n",sum_llh/sum_llh_total,nsig_list[i]);
    }
        
    }
    
}


void style()
{
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(0.0);
    gStyle->SetErrorX(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
   
    
}


void data_upperlimit()
{
    
    ifstream of;
    
    of.open("llh_list3.txt");
    
    double llh_nsig[8000];
    
    for(int i =0;i<8000;i++)
    {
      of>>llh_nsig[i] ;
    }
    
    of.close();
    
    double llh[4000];
    double nsig[4000];
    double llh_smear[4000];
    double yield = 50.29;
    
    for(int i=0;i<4000;i++)
    {
        llh[i] = llh_nsig[2*i];
        
        nsig[i] = llh_nsig[2*i+1];
        
        
    }
    for(int i=0;i<4000;i++)
    {
        llh_smear[i] = smear_sys(nsig[i],llh,nsig)/smear_sys(yield,llh,nsig);
        
    }
    
    integral(llh,nsig);
  
    TCanvas *c4 = new TCanvas("c4","",500,500);
    
    TGraph *llh_scan = new TGraph(4000,nsig,llh);
    TGraph *llh_smear_scan = new TGraph(4000,nsig,llh_smear);
    
    llh_scan->SetLineColor(kBlue);
    llh_scan->SetLineWidth(4);
    llh_smear_scan->SetLineColor(kRed);
    llh_smear_scan->SetLineWidth(4);
    llh_scan->GetXaxis()->SetTitle("N_{sig}");
    llh_scan->GetYaxis()->SetTitle("L/L_{max}");
    llh_scan->GetXaxis()->CenterTitle();
    llh_scan->SetTitle("");
    
    llh_scan->Draw();
    llh_smear_scan->Draw("same");

    TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
     
    legend -> SetBorderSize(0);
    legend -> SetFillStyle(0);
    
    legend->AddEntry(llh_scan,"Likelihood fcn");
    legend->AddEntry(llh_smear_scan,"Smeared likelihood fcn");
    legend->Draw();
    

}
