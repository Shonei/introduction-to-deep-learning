#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QBoxLayout>
#include <QSpinBox>
#include "SolidCubeWidget.h"
#include <QHBoxLayout>
#include "iostream"

class SolidCubeWindow: public QWidget { 
	Q_OBJECT
	public:
		SolidCubeWindow(QWidget *parent);
		~SolidCubeWindow();
		QBoxLayout *windowLayout;
		QHBoxLayout *H_layout;
		SolidCubeWidget *cubeWidget;
		QSlider *nVerticesSlider;
		QDoubleSpinBox *m;
		QDoubleSpinBox *n1;
		QDoubleSpinBox *n2;
		QDoubleSpinBox *n3;
	private slots:
		void updateSuper();
}; 
	
#endif
