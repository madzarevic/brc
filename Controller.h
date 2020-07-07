#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <QList>
#include <QString>
#include "Map.h"
#include "FloydWarshall.h"

class Controller : public QObject
{
	Q_OBJECT
	
public:
	Controller();
	void init() { emit newMap(mMap); }
	
signals:
	void newMap(MapSP);
	void newPath(QList<QString>);
	
public slots:
	void findShortestPath(QString start, QString destination);
	
private:
	MapSP mMap;
	FloydWarshallSP mFloydWarshall;
};

#endif