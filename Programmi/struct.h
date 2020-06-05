#include <vector>
using namespace std;

//Contiene i dati grezzi
struct raw_data
{
    double n_camp;
    vector<double> temp;
    vector<double> pressure;
    vector<double> volume;
};

//Strutura per le informazioni generali per campione
struct info
{
    double n_moli;
    double err_n_moli;
    double n_camp;
    double temp_media;
    double err_temp_media;
};