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

#define MAX 100000
using namespace std;

class Punto{
public:
	double x,y,z;
	Punto(double n_x,double n_y,double n_z){
		x=n_x;
		y=n_y;
		z=n_z;
	}
	void imprimir(){
		cout<<x<<"\t\t"<<y<<"\t\t"<<z<<endl;
	}
};
///////////////////////////////////////////////
class Cuadrante{
public:
	Punto* puntos[8];
	Cuadrante(Punto A,Punto B){
		vector<double>x;x.resize(2);
		vector<double>y;y.resize(2);
		vector<double>z;z.resize(2);
		x[0]=A.x;x[1]=B.x;
		y[0]=A.y;y[1]=B.y;
		z[0]=A.z;z[1]=B.z;
		int pos=0;
		for(int i=0;i<2;++i){
			for(int j=0;j<2;++j){
				for(int w=0;w<2;++w){
					puntos[pos]->x=x[j];
					puntos[pos]->y=y[w];
					puntos[pos]->z=z[i];
					pos++;
				}
			}
		}
	}
};
///////////////////////////////////////////////
class Espacio{
public:
	Punto* puntos[MAX];
	int tamanio=0;
	double min_x,min_y,min_z;
	double max_x,max_y,max_z;
	void set_tamanio(int t){
		tamanio=t;
	}
	void set_minimo(double x,double y,double z){
		min_x=x;min_y=y;min_z=z;
	}
	void set_maximo(double x,double y,double z){
		max_x=x;max_y=y;max_z=z;
	}
///
	void imprimir(){
		for(int i=0;i<tamanio;++i)
			cout<<puntos[i]->x<<" "<<puntos[i]->y<<" "<<puntos[i]->z<<endl;
	}
	void imprimir_min(){
		cout<<min_x<<"\t\t "<<min_y<<"\t\t "<<min_z<<endl;
	}
	void imprimir_max(){
		cout<<max_x<<"\t\t "<<max_y<<"\t\t "<<max_z<<endl;
	}
	
};
///////////////////////////////////////////////
Espacio Generar_espacio(string nombre_archivo){
	ifstream obj_file(nombre_archivo);	
	Espacio esp_temp;
	int size=0;
	double min_x=0,max_x=0, min_y=0,max_y=0, min_z=0,max_z=0;
	
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
					min_x=coord[0];max_x=coord[0];
					min_y=coord[1];max_y=coord[1];
					min_z=coord[2];max_z=coord[2];
				}
				else{
					if(coord[0] < min_x){
						min_x=coord[0];
					}
					else if(coord[0] > max_x ){
						max_x=coord[0];
					}
					///
					if(coord[1] < min_y){
						min_y=coord[1];
					}
					else if(coord[1] > max_y ){
						max_y=coord[1];
					}
					///
					if(coord[2]<min_z){
						min_z=coord[2];
					}
					else if(coord[2] > max_z ){
						max_z=coord[2];
					}
				}
				esp_temp.puntos[size]=new Punto(coord[0],coord[1],coord[2]);
				size+=1;
			}
		}
		obj_file.close();
	}
	esp_temp.set_minimo(min_x,min_y,min_z);
	esp_temp.set_maximo(max_x,max_y,max_z);
	esp_temp.set_tamanio(size);
	return esp_temp;
}
///////////////////////////////////////////////
class Octree{
public:
	Punto *mat_pun[2];//minimo,maximo
	Cuadrante* cuadrado;
	Octree *hijos [8];
	int numero_puntos=0;
	Octree(Espacio &e,int cant_puntos){
		mat_pun[0]=new Punto(e.min_x,e.min_y,e.min_z);//MINIMO
		mat_pun[1]=new Punto(e.max_x,e.max_y,e.max_z);//MAXIMO
		cant_puntos_dentro(e);//calcular cuantos puntos hay dentro
		mat_pun[0]->imprimir();
		mat_pun[1]->imprimir();
		
		bool contiene_porcentaje=verificar_area(cant_puntos);
		cout<<"Contiene "<<numero_puntos<<" puntos"<<endl;
		cout<<"//////////////////////////////////////////\n";
		if(numero_puntos > 0 && contiene_porcentaje && numero_puntos!=1 ){
			numero_puntos=0;
			double t_x=(mat_pun[1]->x - mat_pun[0]->x)/2;
			double t_y=(mat_pun[1]->y - mat_pun[0]->y)/2;
			double t_z=(mat_pun[1]->z - mat_pun[0]->z)/2;
			///1
			e.set_minimo(mat_pun[0]->x, mat_pun[0]->y, mat_pun[0]->z +t_z);
			e.set_maximo(mat_pun[1]->x -t_x, mat_pun[1]->y-t_y, mat_pun[1]->z);
			hijos[0]=new Octree(e,cant_puntos);
			///2
			e.set_minimo(mat_pun[0]->x + t_x, mat_pun[0]->y, mat_pun[0]->z +t_z);
			e.set_maximo(mat_pun[1]->x, mat_pun[1]->y - t_y, mat_pun[1]->z);
			hijos[1]=new Octree(e,cant_puntos);
			///3
			e.set_minimo(mat_pun[0]->x+t_x, mat_pun[0]->y +t_y, mat_pun[0]->z +t_z);
			e.set_maximo(mat_pun[1]->x, mat_pun[1]->y, mat_pun[1]->z);
			hijos[2]=new Octree(e,cant_puntos);
			///4
			e.set_minimo(mat_pun[0]->x, mat_pun[0]->y + t_y, mat_pun[0]->z +t_z);
			e.set_maximo(mat_pun[1]->x -t_x, mat_pun[1]->y, mat_pun[1]->z);
			hijos[3]=new Octree(e,cant_puntos);
			///5
			e.set_minimo(mat_pun[0]->x, mat_pun[0]->y, mat_pun[0]->z);
			e.set_maximo(mat_pun[1]->x -t_x, mat_pun[1]->y-t_y, mat_pun[1]->z-t_z);
			hijos[4]=new Octree(e,cant_puntos);
			///6
			e.set_minimo(mat_pun[0]->x +t_x, mat_pun[0]->y, mat_pun[0]->z);
			e.set_maximo(mat_pun[1]->x, mat_pun[1]->y-t_y, mat_pun[1]->z-t_z);
			hijos[5]=new Octree(e,cant_puntos);
			///7
			e.set_minimo(mat_pun[0]->x, mat_pun[0]->y +t_y, mat_pun[0]->z);
			e.set_maximo(mat_pun[1]->x -t_x, mat_pun[1]->y, mat_pun[1]->z -t_z);
			hijos[6]=new Octree(e,cant_puntos);
			///8
			e.set_minimo(mat_pun[0]->x + t_x, mat_pun[0]->y+t_y, mat_pun[0]->z);
			e.set_maximo(mat_pun[1]->x, mat_pun[1]->y, mat_pun[1]->z-t_z);
			hijos[7]=new Octree(e,cant_puntos);
			
		}
	}
	///
	void punto_dentro_cuadrante(Punto p){
		if(p.x > mat_pun[1]->x){
			return;
		}
		if(p.y > mat_pun[1]->y){
			return;
		}
		if(p.z > mat_pun[1]->z){
			return;
		}
		////////////////////////////
		if(p.x < mat_pun[0]->x){
			return;
		}
		if(p.y < mat_pun[0]->y){
			return;
		}
		if(p.z < mat_pun[0]->z){
			return;
		}
		numero_puntos+=1;
	}
	///
	int cant_puntos_dentro(Espacio e){
		for(int i=0;i<e.tamanio;++i){
			punto_dentro_cuadrante(*e.puntos[i]);
		}
	}
	bool verificar_area(int porcentaje){
		double l1=mat_pun[1]->x - mat_pun[0]->x;
		double l2=mat_pun[1]->y - mat_pun[0]->y;
		l1*=l2;
		double p_temp=(((float)porcentaje) /((float)100) );
		int p= l1*p_temp;
		cout<<"l1*l2 "<<l1<<endl;
//		cout<<"l2 "<<l2<<endl;
//		cout<<"p "<<p_temp<<endl;
		cout<<"puntos necesarios "<<p<<" tengo "<<numero_puntos<<endl;
		
		if(numero_puntos > p){
			return true;
		}
		return false;
	}
	///
	void agregar(Punto temp){
		
	}
};
#endif

