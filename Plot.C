void Plot(){

TFile *fOutfile = new TFile("file.root"); //opening the output file

TH1D *histElectPt = (TH1D*)fOutfile->Get("name_of_elec_hist");
TH1D *histMuPt    = (TH1D*)fOutfile->Get("name_of_mu_hist");

TCanvas *c1 = new TCanvas();
histElectPt->Draw("e");
histMuPt   ->Draw("esame");



}
