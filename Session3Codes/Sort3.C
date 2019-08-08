#include "Det.h"

void Sort3(string input="Run4.root"){
    
    //Set Kinematic Information
    SetTheta(0,0);
    SetTheta(1,0);
    SetTheta(2,0);
    gBeta=0.0;
    
    // Load calibration parameters 
    SetCal(0,-0.229526,0.00126409,-6.84542e-13);
    SetCal(1,-0.0276738,0.00132368,-9.58032e-14);
    SetCal(2,-0.247034,0.00127735,-4.93374e-13); 
    
    // Open data file and connect to tree branch
	TChain* DataChain = new TChain("AnalysisTree","AnalysisTree");
	DataChain->Add(input.c_str());
    Event* Event=0;
	if(DataChain->FindBranch("Event"))DataChain->SetBranchAddress("Event",&Event);else return;
	Int_t nentries = DataChain->GetEntries();

    // Create an output file
    while(input.find("/")<=input.size())input=input.substr(input.find("/")+1);
    TFile* output_file = new TFile(("sorted_output_"+input).c_str(),"RECREATE");
	output_file->cd();
        TH1D Yesum("Yesum","Gamma Singles;Energy [keV];counts/keV",2500,0,2500);
        TH2D YeID("YeID","Gamma by Channel;Energy [keV]; Channel ;counts/keV",2500,0,2500,3,0,3);
//         TH2D yy("yy","GammaGamma;Energy [keV];Energy [keV];counts/2 keV",1000,0,2000,1000,0,2000);
	gROOT->cd();

	for(int jentry = 0; jentry<nentries; jentry++){
		DataChain->GetEntry(jentry);
        
        for(int i = 0; i<Event->Size(); i++){
            Hit* hit=Event->GetHit(i);
            double E1=hit->GetDopplerEnergy();
            Yesum.Fill(E1);
            YeID.Fill(E1,hit->GetID());
//             for(int j = i+1; j<Event->Size(); j++){
//                 Hit* hitb=Event->GetHit(j);
//                 double E2=hitb->GetDopplerEnergy();
//                 yy.Fill(E2,E1);
//                 yy.Fill(E1,E2);
//             }
        }
        
        if(jentry%100 == 0){
            cout << setiosflags(ios::fixed) << std::setprecision(2) << 100.*(double)jentry/nentries << " % complete."<<"\r" << flush; 
        }
	}
	
 	output_file->Write();
    output_file->Close();
}

	

