#include <iostream>
#include <cmath>
using namespace std;
class Gauss {
private:
    const unsigned int rozm_Mgaussa;
    double **macierz;
public:
    Gauss(unsigned int rozmGauss):rozm_Mgaussa(rozmGauss) //dodaæ wczytywanie wyrazu wolnego
    {
        this->macierz = new double * [rozm_Mgaussa]; //inicjalizacja dynamicznej tablicy
        for(int i=0; i<rozm_Mgaussa; i++)
            this->macierz[i] = new double[rozm_Mgaussa+1];
        for(int i=0; i<rozm_Mgaussa; i++)   //wypelnienie tablicy domyœln¹ wartoœci¹
            for(int j=0;j<=rozm_Mgaussa; j++)
                this->macierz[i][j] = 1;
    }
    Gauss(double **tab_ins, unsigned int rozmGauss):rozm_Mgaussa(rozmGauss) //dodac konstruktor 2 typow wprowadzania wyrazu wolnego
    {
        this->macierz = new double * [rozm_Mgaussa]; //inicjalizacja dynamicznej tablicy
        for(int i=0; i<rozm_Mgaussa; i++)
            this->macierz[i] = new double[rozm_Mgaussa+1];
        for(int i=0; i<rozm_Mgaussa; i++) //wypelnienie tablicy wartoœci¹ z tablicy dynamicznej
            for(int j=0;j<=rozm_Mgaussa; j++)
                this->macierz[i][j] = tab_ins[i][j];
    }
    void printm(char T) //wyswietlenie macierzy
    {
        cout<<"Macierz "<<T<<"\n";
        for(int i=0; i<rozm_Mgaussa; i++)
        {
            for(int j=0;j<rozm_Mgaussa; j++)
                cout<<this->macierz[i][j]<<"\t";
            cout<<"|\t"<<this->macierz[i][rozm_Mgaussa]<<"\n";
        }
    }
    bool ifCrossZero(unsigned int z) //sprawdza, czy zero w przek¹tnej
    {
        if(abs(this->macierz[z][z]) < 1e-9) //z b³êdem
            return true;
        return false;
    }
    double wspMacierzy(unsigned int n, unsigned int offset = 0) //oblicza wspó³czynnik macierzy
    {
        cout<<"TEST"<<endl;
        return this->macierz[n+1+offset][n]/this->macierz[n][n];
    }
    void calcRow(unsigned int n) //przelicza wiersze dla kroku n wierza n+1
    {
        for(int i=0;i<rozm_Mgaussa-n-1;i++)
        {
            double wsp = this->wspMacierzy(n,i);
            for(int j=n+1;j<=rozm_Mgaussa;j++)
                this->macierz[i+1][j] = this->macierz[i+1][j] - wsp * this->macierz[n][j];
            for(int j=0;j<=n;j++)
                this->macierz[i+1][j] = 0;
        }
    }
    ~Gauss() //zwalnianie pamieci
    {
        for (int i = 0; i<rozm_Mgaussa; i++)
        delete [] macierz[i];
        delete [] macierz;
    }
};



int main()
{
    Gauss g1 = Gauss(4);
    g1.printm('G');


    return 0;
}
