#include <iostream>
#include <fstream>
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

    //scarto code malvagie (sc=senza code), ovvero dove non può essere fisicamente isoterma
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
    vector<int> index_compr = {0, 299, 239, 0, 161, 83}; //dove la temperatura è massima dopo il calo a parabola, problema sono nella decompressione
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

    //TEST calcolo di moli da tutti i dati tramite terne, sono volume di siringa
    auto moli_tot = get_moli(compressione);

    for (auto d : moli_tot)
    {
        cout << d.n_moli << "+/-" << d.err_n_moli << endl;
    }

    //Calcola le info di interpolazione grafici 1/p e v
    auto interpolaz_compress = get_interpolazioni(compressione_sc);
    auto interpolaz_decompress = get_interpolazioni(decompressione_sc);
    auto interpolazioni_compress_conf = get_interpolazioni(compressione_conf_sc);
    auto interpolazioni_decompress_conf = get_interpolazioni(decompressione_conf_sc);

    //Associa la giusta temperatura calcolata dal giusto numero di punti
    auto joined_compress = join_info(interpolaz_compress, temperature_compress);
    auto joined_decompress = join_info(interpolaz_decompress, temperature_decompress);
    auto joined_compress_conf = join_info(interpolazioni_compress_conf, temperature_compress_part);
    auto joined_decompress_conf = join_info(interpolazioni_decompress_conf, temperature_decompress_part);

    //Stampa tabella compressione e decompressione per tutte le stime di fit
    cout << "Tabella compressioni" << endl;
    cout << "inedx\ttemp\taintercetta\tbangolare\tmoli\tsigmapost" << endl;
    for (int i = 0; i < joined_compress.size(); i++)
    {
        auto &temp = joined_compress[i];
        cout << i << "\t" << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.a_intercetta << "+/-" << temp.err_a_intercetta << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << "\t" << temp.n_moli << "+/-" << temp.err_n_moli << "\t" << temp.sigma_y_post << endl;
    }

    cout << "Tabella decompressioni" << endl;
    cout << "inedx\ttemp\taintercetta\tbangolare\tmoli\tsigmapost" << endl;
    for (int i = 0; i < joined_decompress.size(); i++)
    {
        auto &temp = joined_decompress[i];
        cout << i << "\t" << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.a_intercetta << "+/-" << temp.err_a_intercetta << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << "\t" << temp.n_moli << "+/-" << temp.err_n_moli << "\t" << temp.sigma_y_post << endl;
    }

    //Salva le info per fare grafici, temperatura e coeff angolare ricavato precedentemente
    ofstream moli_d("../Dati/Dati_moli/moli_decompress.txt");
    for (int i = 0; i < joined_decompress.size(); i++)
    {
        auto &temp = joined_decompress[i];
        moli_d << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << endl;
    }
    ofstream moli_c("../Dati/Dati_moli/moli_compress.txt");
    for (int i = 0; i < joined_compress.size(); i++)
    {
        auto &temp = joined_compress[i];
        moli_c << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << endl;
    }

    //stampa di campioni lento e veloce, per verificare che siano la stessa cosa, più o meno
    //NON SERVE A INTERPOLAZIONE
    ofstream moli_c_conf("../Dati/Dati_moli/moli_compress_conf.txt");
    for (int i = 0; i < joined_compress_conf.size(); i++)
    {
        auto &temp = joined_compress_conf[i];
        moli_c_conf << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << endl;
    }
    ofstream moli_d_conf("../Dati/Dati_moli/moli_decompress_conf.txt");
    for (int i = 0; i < joined_decompress_conf.size(); i++)
    {
        auto &temp = joined_decompress_conf[i];
        moli_d_conf << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << endl;
    }
    // fine stampa
    cout << ttest_campioni( joined_decompress,joined_compress) << endl;

    auto moli_interpolatez_compress = interpolazione_moli(joined_compress);
    auto moli_interpolatez_decompress = interpolazione_moli(joined_decompress);
    cout << "Da interpolazione generale" << endl
         << moli_interpolatez_compress.n_moli << "+/-" << moli_interpolatez_compress.err_n_moli << endl
         << moli_interpolatez_decompress.n_moli << "+/-" << moli_interpolatez_decompress.err_n_moli << endl;
    auto comp_moli = comp(moli_interpolatez_compress.n_moli, moli_interpolatez_decompress.n_moli, moli_interpolatez_compress.err_n_moli, moli_interpolatez_decompress.err_n_moli);
    cout << "Compatibilità fra moli compress e decompress" << comp_moli << endl;
    return 0;
}