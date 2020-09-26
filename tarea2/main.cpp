#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#define MAX 100000
using namespace std;
class Punto;
class Cubo;

Punto* puntos_obj[MAX];
int size=0;
Cubo* space;

int xd=1;
class Punto{
public:
	double x,y,z;
	Punto(double n_x=0,double n_y=0,double n_z=0){
		x=n_x;
		y=n_y;
		z=n_z;
	}
		void imprimir(){
			cout<<x<<"    "<<y<<"     "<<z<<endl;
		}
			void operator =(Punto t1){
				this->x=t1.x;
				this->y=t1.y;
				this->z=t1.z;
			}
};
///////////////////////////////////////////////
class Cubo{
public:
	Punto *mat_pun[2];//minimo,maximo
	Cubo *hijos [8];
	vector<Punto> puntos;
	double val_volumen,val_area,lado;
	int puntos_necesarios=0;
	bool lleno=false;
	int porcent;
	int te=0;
	Cubo(Punto A,Punto B,int por){
		mat_pun[0]=new Punto(A.x,A.y,A.z);
		mat_pun[1]=new Punto(B.x,B.y,B.z);
		///
		double v= calcular_volumen();
		double a= calcular_area();
		double cant_nece_dots_1=v*(por/(double)100);
		double cant_nece_dots_2=a*(por/(double)100);
		puntos_necesarios=cant_nece_dots_2;
		//		//imprimir();
		porcent=por;
		for(int i=0;i<8;++i){
			hijos[i]=nullptr;
		}
	}
		///////////////////////////////////////////////////////////////////////	
		bool insertar(Punto t){
			if(!punto_dentro_cuadrante(t)){
				return false;
			}
			if(puntos.size()+1 > puntos_necesarios && verificar_part()){
				puntos.push_back(t);
				cout<<"/////////////////////////"<<endl;
				cout<<"se inserto"<<endl;
				t.imprimir();
				cout<<"en el Cuadrante"<<endl;
				imprimir();
				if(puntos.size()==puntos_necesarios)
					lleno=true;
				return true;
			}
			if(puntos.size() < puntos_necesarios ){
				double d=lado/2;
				Punto t1=*mat_pun[0];
				Punto t2=*mat_pun[1];
				hijos[0]=new Cubo(Punto(t1.x,t1.y,t1.z+d),Punto(t2.x-d,t2.y-d,t2.z),porcent );
				hijos[1]=new Cubo(Punto(t1.x+d,t1.y,t1.z+d),Punto(t2.x,t2.y-d,t2.z),porcent );
				hijos[2]=new Cubo(Punto(t1.x+d,t1.y+d,t1.z+d),Punto(t2.x,t2.y,t2.z),porcent );
				hijos[3]=new Cubo(Punto(t1.x,t1.y+d,t1.z+d),Punto(t2.x-d,t2.y,t2.z),porcent );
				hijos[4]=new Cubo(Punto(t1.x,t1.y,t1.z),Punto(t2.x-d,t2.y-d,t2.z-d),porcent );
				hijos[5]=new Cubo(Punto(t1.x+d,t1.y,t1.z),Punto(t2.x,t2.y-d,t2.z-d),porcent );
				hijos[6]=new Cubo(Punto(t1.x,t1.y+d,t1.z),Punto(t2.x-d,t2.y,t2.z-d),porcent );
				hijos[7]=new Cubo(Punto(t1.x+d,t1.y,t1.z+d),Punto(t2.x,t2.y,t2.z-d),porcent );
				for(int i=0;i<8;++i){
					if(hijos[i]->insertar(t)){
						break;
					}
				}
			}
			if(comprobar()){
				for(int i=0;i<8;++i){
					for(int w=0;w<hijos[i]->puntos.size();w++){
						puntos.push_back(hijos[i]->puntos[w]);
					}
					delete hijos[i];
					hijos[i]=nullptr;
				}
			}
			return false;
		}
			//////////////////////////////////////////
			bool punto_dentro_cuadrante(Punto p){
				if(p.x > mat_pun[1]->x){
					return false;
				}
				if(p.y > mat_pun[1]->y){
					return false;
				}
				if(p.z > mat_pun[1]->z){
					return false;
				}
				////////////////////////////
				if(p.x < mat_pun[0]->x){
					return false;
				}
				if(p.y < mat_pun[0]->y){
					return false;
				}
				if(p.z < mat_pun[0]->z){
					return false;
				}
				return true;
			}
				double calcular_volumen(){
					double l1=mat_pun[1]->x - mat_pun[0]->x;
					lado=l1;
					l1=pow(l1,3);
					
					return l1;
				}
					double calcular_area(){
						double l1=mat_pun[1]->x - mat_pun[0]->x;
						l1*=l1;
						return l1;
					}
						void imprimir(){
							cout<<"minimo punto del cubo \n";
							mat_pun[0]->imprimir();
							cout<<"maximo punto del cubo \n";
							mat_pun[1]->imprimir();
						}
							bool verificar_part(){
								for(int i=0;i<8;++i){
									if(hijos[i]!=nullptr){
										return false;
									}
								}
								return true;
							}
								bool comprobar(){
									for(int i=0;i<8;++i){
										if(!hijos[i]->lleno){
											return false;
										}
									}
									return true;
								}
};
///////////////////////////////////////////////
void Generar_espacio(string nombre_archivo,int por){
	ifstream obj_file(nombre_archivo);	
	double min_total=0;
	double max_total=0;
	if(obj_file.is_open()){
		string linea;
		string::size_type sz;
		while(getline(obj_file,linea)){
			int espacio=linea.find(" ");
			string cond=linea.substr(0,espacio);
			if(linea[espacio+1]==' '){
				espacio+=1;
			}
			linea=linea.substr(espacio+1);
			if(cond=="v"){
				double coord[3];
				for(int i=0;i<3;++i){
					espacio=linea.find(" ");
					cond=linea.substr(0,espacio);
					coord[i]=stod(cond,&sz);
					linea=linea.substr(espacio+1);
				}
				if(size==0){
					double t_mn=coord[0];
					double t_my=coord[0];
					for(int i=1;i<3;++i){
						if(t_mn<coord[i]){
							t_mn=coord[i];
						}
						else if(t_my>coord[i]){
							t_my=coord[i];
							
						}
					}
				}
				else{
					for(int i=0;i<3;++i){
						if(coord[i] < min_total){
							min_total=coord[i];
						}
						else if(coord[i] > max_total ){
							max_total=coord[i];
						}
					}
				}
				puntos_obj[size]=new Punto(coord[0],coord[1],coord[2]);
				size+=1;
			}
		}
		obj_file.close();
	}
	if(min_total<0){
		min_total*=-1;
	}
	double c=max(min_total,max_total);
	Punto minimo(-c,-c,-c);
	Punto maximo(c,c,c);
	space=new Cubo(minimo,maximo,por);
}
	///////////////////////////////////////////////
	class New_octree{
	public:
		Cubo *root;
		double porcent;
		New_octree(string file,double por){
			Generar_espacio(file,por);
			root=space;
		}
			void insertar(Punto t){
				Cubo *p=root;
				 p->insertar(t);
			}
	};
	///////////////////////////////////////////////
	int main (int argc, char **argv) {
		New_octree n("gato.obj",40);
		for(int i=0;i<size;++i){
			n.insertar(*puntos_obj[i]);
		}
		return 0;
	}
	
