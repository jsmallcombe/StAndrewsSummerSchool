inline Double_t ExampleFn(Double_t* x, Double_t* par)
{
  double sum = par[0]*exp(-pow((x[0]-par[1])/par[2],2));
  sum += par[3]+par[4]*x[0];
  return sum;
}

TF1* ExampleFit(TH1* H1=0){
	if(!H1)return new TF1();
    int b=H1->GetMaximumBin();
    H1->GetXaxis()->SetRange(0.95*b,b*1.05);
    TF1 *TestFit=new TF1("TestFit","gausn(0)+pol1(3)");
    //TF1 *TestFit=new TF1("TestFit",ExampleFn,0.9*b,b*1.1,5);
    //TF1 *TestFit=new TF1("TestFit","[0]*exp(-pow((x-[1])/[2],2))+[3]+[4]*x");
    TestFit->SetParameters(H1->GetBinContent(b),H1->GetXaxis()->GetBinCenter(b),H1->GetXaxis()->GetBinCenter(b)*0.001,0,0);
	H1->Fit(TestFit);
	return TestFit;
}

	

