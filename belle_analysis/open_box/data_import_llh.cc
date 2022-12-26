#include <iostream>
#include <fstream>
using namespace std;

void data_import_llh()
{
    ofstream of;
    of.open("llh_list_plambdapi2.txt");
    
    TChain *psigmapi = new TChain("h1");
    TChain *rare = new TChain("h1");
    TChain *dataset = new TChain("h1");
    TChain *scf = new TChain("h1");
   

    psigmapi->Add("rareplambdapi_final.root");
    rare->Add("../sample/MC/rare/plambdapi_rare_final.root");
    dataset->Add("plambdapi_data_final.root");
    scf->Add("/Users/zhangchuanyu/Desktop/6.24.02/share/root/macros/code/fitter_test/plambdapi_final_bcs_scf.root");

    using namespace RooFit;
    using namespace RooStats;

    RooRealVar mbc0("mbc0","mbc0",5.23,5.30);
    RooRealVar de("de","de",-0.14,0.2);
    RooRealVar m_pl("m_pl","",2.,6.);
   
    RooArgSet *varset = new RooArgSet(de,mbc0);

    RooDataSet data("psigmapi","psigmapi",psigmapi,*varset);
    RooDataSet data7("rare","rare",rare,*varset);
    RooDataSet data8("data8","data",dataset,*varset);
    RooDataSet data9("data9","",scf,*varset);

   // RooDataSet *data_local = (RooDataSet *)data8.reduce(RooArgSet(de,mbc0),"2.0<m_pl&&m_pl<2.8");
//-------------------- Histogram fullfill for Hist PDF -----------------------------
   
  
    TH2D *hist2 = new TH2D("hist2","",20,-0.14,0.2,100,5.23,5.30);
    psigmapi->Draw("mbc0:de>>hist2");

    TH2D *hist4 = new TH2D("hist4","",40,-0.14,0.2,50,5.23,5.30);
    rare->Draw("mbc0:de>>hist4");

    
    TH1D *de_psigma_hist = new TH1D("de_psigma_hist","",13,-0.14,0.2);
    TH1D *mbc_psigma_hist = new TH1D("mbc_psigma_hist","",100,5.23,5.30);

    psigmapi->Draw("de>>de_psigma_hist");
    psigmapi->Draw("mbc0>>mbc_psigma_hist");

    RooDataHist psigma("psigma","",RooArgList(de,mbc0),hist2);
    RooDataHist rare_hist("rare_hist","",RooArgList(de,mbc0),hist4);
    RooDataHist psigma_de("psigma_de","",RooArgList(de),de_psigma_hist);
    RooDataHist psigma_mbc("psigma_mbc","",RooArgList(mbc0),mbc_psigma_hist);


    std::cout<<"test4"<<std::endl;

//--------------------------  mbc Pdf ----------------------
  

    mbc0.setRange("Signal_box_mbc",5.23,5.30);
    
    //bb
    
    RooRealVar m0("m0","",5.28906e+00);
    RooRealVar k("k","",-2.60718e+01,-30,-2);
    RooArgusBG mbc_bb("mbc_bb","",mbc0,m0,k);

    //psigmapi
    RooHistPdf mbc_psigmapi("mbc_psigmapi","",RooArgList(mbc0),psigma_mbc,1);


    //plambdapi
  
    RooRealVar mbc_mean("mbc_mean","",5.27948e+00);
    RooRealVar mbc_width("mbc_width","",2.57866e-03);
    RooRealVar mbc_width2("mbc_width2","",1.26457e-02);

    RooGaussian mbc_gaus("mbc_gaus","",mbc0,mbc_mean,mbc_width);
    RooGaussian mbc_gaus2("mbc_gaus2","",mbc0,mbc_mean,mbc_width2);

    RooRealVar mbc_frac3("mbc_frac3","",9.95054e-01);

    RooAddPdf mbc_pdf("mbc_pdf","",RooArgList(mbc_gaus,mbc_gaus2),mbc_frac3);
 
//---------------------de Pdf-----------------------------

   
    de.setRange("Signal_box_de",-0.3,0.2);
     

    //bb
    
     RooRealVar a0("a0","a0",-1.99739e+00) ;
     RooRealVar a1("a1","a1",2.79635e+00) ;
  
     RooPolynomial de_bb("de_bb","",de,RooArgSet(a0,a1)) ;
   
     //psigmapi

     RooHistPdf de_psigmapi("de_psigmapi","",RooArgList(de),psigma_de,1);


    //plambdapi

    RooRealVar de_mean("de_mean","de_mean",2.38781e-04);
    RooRealVar de_width1("de_width1","de_width1",9.70057e-03);
    RooRealVar de_width2("de_width2","de_width2",2.10188e-02);
    RooRealVar de_width3("de_width3","de_width3",7.06829e-02);

    RooGaussian de_gaus("de_gaus","",de,de_mean,de_width1);
    RooGaussian de_gaus2("de_gaus2","",de,de_mean,de_width2);
    RooGaussian de_gaus3("de_gaus3","",de,de_mean,de_width3);
    RooRealVar de_frac("de_frac","",8.36888e-01);
    RooRealVar de_frac2("de_frac2","",4.73657e-02);

    RooAddPdf de_pdf("de_pdf","",RooArgList(de_gaus,de_gaus2),de_frac);
    RooAddPdf de_pdf2("de_pdf2","",RooArgList(de_gaus3,de_pdf),de_frac2);
 

    RooProdPdf final_plambdapi("final_plambdapi","",RooArgList(de_pdf2,mbc_pdf));//plambdapi 2d PDF
    RooProdPdf final_bb("final_bb","",RooArgList(de_bb,mbc_bb)); //bb 2d PDF
    RooHistPdf final_rare("final_rare","",RooArgList(de,mbc0),rare_hist,1);
    RooProdPdf final_psigmapi("final_psigmapi","",RooArgList(de_psigmapi,mbc_psigmapi));//psigmapi 2d PDF
    RooNDKeysPdf final_scf("final_scf","",*varset,data9,"am");
 //   RooNDKeysPdf final_rare("final_rare","",RooArgList(de,mbc0),data7,"am",2);
 //   RooHistPdf final_psigmapi("final_psigmapi","",RooArgList(de,mbc0),psigma,1); //psigmapi 2d PDF
    

    RooRealVar frac("frac","",0.0175);

    
    RooRealVar nsig ("nsig","signal events",150,-100,1000);
    RooRealVar nplambda ("nplambda","",2.15447e+02);
    RooRealVar nbkg ("nbkg","",5000,100,10000);
    RooRealVar nrare("nrare","",10,1000);
    RooProduct nscf("nscf","",RooArgList(frac,nplambda));

    RooAddPdf final_pdf("final_pdf","final_pdf",RooArgList(final_psigmapi,final_plambdapi,final_bb,final_scf),RooArgList(nsig,nplambda,nbkg,nscf));


     
    RooFitResult *final_result = new RooFitResult("final_result","final_result");
    final_result = final_pdf.fitTo(data8,Minos(kTRUE),Save(true));

    float likelihood[4000];
    float nsig_bin[4000];
    
    float sig_yield = nplambda.getVal();
    
    RooAbsReal *nll_ext = final_pdf.createNLL(data8);
    
  
    TH1D *scan = new TH1D("scan","",4000,-0.2,400.);
    TH1D *scan_smear = new TH1D("scan_smear","",4000,-0.2,400.);
    TH1D *scan_llh = new TH1D("scan_llh","",4000,-0.2,400.);
    
    for(int bin=1;bin<=scan->GetNbinsX();bin++)
    {
        nplambda.setVal(scan->GetBinCenter(bin));
        nplambda.setConstant(true);
      
        final_pdf.fitTo(data8);
        
        scan->SetBinContent(bin,(nll_ext->getVal()-final_result->minNll())*2.);
        likelihood[bin-1] = TMath::Exp(-nll_ext->getVal()+final_result->minNll());
        nsig_bin[bin-1] = scan->GetBinCenter(bin);
        scan_llh->SetBinContent(bin,TMath::Exp(-nll_ext->getVal()+final_result->minNll()));
   
      
    
        of << TMath::Exp(-nll_ext->getVal()+final_result->minNll())<<" "<< scan->GetBinCenter(bin) <<"\n";
        //of << TMath::Exp(-nll_ext->getVal())<<" "<< scan->GetBinCenter(bin) <<"\n";
        
    
     
        
        
       
        
    }
    of.close();
    
   
}
