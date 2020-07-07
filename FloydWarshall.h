#ifndef _FLOYDWARSHALL_H_
#define _FLOYDWARSHALL_H_

#include <QList>
#include <QString>
#include <QHash>
#include <QSharedPointer>
#include "Matrix.h"
#include "Map.h"

class FloydWarshall
{
public:
	FloydWarshall(MapSP map);
	QList<QString> shortestPath(QString start, QString destination) const;
	double distance(QString start, QString destination) const;
	
private:
	void setup();
	void buildShortestPaths();
	
private:
	MapSP mMap;
	Matrix<double> mDist;
	Matrix<int> mNext;
	QHash<QString, int> mVertexIndices;
	QList<QString> mVertexNames;
};

typedef QSharedPointer<FloydWarshall> FloydWarshallSP;

#endif