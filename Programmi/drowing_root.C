#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "fstream"

#include "TFitResult.h"
#include "TMatrixD.h"
using namespace std;

void drowing_root()
{

    //*************************TCANVAS OPTIONS**********************************

    auto c1 = new TCanvas("c1", "Grafici Piani di Clapeyron", 1000, 600); //creo il canvas sul quale mettere il grafico (TGraphErrors)

    if (0)
    {
        gPad->SetGrid();
        gPad->SetFillColor(0);
        TGraphErrors *fileInput0 = new TGraphErrors("../Dati/Dati_comp/0_sc.txt", "%lg %lg %lg");

        fileInput0->SetMarkerColor(4);
        fileInput0->SetMarkerStyle(20);
        fileInput0->SetMarkerSize(0.5);
        fileInput0->SetTitle("zero");
        fileInput0->GetXaxis()->SetTitle("1/ P [ ]");
        fileInput0->GetYaxis()->SetTitle("Volume [ ]");
        fileInput0->GetXaxis()->SetAxisColor(14);
        fileInput0->GetYaxis()->SetAxisColor(14);

        fileInput0->Draw("AP");
        TF1 *retta = new TF1("f2", "pol1"); //ho definito una funzione

        TFitResultPtr r = fileInput0->Fit(retta, "S");

        TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
        TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
        cov.Print();
        cor.Print();
    }
    if (0)
    {
        gPad->SetGrid();
        gPad->SetFillColor(0);

        TGraphErrors *fileInput1 = new TGraphErrors("../Dati/Dati_comp/1_sc.txt", "%lg %lg %lg");
        fileInput1->SetMarkerColor(4);
        fileInput1->SetMarkerStyle(20);
        fileInput1->SetMarkerSize(0.5);
        fileInput1->SetTitle("uno");
        fileInput1->GetXaxis()->SetTitle("1/ P [ ]");
        fileInput1->GetYaxis()->SetTitle("Volume [ ]");
        fileInput1->GetXaxis()->SetAxisColor(14);
        fileInput1->GetYaxis()->SetAxisColor(14);

        fileInput1->Draw("AP");
        TF1 *retta = new TF1("f2", "pol1"); //ho definito una funzione

        TFitResultPtr r = fileInput1->Fit(retta, "S");

        TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
        TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
        cov.Print();
        cor.Print();
    }
    if (0)
    {
        gPad->SetGrid();
        gPad->SetFillColor(0);

        TGraphErrors *fileInput2 = new TGraphErrors("../Dati/Dati_comp/2_sc.txt", "%lg %lg %lg");
        fileInput2->SetMarkerColor(4);
        fileInput2->SetMarkerStyle(20);
        fileInput2->SetMarkerSize(0.5);
        fileInput2->SetTitle("due");
        fileInput2->GetXaxis()->SetTitle("1/ P [ ]");
        fileInput2->GetYaxis()->SetTitle("Volume [ ]");
        fileInput2->GetXaxis()->SetAxisColor(14);
        fileInput2->GetYaxis()->SetAxisColor(14);

        fileInput2->Draw("AP");
        TF1 *retta = new TF1("f2", "pol1"); //ho definito una funzione

        TFitResultPtr r = fileInput2->Fit(retta, "S");

        TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
        TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
        cov.Print();
        cor.Print();
    }
    if (0)
    {
        gPad->SetGrid();
        gPad->SetFillColor(0);

        TGraphErrors *fileInput3 = new TGraphErrors("../Dati/Dati_comp/3_sc.txt", "%lg %lg %lg");
        fileInput3->SetMarkerColor(4);
        fileInput3->SetMarkerStyle(20);
        fileInput3->SetMarkerSize(0.5);
        fileInput3->SetTitle("tre");
        fileInput3->GetXaxis()->SetTitle("1/ P [ ]");
        fileInput3->GetYaxis()->SetTitle("Volume [ ]");
        fileInput3->GetXaxis()->SetAxisColor(14);
        fileInput3->GetYaxis()->SetAxisColor(14);

        fileInput3->Draw("AP");
        TF1 *retta = new TF1("f2", "pol1"); //ho definito una funzione

        TFitResultPtr r = fileInput3->Fit(retta, "S");

        TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
        TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
        cov.Print();
        cor.Print();
    }
    if (0)
    {

        gPad->SetGrid();
        gPad->SetFillColor(0);

        TGraphErrors *fileInput4 = new TGraphErrors("../Dati/Dati_comp/4_sc.txt", "%lg %lg %lg");
        fileInput4->SetMarkerColor(4);
        fileInput4->SetMarkerStyle(20);
        fileInput4->SetMarkerSize(0.5);
        fileInput4->SetTitle("quattro");
        fileInput4->GetXaxis()->SetTitle("1/ P [ ]");
        fileInput4->GetYaxis()->SetTitle("Volume [ ]");
        fileInput4->GetXaxis()->SetAxisColor(14);
        fileInput4->GetYaxis()->SetAxisColor(14);

        fileInput4->Draw("AP");
        TF1 *retta = new TF1("f2", "pol1"); //ho definito una funzione

        TFitResultPtr r = fileInput4->Fit(retta, "S");

        TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
        TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
        cov.Print();
        cor.Print();
    }
    if (1)
    {
        gPad->SetGrid();
        gPad->SetFillColor(0);

        TGraphErrors *fileInput5 = new TGraphErrors("../Dati/gaeDecomp.txt", "%lg %lg %lg");
        fileInput5->SetMarkerColor(4);
        fileInput5->SetMarkerStyle(20);
        fileInput5->SetMarkerSize(0.5);
        fileInput5->SetTitle("cinque");
        fileInput5->GetXaxis()->SetTitle("1/ P [ ]");
        fileInput5->GetYaxis()->SetTitle("Volume [ ]");
        fileInput5->GetXaxis()->SetAxisColor(14);
        fileInput5->GetYaxis()->SetAxisColor(14);

        fileInput5->Draw("AP");
        TF1 *retta = new TF1("f2", "pol1"); //ho definito una funzione

        TFitResultPtr r = fileInput5->Fit(retta, "S");

        TMatrixD cov = r->GetCorrelationMatrix(); // definisco r per avere la matrice di covarianza e correlazione,
        TMatrixD cor = r->GetCovarianceMatrix();  //OCCHIO AGLI HEADER FILES, TMATRIXD
        cov.Print();
        cor.Print();
    }
    ////********************************LEGEND OPTIONS*************************************************************
    //TLegend *legend = new TLegend(0.4, 0.6, 0.89, 0.89);
    //legend->SetHeader("966 mHz", "C"); // option "C" allows to center the header
    //legend->AddEntry(fileInput, "Misurazione #theta(t)", "p"); //P MEANS CHE È  RIFERITO A I PUNTI
    //legend->AddEntry("f2", "ax^{2}+bx+c", "l"); //ALLA LINEA
    //
    //c1->cd(1);
    //
    //legend->Draw();
}