#include "Det.h"

void ExampleSort(string input="Run1.root"){
    
    // Open data file and connect to tree branch
	TChain* DataChain = new TChain("AnalysisTree","AnalysisTree");
	DataChain->Add(input.c_str());
    Event* Event=0;
	if(DataChain->FindBranch("Event"))DataChain->SetBranchAddress("Event",&Event);else return;
	Int_t nentries = DataChain->GetEntries();

    // Create an output file
    while(input.find("/")<=input.size())input=input.substr(input.find("/")+1);
    TFile* output_file = new TFile(("Sorted_"+input).c_str(),"RECREATE");
	output_file->cd();
        TH2D chr("chr","Charge vs Channel;Charge [daq units];Channel;counts/bin",2000,0,2E6,3,0,3);
        TH1D sum("sum","Sum;Charge [daq units];Channel;counts/bin",2000,0,2E6);
	gROOT->cd();

	//Loop over all entries in the tree
	for(int jentry = 0; jentry<nentries; jentry++){
		DataChain->GetEntry(jentry);
        
		//Loop over all detector hits in the event
        for(int i = 0; i<Event->Size(); i++){
			//Fill Histogramas
            Hit* hit=Event->GetHit(i);
            sum.Fill(hit->GetCharge());
            chr.Fill(hit->GetCharge(),hit->GetID());
        }
        
        //Progress counter on command line
        if(jentry%100 == 0){
            cout << setiosflags(ios::fixed) << std::setprecision(2) << 100.*(double)jentry/nentries << " % complete."<<"\r" << flush; 
        }
	}
	cout<<"Sorting Complete."<<endl;
	
	//Save output
 	output_file->Write();
    output_file->Close();
	delete DataChain;
}

	

