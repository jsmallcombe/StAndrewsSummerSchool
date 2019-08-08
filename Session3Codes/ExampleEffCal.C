inline Double_t PhotoEfficiency(Double_t* x, Double_t* par)
{

  double sum = 0.0;
  for(int i = 0; i < 3; ++i) {
     sum += par[i] * TMath::Power(TMath::Log(x[0]), i);
  }

  return TMath::Exp(sum);
}

void ExampleEffCal(){
	TGraphErrors* SourceData= new TGraphErrors("152Eu.txt");
	SourceData->GetHistogram()->SetMinimum(0);
    
	TCanvas *C1=new TCanvas();
	gPad->Update();

	SourceData->Draw("ap");
	TF1* photoeff=new TF1("photoeff",PhotoEfficiency,50,2000,3);
	SourceData->Fit(photoeff,"RNE");
	photoeff->SetNpx(999);
	SourceData->Draw("same");

    TH1D* hist=new TH1D("err","eer",500,50,2050);
    (TVirtualFitter::GetFitter())->GetConfidenceIntervals(hist,0.68);
    hist->Draw("same");
    

    for(int p=0;p<3;p++){
    TGraph *ChiSq=new TGraph();
    TGraph *ChiSqHalf=new TGraph();
        double CHI0=photoeff->GetChisquare();
        double p0=photoeff->GetParameter(p);
        double p0E=photoeff->GetParError(p);
        
        for(int i=-100;i<100;i++){
            
            double ptmp=p0+p0E*i/50;
            photoeff->FixParameter(p,ptmp);
            SourceData->Fit(photoeff,"RNQ");
            
            ChiSq->SetPoint(ChiSq->GetN(),ptmp,photoeff->GetChisquare()-CHI0); 
            if(i>=0)ChiSqHalf->SetPoint(ChiSqHalf->GetN(),photoeff->GetChisquare()-CHI0,ptmp);       
        }
        
        TCanvas *C2=new TCanvas();
        C2->Divide(2);
        C2->cd(1);
        gPad->Update();
        ChiSq->Draw("al");
        C2->cd(2);
        gPad->Update();
//         ChiSqHalf->GetHistogram()->SetMinimum(p0);
        ChiSqHalf->Draw("al");
        
        cout<<endl<<"Value at ChiSq 1 = "<<ChiSqHalf->Eval(1)-p0<<endl;
        cout<<"TF1 Error = "<<p0E<<endl;
        photoeff->ReleaseParameter(p);
        photoeff->SetParameter(p,p0);
        SourceData->Fit(photoeff,"RNQE");
    }
        
}
