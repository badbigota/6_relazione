#include <vector>
using namespace std;

//Contiene i dati grezzi
struct raw_data
{
    double n_camp;
    vector<double> temp;
    vector<double> pressure;
    vector<double> volume;
    vector<double> err_volume;
};

//Strutura per le informazioni generali per campione
struct info
{
    double n_camp;
    double n_moli;
    double err_n_moli;
    double temp_media;
    double err_temp_media;
    double b_ang;
    double err_b_ang;
    double a_intercetta;
    double err_a_intercetta;
    double sigma_y_post;
};