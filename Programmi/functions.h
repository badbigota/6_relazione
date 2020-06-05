#include <vector>
#include <fstream>
#include <iostream>
#include "struct.h" //dove sono definite tutte le strutture per i ritorni delle funzioni

using namespace std;

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
        depress.push_back(temp_depression);
    }
    return depress;
}

vector<int> get_center(vector<raw_data> &raw)
{
    vector<int> centri;
    for(int i = 0; i < raw.size(); i++)
    {
        centri.push_back(raw[i].temp.size()/2);
    }
    return centri;
}