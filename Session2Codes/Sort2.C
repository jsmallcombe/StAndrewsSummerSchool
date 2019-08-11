#include "Det.h"

TH1* Sort2(string input="Run2.root"){
    
// // // Part 1 Energy Calibration
//     SetCal(0,-0.229526,0.00126409,-6.84542e-13);
//     SetCal(1,-0.0276738,0.00132368,-9.58032e-14);
//     SetCal(2,-0.247034,0.00127735,-4.93374e-13); 
    
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
        }
        
        //Progress counter on command line
        if(jentry%100 == 0){
            cout << setiosflags(ios::fixed) << std::setprecision(2) << 100.*(double)jentry/nentries << " % complete."<<"\r" << flush; 
        }
	}
	cout<<"Sorting Complete."<<endl;
    
// // // Part 2 Calculating a smooth background by using TSpectrum functions
//     TH1 *Backgnd=TSpectrum::StaticBackground(&Esum,2);
//      Backgnd->SetLineColor(2);
//     Backgnd->SetLineWidth(2);
//     TCanvas *C1=new TCanvas();
//     Esum.DrawCopy();
//     Backgnd->Draw("same");
    
// // // Part 3 subtracting the background
//     TCanvas *C2=new TCanvas();
// // // // //     Esum.Sumw2();
//     Esum.Add(Backgnd,-1);
//     Esum.DrawCopy();
    
// // // Part 4 extracting an integral
// // // // NOTE : Esum is NOT 1 keV/bin
//     double Error=0;
//     TAxis *x=Esum.GetXaxis();
//     double Counts=Esum.IntegralAndError(x->FindBin(341),x->FindBin(346),Error);
//     cout<<endl<<Counts<< " +- "<<Error<<endl;
    
	//Save output
    TH1* RetHist = (TH1*)Esum.Clone("Eclone");
 	output_file->Write();
    output_file->Close();
	delete DataChain;

    return RetHist;
}
