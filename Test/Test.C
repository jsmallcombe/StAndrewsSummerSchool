#include "Det.h"

void Test(){
    
    // Open data file and connect to tree branch
	TChain* DataChain = new TChain("AnalysisTree","AnalysisTree");
	DataChain->Add("Run1.root");
    Event* Event=0;
	if(DataChain->FindBranch("Event"))DataChain->SetBranchAddress("Event",&Event);else return;
	Int_t nentries = DataChain->GetEntries();

    TH1D H1("H1","Test Hist",1000,0,2E6);

	for(int jentry = 0; jentry<nentries; jentry++){
		DataChain->GetEntry(jentry);
        for(int i = 0; i<Event->Size(); i++){
            Hit* hit=Event->GetHit(i);
            if(hit->GetID()==0)H1.Fill(hit->GetCharge());
        }
	}
	
    int b=H1.GetMaximumBin();
    H1.GetXaxis()->SetRange(0.9*b,b*1.1);
    TF1 TestFit("TestFit","gaus(0)+pol1(3)");
    TestFit.SetParameters(H1.GetBinContent(b),H1.GetXaxis()->GetBinCenter(b),2000,0,0);
    
	TCanvas *C1=new TCanvas();
    gPad->Update();
    H1.Fit(&TestFit);
    H1.DrawCopy();
    
    TText text;
    text.SetTextColor(2);
    text.DrawTextNDC(0.12,0.5,"You are ready for StAndrews");
}

	

