#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "functions.h"

using namespace std;

int main()
{
    //Errori casuali forniti
    double err_press = 0.01;
    double err_vol = 0.1;
    double err_temp = 0.1;

    //Legge i dati grezzi
    auto isoterme = load_data("../Dati/mappa.txt");
    auto isoterme_confronto = load_data("../Dati/mappa_speciali.txt");

    //creo vettore contenete informazione per divisione di espansione e compressione
    vector<double> n_divisione = {784, 740, 705, 745, 760, 700};
    vector<double> n_divisione_confronto = {705, 2170, 185}; // lenta e vel hanno le stesse caratteristiche del camp 2

    auto compressione = get_compression(isoterme, n_divisione);
    auto decompressione = get_depression(isoterme, n_divisione);
    auto compressione_conf = get_compression(isoterme_confronto, n_divisione_confronto);
    auto decompressione_conf = get_depression(isoterme_confronto, n_divisione_confronto);

    for (int i = 0; i < compressione.size(); i++)
    {
        ofstream fout_compress("../Dati/Dati_comp/" + to_string(i) + ".txt");
        for (int j = 0; j < compressione[i].temp.size(); j++)
        {
            fout_compress << 1. / compressione[i].pressure[j] << "\t" << compressione[i].volume[j] << "\t" << err_vol << endl;
        }
    }

    for (int i = 0; i < decompressione.size(); i++)
    {
        ofstream fout_decomp("../Dati/Dati_decomp/" + to_string(i) + ".txt");
        for (int j = 0; j < decompressione[i].temp.size(); j++)
        {
            fout_decomp << 1. / decompressione[i].pressure[j] << "\t" << decompressione[i].volume[j] << "\t" << sqrt(pow(err_press / pow(decompressione[i].pressure[j], 2), 2)) << endl;
        }
    }

    //ottengo i centri
    auto centri = get_center(isoterme);

    return 0;
}