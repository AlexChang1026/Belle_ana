//Std_hist1D(const char *file, int color,const char *type, const char *title, const char *variable,float low,float upp,int bin_num)
//https://root.cern.ch/doc/master/classTColor.html
#include "draw_hist.h"

void draw_test()
{
    draw_hist h1;
    
    TH1D *h = h1.Std_hist1D("bbplambdapix10_cs_pre.root",2,"f","M_{bc}","mbc0",5.23,5.29,100);

    
    h->Draw();
    
}
