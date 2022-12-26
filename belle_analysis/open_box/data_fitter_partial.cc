#include <iostream>
#include <fstream>

using namespace std;
void style()
{
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(0.0);
    gStyle->SetErrorX(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    
}
void data_fitter_partial()
{
       //import data
    
        TChain *psigmapi = new TChain("h1");
        TChain *rare = new TChain("h1");
        TChain *dataset = new TChain("h1");
        TChain *scf = new TChain("h1");
        TChain *plambdapi = new TChain("h1");
        TChain *gen_plambdapi = new TChain("h1");
        TChain *gen_psigmapi = new TChain("h1");

        psigmapi->Add("/Users/zhangchuanyu/Desktop/6.24.02/share/root/macros/code/sample_final/rareplambdapi_final_bcs.root");
        rare->Add("../sample/MC/rare/plambdapi_rare_final.root");
        dataset->Add("plambdapi_data_final_bcs.root");
        scf->Add("/Users/zhangchuanyu/Desktop/6.24.02/share/root/macros/code/fitter_test/plambdapi_final_bcs_scf.root");
        //plambdapi->Add("/Users/zhangchuanyu/Desktop/6.24.02/share/root/macros/code/sample_final/plambdapi_final_bcs.root");
        plambdapi->Add("test_plambdapi_final_bcs.root");
        gen_plambdapi->Add("genlevel_plambdapi2.root");
        gen_psigmapi->Add("gen_rareplambdapi2.root");
    
        style();
    
        using namespace RooFit;
        using namespace RooStats;
    
        RooRealVar mbc0("mbc0","mbc0",5.23,5.30);
        RooRealVar de("de","de",-0.14,0.2);
        RooRealVar m_pl("m_pl","",2.,6.);
        RooRealVar cos_ppi("cos_ppi","",-1,1);
       
        RooArgSet *varset = new RooArgSet(de,mbc0,m_pl);
    
        RooDataSet data("psigmapi","psigmapi",psigmapi,*varset);
        RooDataSet data7("rare","rare",rare,*varset);
        RooDataSet data8("data8","data",dataset,RooArgSet(de,mbc0,m_pl,cos_ppi));
        RooDataSet data9("data9","",scf,*varset);
    
    
        double m_pl_bin[9] = {2.0537,2.2,2.4,2.6,2.8,3.4,4,4.6,5.151};
        double cos_bin[6] ={-1.,-0.6,-0.2,0.2,0.6,1.};
 
        double partial_br_mc[8];
        double partial_br[8];
        double partial_br_hierror[8];
        double partial_br_loerror[8];
    
        double partial_br_mc_ps[8];
        double partial_br_ps[8];
        double partial_br_hierror_ps[8];
        double partial_br_loerror_ps[8];
    
        ofstream ofs;
        //ofs.open("m_pl_data_bin_info.txt");
        ofs.open("cos_ppi_data_bin_info.txt");
    
        for(int i=0;i<5;i++) // fitting part in each bin
    {
            
        char m_pl_selection[40];
        char cos_selection[40];
        char cos_selection_plambdapi[80];
        char m_pl_selection_plambdapi[50];
        char m_lpi_select3[40];
        char m_lpi_select1[40];
        char m_lpi_select2[40];
        
        sprintf(m_pl_selection,"%f%s%f",m_pl_bin[i],"<m_pl&&m_pl<",m_pl_bin[i+1]);
        sprintf(m_pl_selection_plambdapi,"%f%s%f",m_pl_bin[i],"<m_pl&&(hindex==1||hindex==4)&&m_pl<",m_pl_bin[i+1]);
        sprintf(cos_selection,"%f%s%f%s",cos_bin[i],"<cos_ppi&&cos_ppi<",cos_bin[i+1],"&&m_pl<2.8");
        sprintf(cos_selection_plambdapi,"%f%s%f%s",cos_bin[i],"<cos_ppi&&cos_ppi<",cos_bin[i+1],"&&m_pl<2.8&&(hindex==1||hindex==4)");
        //sprintf(m_lpi_select1,"%f%s%f",m_lpi_bin[i],"<m_pl&&m_pl<",m_lpi_bin[i+1]);
        sprintf(m_lpi_select2,"%s%d%s","de_bin_data",i,".pdf");
        sprintf(m_lpi_select3,"%s%d%s","mbc_bin_data",i,".pdf");

        RooDataSet *data_local = (RooDataSet *)data8.reduce(RooArgSet(de,mbc0),cos_selection);
        
 
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
        RooNDKeysPdf final_scf("final_scf","",RooArgList(de,mbc0),data9,"am");

        RooRealVar frac("frac","",0.0175);
 
        
        RooRealVar nsig ("nsig","signal events",150,-100,1000);
        RooRealVar nplambda ("nplambda","",200,-150,5000);
        RooRealVar nbkg ("nbkg","",5000,100,10000);      
        RooRealVar nrare("nrare","",10,1000);
        RooProduct nscf("nscf","",RooArgList(frac,nplambda));
    
        RooAddPdf final_pdf("final_pdf","final_pdf",RooArgList(final_psigmapi,final_plambdapi,final_bb,final_scf),RooArgList(nsig,nplambda,nbkg,nscf));
     
        RooFitResult *final_result = new RooFitResult("final_result","final_result");
        final_result = final_pdf.fitTo(*data_local,Minos(kTRUE),Save(true));
        
       /*
        
        TCanvas *cav1 = new TCanvas("cav1","",500,500);
            
        RooPlot *plot_mbc_projection = mbc0.frame(Bins(20),Name("plot_mbc_projection"),Title("plot_mbc_projection"));
        data_local->plotOn(plot_mbc_projection,Name("mbc_hist"),CutRange("signal_box_mbc"),MarkerColor(kBlack),XErrorSize(0));
             
        final_pdf.plotOn(plot_mbc_projection,Name("mbc_total"),ProjectionRange("signal_box_mbc"),Components("final_pdf"),LineColor(kBlue),LineStyle(kSolid),LineWidth(4));
        final_pdf.plotOn(plot_mbc_projection,Name("mbc_signal"),ProjectionRange("signal_box_mbc"),Components("final_psigmapi"),LineColor(kRed),LineStyle(7),LineWidth(4));
        final_pdf.plotOn(plot_mbc_projection,Name("mbc_plambdapi"),ProjectionRange("signal_box_mbc"),Components("final_plambdapi"),LineColor(kBlack),LineStyle(7),LineWidth(4));
        final_pdf.plotOn(plot_mbc_projection,Name("mbc_bg"),ProjectionRange("signal_box_mbc"),Components("final_bb"),LineColor(kViolet),LineStyle(7),LineWidth(4));
        final_pdf.plotOn(plot_mbc_projection,Name("mbc_scf"),ProjectionRange("signal_box_mbc"),Components("final_scf"),LineColor(kGreen),LineStyle(7),LineWidth(4));
         
        plot_mbc_projection -> GetXaxis()->SetTitle("M_{bc}[GeV/c^{2}]");
        plot_mbc_projection -> GetXaxis()->CenterTitle();
        plot_mbc_projection -> SetTitleSize(0.045,"x");
        plot_mbc_projection ->SetTitleOffset(1.5,"y");
        plot_mbc_projection -> SetTitle("");
        plot_mbc_projection -> Draw("");
       
        cav1->SaveAs(m_lpi_select2);

        TCanvas *cav2 = new TCanvas("cav2","",500,500);
            
        RooPlot *plot_de_projection = de.frame(Bins(20),Name("plot_de_projection"),Title("plot_de_projection"));
        data_local->plotOn(plot_de_projection,Name("de_hist"),CutRange("signal_box_de"),MarkerColor(kBlack),XErrorSize(0));
              
        final_pdf.plotOn(plot_de_projection,Name("de_total"),ProjectionRange("signal_box_de"),Components("final_pdf"),LineColor(kBlue),LineStyle(kSolid),LineWidth(4));
        final_pdf.plotOn(plot_de_projection,Name("de_signal"),ProjectionRange("signal_box_de"),Components("final_psigmapi"),LineColor(kRed),LineStyle(7),LineWidth(4));
        final_pdf.plotOn(plot_de_projection,Name("de_plambdapi"),ProjectionRange("signal_box_de"),Components("final_plambdapi"),LineColor(kBlack),LineStyle(7),LineWidth(4));
        final_pdf.plotOn(plot_de_projection,Name("de_bg"),ProjectionRange("signal_box_de"),Components("final_bb"),LineColor(kViolet),LineStyle(7),LineWidth(4));
        final_pdf.plotOn(plot_de_projection,Name("de_scf"),ProjectionRange("signal_box_de"),Components("final_scf"),LineColor(kGreen),LineStyle(7),LineWidth(4));



        plot_de_projection -> GetXaxis()->SetTitle("#Delta E[GeV]");
        plot_de_projection -> GetXaxis()->CenterTitle();
        plot_de_projection -> SetTitleSize(0.045,"x");
        plot_de_projection ->SetTitleOffset(1.5,"y");
        plot_de_projection -> SetTitle("");
        plot_de_projection -> Draw("");
                                     
        cav2->SaveAs(m_lpi_select3);
        */
        
        //Differential Branching faction calculation
        
        // plambdapi
        
        float total = plambdapi->GetEntries();
        float ratio = plambdapi->GetEntries(cos_selection_plambdapi)/total;
        float ent_mc = plambdapi->GetEntries(cos_selection_plambdapi);
        float ent_gen = gen_plambdapi->GetEntries(cos_selection);
        float eff_bin = ent_mc/ent_gen;
        
        
        partial_br[i] = nplambda.getVal()/(771581000*eff_bin*0.881*(cos_bin[i+1]-cos_bin[i]));
        partial_br_mc[i] = (ratio*3.18)/(cos_bin[i+1]-cos_bin[i]);
        partial_br_hierror[i] = nplambda.getAsymErrorHi()/(771581000*eff_bin*0.881*(cos_bin[i+1]-cos_bin[i]));
        partial_br_loerror[i] = -(nplambda.getAsymErrorLo()/(771581000*eff_bin*0.881*(cos_bin[i+1]-cos_bin[i])));
        
        float br = nplambda.getVal()/(771581000*eff_bin*0.881);
        float br_hierror = nplambda.getAsymErrorHi()/(771581000*eff_bin*0.881);
        float br_loerror = -(nplambda.getAsymErrorLo()/(771581000*eff_bin*0.881));
        
        // psigmapi
        
        float ent_mc_ps = psigmapi->GetEntries(cos_selection_plambdapi);
        float ent_gen_ps = gen_psigmapi->GetEntries(cos_selection)*(771000./743570.);
        float eff_bin_ps = ent_mc_ps/ent_gen_ps;
        
        
        partial_br_ps[i] = nsig.getVal()/(771581000*eff_bin*0.878*(cos_bin[i+1]-cos_bin[i]));
        partial_br_hierror_ps[i] = nsig.getAsymErrorHi()/(771581000*eff_bin_ps*0.878*(cos_bin[i+1]-cos_bin[i]));
        partial_br_loerror_ps[i] = -(nsig.getAsymErrorLo()/(771581000*eff_bin_ps*0.878*(cos_bin[i+1]-cos_bin[i])));
        
        float br_ps = nsig.getVal()/(771581000*eff_bin_ps*0.878);
        float br_hierror_ps = nsig.getAsymErrorHi()/(771581000*eff_bin_ps*0.878);
        float br_loerror_ps = -(nsig.getAsymErrorLo()/(771581000*eff_bin_ps*0.878));
        
        
        
        ofs<<"bin"<<i+1<<":"<<br<<" "<<br_hierror<<" "<<br_loerror<<" "<<eff_bin<<" "<<nplambda.getVal()<<" "<<nplambda.getAsymErrorHi()<<" "<<nplambda.getAsymErrorLo()<<"\n" ;
        
   
    }
   // Drawing part
    
        TCanvas *canvas1 = new TCanvas("canvas1","",600,600);
  
        double m_pl_val[8] ={2.1,2.3,2.5,2.7,3.1,3.7,4.3,4.9};
        double m_pl_error[8] = {0,0,0,0,0,0,0,0};
        double cos_val[5] = {-0.8,-0.4,0,0.4,0.8};
        double cos_val_error[5] = {0.2,0.2,0.2,0.2,0.2};
      
        //TGraphAsymmErrors *m_pl_plot = new TGraphAsymmErrors(8,m_pl_val,partial_br,m_pl_error,m_pl_error,partial_br_loerror,partial_br_hierror);
        TGraphAsymmErrors *m_pl_plot = new TGraphAsymmErrors(5,cos_val,partial_br,cos_val_error,cos_val_error,partial_br_loerror,partial_br_hierror);
   
        m_pl_plot->SetMarkerStyle(20);
        m_pl_plot->GetXaxis()->SetTitle("cos#theta_{p}");
        m_pl_plot->GetYaxis()->SetTitle("dBr/dcos#theta_{p}\ [1/0.25]");
        m_pl_plot->SetTitle("");
        m_pl_plot->GetXaxis()->CenterTitle();
        m_pl_plot->GetXaxis()->SetTitleSize(0.045);
    
        m_pl_plot->Draw("AP");
     
  
    /*
    TH1D *m_pl_plambda_fitting_hist = new TH1D("m_pl_plambda_fitting","",8,m_pl_bin);
    TH1D *m_pl_plambda_expect_hist = new TH1D("m_pl_plambda_expect","",8,m_pl_bin);

    for(int i = 0;i<8;i++)
    {
       
        m_pl_plambda_expect_hist->SetBinContent(i+1,partial_br_mc[i]*1e-6);
        m_pl_plambda_fitting_hist->SetBinContent(i+1,partial_br[i]);
        m_pl_plambda_fitting_hist->SetBinError(i+1,partial_br_hierror[i]);
        
        printf("%f\n",partial_br[i]);
    }
    
    m_pl_plambda_expect_hist->SetFillColor(30);
    m_pl_plambda_expect_hist->SetLineColor(30);
    
    m_pl_plambda_expect_hist->GetXaxis()->SetTitle("M_{p#bar{#Lambda}}\ [GeV/c^{2}]");
    m_pl_plambda_expect_hist->GetYaxis()->SetTitle("dBr/dM_{p#bar{#Lambda}}\ [1/(GeV/c^{2})]");
    m_pl_plambda_expect_hist->GetXaxis()->CenterTitle();
    m_pl_plambda_expect_hist->GetXaxis()->SetTitleSize(0.045);
    m_pl_plambda_expect_hist->GetYaxis()->SetTitleSize(0.045);
    
    
    m_pl_plambda_fitting_hist->SetMarkerColor(1);
    m_pl_plambda_fitting_hist->SetMarkerSize(1.5);
    m_pl_plambda_fitting_hist->SetMarkerStyle(20);
    
    m_pl_plambda_expect_hist->Draw();
    m_pl_plambda_fitting_hist->Draw("same");
    
    TLegend *legend_plot2 = new TLegend(0.1,0.7,0.48,0.9);
            
    legend_plot2 -> SetBorderSize(0);
    legend_plot2 -> SetFillStyle(0);

    legend_plot2 -> AddEntry(m_pl_plambda_expect_hist,"MC with TE model","f");
    legend_plot2 -> AddEntry(m_pl_plambda_fitting_hist,"Data","lep");

    legend_plot2 -> Draw();
    
    
    */
     
}

