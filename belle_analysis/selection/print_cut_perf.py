import ROOT

def eff(num):
    return num/771000

def expected_yeild(eff):
    return eff*771581000*1.9*1e-6
    
def scf_ratio(scf,sig):
    return 100*(scf/(scf+sig))
    


signal_TE = ROOT.TChain("h1")
signal_phsp = ROOT.TChain("h1")
qq = ROOT.TChain("h1")
bb = ROOT.TChain("h1")
rare = ROOT.TChain("h1")
scf_phase = ROOT.TChain("h1")
scf_enhan = ROOT.TChain("h1")
plambdapi = ROOT.TChain("h1")

#signal_TE.Add("../sample/MC/B2psigmapi/rareplambdapi_cs_pre.root")
signal_TE.Add("rareplambdapi_cs.root")
signal_phsp.Add("../sample/MC/B2psigmapi/rareplambdapi_phase_pre_cs.root")
qq.Add("../sample/MC/qq/qqplambdapix6_cs.root")
bb.Add("../sample/MC/BB/bbplambdapix10_cs.root")
rare.Add("../sample/MC/rare/plambdapi_rare_cs2_1013.root")
scf_phase.Add("../sample/MC/B2psigmapi/rareplambdapi_phase_scf_cs.root")
scf_enhan.Add("../sample/MC/B2psigmapi/rareplambdapi_scf_cs.root")
#plambdapi.Add("../sample/MC/B2plambdapi/plambdapi_cs.root")
plambdapi.Add("test_plambdapi_cs.root")

pre_cut="de>-0.14&&mbc0>5.23&&pid0>0.6&&pid01>0.6&&pid2<0.4&&dr0<0.3&&dr0>-0.3&&dz0<4&&dz0>-4&&dr2<0.3&&dr2>-0.3&&dz2>-4&&mass1>1.111&&mass1<1.121&&((pid111>0.6&&pid112>0.6&&mass11>0.9)||(pid101>0.6&&pid102>0.6&&mass10>0.9))"

pre_cut_sig1="de<0&&de>-0.14&&mbc0>5.26&&pid0>0.6&&pid01>0.6&&pid2<0.4&&dr0<0.3&&dr0>-0.3&&dz0<2&&dz0>-2&&dr2<0.3&&dr2>-0.3&&dz2>-4&&dz2<4&&mass1>1.111&&mass1<1.121&&((pid111>0.6&&pid112>0.6&&mass11>0.9)||(pid101>0.6&&pid102>0.6&&mass10>0.9))"

pre_cut_sig2="de>-0.05&&de<0.05&&mbc0>5.27&&pid0>0.6&&pid01>0.6&&pid2<0.4&&dr0<0.3&&dr0>-0.3&&dz0<2&&dz0>-2&&dr2<0.3&&dr2>-0.3&&dz2>-4&&dz2<4&&mass1>1.111&&mass1<1.121&&((pid111>0.6&&pid112>0.6&&mass11>0.9)||(pid101>0.6&&pid102>0.6&&mass10>0.9))"

ex_cut = ["&&nb_vlike>0.8","&&(m_lpi>2.3||m_lpi<2.15)"]
cate = ["Before CS","After CS","After veto"]


