#include "GL/freeglut.h"
#include "GL/gl.h"
#include <stdio.h>
#include <vector>

using namespace std;

#define xminb 0.0
#define xmaxb 30.0
#define yminb -10.0
#define ymaxb 200.0

double xmin, xmax, ymin, ymax;
GLint width, height;
double * graph1;
double * graph2;
double * graph3;
int gr1, gr2, gr3;
GLuint ind;
double stepx=0.1;
double stepy=1;
double translate[4][2]={{0,0},{0,0},{0,0},{0,0}};
int cur=1;
double colors[3][3][3]=
	{
		{ {1.0,0.0,0.0},{0.6,0.1,0.1},{0.4,0.2,0.2} },
		{ {0.0,1.0,0.0},{0.1,0.6,0.1},{0.2,0.4,0.2} },
		{ {0.0,0.0,1.0},{0.1,0.1,0.6},{0.2,0.2,0.4} }
	};
	
int mx, my;
double mxd, myd;
bool down=false;
bool mleft=false;
bool mright=false;


void readgraph(FILE * f, double ** g, int * gr){
	vector<double> graph;
	double t,Fx,Fy,Fz;
	int k=0;
	while (!feof(f)){
		fscanf(f,"%lf %lf %lf %lf\n",&t,&Fx,&Fy,&Fz);
		bool b=(Fx!=0)&&(Fy!=0)&&(Fz!=0)&&(Fz<200.0);
		if(b){
			graph.push_back(t);
			graph.push_back(Fx);
			graph.push_back(Fy);
			graph.push_back(Fz);
			k++;
		}
	}
	*g = new double[graph.size()];
	for(int i=0;i<graph.size();i++)
		(*g)[i]=graph[i];

	*gr=k;
	graph.clear();
}

void prepareList(int listn,double * g, int gr, int d, int grn){
	glNewList(ind+listn,GL_COMPILE);
	glColor3dv(colors[grn-1][d-1]);
	glBegin(GL_LINE_STRIP);
		for(int i=0;i<gr;i++){
			glVertex2d(g[4*i],g[4*i+d]);
		}
	glEnd();
	glEndList();
}

void loadgraphs(){
	FILE * f1 = fopen("s005_1.txt","r");
	FILE * f2 = fopen("s005_2.txt","r");
	FILE * f3 = fopen("s005_3.txt","r");
	
	readgraph(f1,&graph1,&gr1);
	readgraph(f2,&graph2,&gr2);
	readgraph(f3,&graph3,&gr3);
	
	fclose(f1);
	fclose(f2);
	fclose(f3);
	
	for(int i=0;i<3;i++){
		prepareList(i+0,graph1,gr1,i+1,1);
		prepareList(i+3,graph2,gr2,i+1,2);
		prepareList(i+6,graph3,gr3,i+1,3);
	}
}

void drawGraphs(){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();
	glTranslated(translate[3][0],translate[3][1],0);
	glPushMatrix();
	glTranslated(translate[0][0],translate[0][1],0);
	glCallList(ind+0);
	glCallList(ind+1);
	glCallList(ind+2);
	glPopMatrix();
	glPushMatrix();
	glTranslated(translate[1][0],translate[1][1],0);
	glCallList(ind+3);
	glCallList(ind+4);
	glCallList(ind+5);
	glPopMatrix();
	glPushMatrix();
	glTranslated(translate[2][0],translate[2][1],0);
	glCallList(ind+6);
	glCallList(ind+7);
	glCallList(ind+8);
	glPopMatrix();
	glPopMatrix();
	
	
	glColor3d(0.7,0.7,0.7);
	glBegin(GL_LINES);
		for(int i=0;i<30;i++){
			glVertex2d(-100,i*10.0);
			glVertex2d(100,i*10.0);
			glVertex2d(i*1.0,-1000);
			glVertex2d(i*1.0,1000);
		}
	glEnd();
}

