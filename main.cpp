#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono> 
using namespace std;


////////////////////////////////////////////////////
class Conjunto_datos{
public:
	vector<vector<int>> Conjunto;
	int numero_elementos;
	int numero_dimension;
	///
	Conjunto_datos(int dimension ,int elementos){
		numero_dimension=dimension;numero_elementos=elementos;
		Conjunto.resize(elementos);
		for(int i=0;i<elementos;i++){
			Conjunto[i].resize(dimension);
		}
		srand(time(NULL));
		for(int i=0;i<elementos;i++){
			for(int j=0;j<dimension;j++){
				Conjunto[i][j]=rand()%elementos+1;
			}
		}
	}
	///
	void imprimir(){
		for(int i=0;i<numero_elementos;i++){
			cout<<i+1<<": ";
			for(int j=0;j<numero_dimension;j++){
				cout<<Conjunto[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	///
	float distancia_euclidiana(vector<int> Punto_a,vector<int> Punto_b){
		float result=0;
		for(int i=0;i<Punto_a.size();i++){
			result+=pow(Punto_a[i]-Punto_b[i],2);
		}
		return sqrt(result);
	}
	///
	void calcular_euclidiana(){
		for(int i=0;i<numero_elementos;i++){
			for(int j=0;j<numero_elementos;j++){
				distancia_euclidiana(Conjunto[i],Conjunto[j]);
			}
		}
	}
};

////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
	vector<int> dimensiones={4,6,8,10,12,18,20};
	vector<int> elementos={10000,15000,20000,25000};
	Conjunto_datos *c;
	
	for(int i=0;i<dimensiones.size();i++){
		for(int j=0;j<elementos.size();j++){
			cout<<"dimension "<<dimensiones[i]<<" con elementos "<<elementos[j]<<endl;
			c=new Conjunto_datos(dimensiones[i],elementos[j]);
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			
			c->calcular_euclidiana();
			
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			std::cout << "Ha pasado " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000.0  <<"\n\n";
			delete c;
		}
	}
	
	return 0;
}











