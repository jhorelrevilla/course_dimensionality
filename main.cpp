#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
#include <iostream>

#include "Octreemejorado.h"

#define window_x 800
#define window_y 800
using namespace std;

double cam_x=0,cam_y=0,zoom=0;
struct Point{
	double x,y,z;
	Point(double tx=0,double ty=0,double tz=0){
		x=tx;
		y=ty;
		z=tz;
	}
		void operator =(Point t1){
			this->x=t1.x;
			this->y=t1.y;
			this->z=t1.z;
		}
		void imprimir(){
			cout<<x<<", "<<y<<", "<<z<<endl;
		}
			void set_pos(double tx=0,double ty=0,double tz=0){
				x=tx;
				y=ty;
				z=tz;
			}
};
/////////////////////////////////////////////////////////
struct Cube{
	Point *p[2];
	Cube(Point &A,Point &B){// a->menor b->mayor
		p[0]=&A;
		p[1]=&B;
	}
		void dibujar(void){
			double dif=p[1]->x-p[0]->x;
			Point arr_p[4];
			arr_p[0]=*p[0];arr_p[2]=*p[0];
			arr_p[1]=*p[0];arr_p[3]=*p[0];
			arr_p[1].x+=dif;
			arr_p[2].x+=dif;
			arr_p[2].y+=dif;
			arr_p[3].y+=dif;
			///Colores
			Point c[6];
			c[0].set_pos(1,0,0);
			c[1].set_pos(0,1,0);
			c[2].set_pos(0,0,1);
			c[3].set_pos(150,0,150);
			c[4].set_pos(0,150,100);
			c[5].set_pos(200,160,0);
			///tapa
			int XD=0;
			for(int w=0;w<2;++w){
				glBegin(GL_POLYGON);
				glColor3f(c[XD].x,c[XD].y,c[XD].z);
				for(int i=0;i<4;++i){
					glVertex3d(arr_p[i].x,arr_p[i].y,p[w]->z);
				}
				glEnd();
				XD++;
			}
			///lados
			for(int i=0;i<3;++i){
				glBegin(GL_POLYGON);
				glColor3f(c[XD].x,c[XD].y,c[XD].z);
				glVertex3d(arr_p[i].x,arr_p[i].y,p[0]->z);
				glVertex3d(arr_p[i+1].x,arr_p[i+1].y,p[0]->z);
				glVertex3d(arr_p[i+1].x,arr_p[i+1].y,p[1]->z);
				glVertex3d(arr_p[i].x,arr_p[i].y,p[1]->z);
				glEnd();
				XD++;
			}
			glBegin(GL_POLYGON);
			glColor3d(c[XD].x,c[XD].y,c[XD].z);
			glVertex3d(arr_p[3].x,arr_p[3].y,p[0]->z);
			glVertex3d(arr_p[0].x,arr_p[0].y,p[0]->z);
			glVertex3d(arr_p[0].x,arr_p[0].y,p[1]->z);
			glVertex3d(arr_p[3].x,arr_p[3].y,p[1]->z);
			glEnd();
			glutSwapBuffers();
			
		}
};
/////////////////////////////////////////////////////////
struct Centro{
	void dibujar(void){
		glRotatef( cam_x, 0.0, 0.0, 1.0 );//A-ABA
		glRotatef( cam_y, 0.0, 1.0, 0.0 );//D-I
		cam_x=0;
		cam_y=0;
	}
};
/////////////////////////////////////////////////////////
void displayCoord(){
	glBegin(GL_LINES);
	glColor3d(255,0,0);//x
	glVertex3d(0, 0,0);
	glVertex3d(window_x, 0,0);
	glColor3d(0, 255, 0);//y
	glVertex3d(0, 0,0);
	glVertex3d(0, window_y,0);
	glColor3d(0, 0, 255);//y
	glVertex3d(0, 0,0);
	glVertex3d(0, 0,window_y);
	glEnd();
	glutSwapBuffers();
}
	/////////////////////////////////////////////////////////
	void Pintar() {
		Centro centroide;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/// Cargar puntos
	glColor3f(255,0,0);
	glPointSize(5);
	glBegin(GL_POINTS);
	for(int i=0;i<size;i++)
		glVertex3d(puntos_obj[i].x,puntos_obj[i].y,puntos_obj[i].z);
	glEnd();
	glutSwapBuffers();
	///
		displayCoord();
		centroide.dibujar();
	}
	/////////////////////////////////////////////////////////
	void specialKeys( int key, int x, int y ) {
		if (key == GLUT_KEY_RIGHT)
			cam_y -= 2;
		else if (key == GLUT_KEY_LEFT)
			cam_y += 2;
		else if (key == GLUT_KEY_UP)
			cam_x += 2;
		else if (key == GLUT_KEY_DOWN)
			cam_x -= 2;
		glutPostRedisplay();
	}
	/////////////////////////////////////////////////////////
	GLvoid window_redraw(GLsizei width, GLsizei height) {
		glViewport(0, 0, window_x, window_y );
		glMatrixMode(GL_PERSPECTIVE_CORRECTION_HINT);
		double over=5;
		double x=space->mat_pun[0]->x  -over;
		double y=space->mat_pun[1]->x  +over;
		cout<<"espacio desde "<<space->mat_pun[0]->x<<" , "<<space->mat_pun[1]->x<<endl;
//		double over=2;
//		double x=(width*-1)-over;
//		double y=height+over;
		
		/*glOrtho(x,y,x,y,x,y);*/
		
		//glOrtho(-30,30,-30,30,x,y);
		glOrtho(x,y,x,y,x,y);
		//punto_bajo_x,punto_alto_x,punto_bajo_y,punto_alto_y,fondo,delante
	}
	/////////////////////////////////////////////////////////
	int main (int argc, char **argv) {
		puntos_obj.resize(MAX);
		New_octree n("gato.obj",30);
		
		for(int i=0;i<size;i++){
			n.insertar(puntos_obj[i]);
		}
		/*Punto t1(5.0007,1.0268,-14.4955);*/
//		Punto t2(2,2,2);
		/*n.insertar(t1);*/
//		cout<<"///////////////////////////"<<endl;
//		n.insertar(t2);
		
		///////////////////////////////////////////////////////
		glutInit (&argc, argv);
		glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
		glutInitWindowSize (window_x,window_y);
		glutInitWindowPosition (0,0);
		glutCreateWindow ("Cubos");
		glEnable(GL_DEPTH_TEST);
		glutReshapeFunc(&window_redraw);
		///
		glutDisplayFunc (Pintar);
		
		///
		glutSpecialFunc(specialKeys);
		glClearColor(0,0,0,0);
		glutMainLoop();
		return 0;
	}






















