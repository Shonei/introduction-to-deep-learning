#include <GL/glu.h>
#include <GL/glut.h>
#include <QGLWidget>
#include "SolidCubeWidget.h"

// Setting up material properties
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;

static materialStruct brassMaterials = {
  { 0.33, 0.22, 0.03, 1.0},
  { 0.78, 0.57, 0.11, 1.0},
  { 0.99, 0.91, 0.81, 1.0},
  27.8 
};

static materialStruct earthGreen = {
  { 0.0, 0.0, 0.0, 1.0 },
  { 0.1, 0.35, 0.1, 1.0 },
  { 0.45, 0.55, 0.45, 1.0 },
  32.0
};

static materialStruct emerald = {
  { 0.0215, 0.1745, 0.0215, 1.0 },
  { 0.07568, 0.61424, 0.07568, 1.0  },
  { 0.633, 0.727811, 0.633, 1.0 },
  32.0
};

static materialStruct red_plastic = {
  { 0.0, 0.0, 0.0, 1.0 },
  { 0.5, 0.0, 0.0, 1.0 },
  { 0.7, 0.6, 0.6, 1.0 },
  32.0
};

// constructor
SolidCubeWidget::SolidCubeWidget(QWidget *parent) : QGLWidget(parent) { 
	angle = 0;
  globe_angle = 0;
  super_angle = 0;
  planet1_angle = 0.0;
  planet2_angle = 0.0;
  planet3_angle = 0.0;
  planet1_y = 0.0;
  planet1_y_goal = 8.0;
  planet2_x = 0.0;
  planet2_x_goal = 20.0;
  planet2_y = 0.0;
  planet2_y_goal = 8.0;
  planet3_y = 0.0;
  planet3_y_goal = 4.0;
  m = 12.0;
  n1 = 33.0;
  n2 = -10.0;
  n3 = 1.35; 

  earth = new QImage();
  if(earth->load("earth.ppm")) {
    std::cout << earth->size().width() << std::endl;
    std::cout << earth->size().height() << std::endl;
  }

  marc = new QImage();
  if(marc->load("marc.ppm")) {
    std::cout << marc->size().width() << std::endl;
    std::cout << marc->size().height() << std::endl;
  }

  gen_texture();
} 

// called when OpenGL context is set up
void SolidCubeWidget::initializeGL() { 
  glClearColor(0.3, 0.3, 0.3, 0.0);
  this->sphere(7.0);
  this->super_sphere();

  glEnable(GL_TEXTURE_2D);

  GLuint textures[2];
  glGenTextures(1, &textures[0]);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glGenTextures(1, &textures[1]);
  glBindTexture(GL_TEXTURE_2D, textures[1]);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(20);
} 

void SolidCubeWidget::angleChange(int a) {
  angle = a;
}

// called every time the widget is resized
void SolidCubeWidget::resizeGL(int w, int h) { 
	glViewport(0, 0, w, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
   
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
                                                                                                                                                               
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);

} 

