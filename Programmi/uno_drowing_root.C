#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "fstream"

#include "TFitResult.h"
#include "TMatrixD.h"
using namespace std;


void uno_drowing_root()
{
    auto c1 = new TCanvas("c1", "Grafici Piani di Clapeyron", 1200, 600); //creo il canvas sul quale mettere il grafico (TGraph)
    c1-> Divide(2,1,0,0);

    c1-> cd(1);
    gPad->SetGrid();
    gPad->SetFillColor(0);
    TGraphErrors *fileInput0 = new TGraphErrors("../Dati/Dati_comp/0.txt", "%lg %lg %lg");

    fileInput0->SetMarkerColor(4);
    fileInput0->SetLineColor(4);
    fileInput0->SetMarkerStyle(20);
    fileInput0->SetMarkerSize(0.5);
    fileInput0->SetTitle("zero");
    fileInput0->GetXaxis()->SetTitle("1/ P [ ]");
    fileInput0->GetYaxis()->SetTitle("Volume [ ]");
    fileInput0->GetXaxis()->SetAxisColor(14);
    fileInput0->GetYaxis()->SetAxisColor(14);

    fileInput0->Draw("AP");

    TF1 *retta = new TF1("f2", "pol1", 0.3,0.92); //ho definito una funzione

    TFitResultPtr r = fileInput0->Fit(retta, "SR");
    
    c1-> cd(2);
    TGraph *gr0 = (TGraph *)gMinuit->Contour(100, 0, 2);

    TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
    TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
    cov.Print();
    cor.Print();
}