#include <vector>
#include <fstream>
#include <iostream>
#include "struct.h" //dove sono definite tutte le strutture per i ritorni delle funzioni
#include "statistica.h"

using namespace std;

double r_gas = 8.3136;
double g = 9.806;

//legge i file da Dati/Dati_Grezzi secondo quanto detto da mappa.txt
vector<raw_data> load_data(string mappa)
{
    vector<raw_data> loaded_data;
    vector<double> n_campione;
    vector<string> nome_campione;
    vector<string> nome_file;
    ifstream fin_mappa(mappa);
    double num;
    string campione, file;
    while (fin_mappa >> num >> campione >> file) //archivia da file mappa
    {
        n_campione.push_back(num);
        nome_campione.push_back(campione);
        nome_file.push_back(file);
    }
    for (int i = 0; i < nome_file.size(); i++)
    {
        raw_data temp_data;
        ifstream fin_raw_data("../Dati/Dati_Grezzi/" + nome_file[i]);
        double p, t, v;
        temp_data.n_camp = n_campione[i];
        while (fin_raw_data >> p >> v >> t)
        {

            temp_data.pressure.push_back(p);
            temp_data.volume.push_back(v);
            temp_data.temp.push_back(t + 273.15); //convertita in Kelvin
        }
        loaded_data.push_back(temp_data);
    }
    return loaded_data;
};

//Ritorna struttura di triplette in compressione.
vector<raw_data> get_compression(vector<raw_data> &raw, vector<double> &indexes)
{
    vector<raw_data> compress;
    for (int i = 0; i < raw.size(); i++)
    {
        raw_data temp_compression;
        for (int j = 0; j < indexes[i]; j++)
        {
            temp_compression.temp.push_back(raw[i].temp[j]);
            temp_compression.volume.push_back(raw[i].volume[j]);
            temp_compression.pressure.push_back(raw[i].pressure[j]);
        }
        temp_compression.n_camp = raw[i].n_camp;
        compress.push_back(temp_compression);
    }
    return compress;
}

// ritorna struttura di dati in decompressione
vector<raw_data> get_depression(vector<raw_data> &raw, vector<double> &indexes)
{
    vector<raw_data> depress;
    for (int i = 0; i < raw.size(); i++)
    {
        raw_data temp_depression;
        for (int j = indexes[i]; j < raw[i].temp.size(); j++)
        {
            temp_depression.temp.push_back(raw[i].temp[j]);
            temp_depression.volume.push_back(raw[i].volume[j]);
            temp_depression.pressure.push_back(raw[i].pressure[j]);
        }
        temp_depression.n_camp = raw[i].n_camp;
        depress.push_back(temp_depression);
    }
    return depress;
}

//Scarta entrambe le code con condizione su pressione e volume
vector<raw_data> scarta_code(vector<raw_data> &dati_grezzi)
{
    vector<raw_data> temp_vec;
    for (auto d : dati_grezzi)
    {
        raw_data temp_new_data;
        for (int i = 0; i < d.pressure.size(); i++)
        {
            if ((d.volume[i] <= 22.6) && (d.pressure[i] < 3.921)) //le condizioni che abbiamo valutato osservando i grafici
            {
                temp_new_data.pressure.push_back(d.pressure[i]);
                temp_new_data.volume.push_back(d.volume[i]);
                temp_new_data.temp.push_back(d.temp[i]);
            }
        }
        temp_new_data.n_camp = d.n_camp;
        temp_vec.push_back(temp_new_data);
    }
    return temp_vec;
}

vector<int> get_center(vector<raw_data> &raw)
{
    vector<int> centri;
    for (int i = 0; i < raw.size(); i++)
    {
        centri.push_back(raw[i].temp.size() / 2);
    }
    return centri;
}

vector<int> ricerca_massimo(vector<raw_data> &raw, vector<int> indexes)
{
    vector<int> massimi;
    for (int i = 0; i < raw.size(); i++)
    {
        int min = raw[i].temp.size() - 1;
        int max = indexes[i];
        for (int k = indexes[i]; k < raw[i].temp.size(); k++)
        {
            if (raw[i].temp[indexes[i]] < raw[i].temp[raw[i].temp.size() - 1])
            {
                if (raw[i].temp[k] < raw[i].temp[min])
                {
                    min = k;
                }
            }
            else
            {
                if (raw[i].temp[k] > raw[i].temp[min])
                {
                    max = k;
                }
            }
        }
        if (raw[i].temp[indexes[i]] < raw[i].temp[raw[i].temp.size() - 1])
        {
            massimi.push_back(min);
        }
        else
        {
            massimi.push_back(max);
        }
    }
    return massimi;
}

//Get temperatura media, attenzione a cosa si mette come dati grezzi
vector<info> get_temperatura(vector<raw_data> &dati, vector<int> &indexes)
{
    vector<info> vec_info;
    for (int i = 0; i < dati.size(); i++)
    {
        info temp_info;
        temp_info.temp_media = media(dati[i].temp, indexes[i], dati[i].temp.size());
        temp_info.err_temp_media = dstd_media(dati[i].temp, indexes[i], dati[i].temp.size());
        vec_info.push_back(temp_info);
    }
    return vec_info;
}

//Stima del numero di moli con tutte le terne fornite in dati grezzi. Non importa della correlazione fra i valori
vector<info> get_moli(vector<raw_data> &dati_grezzi)
{
    vector<info> temp_info;
    for (auto d : dati_grezzi)
    {
        info temp_campione;
        vector<double> temp_n;
        for (int i = 0; i < d.pressure.size(); i++)
        {
            temp_n.push_back(d.pressure[i] * d.volume[i] * g * 0.1 / (d.temp[i] * r_gas)); //convertite tutte in moli come dio vuole con le unità del SI
        }
        temp_campione.n_moli = media(temp_n);
        temp_campione.err_n_moli = dstd_media(temp_n);
        temp_campione.n_camp = d.n_camp;
        temp_info.push_back(temp_campione);
    }
    return temp_info;
}