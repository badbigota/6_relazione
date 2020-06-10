#include <fstream>
#include <iostream>
#include "struct.h" //dove sono definite tutte le strutture per i ritorni delle funzioni
#include "statistica.h"

using namespace std;

double r_gas = 8.314462618;
double r_gas_convertita = 84.754 /*84.78890475219254*/;
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
}

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
            temp_compression.err_volume.push_back(0.1);
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
            temp_depression.err_volume.push_back(0.1);
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
                temp_new_data.err_volume.push_back(0.1);
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
            temp_n.push_back(d.pressure[i] * d.volume[i] * g * 0.01 / (d.temp[i] * r_gas)); //convertite tutte in moli come dio vuole con le unità del SI
        }
        temp_campione.n_moli = media(temp_n);
        temp_campione.err_n_moli = dstd_media(temp_n);
        temp_campione.n_camp = d.n_camp;
        temp_info.push_back(temp_campione);
    }
    return temp_info;
}

//Per restituire il giusto reciproco
vector<double> reciproco(vector<double> &dati_pressione)
{
    vector<double> temp_vec;
    for (auto d : dati_pressione)
    {
        temp_vec.push_back(1 / d);
    }
    return temp_vec;
}

//Effettua interpolazione su 1/p in ascissa e v in ordinata
vector<info> get_interpolazioni(vector<raw_data> &dati)
{
    vector<info> infos;
    for (int i = 0; i < dati.size(); i++)
    {
        info temp_info;
        temp_info.b_ang = b_angolare_err_uguali(reciproco(dati[i].pressure), dati[i].volume);
        temp_info.err_b_ang = sigma_b(reciproco(dati[i].pressure), dati[i].volume, dati[i].err_volume);
        temp_info.a_intercetta = a_intercetta_err_uguali(reciproco(dati[i].pressure), dati[i].volume);
        temp_info.err_a_intercetta = sigma_a(reciproco(dati[i].pressure), dati[i].volume, dati[i].err_volume);
        temp_info.sigma_y_post = sigma_y_posteriori(reciproco(dati[i].pressure), dati[i].volume);

        infos.push_back(temp_info);
    }
    return infos;
}

//Unisce i dati di temperature (fatte solo sui dati dai massimi) e delle interpolazioni (senza le codine)
vector<info> join_info(vector<info> &interpolazioni, vector<info> &temperature)
{
    vector<info> temp_info;
    for (int i = 0; i < interpolazioni.size(); i++)
    {
        info temp_joining;
        temp_joining.temp_media = temperature[i].temp_media;
        temp_joining.err_temp_media = temperature[i].err_temp_media;
        temp_joining.a_intercetta = interpolazioni[i].a_intercetta;
        temp_joining.err_a_intercetta = interpolazioni[i].err_a_intercetta;
        temp_joining.b_ang = interpolazioni[i].b_ang;
        temp_joining.err_b_ang = interpolazioni[i].err_b_ang;
        temp_joining.n_moli = interpolazioni[i].b_ang / (r_gas_convertita * temperature[i].temp_media);
        temp_joining.err_n_moli = sqrt(pow(1. / (r_gas_convertita * temperature[i].temp_media), 2) * pow(temp_joining.err_b_ang, 2) + pow((-1. / pow(temperature[i].temp_media, 2)) * (temp_joining.b_ang / r_gas_convertita), 2) * pow(temperature[i].err_temp_media, 2));
        //temp_joining.err_n_moli = sqrt(pow(1. / (r_gas_convertita * temperature[i].temp_media), 2) * pow(temp_joining.err_b_ang, 2) + pow((-1. / pow(temperature[i].temp_media, 2)) * (temp_joining.b_ang / r_gas_convertita), 2) * pow(0.1, 2));
        temp_joining.sigma_y_post = interpolazioni[i].sigma_y_post;

        temp_info.push_back(temp_joining);
    }
    return temp_info;
}

info interpolazione_moli(vector<info> &joined)
{
    info temp_infoz;
    vector<double> x;
    vector<double> y, err_y;
    for (auto d : joined)
    {
        x.push_back(d.temp_media);
        y.push_back(d.b_ang);
        err_y.push_back(d.err_b_ang);
    }
    temp_infoz.b_ang = b_angolare(x, y, err_y);
    temp_infoz.err_b_ang = sigma_b(x, y, err_y);
    temp_infoz.a_intercetta = a_intercetta(x, y, err_y);
    temp_infoz.err_a_intercetta = sigma_a(x, y, err_y);
    temp_infoz.n_moli = b_angolare(x, y, err_y) * g * 0.01 / r_gas;
    temp_infoz.err_n_moli = sigma_b(x, y, err_y) * g * 0.01 / r_gas;
    return temp_infoz;
}

double ttest_campioni(vector<info> &camp1, vector<info> &camp2)
{
    vector<double> x;
    vector<double> y;
    for (auto d : camp1)
    {
        x.push_back(d.n_moli);
    }
    for (auto c : camp2)
    {
        y.push_back(c.n_moli);
    }
    return tstudent_campioni(x, y);
}

//stampare i dati delle interpolazioni