import ROOT
import sys

#input = sys.argv[1]
#output = sys.argv[2]

#f = ROOT.RDataFrame("h1","/Users/zhangchuanyu/Desktop/6.24.02/share/root/macros/sample/MC/B2psigmapi/rareplambdapi_cs_pre.root") ## import data sample
f = ROOT.RDataFrame("h1","plambdapi_cs_pre.root")

pre_cut = "de>-0.14&&mbc0>5.23&&pid0>0.6&&pid01>0.6&&pid2<0.4&&dr0<0.3&&dr0>-0.3&&dz0<2&&dz0>-2&&dr2<0.3&&dr2>-0.3&&dz2<4&&dz2>-4&&mass1>1.111&&mass1<1.121&&((pid111>0.6&&pid112>0.6&&mass11>0.9)||(pid101>0.6&&pid102>0.6&&mass10>0.9))"
continue_suppression = "&&nb_vlike>0.8"
veto = "&&(m_lpi>2.3||m_lpi<2.15)"
rare_veto = "&&(m_pl<4.95&&m_ppi>1.121)"

exclude = "(bpd1!=-2212||bpd2!=3212||bpd3!=321)&&(bnd1!=2212||bnd2!=-3212||bnd3!=-321)&&(bpd1!=-3122||bpd2!=2212||bpd3!=-321)&&(bnd1!=3122||bnd2!=-2212||bnd3!=321)&&(bpd1!=-3122||bpd2!=3122||bpd3!=211)&&(bnd1!=3122||bnd2!=-3122||bnd3!=-211)&&(bpd1!=-2212||bpd2!=3122||bpd3!=321)&&(bnd1!=2212||bnd2!=-3122||bnd3!=-321)&&(bpd1!=-3122||bpd2!=2212||bpd3!=-213)&&(bnd1!=3122||bnd2!=-2212||bnd3!=213)"

d = f.Filter(pre_cut+continue_suppression+veto) ## Selection criteria

#d_new = d.Define("mbc1","mbc0*2") ## Add new variable

d.Snapshot("h1","plambdapi_final.root") ## create new ROOT file

