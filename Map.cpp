#include <iostream>
#include <limits>
#include <QHash>
#include <QSharedPointer>
#include <QObject>
#include "Vertex.h"
#include "Edge.h"
#include "Map.h"
#include "Matrix.h"

void Map::clear()
{
	mVertices.clear();
	mEdges.clear();
	mEdgeList.clear();
}

QRectF Map::rect() const
{
	double left = numeric_limits<double>::infinity();
	double right = -numeric_limits<double>::infinity();
	double top = numeric_limits<double>::infinity();
	double bottom = -numeric_limits<double>::infinity();
	for (int i = 0; i < mVertexList.size(); i++)
	{
		QPointF position = mVertexList.at(i)->position();
		
		if (position.x() < left)
			left = position.x();
		
		if (position.x() > right)
			right = position.x();

		if (position.y() < top)
			top = position.y();
		
		if (position.y() > bottom)
			bottom = position.y();
	}
	return QRectF(left, top, right - left, bottom - top);
}

bool Map::addVertex(IVertexSP vertex)
{
	if (mVertices.contains(vertex->name()))
		return false;

	mVertices.insert(vertex->name(), vertex);
	mVertexList.append(vertex);
	return true;
}

bool Map::removeEdge(QString vertex1, QString vertex2)
{
	if (!mVertices.contains(vertex1))
		return false;

	if (!mVertices.contains(vertex2))
		return false;

	if (!mEdges.contains(vertex1))
		return false;

	QHash<QString, IEdgeSP> hash1 = mEdges.value(vertex1);
	if (!hash1.contains(vertex2))
		return false;

	IEdgeSP edge = hash1.take(vertex2);
	if (hash1.isEmpty())
		mEdges.remove(vertex1);
	else
		mEdges.insert(vertex1, hash1);

	QHash<QString, IEdgeSP> hash2 = mEdges.value(vertex2);
	hash2.remove(vertex1);
	if (hash2.isEmpty())
		mEdges.remove(vertex2);
	else
		mEdges.insert(vertex2, hash2);

	mEdgeList.removeAll(edge);
	return true;
}

bool Map::removeVertex(QString name)
{
	if (!mVertices.contains(name))
		return false;

	QList<IEdgeSP> list = mEdges.value(name).values();
	for (int i = 0; i < list.size(); i++)
	{
		IEdgeSP edge = list.at(i);
		removeEdge(edge->vertex1()->name(), edge->vertex2()->name());
	}
	IVertexSP vertex = mVertices.take(name);
	mVertexList.removeAll(vertex);
	return true;
}


bool Map::addEdge(IEdgeSP edge)
{
	if (!edge)
		return false;

	QString vertex1 = edge->vertex1()->name();
	QString vertex2 = edge->vertex2()->name();
	
	if (edgeExists(vertex1, vertex2))
		return false;

	mEdges[vertex1][vertex2] = edge;
	mEdges[vertex2][vertex1] = edge;
	mEdgeList.append(edge);

	return true;
}
