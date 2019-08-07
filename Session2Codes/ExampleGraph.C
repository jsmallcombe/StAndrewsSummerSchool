TF1* ExampleGraph(){
    // Load an input file
	
    TGraph *Example = new TGraph;
	Example->SetPoint(Example->GetN(),1.2,1.5);
	Example->SetPoint(Example->GetN(),1.7,1.3);
	Example->SetPoint(Example->GetN(),2.5,1.7);
	Example->SetPoint(Example->GetN(),3.7,1.2);
    
    TCanvas *C1=new TCanvas();
    gPad->Update();
    Example->SetMarkerStyle(33);
    Example->Draw("apl");
    
    TGraphErrors *ExErr = new TGraphErrors;
	ExErr->SetPoint(0,1.2,1.5);
	ExErr->SetPointError(0,0,0.5);
	ExErr->SetPoint(1,1.7,1.3);
	ExErr->SetPointError(1,0,0.4);
	ExErr->SetPoint(2,2.5,1.7);
	ExErr->SetPointError(2,0,0.3);
	ExErr->SetPoint(3,3.7,1.2);
	ExErr->SetPointError(3,0,0.1);
    
    TCanvas *C2=new TCanvas();
    gPad->Update();
    ExErr->Draw("ap");
    
    TF1* FitFn=new TF1("CppFn","[0]*pow(x,2)+[1]");
    ExErr->Fit(FitFn,"Q");
        
    return FitFn;
}
