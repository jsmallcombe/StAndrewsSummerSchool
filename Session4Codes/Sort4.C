#include "Det.h"

void Sort4(string input="Run4.root"){
    // Time Gate
    double TgateLower=-1E9;
    double TgateUpper=1E9;
    double TrueCoincidenceFraction=1.0;
    
    //Set Kinematic Information
    SetTheta(0,TMath::Pi()/4.);
    SetTheta(1,TMath::Pi()/2.);
    SetTheta(2,TMath::Pi()*3/4.);
    gBeta=0.0577059;
    
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
        TH1D Time("Time","Gamma Time Difference;#DeltaT [ns];counts/bin",1000,-1000,1000);
        TH2D yy("yy","GammaGamma;Energy [keV];Energy [keV];counts/2 keV",500,0,2000,500,0,2000);
        TH2 *back=(TH2*)yy.Clone("back");
        back->Reset();
	gROOT->cd();
    
    //Create a background histograms of with identical binning by cloning.

    //Event loop
	for(int jentry = 0; jentry<nentries; jentry++){
		DataChain->GetEntry(jentry);
        
        //Hit loop
        for(int i = 0; i<Event->Size(); i++){
            
            Hit* hit=Event->GetHit(i);
            double E1=hit->GetDopplerEnergy();
            Yesum.Fill(E1);
            
            //Coincident hit loop
            for(int j = i+1; j<Event->Size(); j++){
                
                Hit* hitb=Event->GetHit(j);
                double E2=hitb->GetDopplerEnergy();
                long deltaT=hitb->GetTime()-hit->GetTime();
                Time.Fill(deltaT);
                
                // Fill based on time gate
                if(deltaT>TgateLower&&deltaT<TgateUpper){
                    yy.Fill(E2,E1);
                    yy.Fill(E1,E2);
                }else{
                    back->Fill(E2,E1);
                    back->Fill(E1,E2);
                }
            }
        }
        
        if(jentry%100 == 0){
            cout << setiosflags(ios::fixed) << std::setprecision(2) << 100.*(double)jentry/nentries << " % complete."<<"\r" << flush; 
        }
	}
	
	if(back->Integral()>0){
        double BackScale=-(1-TrueCoincidenceFraction)*yy.Integral()/back->Integral();
        //yy.Sumw2();
        yy.Add(back,BackScale);
    }
    
    TCanvas *C1=new TCanvas();
    yy.DrawCopy("col");
    
 	output_file->Write();
    output_file->Close();
}

	

