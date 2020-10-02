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
New_octree * punt_arb;


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
		Punto p1(20,20,20);
		Punto p2(30,30,30);
		Cube dc(p1,p2);
		
		/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
		/// Cargar puntos
//		glColor3f(255,0,0);
//		glPointSize(5);
//		glBegin(GL_POINTS);
//		for(int i=0;i<size;i++)
//			glVertex3d(puntos_obj[i].x,puntos_obj[i].y,puntos_obj[i].z);
//		glEnd();
//		glutSwapBuffers();
		///Cargar cubos
		for(int i=0;i<size_cubos;i++){
			cubos_para_pintar[i]->dibujar();
		}
		///
		displayCoord();
		centroide.dibujar();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	/////////////////////////////////////////////////////////
	void specialKeys( int key, int x, int y ) {
		if (key == GLUT_KEY_RIGHT)
			cam_y -= 3;
		else if (key == GLUT_KEY_LEFT)
			cam_y += 3;
		else if (key == GLUT_KEY_UP)
			cam_x += 3;
		else if (key == GLUT_KEY_DOWN)
			cam_x -= 3;
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
		glOrtho(x,y,x,y,x-100,y+100);
		//punto_bajo_x,punto_alto_x,punto_bajo_y,punto_alto_y,fondo,delante
	}
	/////////////////////////////////////////////////////////
	int main (int argc, char **argv) {
		puntos_obj.resize(MAX);
		New_octree n("gato.obj",10);
		
		for(int i=0;i<size;i++){
			n.insertar(puntos_obj[i]);
		}
//		Punto t1(5.0007,1.0268,-14.4955);
//		Punto t2(5,5,5);
//		n.insertar(t2);
//		cout<<"///////////////////////////"<<endl;
//		n.insertar(t2);
		n.dibujar();
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






