void SolidCubeWidget::house() {
  GLfloat normals[][3] = { {0.333, 1., 0.333 }, {0.333, 1., 0.333}, {0.333, 1., 0.333}, {0.3333, 0.3333, 0.333}};
  materialStruct *p_front = &brassMaterials;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,    p_front->ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,    p_front->diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,   p_front->specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,   p_front->shininess);

  // back wall
  glNormal3fv(normals[4]); 
  glBegin(GL_POLYGON);
    glVertex3f( -20.0,  0.0,  15.0);
    glVertex3f( -20.0,  0.0, -15.0);
    glVertex3f( -20.0, 15.0, -15.0);
    glVertex3f( -20.0, 15.0,  15.0);
  glEnd();

  // front wall right
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -10.0, 15.0, -10.0);
    glVertex3f( -10.0, 15.0, -15.0);
    glVertex3f( -10.0,  0.0, -15.0);
    glVertex3f( -10.0,  0.0, -10.0);
  glEnd();

  // front wall top
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -10.0, 15.0, -10.0);
    glVertex3f( -10.0, 15.0,  -5.0);
    glVertex3f( -10.0, 12.0,  -5.0);
    glVertex3f( -10.0, 12.0, -10.0);
  glEnd();

  // front wall botom
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -10.0,  5.0, -10.0);
    glVertex3f( -10.0,  5.0,  -5.0);
    glVertex3f( -10.0,  0.0,  -5.0);
    glVertex3f( -10.0,  0.0, -10.0);
  glEnd();

  // front wall left
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -10.0, 15.0,  -5.0);
    glVertex3f( -10.0, 15.0,  15.0);
    glVertex3f( -10.0,  0.0,  15.0);
    glVertex3f( -10.0,  0.0,  -5.0);
  glEnd();

  // left wall
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -20.0, 15.0, -15.0);
    glVertex3f( -20.0,  0.0, -15.0);
    glVertex3f( -10.0,  0.0, -15.0);
    glVertex3f( -10.0, 15.0, -15.0);
  glEnd();

  // right wall
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -20.0, 15.0,  15.0);
    glVertex3f( -20.0,  0.0,  15.0);
    glVertex3f( -10.0,  0.0,  15.0);
    glVertex3f( -10.0, 15.0,  15.0);
  glEnd();

  // right under roof
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -20.0, 15.0, -15.0);
    glVertex3f( -10.0, 15.0, -15.0);
    glVertex3f( -15.0, 22.0, -15.0);
  glEnd();

  // left under roof
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -20.0, 15.0,  15.0);
    glVertex3f( -10.0, 15.0,  15.0);
    glVertex3f( -15.0, 22.0,  15.0);
  glEnd();

  glPushMatrix();
  GLfloat brown[4] = {0.627, 0.322, 0.176, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brown);

  // door
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -9.99,  0.0,  5.0);
    glVertex3f( -9.99,  0.0,  10.0);
    glVertex3f( -9.99,  7.0,  10.0);
    glVertex3f( -9.99,  7.0,  5.0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  GLfloat brown_trans[4] = {0.627, 0.322, 0.176, 0.5};
  glMaterialfv(GL_FRONT, GL_AMBIENT, brown_trans);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, brown_trans);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brown_trans);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // window
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -10.0,  5.0, -5.0);
    glVertex3f( -10.0,  5.0, -10.0);
    glVertex3f( -10.0, 12.0, -10.0);
    glVertex3f( -10.0, 12.0, -5.0);
  glEnd();
  glPopMatrix();

  glDisable(GL_BLEND);

  glPushMatrix();
  GLfloat red[4] = {0.698, 0.133, 0.133, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT, red);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glMaterialfv(GL_FRONT, GL_SPECULAR, red);
  // front roof
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -15.0, 22.0, -15.5);
    glVertex3f( -15.0, 22.0,  15.5);
    glVertex3f(  -9.0, 14.0,  15.5);
    glVertex3f(  -9.0, 14.0, -15.5);
  glEnd();

  // back roof
  glNormal3fv(normals[1]); 
  glBegin(GL_POLYGON);
    glVertex3f( -15.0, 22.0, -15.5);
    glVertex3f( -15.0, 22.0,  15.5);
    glVertex3f( -21.0, 14.0,  15.5);
    glVertex3f( -21.0, 14.0, -15.5);
  glEnd();
  glPopMatrix();
}
	
void SolidCubeWidget::sphere(float radios) {
  double loop = M_PI / 36;
  for (double theta = 0; theta <= M_PI; theta += loop) {
    QVector<point> temp;
    for (double phi = 0; phi <= 2*M_PI; phi += loop*2) {
      point t;
      t.r = qSin(6*phi + 1);
      t.g = qSin(6*phi + 3);
      t.b = qSin(6*phi + 4);
      t.x = qSin(theta)*qCos(phi)*radios;
      t.y = qSin(theta)*qSin(phi)*radios;
      t.z = qCos(theta)*radios;
      temp.prepend(t);
    }
    globe.prepend(temp);
  }
}

