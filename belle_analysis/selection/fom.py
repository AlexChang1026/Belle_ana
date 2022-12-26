from math import *
import ROOT
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

################ FOM Calculation ##################


def fom(s,b):
  c = sqrt(s+b)

  if(b==0 and s==0):
    return 0
  else:
    return s/c
    
############### Import Data Sample ################

signal1 = ROOT.TChain("h1")
signal2 = ROOT.TChain("h1")
background = ROOT.TChain("h1")

signal1.Add("../sample/MC/B2psigmapi/rareplambdapi_phase_pre_cs.root")
signal2.Add("../sample/MC/B2psigmapi/rareplambdapi_cs_pre.root")
background.Add("../sample/MC/qq/qqplambdapix6_cs_pre.root")

fom1 = []
fom2 = []
output = []

stat = "mbc0>5.26&&de>-0.14&&de<0.&&nb_vlike>"

for x in range(201):
    
    output.append(-1+0.01*x)
    
    select = "%s%f"%(stat,-1+0.01*x)
  
    entry_s1 = signal1.GetEntries(select)
    entry_s2 = signal2.GetEntries(select)
    
    s1 = 771581000*1.9*1e-6*(entry_s1/771000.0)*1.12
    s2 = 771581000*1.9*1e-6*(entry_s2/771000.0)*1.1
    bkg = 1.5*((background.GetEntries(select))/6)
    
    fom1.append(fom(s1,bkg))
    fom2.append(fom(s2,bkg))
    

plt.figure(figsize=(8,8))
plt.rc('font', size=25)
plt.plot(output,fom2,label=r"Threshold enhancement $B^0 \to p\bar{\Sigma}^0\pi^-$ ",linewidth=2.0)
#plt.plot(output,fom1,label=r"Phase space $B^0 \to p\bar{\Sigma}^0\pi^-$",linewidth=2.0)
plt.xlabel("CSMVA_Output",fontsize=25)
plt.ylabel("FOM",fontsize=25)
plt.grid()
#plt.legend(loc='best', prop={'size': 15})
plt.savefig("fom.pdf")
plt.show()
