#include "Det.h"

TH1* ExampleSortMod(string input="Run2.root"){
    
    SetCal(0,-0.229526,0.00126409,-6.84542e-13);
    SetCal(1,-0.0276738,0.00132368,-9.58032e-14);
    SetCal(2,-0.247034,0.00127735,-4.93374e-13); 
    
    // Open data file and connect to tree branch
	TChain* DataChain = new TChain("AnalysisTree","AnalysisTree");
	DataChain->Add(input.c_str());
    Event* Event=0;
	if(DataChain->FindBranch("Event"))DataChain->SetBranchAddress("Event",&Event);else return 0;
	Int_t nentries = DataChain->GetEntries();

    // Create an output file
    while(input.find("/")<=input.size())input=input.substr(input.find("/")+1);
    TFile* output_file = new TFile(("Sorted_"+input).c_str(),"RECREATE");
	output_file->cd();
        TH2D chr("chr","Charge vs Channel;Charge [daq units];Channel;counts/bin",2000,0,2E3,3,0,3);
        TH1D Esum("Esum","ESum;Energy [keV];counts/bin",4000,0,2000);
	gROOT->cd();

	//Loop over all entries in the tree
	for(int jentry = 0; jentry<nentries; jentry++){
		DataChain->GetEntry(jentry);
        
		//Loop over all detector hits in the event
        for(int i = 0; i<Event->Size(); i++){
			//Fill Histogramas
            Hit* hit=Event->GetHit(i);
            Esum.Fill(hit->GetEnergy());
            chr.Fill(hit->GetCharge(),hit->GetID());
        }
        
        //Progress counter on command line
        if(jentry%100 == 0){
            cout << setiosflags(ios::fixed) << std::setprecision(2) << 100.*(double)jentry/nentries << " % complete."<<"\r" << flush; 
        }
	}
	cout<<"Sorting Complete."<<endl;
    
    TH1 *Backgnd=TSpectrum::StaticBackground(&Esum,20);
    Backgnd->SetLineColor(2);
    Backgnd->SetLineWidth(2);
    TCanvas *C1=new TCanvas();
    Esum.DrawCopy();
    Backgnd->Draw("same");
    
    TCanvas *C2=new TCanvas();
    Esum.Sumw2();
    Esum.Add(Backgnd,-1);
    Esum.DrawCopy();
    
    double Error=0;
    cout<<endl<<Esum.IntegralAndError(682,692,Error);
    cout<<endl<<Error;
    cout<<endl;
    
	//Save output
 	output_file->Write();
    output_file->Close();
	delete DataChain;

    return (TH1*)Esum.Clone("Eclone");
}