void SolidCubeWidget::super_sphere(float radios) {
  double loop = M_PI / 36;
  QVector< QVector<point> > temp_shape;
  for (double theta = -M_PI/2; theta <= M_PI/2; theta += loop) {
    QVector<point> temp;
    double r1 = super_formula(theta);
    for (double phi = -M_PI; phi <= M_PI; phi += loop*2) {
      point t;
      double r2 = super_formula(phi);
      t.r = qSin(2*theta + 0);
      t.g = qSin(2*theta + 2);
      t.b = qSin(2*theta + 4);
      t.x = r1 * qCos(theta) * r2 * qCos(phi) * radios;
      t.y = r1 * qCos(theta) * r2 * qSin(phi) * radios;
      t.z = r1 * qSin(theta) * radios;
      temp.prepend(t);
    }
    temp_shape.prepend(temp);
  }
  super_shape = temp_shape;
}

double  SolidCubeWidget::super_formula(float angle) {
  double part1 = qFabs(qCos(( m * angle ) / 4.0 ) / 1 );
  part1 = qPow(part1, n2);

  double part2 = qFabs(qSin(( m * angle ) / 4.0 ) / 1 );
  part2 = qPow(part2, n3);

  return qPow((part1 + part2), -1/n1);
}

void SolidCubeWidget::drawShape(QVector< QVector<point> > vec) {
  glDisable(GL_LIGHTING);
  glEnable(GL_SMOOTH);

  for (int i = 0; i < vec.size()-1; ++i) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int j = 0; j < vec[i].size(); ++j) {
      point t1 = vec[i][j];
      glColor3f(t1.r, t1.g, t1.b); 
      glVertex3f( t1.x, t1.y, t1.z);

      point t2 = vec[i+1][j];
      glColor3f(t2.r, t2.g, t2.b);
      glVertex3f( t2.x, t2.y, t2.z);
    }
    glEnd();
  }
  glEnable(GL_LIGHTING);
}

void SolidCubeWidget::change_super(float m_n, float n1_n, float n2_n, float n3_n) {
  m = m_n;
  n1 = n1_n;
  n2 = n2_n;
  n3 = n3_n; 
  this->super_sphere();
}

void SolidCubeWidget::system() {
  materialStruct *p_front = &brassMaterials;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,    p_front->ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,    p_front->diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,   p_front->specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,   p_front->shininess);

  materialStruct *planet1 = &emerald;
  materialStruct *planet2 = &earthGreen;
  materialStruct *planet3 = &red_plastic;
  
  glPushMatrix();
    glRotatef(planet3_angle, 0.0, 1.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    this->draw_tex(globe);
  glPopMatrix();

  // glutSolidSphere(7.0, 10, 10);

  // PLANET 1
  glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  planet1->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  planet1->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, planet1->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, planet1->shininess);
    planet1_angle += 1;
    if(planet1_angle > 360) {
      planet1_angle -= 360;
    }

    planet1_y += 0.005*planet1_y_goal;
    if(planet1_y > planet1_y_goal && planet1_y > planet1_y_goal*-1) {
      planet1_y_goal *= -1;
    } else if(planet1_y < planet1_y_goal && planet1_y < planet1_y_goal*-1) {
      planet1_y_goal *= -1;
    }
    glRotatef(planet1_angle, 0.0, 1.0, 0.0);
    glTranslatef(0.0, planet1_y, 15.0);
    glutSolidSphere(3.5, 7.0, 7.0);
  glPopMatrix();

  // PLANET 2
  glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  planet2->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  planet2->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, planet2->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, planet2->shininess);
    planet2_angle += 1.7;
    if(planet2_angle > 360) {
      planet2_angle -= 360;
    }

    planet2_x += 0.01*planet2_x_goal;
    if(planet2_x > planet2_x_goal && planet2_x > planet2_x_goal*-1) {
      planet2_x_goal *= -1;
    } else if(planet2_x < planet2_x_goal && planet2_x < planet2_x_goal*-1) {
      planet2_x_goal *= -1;
    }

    planet2_y += 0.005*planet2_y_goal;
    if(planet2_y > planet2_y_goal && planet2_y > planet2_y_goal*-1) {
      planet2_y_goal *= -1;
    } else if(planet2_y < planet2_y_goal && planet2_y < planet2_y_goal*-1) {
      planet2_y_goal *= -1;
    }
    glRotatef(planet2_angle, 0.0, 1.0, 0.0);
    glTranslatef(planet2_x, planet2_y, 25.0);
    glutSolidSphere(3.0, 7.0, 7.0);
  glPopMatrix();

  // PLANET 3
  glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  planet3->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  planet3->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, planet3->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, planet3->shininess);
    planet3_angle += 0.5;
    if(planet3_angle > 360) {
      planet3_angle -= 360;
    }

    planet3_y += 0.001*planet3_y_goal;
    if(planet3_y > planet3_y_goal && planet3_y > planet3_y_goal*-1) {
      planet3_y_goal *= -1;
    } else if(planet3_y < planet3_y_goal && planet3_y < planet3_y_goal*-1) {
      planet3_y_goal *= -1;
    }
    glRotatef(planet3_angle, 0.0, 1.0, 0.0);
    glTranslatef(planet3_y, 0.0, 40.0);
    glutSolidSphere(4.0, 7.0, 7.0);
  glPopMatrix();
}