void newOrtho(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xmin,xmax,ymin,ymax,-1.0,1.0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void resizeCall(GLint newwidth, GLint newheight){
	if (newheight==0) height=1;
	width=newwidth;
	height=newheight;
	newOrtho();
	drawGraphs();
	glFlush();
}

void zoomin(){
	double xm,ym;
	xm=xmin;
	xmin=(xmin+xmax)/2-(xmax-xmin)*0.95/2;
	xmax=(xm+xmax)/2+(xmax-xm)*0.95/2;
	ym=ymin;
	ymin=(ymin+ymax)/2-(ymax-ymin)*0.95/2;
	ymax=(ym+ymax)/2+(ymax-ym)*0.95/2;
	newOrtho();
}

void zoomout(){
	double xm,ym;
	xm=xmin;
	xmin=(xmin+xmax)/2-(xmax-xmin)*1.05/2;
	xmax=(xm+xmax)/2+(xmax-xm)*1.05/2;
	ym=ymin;
	ymin=(ymin+ymax)/2-(ymax-ymin)*1.05/2;
	ymax=(ym+ymax)/2+(ymax-ym)*1.05/2;
	newOrtho();
}

void keyboard(unsigned char key,int x, int y){
	printf("%i\n",key);
	
	switch (key){
	case 49:
		cur=1;
		break;
	case 50:
		cur=2;
		break;
	case 51:
		cur=3;
		break;
	case 52:
		cur=4;
		break;
	case 97:
		translate[cur-1][0]-=stepx;
		break;
	case 119:
		translate[cur-1][1]+=stepy;
		break;
	case 100:
		translate[cur-1][0]+=stepx;
		break;
	case 115:
		translate[cur-1][1]-=stepy;
		break;
	case 113:
		zoomout();
		break;
	case 101:
		zoomin();
		break;
	case 9:
		xmin=xminb;
		xmax=xmaxb;
		ymin=yminb;
		ymax=ymaxb;
		newOrtho();
		break;
	}
	drawGraphs();
	glFlush();
}

void mouse(int button, int state, int x, int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if((state==GLUT_DOWN)&&(!down)){
				mx=x;
				my=y;
				down=true;
				mleft=true;
				mxd=xmin+(xmax-xmin)*mx/width;
				myd=ymin+(ymax-ymin)*(height-my)/height;
				//printf("left down %i %i\n",x,y);
			}
			if((state==GLUT_UP)&&(down)){
				down=false;
				mleft=false;
				//printf("left up %i %i\n",x,y);
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if((state==GLUT_DOWN)&&(!down)){
				mx=x;
				my=y;
				down=true;
				mright=true;
				mxd=xmin+(xmax-xmin)*mx/width;
				myd=ymin+(ymax-ymin)*(height-my)/height;
			}
			if((state==GLUT_UP)&&(down)){
				down=false;
				mright=false;
				double xd,yd;
				xd=xmin+(xmax-xmin)*x/width;
				yd=ymin+(ymax-ymin)*(height-y)/height;
				xmin=min(xd,mxd);
				xmax=max(xd,mxd);
				ymin=min(yd,myd);
				ymax=max(yd,myd);
				newOrtho();
				drawGraphs();
				glFlush();
			}
			break;
		case 3:
			zoomin();
			drawGraphs();
			glFlush();
			break;
		case 4:
			zoomout();
			drawGraphs();
			glFlush();
			break;
	}
	//printf("click %i %i\n",x,y);
}

void mousemove(int x, int y){
	//printf("%i %i\n",x,y);
	double xd, yd;
	if (mleft||mright){
		xd=xmin+(xmax-xmin)*x/width;
		yd=ymin+(ymax-ymin)*(height-y)/height;
	}
	if(mleft){
		//printf("%i %i\n",x,y);
		xmin+=(mxd-xd);
		xmax+=(mxd-xd);
		ymin+=(myd-yd);
		ymax+=(myd-yd);
		newOrtho();
		drawGraphs();
		glFlush();
	}
	if(mright){
		drawGraphs();
		glColor3d(0.4,0.5,0.2);
		glBegin(GL_LINE_LOOP);
			glVertex2d(mxd,myd);
			glVertex2d(xd,myd);
			glVertex2d(xd,yd);
			glVertex2d(mxd,yd);
		glEnd();
		glFlush();
	}
}

int main(int argc, char **argv)
{
	xmin=xminb;
	xmax=xmaxb;
	ymin=yminb;
	ymax=ymaxb;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("graphHandMatch");
    glutDisplayFunc(drawGraphs);
    glutReshapeFunc(resizeCall);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
    ind = glGenLists(9);
    loadgraphs();
    
    glutMainLoop();
    glDeleteLists(ind,9);
    return 0;
}
