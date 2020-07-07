#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include <QHash>
#include <QSharedPointer>
#include <QObject>
#include <QRectF>
#include "Vertex.h"
#include "Edge.h"

class Map
{
public:
	void clear();
	IVertexSP getVertex(QString name) const { return mVertices.value(name); }
	IEdgeSP getEdge(QString v1, QString v2) const { return mEdges.value(v1).value(v2); }
	bool addVertex(IVertexSP vertex);
	bool removeVertex(QString name);
	bool addEdge(IEdgeSP edge);
	bool removeEdge(QString vertex1, QString vertex2);
	QList<IVertexSP> vertexList() const { return mVertexList; }
	QList<IEdgeSP> edgeList() const { return mEdgeList; }
	int vertexCount() { return mVertices.size(); }
	int edgeCount() { return mEdgeList.size(); }
	bool edgeExists(QString v1, QString v2) const { return getEdge(v1, v2); }
	QRectF rect() const;

private:
	QHash<QString, IVertexSP> mVertices;
	QHash<QString, QHash<QString, IEdgeSP > > mEdges;
	QList<IEdgeSP> mEdgeList;
	QList<IVertexSP> mVertexList;
};

typedef QSharedPointer<Map> MapSP;

#endif