void SolidCubeWidget::gen_texture() {
  int width = earth->size().width();
  int height = earth->size().height();

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      QColor c = QColor(earth->pixel(i, j));
      earth_tex[i][j][0] = c.red();
      earth_tex[i][j][1] = c.green();
      earth_tex[i][j][2] = c.blue();
    }
  }

  width = marc->size().width();
  height = marc->size().height();

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      QColor c = QColor(marc->pixel(i, j));
      marc_tex[i][j][0] = c.red();
      marc_tex[i][j][1] = c.green();
      marc_tex[i][j][2] = c.blue();
    }
  }
}

void SolidCubeWidget::draw_tex(QVector< QVector<point> > vec) {
  glPushMatrix();
  glDisable(GL_LIGHTING);
  
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, earth_tex);

  glPushMatrix();
  glTranslatef(0.0, 5.0, 0.0);
  float tex_step = 0.5 / vec.size(); 
  for (int i = 0; i < vec.size()-1; ++i) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int j = 0; j < vec[i].size(); ++j) {
      point t1 = vec[i][j];
      glTexCoord2f(i*tex_step, j*tex_step);
      glVertex3f( t1.x, t1.y, t1.z);

      point t2 = vec[i+1][j];
      glTexCoord2f(i*tex_step, j*tex_step);
      glVertex3f( t2.x, t2.y, t2.z);
    }
    glEnd();
  }
  glPopMatrix();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, marc_tex);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.,0);
    glVertex3f( -14.0, 22.0, -15.5);
    glTexCoord2f(1.,0.);
    glVertex3f( -14.0, 22.0,  15.5);
glTexCoord2f(1.,1.);
    glVertex3f(  -8.0, 14.0,  15.5);
    glTexCoord2f(0.,1.);
    glVertex3f(  -8.0, 14.0, -15.5);
  glEnd();

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
  glEnable(GL_LIGHTING);
  glPopMatrix();
}

// called every time the widget needs painting
void SolidCubeWidget::paintGL() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);        
  glEnable(GL_TEXTURE_2D);                                                                                     
  glShadeModel(GL_SMOOTH);
	glPushMatrix();
	glLoadIdentity();
    GLfloat light_pos[] = {0.0, 0.0, 100.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,15.);
	glPopMatrix();

  glRotatef(angle, 0.0, 1.0, 0.0);

  this->house();

  glPushMatrix();
    glTranslatef(0.0, 5.0, 30.0);
    globe_angle += 2;
    if(globe_angle > 360) {
      globe_angle -= 360;
    }
    glRotatef(globe_angle, 0.0, 0.0, 1.0);
    this->drawShape(globe);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.0, 5.0, -30.0);
    super_angle += 2;
    if(super_angle > 360) {
      super_angle -= 360;
    }
    glRotatef(super_angle, 0.0, 1.0, 0.0);
    this->drawShape(super_shape);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.0, -15.0, 0.0);
    this->system();
  glPopMatrix();

  glScalef(0.5, 0.5, 0.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  gluLookAt(10.0,4.0,0.0, 0.0,0.0,0.0, 0.0,1.0,0.0);     
	
	// flush to screen
	glFlush();	

} 