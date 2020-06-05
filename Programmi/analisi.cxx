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

    //scarto code malvagie (sc=senza code)
    auto compressione_sc = scarta_code(compressione);
    auto decompressione_sc = scarta_code(decompressione);
    auto compressione_conf_sc = scarta_code(compressione_conf);
    auto decompressione_conf_sc = scarta_code(decompressione_conf);

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
            fout_decomp << 1. / decompressione[i].pressure[j] << "\t" << decompressione[i].volume[j] << "\t" << err_vol << endl;
        }
    }

    for (int i = 0; i < compressione_sc.size(); i++)
    {
        ofstream fout_compress_sc("../Dati/Dati_comp/" + to_string(i) + "_sc.txt");
        for (int j = 0; j < compressione_sc[i].temp.size(); j++)
        {
            fout_compress_sc << 1. / compressione_sc[i].pressure[j] << "\t" << compressione_sc[i].volume[j] << "\t" << err_vol << "\t" << compressione_sc[i].temp[j] << endl;
        }
    }

    for (int i = 0; i < decompressione_sc.size(); i++)
    {
        ofstream fout_decomp_sc("../Dati/Dati_decomp/" + to_string(i) + "_sc.txt");
        for (int j = 0; j < decompressione_sc[i].temp.size(); j++)
        {
            fout_decomp_sc << 1. / decompressione_sc[i].pressure[j] << "\t" << decompressione_sc[i].volume[j] << "\t" << err_vol << "\t" << decompressione_sc[i].temp[j] << endl;
        }
    }

    //Definisco range indice iniziale per prendere le temperature, separatamente per compress e decompress
    vector<int> index_compr = {0, 299, 239, 0, 161, 83};
    vector<int> index_depress = {0, 0, 0, 0, 0, 0};
    vector<int> index_compr_part = {239, 0, 0};
    vector<int> index_decompr_part = {0, 0, 0};

    auto temperature_compress = get_temperatura(compressione, index_compr);
    auto temperature_decompress = get_temperatura(decompressione, index_depress);
    auto temperature_compress_part = get_temperatura(compressione_conf, index_compr_part);
    auto temperature_decompress_part = get_temperatura(decompressione_conf, index_decompr_part);

    for (int i = 0; i < temperature_compress.size(); i++)
    {
        cout << temperature_compress[i].temp_media << "+/-" << temperature_compress[i].err_temp_media << "\t" << temperature_decompress[i].temp_media << "+/-" << temperature_decompress[i].err_temp_media << endl;
    }

    //TEST calcolo di moli da tutti i dati
    auto moli_tot = get_moli(compressione);

    for (auto d : moli_tot)
    {
        cout << d.n_moli <<"+/-"<<d.err_n_moli<< endl;
    }
    return 0;
}