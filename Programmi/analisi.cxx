#include <iostream>
#include <fstream>
#include <cmath>
#include <unistd.h>
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

    //per lenra e veloce
    for (int i = 0; i < compressione_conf_sc.size(); i++)
    {
        ofstream fout_compress_conf_sc("../Dati/Dati_comp/" + to_string(i) + "conf_sc.txt");
        for (int j = 0; j < compressione_conf_sc[i].temp.size(); j++)
        {
            fout_compress_conf_sc << 1. / compressione_conf_sc[i].pressure[j] << "\t" << compressione_conf_sc[i].volume[j] << "\t" << err_vol << "\t" << compressione_conf_sc[i].temp[j] << endl;
        }
    }

    for (int i = 0; i < decompressione_conf_sc.size(); i++)
    {
        ofstream fout_decomp_conf_sc("../Dati/Dati_decomp/" + to_string(i) + "conf_sc.txt");
        for (int j = 0; j < decompressione_conf_sc[i].temp.size(); j++)
        {
            fout_decomp_conf_sc << 1. / decompressione_conf_sc[i].pressure[j] << "\t" << decompressione_conf_sc[i].volume[j] << "\t" << err_vol << "\t" << decompressione_conf_sc[i].temp[j] << endl;
        }
    }

    //Definisco range indice iniziale per prendere le temperature, separatamente per compress e decompress
    vector<int> index_compr = {0, 299, 239, 0, 161, 83}; //dove la temperatura è massima dopo il calo a parabola, problema sono nella decompressione
    vector<int> index_depress = {0, 0, 0, 0, 0, 0};
    vector<int> index_compr_part = {239, 0, 0};
    vector<int> index_decompr_part = {0, 0, 0};

    auto temperature_compress = get_temperatura(isoterme, index_compr);                     //DÓRA IN AVANTI SOLO UNA TEMEPRATURA, PRIMA ERA compressione, index_compr
    auto temperature_decompress = get_temperatura(isoterme, index_compr);                   //
    auto temperature_compress_part = get_temperatura(isoterme_confronto, index_compr_part); //anche qui solo una temperatura
    auto temperature_decompress_part = get_temperatura(isoterme_confronto, index_compr_part);

    for (int i = 0; i < temperature_compress.size(); i++)
    {
        cout << temperature_compress[i].temp_media << "+/-" << temperature_compress[i].err_temp_media << "\t" << temperature_decompress[i].temp_media << "+/-" << temperature_decompress[i].err_temp_media << "\t" << comp(temperature_compress[i].temp_media, temperature_decompress[i].temp_media, temperature_compress[i].err_temp_media, temperature_decompress[i].err_temp_media) << endl;
    }
    for (int i = 0; i < temperature_compress_part.size(); i++)
    {
        cout << temperature_compress_part[i].temp_media << "+/-" << temperature_compress_part[i].err_temp_media << "\t" << temperature_decompress_part[i].temp_media << "+/-" << temperature_decompress_part[i].err_temp_media << "\t" << comp(temperature_compress_part[i].temp_media, temperature_decompress_part[i].temp_media, temperature_compress_part[i].err_temp_media, temperature_decompress_part[i].err_temp_media) << endl;
    }

    //TEST calcolo di moli da tutti i dati tramite terne, sono volume di siringa
    auto moli_tot = get_moli(compressione);

    for (auto d : moli_tot)
    {
        cout << d.n_moli << "+/-" << endl; // d.err_n_moli << endl;
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

    //LENTA E VELOCE, STESSA COSA DI SOPRA
    cout << "Tabella compressioni confronto" << endl;
    cout << "inedx\ttemp\taintercetta\tbangolare\tmoli\tsigmapost" << endl;
    for (int i = 0; i < joined_compress_conf.size(); i++)
    {
        auto &temp = joined_compress_conf[i];
        cout << i << "\t" << temp.temp_media << "+/-" << temp.err_temp_media << "\t" << temp.a_intercetta << "+/-" << temp.err_a_intercetta << "\t" << temp.b_ang << "+/-" << temp.err_b_ang << "\t" << temp.n_moli << "+/-" << temp.err_n_moli << "\t" << temp.sigma_y_post << endl;
    }

    cout << "Tabella decompressioni confronto" << endl;
    cout << "inedx\ttemp\taintercetta\tbangolare\tmoli\tsigmapost" << endl;
    for (int i = 0; i < joined_decompress_conf.size(); i++)
    {
        auto &temp = joined_decompress_conf[i];
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

    //Per la stima dello zero assoluto
    auto moli_interpolatez_compress = interpolazione_moli(joined_compress);
    auto moli_interpolatez_decompress = interpolazione_moli(joined_decompress);
    //auto moli_interpolatez_decomp_confr= interpolazione_moli(joined_decompress_conf);
    //auto moli_interpolatez_comp_confr= interpolazione_moli(joined_compress_conf);
    cout << "Da interpolazione generale ovvero zero assoluto" << endl
         << moli_interpolatez_compress.n_moli << "+/-" << moli_interpolatez_compress.err_n_moli << endl
         << moli_interpolatez_decompress.n_moli << "+/-" << moli_interpolatez_decompress.err_n_moli << endl;
    auto comp_moli = comp(moli_interpolatez_compress.n_moli, moli_interpolatez_decompress.n_moli, moli_interpolatez_compress.err_n_moli, moli_interpolatez_decompress.err_n_moli);
    cout << "Compatibilità fra moli compress e decompress" << comp_moli << endl;

    cout << endl;
    cout << "T-Student" << endl;
    cout << ttest_campioni(joined_decompress, joined_compress) << endl;

    cout << endl;
    cout << "T_0 compressione" << endl;

    double propagazionet0c;
    double unoc = (1. / moli_interpolatez_compress.b_ang);
    double duec = (moli_interpolatez_compress.a_intercetta / pow(moli_interpolatez_compress.b_ang, 2));
    double covc = -(304.200 - 273.15) * pow(moli_interpolatez_compress.err_b_ang, 2); //QUA CI STAVA IL MENO, MENOMALE CHE NON MI FIDO MAI DI TOM <3
    propagazionet0c = sqrt(pow(unoc, 2) * pow(moli_interpolatez_compress.err_a_intercetta, 2) + pow(duec, 2) * pow(moli_interpolatez_compress.err_b_ang, 2) + (2 * unoc * duec * covc));
    cout << moli_interpolatez_compress.a_intercetta / moli_interpolatez_compress.b_ang << "\t" << propagazionet0c << endl;
    cout << endl;

    cout << "Stima moli da grafici T_0--------------------------------" << endl;
    cout << moli_interpolatez_compress.b_ang / r_gas_convertita << "+/-" << (1. / (r_gas * 100)) * sqrt(pow(moli_interpolatez_compress.b_ang * moli_interpolatez_compress.err_b_ang, 2) + pow(g * 0.001, 2)) << endl;

    cout << "Sigma moli posteriori" << endl;
    cout << moli_interpolatez_compress.sigma_b_post / r_gas_convertita << endl;

    cout << endl;
    sleep(50); //non so perchè non funziona :(
    cout << "T_0 decompressione" << endl;
    double propagazionet0d;
    double unod = (1. / moli_interpolatez_decompress.b_ang);
    double dued = (moli_interpolatez_decompress.a_intercetta / pow(moli_interpolatez_decompress.b_ang, 2));
    double covd = -(303.109 - 273.15) * pow(moli_interpolatez_decompress.err_b_ang, 2);
    propagazionet0d = sqrt(pow(unod, 2) * pow(moli_interpolatez_decompress.err_a_intercetta, 2) + pow(dued, 2) * pow(moli_interpolatez_decompress.err_b_ang, 2) + (2 * unod * dued * covd));
    cout << moli_interpolatez_decompress.a_intercetta / moli_interpolatez_decompress.b_ang << "\t" << propagazionet0d << endl;
    cout << endl;

    cout << "Stima moli da grafici T_0-------------------------------" << endl;
    cout << moli_interpolatez_decompress.b_ang / r_gas_convertita << "+/-" << (1. / (r_gas * 100)) * sqrt(pow(moli_interpolatez_decompress.b_ang * moli_interpolatez_decompress.err_b_ang, 2) + pow(g * 0.001, 2)) << endl;

    cout << "Sigma moli posteriori" << endl;
    cout << moli_interpolatez_decompress.sigma_b_post / r_gas_convertita << endl;

    /*
Valuta compatibilità e quasi stticità compress e decompress
 */
    cout << "Calcolo compatibilità tra coeff angolari comp e decomp" << endl;
    auto quasi_staticita = quasi_statiche(interpolaz_compress, interpolaz_decompress);
    for (int i = 0; i < quasi_staticita.size(); i++)
    {
        cout << i << "\t";
    }
    cout << endl;
    for (int i = 0; i < quasi_staticita.size(); i++)
    {
        cout << quasi_staticita[i] << "\t";
    }
    cout << endl;

    cout << "Calcolo quasi compatibilità coeff angolari comp e decomp confronto" << endl;
    auto qs_conf_v_l = quasi_statiche(interpolazioni_compress_conf, interpolazioni_decompress_conf);
    for (int i = 0; i < qs_conf_v_l.size(); i++)
    {
        cout << i << "\t" << qs_conf_v_l[i] << endl;
    }
    cout << endl;
    /*Analisi dati isocore
    ----------------------------------------------------------------------------------------------
    */

    auto isocora8 = get_isocora("../Dati/isocora.txt");
    auto isocora21 = get_isocora("../Dati/isocora21.txt");
    double s_g = 0.001; //errore su gravità

    double v_1 = 8.63;
    double s_v_1 = 0.1;
    double v_2 = 21.01;
    double s_v_2 = 0.1;
    double s_r_conv = r_gas * 100 / pow(g, 2) * s_g; //errore si r gas convertita
    double b_1 = b_angolare(isocora8.temp, isocora8.pressure, isocora8.err_pressione);
    double s_b_1 = sigma_b(isocora8.temp, isocora8.pressure, isocora8.err_pressione);
    double b_2 = b_angolare(isocora21.temp, isocora21.pressure, isocora21.err_pressione);
    double s_b_2 = sigma_b(isocora21.temp, isocora21.pressure, isocora21.err_pressione);
    double a_1 = a_intercetta(isocora8.temp, isocora8.pressure, isocora8.err_pressione);
    double s_a_1 = sigma_a(isocora8.temp, isocora8.pressure, isocora8.err_pressione);
    double a_2 = a_intercetta(isocora21.temp, isocora21.pressure, isocora21.err_pressione);
    double s_a_2 = sigma_a(isocora21.temp, isocora21.pressure, isocora21.err_pressione);

    double prop_1 = sqrt(pow((v_1 * s_b_1 / r_gas_convertita), 2) + pow((b_1 * s_v_1 / r_gas_convertita), 2) + pow((b_1 * v_1 * s_r_conv / pow(r_gas_convertita, 2)), 2));
    double prop_2 = sqrt(pow((v_2 * s_b_2 / r_gas_convertita), 2) + pow((b_2 * s_v_2 / r_gas_convertita), 2) + pow((b_2 * v_2 * s_r_conv / pow(r_gas_convertita, 2)), 2));

    cout << "Valori fit isocore" << endl;
    cout << "B errB\tA errA\tchi\tp\ttstud" << endl;
    cout << b_angolare(isocora8.temp, isocora8.pressure, isocora8.err_pressione) << "\t" << sigma_b(isocora8.temp, isocora8.pressure, isocora8.err_pressione) << "\t" << a_intercetta(isocora8.temp, isocora8.pressure, isocora8.err_pressione) << "\t" << sigma_a(isocora8.temp, isocora8.pressure, isocora8.err_pressione) << "\t" << test_chi(isocora8.temp, isocora8.pressure, isocora8.err_pressione) << "\t" << pearson(isocora8.temp, isocora8.pressure) << "\t" << student(isocora8.temp, isocora8.pressure) << endl;
    cout << b_angolare(isocora21.temp, isocora21.pressure, isocora21.err_pressione) << "\t" << sigma_b(isocora21.temp, isocora21.pressure, isocora21.err_pressione) << "\t" << a_intercetta(isocora21.temp, isocora21.pressure, isocora21.err_pressione) << "\t" << sigma_a(isocora21.temp, isocora21.pressure, isocora21.err_pressione) << "\t" << test_chi(isocora21.temp, isocora21.pressure, isocora21.err_pressione) << "\t" << pearson(isocora21.temp, isocora21.pressure) << "\t" << student(isocora21.temp, isocora21.pressure) << endl;

    cout << b_angolare(isocora8.temp, isocora8.pressure, isocora8.err_pressione) * 8.63 / r_gas_convertita << "+/-" << prop_1 << endl;
    cout << b_angolare(isocora8.temp, isocora8.pressure, isocora8.err_pressione) * 21.01 / r_gas_convertita << "+/-" << prop_2 << endl;
    return 0;
}