#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif

#ifndef OCTREE_H
#define OCTREE_H
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

vector<Punto> puntos_obj;

int size=0;
Cubo* space;

int xd=0;
class Punto{
public:
	GLfloat x,y,z;
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
	double porcent;
	int num_dots=0;
	Cubo(Punto A,Punto B,double por){
		
		mat_pun[0]=new Punto(A.x,A.y,A.z);
		mat_pun[1]=new Punto(B.x,B.y,B.z);
		///
		double v= calcular_volumen();
		double a= calcular_area();
		double cant_nece_dots_1=v*(por/(double)100);
		double cant_nece_dots_2=a*(por/(double)100);
		puntos_necesarios=cant_nece_dots_2;
//		mat_pun[0]->imprimir();
//		mat_pun[1]->imprimir();
		cout<<"//////////////////////////////\n";
////		imprimir();
//		cout<<"necesita "<<puntos_necesarios<<" y tengo "<<puntos.size()<<endl;
		porcent=por;
		for(int i=0;i<8;++i){
			hijos[i]=nullptr;
		}
	}
///////////////////////////////////////////////////////////////////////	
	bool insertar(Punto &t){
		if(!punto_dentro_cuadrante(t)){///si no se encuentra en el cuadrante
			//cout<<"1"<<endl;
			return false;
		}
		/*cout<<"puntos size "<<num_dots<<" >= "<<puntos_necesarios<<endl;*/
		if(puntos.size()+1>=puntos_necesarios){///Si es una hoja y si esta dentro del limite
//			mat_pun[0]->imprimir();
//			mat_pun[1]->imprimir();
//			/*cout<<"xd "<<xd<<endl;*/
//			t.imprimir();
			num_dots++;
			xd++;
			cout<<"xd "<<xd<<endl;
			puntos.push_back(t);
			if(puntos.size()>=puntos_necesarios)
				lleno=true;
			return true;
		}
		else if(puntos_necesarios>0 && verificar_part()){///si no tiene hijos y no es nulo(hoja)
			//cout<<"2"<<endl;
			double d=lado/2;
			Punto t1=*mat_pun[0];
			Punto t2=*mat_pun[1];
			hijos[0]=new Cubo(Punto(t1.x,t1.y,t1.z),// -
							  Punto(t2.x-d,t2.y-d,t2.z-d),porcent );// -x -y -z
			hijos[1]=new Cubo(Punto(t1.x+d,t1.y,t1.z),// +x
							  Punto(t2.x,t2.y-d,t2.z-d),porcent );// -y -z
			hijos[2]=new Cubo(Punto(t1.x,t1.y+d,t1.z),// +y 
							  Punto(t2.x-d,t2.y,t2.z-d),porcent );// -x -z
 			hijos[3]=new Cubo(Punto(t1.x+d,t1.y+d,t1.z),// +x +y
							  Punto(t2.x,t2.y,t2.z-d),porcent );// -z
			
			
			hijos[4]=new Cubo(Punto(t1.x,t1.y,t1.z+d),// +z
							  Punto(t2.x-d,t2.y-d,t2.z),porcent );// -x -y
			hijos[5]=new Cubo(Punto(t1.x+d,t1.y,t1.z+d),// +x +z
							  Punto(t2.x,t2.y-d,t2.z),porcent );//  -y
			hijos[6]=new Cubo(Punto(t1.x,t1.y+d,t1.z+d),// +y +z
							  Punto(t2.x-d,t2.y,t2.z),porcent );//  -x
			hijos[7]=new Cubo(Punto(t1.x+d,t1.y+d,t1.z+d),//+x +y +z 
							  Punto(t2.x,t2.y,t2.z),porcent );// -
		}
		if(!verificar_part() && puntos_necesarios>0 ){//si tiene hijos y no es nulo(rama)
			//cout<<"3"<<endl;
			for(int i=0;i<8;++i){
				/*	cout<<"voy al hijo "<<i+1<<endl;*/
				if(hijos[i]->insertar(t)==true){
					comprobar();
					break;
				}
			}
		}
		return true;
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
		cout<<"min dot cubo \n";
		mat_pun[0]->imprimir();
		cout<<"max dot cubo \n";
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
				puntos_obj[size]=Punto(coord[0],coord[1],coord[2]);
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
		cout<<"Se cargaron "<<size<<" puntos\n";
		root=space;
	}
	bool insertar(Punto t){
		Cubo *p=root;
		p->insertar(t);
	}
	
};
#endif

