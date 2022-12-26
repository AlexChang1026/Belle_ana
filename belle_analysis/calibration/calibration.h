#include <iostream>

class calibration
{
public:
    
   void load_cal_table(const char *name, float cal_table[][12]);
   float cal_factor(const char *file_name,float cal_table[][12]);
};

void calibration::load_cal_table(const char *name, float cal_table[][12])
{
    char txt_name[20];
    sprintf(txt_name,"%s%s",name,".txt");
    
    ifstream table(txt_name);
    
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<12;j++)
        {
            table>>cal_table[i][j] ;
        
        }
    }
    
};

float calibration::cal_factor(const char *file_name,float cal_table[][12])
{
    double p[12] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.5,3.0};
    double cos[13] = {-1.000,-0.612,-0.511,-0.300,-0.152,0.017,0.209,0.355,0.435,0.542,0.692,0.842,1.000};
    
    double cal_factor1 ;
    double cal_factor2 ;
    double error1 ;
    double error2 ;
    double error_total;
    double eff_cal_factor = 10.;
    double event = 0;
    
    TFile *fin = new TFile(file_name);
    TTree *B = (TTree *)fin->Get("h1");
    
    float hi10,hi11;
    float p0,p10,p11;
    float cos0,cos10,cos11;
    float sum = 0;
    float sum_error = 0;
    double n_entry = B->GetEntries();
    float expnum;
    
    B -> SetBranchAddress("hi10",&hi10);
    B -> SetBranchAddress("hi11",&hi11);
    B -> SetBranchAddress("p0",&p0);
    B -> SetBranchAddress("p10",&p10);
    B -> SetBranchAddress("p11",&p11);
    B -> SetBranchAddress("cos0",&cos0);
    B -> SetBranchAddress("cos10",&cos10);
    B -> SetBranchAddress("cos11",&cos11);
    B -> SetBranchAddress("expnum",&expnum);
    
    for(int i = 0;i<B->GetEntries();i++)
    {
        B->GetEntry(i);
        
       // double eff_cal_factor = 10 ;
        
        int p_index1 = 0 ;
        int cos_index1= 0 ;
        int p_index2 = 0 ;
        int cos_index2 = 0 ;
        
        float p2 = 0;
        float cos2 = 0 ;
        float hi2 = 0;
        
        if(abs(hi11)==2212)
        {
            p2 = p11;
            
            cos2 = cos11;
        
            hi2 = hi11;
        }
        
        if(abs(hi10)==2212)
        {
            p2 = p10;
            
            cos2 = cos10;
       
            hi2 = hi10;
        }
        
        
        
        for (int j = 0;j<12;j++)
        {
            if(p0>p[11])
            {
                p_index1 = 11 ;
                
            }
            
           if(j<11)
           {
               if(p[j+1]>p0&&p0>p[j])
              {
                p_index1 = j;
              }
               
           }
            
        }
        
       for(int k = 0;k<12;k++)
       {

           if(cos[k+1]>cos0&&cos0>cos[k])
           {
               cos_index1 = k;
           }
           
       }
        
       if(expnum<28)
    {
       if(hi2>0)
       {
           cal_factor1 = p_SVD1[p_index1][cos_index1];
           error1 = p_SVD1_error[p_index1][cos_index1];
       }
        else
        {
            cal_factor1 = anti_p_SVD1[p_index1][cos_index1];
            error1 = anti_p_SVD1_error[p_index1][cos_index1];
        }
    }
      else
      {
          if(hi2>0)
          {
              cal_factor1 = p_SVD2[p_index1][cos_index1];
              error1 = p_SVD2_error[p_index1][cos_index1];
          
          }
         else
         {
             cal_factor1 = anti_p_SVD2[p_index1][cos_index1];
             error1 = anti_p_SVD2_error[p_index1][cos_index1];
         
         }
     
      }
      
        
        
        for (int x = 0;x<12;x++)
        {
            if(p2>p[11])
            {
                
                p_index2 = 11;
                
            }
            
           if(x<11)
           {
             if(p[x+1]>p2&&p2>p[x])
             {
                p_index2 = x;
             }
           }
        }
        
        for(int y = 0;y<12;y++)
        {

            if(cos[y+1]>cos2&&cos2>cos[y])
            {
                cos_index2 = y;
            }
            
        }
        
         if(expnum<28)
       {
         if(hi2>0)
         {
             cal_factor2 = p_SVD1[p_index2][cos_index2];
             error2 = p_SVD1_error[p_index2][cos_index2];
         }
         else
         {
             cal_factor2 = anti_p_SVD1[p_index2][cos_index2];
             error2 = anti_p_SVD1_error[p_index2][cos_index2];
         }
      }
         else
        {
            if(hi2>0)
            {
              cal_factor2 = p_SVD2[p_index2][cos_index2];
              error2 = p_SVD2_error[p_index2][cos_index2];
            
            }
             else
            {
                    cal_factor2 = anti_p_SVD2[p_index2][cos_index2];
                    error2 = anti_p_SVD2_error[p_index2][cos_index2];
            }
       }
      
        error_total = sqrt(error1*error1 + error2*error2);
       // error_total = error1 + error2;
        
        eff_cal_factor = cal_factor1*cal_factor2 ;
        
       // printf(" %f %f %f %f \n",cal_factor1,cal_factor2,eff_cal_factor);
       // printf("%f %f %f\n",error1,error2,error_total);
        
        sum = sum + eff_cal_factor ;
        sum_error = sum_error + error_total ;
        event = event + 1 ;
       
    }
    
    eff_cal_factor = sum/event ;
    error_total = sum_error/event ;

    
    printf ("entries is %f Calibration factor is %f error is %f\n",event,eff_cal_factor,error_total);
    
}
