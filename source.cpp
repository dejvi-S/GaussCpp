#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const double ESPe = 1e-15;

class Gauss {
protected:
    vector<vector<double>> matrixVectors;
    vector<unsigned int> swapStack;
    vector<double> result;
public:
    unsigned int matrixSize;
    Gauss();
    Gauss(string fileName);
    Gauss(const Gauss &objectToClone);
    bool fileRead(string fileName);
    bool printMatrix();
    bool diagonalCheckZero(unsigned int index);
    double matrixFactor(unsigned int index, unsigned int offsetIndex);
    void rowCalculation(unsigned int index);
    bool gaussMainCalculations(unsigned int &index);
    void basicGauss();
    void printResult(bool successSwitch);
    bool maxElementInRow(unsigned int index);
};

int main()
{
    Gauss g1 = Gauss("test.csv");
    g1.printMatrix();
    g1.basicGauss();

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
        matrixVectors.push_back(temp);
        result.assign(matrixSize,0);
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
    result.assign(matrixSize,0);
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
void Gauss::rowCalculation(unsigned int index) {
    for( int i=index; i<matrixSize-1 ;i++ ) {
            double tempFactor = matrixFactor(index,i-index);
            for(int j=index+1; j<=matrixSize; j++)
                matrixVectors[i+1][j] = matrixVectors[i+1][j] - tempFactor * matrixVectors[index][j];
            for( int j=0; j<=index; j++)
                matrixVectors[i+1][j] = 0;
    }
}
bool Gauss::gaussMainCalculations(unsigned int &index)
{
    for(int i=index; i<matrixSize-1; i++)
    {
        if(!diagonalCheckZero(i)) {
            rowCalculation(i);
            for(int i=matrixSize-1; i>=0; i--)
            {
                double sum = 0;
                for(int j=matrixSize-1; j>i; j--)
                    sum = sum+result[j] * matrixVectors[i][j];
                result[i] = (-sum + matrixVectors[i][matrixSize]) / matrixVectors[i][i];
            }
        }
        else {
            index = i;
            return false;
        }
    }
    return true;
}
void Gauss::basicGauss() {
    unsigned int lastIndex = 0;
    (gaussMainCalculations(lastIndex)) ? printResult(true) : printResult(false);
}
void Gauss::printResult(bool successSwitch){
    if(successSwitch) {
        if(result.empty()) {
            cout<<"Zbior wynikow jest pusty!\n";
        } else {
            while(!swapStack.empty()) {
                swap(result[swapStack.back()],result[swapStack.back()-1]);
                swapStack.pop_back(); swapStack.pop_back();
            }
            cout<<"\nWynik:\n";
            for(int i=0; i<result.size(); i++) {
                cout<<"X"<<i<<":    "<<result[i]<<"\n";
            }
        }
    } else cout<<"Macierz nieosobliwa!\n";
}
bool Gauss::maxElementInRow(unsigned int index)
    {
        double tempMaxElement = fabs(matrixVectors[index][index]);
        unsigned int swapIndex = index;
        for(int i=index+1;i<matrixSize;i++)
            if(tempMaxElement<fabs(matrixVectors[i][index]))
            {
                tempMaxElement = fabs(matrixVectors[i][index]);
                swapIndex = i;
            }
        if(index == swapIndex) {
            return false;
        }
        else {
            swapStack.push_back(index);
            swapStack.push_back(swapIndex);
            for(int i=0; i<matrixSize; i++)
                swap(matrixVectors[i][swapIndex],matrixVectors[i][index]);
            return true;
        }
    }
