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