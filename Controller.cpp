#include <iostream>
#include "Controller.h"
#include "Brc2013.h"
#include "Brc2014.h"

using namespace std;

Controller::Controller()
{
	mMap = MapSP(new Brc2014());
	std::cout << "vertices: " << mMap->vertexCount() << std::endl;
	std::cout << "edges: " << mMap->edgeCount() << std::endl;
	mFloydWarshall = FloydWarshallSP(new FloydWarshall(mMap));
}
	
void Controller::findShortestPath(QString start, QString destination) 
{
	QList<QString> path = QList<QString>() << start << mFloydWarshall->shortestPath(start, destination) << destination;
	std::cout << "Path (" << mFloydWarshall->distance(start, destination) << " ft): ";
	for (int i = 0; i < path.size(); i++)
	{
		std::cout << qPrintable(path.at(i));
		if (i < path.size() - 1)
			std::cout << " -> ";
	}
	std::cout << std::endl;
	emit newPath(path);
}