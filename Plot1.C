void Plot1(){

TFile *fOutfile = new TFile("AnalysisResults.root"); //opening the output file

TH1D *histElectPDG = (TH1D*)fOutfile->Get("parents/PDG_e");
 //TH1D *histMuPt    = (TH1D*)fOutfile->Get("name_of_mu_hist");
// auto y = (TH1D*)fOutfile->Get("parents/PDG_e");
// const Int_t n = 20;
//double x[20] = {4,5,21,111,113,213,221,223,313,323,331,333,411,413,421,431,445,531};
TCanvas *B = new TCanvas( "histElectPDG","histElectPDG",200,10,600,400);
//auto histElectPDG = new TGraph(n,x,y);
histElectPDG->SetFillColor(40);
histElectPDG->GetBarWidth();
//histElectPDG->GetXaxis()->SetRangeUser(-450,450);
histElectPDG->Draw("B");
//histMuPt   ->Draw("esame");
}
