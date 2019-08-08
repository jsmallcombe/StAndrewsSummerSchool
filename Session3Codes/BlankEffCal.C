
void BlankEffCal(){
    
    // Part 1 Drawing Data from file
	TGraphErrors* SourceData= new TGraphErrors("152Eu.txt");
	SourceData->GetHistogram()->SetMinimum(0);
	TCanvas *C1=new TCanvas();
	gPad->Update();
    SourceData->Draw("ap");
    
    // Part 2 Fit
//     TF1* exfit=new TF1("exfit","x*[0]+[1]",50,2050);
//     SourceData->Fit(exfit,"RNE");
//     SourceData->Draw("same");

    // Part 3 Confidence Interval
//     TH1D* hist=new TH1D("err","eer",1000,50,2050);
//     (TVirtualFitter::GetFitter())->GetConfidenceIntervals(hist,0.68);
//     hist->SetFillColor(2);
//     hist->SetFillStyle(3001);
//     hist->Draw("E3same");
        
}
