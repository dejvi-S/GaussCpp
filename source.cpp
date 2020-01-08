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
    bool gaussMainCalculations(unsigned int index);
    void basicGauss();
    void printResult(bool successSwitch);
    bool maxElementInRow(unsigned int index);
    bool maxElementInColumn(unsigned int index);
    void maxElementInMatrix(unsigned int index);
    bool gaussInColumn();
    bool gaussInRow();
    bool gaussFull();
};

int main()
{
    Gauss g1 = Gauss("test.csv");
    Gauss g2 = Gauss(g1),g3 = Gauss(g1),g4 = Gauss(g1);
    g1.printMatrix();
    g1.basicGauss();
    g2.gaussInRow();
    g3.gaussInColumn();
    g4.gaussFull();


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
bool Gauss::gaussMainCalculations(unsigned int index)
{
    if(!diagonalCheckZero(index)) {
        rowCalculation(index);
       //kalkulation
        return true;
    }
    else
        return false;
}
void Gauss::basicGauss() {
    bool breaked = false;
    for(int i=0; i<matrixSize-1; i++) {
        if(!gaussMainCalculations(i))
            break;
    }
    if(breaked)
        printResult(false);
    else {
        for(int i=matrixSize-1; i>=0; i--)
        {
            double sum = 0;
            for(int j=matrixSize-1; j>i; j--)
                sum = sum+result[j] * matrixVectors[i][j];
            result[i] = (-sum + matrixVectors[i][matrixSize]) / matrixVectors[i][i];
        }
        printResult(true);
    }
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
        if(tempMaxElement < fabs(matrixVectors[index][i]))    {
            tempMaxElement = fabs(matrixVectors[index][i]);
            swapIndex = i;
        }
    if(index == swapIndex) {
        return false;
    }
    else {
        swapStack.push_back(index);
        swapStack.push_back(swapIndex);
        for(int i=index; i<matrixSize; i++)
            swap(matrixVectors[i][swapIndex],matrixVectors[i][index]);
        return true;
    }
}
bool Gauss::maxElementInColumn(unsigned int index)
{
    double tempMaxElement = fabs(matrixVectors[index][index]);
    unsigned int swapIndex = index;
    for(int i=index+1;i<matrixSize;i++)
        if(tempMaxElement<fabs(matrixVectors[i][index]))
        {
            tempMaxElement = fabs(matrixVectors[i][index]);
            swapIndex = i;
        }
    if(index == swapIndex)
        return false;
    else {
        for(int i=index; i<=matrixSize; i++)
            swap(matrixVectors[swapIndex][i],matrixVectors[index][i]);
        return true;
    }
}
void Gauss::maxElementInMatrix(unsigned int index)
{
    double tempMaxElement = fabs(matrixVectors[index][index]);
    unsigned int swapIndexJ = index;
    unsigned int swapIndexI = index;
    for(int i=index;i<matrixSize;i++)
        for(int j=index;j<matrixSize;j++)
            if(tempMaxElement<fabs(matrixVectors[i][j]))
            {
                tempMaxElement = fabs(matrixVectors[i][j]);
                swapIndexJ = j;
                swapIndexI = i;
            }
    if(index != swapIndexI) {
        for(int i=index; i<=matrixSize; i++)
            swap(matrixVectors[swapIndexI][i],matrixVectors[index][i]);
    }
    if(index != swapIndexJ) {
        swapStack.push_back(index);
        swapStack.push_back(swapIndexJ);
        for(int i=index; i<matrixSize; i++)
            swap(matrixVectors[i][swapIndexJ],matrixVectors[i][index]);
    }
}
bool Gauss::gaussInColumn()
{
    for(int i=0; i<matrixSize-1; i++) {
        maxElementInColumn(i);
        gaussMainCalculations(i);
    }
}
bool Gauss::gaussInRow()
{
    for(int i=0; i<matrixSize-1; i++) {
        maxElementInRow(i);
        gaussMainCalculations(i);
    }
    for(int i=matrixSize-1; i>=0; i--)
    {
        double sum = 0;
        for(int j=matrixSize-1; j>i; j--)
            sum = sum+result[j] * matrixVectors[i][j];
        result[i] = (-sum + matrixVectors[i][matrixSize]) / matrixVectors[i][i];
    }
    printResult(true);
}
bool Gauss::gaussFull()
{
    for(int i=0; i<matrixSize-1; i++) {
        maxElementInMatrix(i);
        gaussMainCalculations(i);
    }
}
