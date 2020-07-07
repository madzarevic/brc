#include <iostream>
#include <limits>
#include "Matrix.h"
#include "FloydWarshall.h"

using namespace std;

FloydWarshall::FloydWarshall(MapSP map) : mMap(map)
{
	setup();
	buildShortestPaths();
}

void
FloydWarshall::setup()
{
	QList<IVertexSP> vertices = mMap->vertexList();
	int dimension = vertices.size();
	
	for (int i = 0; i < vertices.size(); i++)
	{
		mVertexIndices.insert(vertices.at(i)->name(), i);
		mVertexNames.append(vertices.at(i)->name());
	}
	
	mDist = Matrix<double>(dimension, dimension);
	mNext = Matrix<int>(dimension, dimension);

	for (int i = 0; i < vertices.size(); i++)
	{
		for (int j = 0; j < vertices.size(); j++)
		{
			IEdgeSP edge = mMap->getEdge(vertices.at(i)->name(), vertices.at(j)->name());
			if (edge)
			{
				mDist.setElement(i, j, edge->distance());
				mNext.setElement(i, j, -1);
			}
			else
			{
				mDist.setElement(i, j, i == j ? 0 : numeric_limits<double>::infinity());
				mNext.setElement(i, j, -1);
			}
		}
	}
}

void
FloydWarshall::buildShortestPaths()
{
	int dimension = mDist.rowSize();
	
	for (int k = 0; k < dimension; k++)
	{
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				double distance = mDist.element(i, k) + mDist.element(k, j);
				if (distance < mDist.element(i,j))
				{
					mDist.setElement(i, j, distance);
					mNext.setElement(i, j, k);
				}
			}
		}
		
	}
}

QList<QString>
FloydWarshall::shortestPath(QString start, QString destination) const
{
	int i = mVertexIndices.value(start);
	int j = mVertexIndices.value(destination);
	
	int k = mNext.element(i, j);
	if (k == -1)
		return QList<QString>();
	
	QString intermediate = mVertexNames.at(k);
	
	return QList<QString>() << shortestPath(start, intermediate) << intermediate << shortestPath(intermediate, destination);
}

double
FloydWarshall::distance(QString start, QString destination) const
{
	int i = mVertexIndices.value(start);
	int j = mVertexIndices.value(destination);
	return mDist.element(i, j);
}