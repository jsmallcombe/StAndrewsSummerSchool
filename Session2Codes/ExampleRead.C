TH1* ExampleRead(string input="Sorted_Run2.root"){
    
    // Load an input file
	TFile* InFile = new TFile(input.c_str(),"READ");
	if(!InFile->IsOpen()){cout<<endl<<"NO FILE"<<flush; return 0;}
	gROOT->cd();

    // Load an input histogram
	TH1* LoadH=(TH1*)InFile->Get("Esum");
	if(!LoadH){cout<<endl<<"NO HISTOGRAM"<<flush; return 0;}

	
    // Do work with the histogram
    TH1 *Backgnd=TSpectrum::StaticBackground(LoadH,20);
    LoadH->Sumw2();
    LoadH->Add(Backgnd,-1);
	
    // Save a copt of the histogram
    while(input.find("/")<=input.size())input=input.substr(input.find("/")+1);
    TFile* output_file = new TFile(("RESorted_"+input).c_str(),"RECREATE");
    LoadH->Write();
	output_file->Close();
    
    //Return a pointer
    return LoadH;
}
