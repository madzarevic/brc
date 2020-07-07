#ifndef _GUI_H_
#define _GUI_H_

#include <QWidget>
#include <QString>
#include <QList>
#include <QMouseEvent>
#include "Map.h"

class Gui : public QWidget
{
	Q_OBJECT
public:
	Gui();
	
signals:
	void shortestPathRequested(QString v1, QString v2);
	
public slots:
	void setPath(QList<QString> path) { mPath = path; update(); }
	void setMap(MapSP map) { mMap = map; update(); }
	
private slots:
	void selectVertex();
	
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* event);
	
private:
	MapSP mMap;
	double mTranslateX;
	double mTranslateY;
	double mScaleX;
	double mScaleY;
	QList<QString> mPath;
	//IVertexSP mSelectedVertex;
	IVertexSP mHoveredVertex;
	IVertexSP mStartVertex;
	IVertexSP mDestinationVertex;
};

#endif