for i in range(3):
  
    signal_eff_phsp = 1.12*eff(signal_phsp.GetEntries(pre_cut))
    #signal_eff_te = 1.1*eff(signal_TE.GetEntries(pre_cut))
    signal_eff_te = eff(signal_TE.GetEntries(pre_cut+"&&hindex==4"))
    
    expect_num_phsp = expected_yeild(signal_eff_phsp)
    expect_num_te = expected_yeild(signal_eff_te)
    
    scf_prop_phase = scf_ratio(scf_phase.GetEntries(pre_cut),signal_phsp.GetEntries(pre_cut))
    scf_prop_enhan = scf_ratio(scf_enhan.GetEntries(pre_cut),signal_TE.GetEntries(pre_cut))
    
    plambdapi_eff = eff(plambdapi.GetEntries(pre_cut+"&&hindex==1"))
    plambdapi_num = plambdapi_eff*771581000*3.14*1e-6
    scf_prop_plambdapi = scf_ratio(plambdapi.GetEntries(pre_cut+"&&hindex!=1"),plambdapi.GetEntries(pre_cut+"&&hindex==1"))
    
    bb_entry = bb.GetEntries(pre_cut)/10
    qq_entry = qq.GetEntries(pre_cut)/6
    rare_entry = rare.GetEntries(pre_cut+"&&(m_pl<4.95&&m_ppi>1.121)")/50
    
    
    signal_eff_phsp_sig = 1.12*eff(signal_phsp.GetEntries(pre_cut_sig1))
    #signal_eff_te_sig = 1.1*eff(signal_TE.GetEntries(pre_cut_sig1))
    signal_eff_te_sig = 1.1*eff(signal_TE.GetEntries(pre_cut_sig1+"&&hindex==4"))
    
    expect_num_phsp_sig = expected_yeild(signal_eff_phsp_sig)
    expect_num_te_sig = expected_yeild(signal_eff_te_sig)
    
    scf_prop_phase_sig = scf_ratio(scf_phase.GetEntries(pre_cut_sig1),signal_phsp.GetEntries(pre_cut_sig1))
    scf_prop_enhan_sig = scf_ratio(scf_enhan.GetEntries(pre_cut_sig1),signal_TE.GetEntries(pre_cut_sig1))
    
    plambdapi_eff_sig = eff(plambdapi.GetEntries(pre_cut_sig2+"&&hindex==1"))
    plambdapi_num_sig = plambdapi_eff_sig*771581000*3.14*1e-6
    scf_prop_plambdapi_sig = scf_ratio(plambdapi.GetEntries(pre_cut_sig2+"&&hindex!=1"),plambdapi.GetEntries(pre_cut_sig2+"&&hindex==1"))
    
    bb_entry_sig1 = bb.GetEntries(pre_cut_sig1)/10
    qq_entry_sig1 = qq.GetEntries(pre_cut_sig1)/6
    rare_entry_sig1 = rare.GetEntries(pre_cut_sig1+"&&(m_pl<4.95&&m_ppi>1.121)")/50
    
    bb_entry_sig2 = bb.GetEntries(pre_cut_sig2)/10
    qq_entry_sig2 = qq.GetEntries(pre_cut_sig2)/6
    rare_entry_sig2 = rare.GetEntries(pre_cut_sig2+"&&(m_pl<4.95&&m_ppi>1.121)")/50
    
    
    print(cate[i])
    print("Signal(TE) eff : {:.2f}% {:.2f}%".format(signal_eff_te*100,signal_eff_te_sig*100))
    print("Expected Yield(TE) : {:.2f} {:.2f}".format(expect_num_te,expect_num_te_sig))
    print("SCF(TE) : {:.2f}% {:.2f}%".format(scf_prop_enhan,scf_prop_enhan_sig))
    print("Signal(PHSP) eff : {:.2f}% {:.2f}%".format(signal_eff_phsp*100,signal_eff_phsp_sig*100))
    print("Expected Yield(PHSP) : {:.2f} {:.2f}".format(expect_num_phsp,expect_num_phsp_sig))
    print("SCF(PHSP): {:.2f}% {:.2f}% ".format(scf_prop_phase,scf_prop_phase_sig))
    print("plambdapi eff:{:.2f}% {:.2f}%".format(plambdapi_eff*100,plambdapi_eff_sig*100))
    print("plambdapi Yield: {:.2f} {:.2f}".format(plambdapi_num,plambdapi_num_sig))
    print("SCF plambdapi: {:.2f}% {:.2f}%".format(scf_prop_plambdapi,scf_prop_plambdapi_sig))
    print("qq : {} {} {}".format(int(qq_entry),int(qq_entry_sig1),int(qq_entry_sig2)))
    print("bb : {} {} {}".format(int(bb_entry),int(bb_entry_sig1),int(bb_entry_sig2)))
    print("rare : {} {} {}".format(int(rare_entry),int(rare_entry_sig1),int(rare_entry_sig2)))
    print("")
    print("####################################")
    
    pre_cut = pre_cut+ex_cut[i]
    pre_cut_sig1 = pre_cut_sig1+ex_cut[i]
    pre_cut_sig2 = pre_cut_sig2+ex_cut[i]
    


