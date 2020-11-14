#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "fstream"
#include "TMinuit.h"
#include "TFitResult.h"
#include "TMatrixD.h"
using namespace std;

double func(double *x, double *par)
{

    double a = par[0];
    double b = par[1];

    double ret = b * x[0] + a;
    return ret;
}

void root_zero_assoluto()
{

    if (0)
    {
        TVirtualFitter::SetDefaultFitter("Minuit");

        auto c1 = new TCanvas("c1", "Grafico Interpolazioni per T_0", 800, 500);
        //c1->Divide(2, 1, 0.0002, 0.0002);

        //c1->cd(1);
        gPad->SetGrid();
        gPad->SetFillColor(0);
        TGraphErrors *fileInput0 = new TGraphErrors("../Dati/tabella_decompressioni.txt", "%*lg %lg %*lg %*lg %*lg %lg %lg");

        fileInput0->SetMarkerColor(4);
        fileInput0->SetMarkerStyle(20);
        fileInput0->SetMarkerSize(0.5);
        fileInput0->SetTitle("Decompressioni");
        fileInput0->GetXaxis()->SetTitle("Temperatura [K]");
        fileInput0->GetYaxis()->SetTitle("B [Kg * cm]");
        fileInput0->GetXaxis()->SetAxisColor(14);
        fileInput0->GetYaxis()->SetAxisColor(14);

        //fileInput0->Draw("AP");
        TF1 *retta = new TF1("f2", func, 0, 330, 2);
        retta->SetLineColor(kRed);
        retta->SetParameter(0, 1);
        retta->SetParameter(1, 1);

        TFitResultPtr r = fileInput0->Fit(retta, "S");

        ofstream fout("../Dati/Dati_moli/fit_result_decompressioni.txt");
        fout << "a_intercetta= " << retta->GetParameter(0) << "+/-" << retta->GetParError(0) << endl;
        fout << "b_angolare= " << retta->GetParameter(1) << "+/-" << retta->GetParError(1) << endl;
        fout << "chi2=" << retta->GetChisquare() << endl;

        ////********************************LEGEND OPTIONS*************************************************************
        TLegend *legend = new TLegend(0.4, 0.6, 0.89, 0.89);
        legend->SetHeader("Decompressione", "C");            // option "C" allows to center the header
        legend->AddEntry(fileInput0, "Misurazione", "lp"); //P MEANS CHE È  RIFERITO A I PUNTI
        legend->AddEntry(retta, "bx+a", "l");              //ALLA LINEA
       // legend->Draw();

        //c1->cd(2);
        gPad->SetGrid();
        gPad->SetFillColor(0);

        gMinuit->SetErrorDef(9);
        TGraph *gr0 = (TGraph *)gMinuit->Contour(80, 0, 1);
        gr0->SetFillColor(kRed);


        gr0->GetXaxis()->SetTitle("Parametro a [ Kg * cm]");
        gr0->GetYaxis()->SetTitle("Parametro b [ Kg * cm / K]");
        gr0->GetXaxis()->SetAxisColor(14);
        gr0->GetYaxis()->SetAxisColor(14);

        gr0->Draw("ALF");

        gMinuit->SetErrorDef(4);
        TGraph *graph2 = (TGraph *)gMinuit->Contour(80, 0, 1);
        graph2->SetFillColor(kBlue-2);
        graph2->Draw("lf, same");

        gMinuit->SetErrorDef(1);
        TGraph *graph = (TGraph *)gMinuit->Contour(80, 0, 1);
        graph->SetMarkerColor(kBlue-7);
        graph->SetFillColor(kBlue-7);
        graph->Draw("lf,same");

        ////********************************LEGEND OPTIONS*************************************************************
        TLegend *legend1 = new TLegend(0.8, 0.7, 0.89, 0.89);
        //legend1->SetHeader("Likelihood", "C"); // option "C" allows to center the header
        legend1->AddEntry(gr0, "3#sigma", "f");
        legend1->AddEntry(graph2, "2#sigma", "f");
        legend1->AddEntry(graph, "#sigma", "f");

        legend1->Draw();

        TMatrixD cor1 = r->GetCorrelationMatrix();
        TMatrixD cov1 = r->GetCovarianceMatrix();
        cor1.Print();
        cov1.Print();
    }
    if (1)
    {
        TVirtualFitter::SetDefaultFitter("Minuit");

        auto c1 = new TCanvas("c1", "Grafico Interpolazioni per T_0", 1200, 800);
        //c1->Divide(2, 1, 0.0002, 0.0002);

        //c1->cd(1);
        gPad->SetGrid();
        gPad->SetFillColor(0);
        TGraphErrors *fileInput0 = new TGraphErrors("../Dati/tabella_compressioni.txt", "%*lg %lg %*lg %*lg %*lg %lg %lg");

        fileInput0->SetMarkerColor(4);
        fileInput0->SetMarkerStyle(20);
        fileInput0->SetMarkerSize(0.5);
        fileInput0->SetTitle("Compressioni");
        fileInput0->GetXaxis()->SetTitle("Temperatura [K]");
        fileInput0->GetYaxis()->SetTitle("B [Kg * cm]");
        fileInput0->GetXaxis()->SetAxisColor(14);
        fileInput0->GetYaxis()->SetAxisColor(14);

        fileInput0->Draw("AP");
        TF1 *retta = new TF1("f2", func, 0, 330, 2); //ho definito una funzione
        retta->SetParameter(0, 1);
        retta->SetParameter(1, 1);

        TFitResultPtr r = fileInput0->Fit(retta, "S");
        ofstream fout("../Dati/Dati_moli/fit_result_compressioni.txt");
        fout << "a_intercetta= " << retta->GetParameter(0) << "+/-" << retta->GetParError(0) << endl;
        fout << "b_angolare= " << retta->GetParameter(1) << "+/-" << retta->GetParError(1) << endl;
        fout << "chi2=" << retta->GetChisquare() << endl;

        ////********************************LEGEND OPTIONS*************************************************************
        TLegend *legend = new TLegend(0.4, 0.6, 0.89, 0.89);
        legend->SetHeader("Compressione", "C");            // option "C" allows to center the header
        legend->AddEntry(fileInput0, "Misurazione", "lp"); //P MEANS CHE È  RIFERITO A I PUNTI
        legend->AddEntry(retta, "bx+a", "l");              //ALLA LINEA
        //legend->Draw();

        //c1->cd(2);
        gPad->SetGrid();
        gPad->SetFillColor(0);

        gMinuit->SetErrorDef(9);
        TGraph *gr0 = (TGraph *)gMinuit->Contour(80, 0, 1);
        gr0->SetFillColor(kRed);

        gr0->SetTitle("Paraboloide rapporto Likelihood #ell");
        gr0->GetXaxis()->SetTitle("Parametro a [ Kg * cm]");
        gr0->GetYaxis()->SetTitle("Parametro b [ Kg * cm / K]");
        gr0->GetXaxis()->SetAxisColor(14);
        gr0->GetYaxis()->SetAxisColor(14);

        gr0->Draw("ALF");

        gMinuit->SetErrorDef(4);
        TGraph *graph2 = (TGraph *)gMinuit->Contour(80, 0, 1);
        graph2->SetFillColor(kBlue-2);
        graph2->Draw("lf, same");

        gMinuit->SetErrorDef(1);
        TGraph *graph0 = (TGraph *)gMinuit->Contour(80, 0, 1);
        graph0->SetMarkerColor(kBlue-7);
        graph0->SetFillColor(kBlue-7);
        graph0->Draw("lf,same");

        ////********************************LEGEND OPTIONS*************************************************************
        TLegend *legend1 = new TLegend(0.8, 0.7, 0.89, 0.89);
        //legend1->SetHeader("Likelihood", "C"); // option "C" allows to center the header
        legend1->AddEntry(gr0, "3#sigma", "f");
        legend1->AddEntry(graph2, "2#sigma", "f");
        legend1->AddEntry(graph0, "#sigma", "f");

        legend1->Draw();

        TMatrixD cor1 = r->GetCorrelationMatrix();
        TMatrixD cov1 = r->GetCovarianceMatrix();
        cor1.Print();
        cov1.Print();
    }
}
