
class draw_hist
{
public:
    TH1D *Std_hist1D(const char *file, int color,const char *type, const char *title, const char *variable,float low,float upp,int bin_num);
    TH2D *Std_hist2D(const char *file, const char *variable1, const char variable2, const char *type,float low1,float upp1,int bin_num1,float low2,float upp2,int bin_num2);
};
void style()
{
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(0.0);
    gStyle->SetErrorX(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    
}
TH1D *draw_hist::Std_hist1D(const char *file, int color,const char *type,const char *title, const char *variable,float low,float upp,int bin_num)
{
    style();
    
    char fill[15];
    const char line[2] = "l";
    const char full[2] = "f";
    
    TChain *chn = new TChain("h1");
    chn ->Add(file);
    
    sprintf(fill,"%s%s",variable,">>hist");
    
    TH1D *hist = new TH1D("hist","",bin_num,low,upp);
    chn->Draw(fill);
    
    if(strcmp(type,line)==0)
    {
        hist->SetLineColor(color);
        hist->SetLineWidth(color);
    }
    
    if(strcmp(type,full)==0)
    {
        hist->SetFillColor(color);
        hist->SetLineColor(color);
    }
    
    hist->GetXaxis()->SetTitle(title);
    hist->SetTitleSize(0.05,"x");
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->CenterTitle();
 
    return hist;
    
};

TH2D * draw_hist::Std_hist2D(const char *file, const char *variable1, const char variable2, const char *title1, const char *title2,float low1,float upp1,int bin_num1,float low2,float upp2,int bin_num2)
{
    style();
    
    char fill[15];
    
    TChain *chn = new TChain("h1");
    chn ->Add(file);
    
    TH2D *hist = new TH2D("hist","",bin_num1,low1,upp1,bin_num2,low2,upp2);
    
    sprintf(fill,"%s%s%s",variable1,":",variable2,">>hist");
    chn->Draw(fill);
    
    hist->GetXaxis()->SetTitle(title1);
    hist->GetYaxis()->SetTitle(title2);
    hist->SetTitleSize(0.05,"x");
    hist->SetTitleSize(0.05,"y");
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->CenterTitle();
    
    return hist;
    
}
