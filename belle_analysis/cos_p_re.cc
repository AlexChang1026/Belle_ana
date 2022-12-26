void cos_p_re()
{
//---------------- input file -----------------------
    
    TFile *fin = new TFile("test.root");
    TTree *B = (TTree *)fin ->Get("h1");
    
    float px0,py0,pz0,e0;
    float px1,py1,pz1,e1;
    float px2,py2,pz2,e2;
    float bnd1,bpd1,cos_p;
    
    B->SetBranchAddress("px0",&px0);
    B->SetBranchAddress("py0",&py0);
    B->SetBranchAddress("pz0",&pz0);
    B->SetBranchAddress("e0",&e0);
    B->SetBranchAddress("px1",&px1);
    B->SetBranchAddress("py1",&py1);
    B->SetBranchAddress("pz1",&pz1);
    B->SetBranchAddress("e1",&e1);
    B->SetBranchAddress("px2",&px2);
    B->SetBranchAddress("py2",&py2);
    B->SetBranchAddress("pz2",&pz2);
    B->SetBranchAddress("e2",&e2);
    B->SetBranchAddress("bpd1",&bpd1);
    B->SetBranchAddress("bnd1",&bnd1);
    
// --------------- output file ------------------------
    
    TFile *fin2 = new TFile("test2.root","recreate");
    TTree *h1 = new TTree("h1","B1");

    h1->Branch("px0",&px0,"px0");
    h1->Branch("py0",&py0,"py0");
    h1->Branch("pz0",&pz0,"pz0");
    h1->Branch("e0",&e0,"e0");
    h1->Branch("px1",&px1,"px1");
    h1->Branch("py1",&py1,"py1");
    h1->Branch("pz1",&pz1,"pz1");
    h1->Branch("e1",&e1,"e1");
    h1->Branch("px2",&px2,"px2");
    h1->Branch("py2",&py2,"py2");
    h1->Branch("pz2",&pz2,"pz2");
    h1->Branch("e2",&e2,"e2");
    h1->Branch("bpd1",&bpd1,"bpd1");
    h1->Branch("bnd1",&bnd1,"bnd1");
    h1->Branch("cos_ppi",&cos_ppi,"cos_ppi");

//---------------------cosine p calculation ----------------------
    
    for (int i = 0;i<B->GetEntries();i++)
    {
        B->GetEntry(i);
     
      
        TLorentzVector p_momentum(px0,py0,pz0,e0);
        TLorentzVector lambda_momentum(px1,py1,pz1,e1);
        TLorentzVector pi_momentum(px2,py2,pz2,e2);
        TLorentzVector BBframe = p_momentum+lambda_momentum;
        
        p_momentum.Boost(-BBframe.BoostVector());
        pi_momentum.Boost(-BBframe.BoostVector());
        
        TVector3 p_bb_momentum(p_momentum.Vect());
        TVector3 pi_bb_momentum(pi_momentum.Vect());
        
        cos_p = p_bb_momentum.Dot(pi_bb_momentum)/(p_bb_momentum.Mag()*pi_bb_momentum.Mag());
        
        h1->Fill();
    
    
}
    h1 ->Write();
    fin2 ->Close();
}
