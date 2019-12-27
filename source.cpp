#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const double ESPe = 1e-20;

class Gauss {
protected:
    vector<vector<double>> matrixVectors;
    vector<unsigned int> swapStack;
public:
    unsigned int matrixSize;
    Gauss();
    Gauss(string fileName);
    Gauss(const Gauss &objectToClone);
    bool fileRead(string fileName);
    bool printMatrix();
    bool diagonalCheckZero(unsigned int index);
    double matrixFactor(unsigned int index, unsigned int offsetIndex);
};

int main()
{
    Gauss g1 = Gauss();
    g1.printMatrix();
    Gauss g2 = Gauss("test.csv");
    g2.printMatrix();
    cout << "\n";
    Gauss g3 = Gauss(g2);
    g3.printMatrix();
    return 0;
}
Gauss::Gauss(string fileName) {
    if(!fileRead(fileName)) {
        cout << "Uruchamiam konstruktor domyslny!\n";
        *this = Gauss();
    }
}
Gauss::Gauss() {
    this->matrixSize = 3;
    for( int i = 0; i < matrixSize; i++) {
        vector<double> temp;
        for( int j=0; j <= matrixSize; j++)
                temp.push_back(j);
        this->matrixVectors.push_back(temp);
    }
}
bool Gauss::fileRead(string fileName) {
    ifstream sourceFile;
    sourceFile.open(fileName);
    if ( !sourceFile.is_open() ) {
        cout << "Plik nie zostal otwarty!\n";
        return false;
    }
    sourceFile >> matrixSize;
    char * charThrashCan = new char(0);
    for ( int i = 0; i <= matrixSize; i++ )
        sourceFile >> *charThrashCan;
    for ( int i = 0; i < matrixSize; i++ )
    {
        vector<double> temp;
        double * tempDouble = new double(0);
        for (int j = 0; j < matrixSize; j++)
        {
            sourceFile >> *tempDouble;
            temp.push_back(*tempDouble);
            sourceFile >> *charThrashCan;
        }
        sourceFile >> *charThrashCan;
        sourceFile >> *tempDouble;
        temp.push_back(*tempDouble);
        matrixVectors.push_back(temp);
        delete tempDouble;
    }
    sourceFile.close();
    return true;
}
bool Gauss::printMatrix() {
    if( this->matrixVectors.empty() ) {
        cout << "Macierz jest pusta! Nie mam co wypisac!\n";
        return false;
    }
    for(int i=0; i<matrixSize; i++) {
        for(int j=0; j<=matrixSize; j++) {
            cout << matrixVectors[i][j] << "\t";
        }
        cout << "\n";
    }
}
Gauss::Gauss(const Gauss &objectToClone) {
    this->matrixSize = objectToClone.matrixSize;
    this->matrixVectors = objectToClone.matrixVectors;
}
bool Gauss::diagonalCheckZero(unsigned int index) {
    return (fabs(matrixVectors[index][index]) < ESPe) ? true : false;
}
double Gauss::matrixFactor(unsigned int index, unsigned int offsetIndex) {
    return matrixVectors[index+1+offsetIndex][index]/matrixVectors[index][index];
}
