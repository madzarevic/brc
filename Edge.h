#ifndef _EDGE_H_
#define _EDGE_H_

#include <iostream>
#include <QPainterPath>
#include <QSharedPointer>
#include <QColor>
#include "Circle.h"
#include "Arc.h"
#include "Vertex.h"
#include "util.h"

class IEdge
{
public:
	IEdge(double width, bool visible) : mWidth(width), mVisible(visible) {}
	
	virtual ~IEdge() {}
	virtual IVertexSP vertex1() const = 0;
	virtual IVertexSP vertex2() const = 0;
	virtual double distance() const = 0;
	virtual QPainterPath painterPath() const = 0;
	virtual QString toString() const = 0;
	
	double width() const { return mWidth; }
	bool visible() const { return mVisible; }
	
private:
	double mWidth;
	bool mVisible;
};

typedef QSharedPointer<IEdge> IEdgeSP;

class EdgeLine : public IEdge
{
public:
	EdgeLine(IVertexSP vertex1, IVertexSP vertex2, double width, bool visible = true) :
		IEdge(width, visible),
		mVertex1(vertex1),
		mVertex2(vertex2),
		mDistance(euclidianDistance(vertex1->position(), vertex2->position()))
	{
		mPainterPath.moveTo(vertex1->position());
		mPainterPath.lineTo(vertex2->position());
	}
	
	IVertexSP vertex1() const { return mVertex1; }
	IVertexSP vertex2() const { return mVertex2; }
	double distance() const { return mDistance; }
	QPainterPath painterPath() const { return mPainterPath; }
	QString toString() const { return QString("%1 -> %2").arg(vertex1()->toString()).arg(vertex2()->toString()); }

private:
	IVertexSP mVertex1;
	IVertexSP mVertex2;
	double mDistance;
	QPainterPath mPainterPath;
};

class EdgeArc : public IEdge
{
public:
	EdgeArc(VertexCircularSP vertex1, VertexCircularSP vertex2, double width, bool visible = true) :
		IEdge(width, visible),
		mVertex1(vertex1),
		mVertex2(vertex2),
		mArc(createArc(vertex1->circle(), vertex1, vertex2))
	{}
	
	EdgeArc(CircleSP circle, VertexCircularSP vertex1, VertexCircularSP vertex2, double width, bool visible = true) :
		IEdge(width, visible),
		mVertex1(vertex1),
		mVertex2(vertex2),
		mArc(createArc(circle, vertex1, vertex2))
	{}
	
	IVertexSP vertex1() const { return mVertex1.dynamicCast<IVertex>(); }
	IVertexSP vertex2() const { return mVertex2.dynamicCast<IVertex>(); }
	ArcSP arc() const { return mArc; }
	double distance() const { return arc()->distance(); }
	QPainterPath painterPath() const { return arc()->painterPath(); }
	QString toString() const { return QString("%1 -> %2").arg(vertex1()->toString()).arg(vertex2()->toString()); }
	
public:
	static ArcSP createArc(CircleSP circle, VertexCircularSP vertex1, VertexCircularSP vertex2)
	{
		double startAngle = vertex1->angle(circle);
		double endAngle = vertex2->angle(circle);
		double spanAngle = endAngle - startAngle;
		
		if (spanAngle < -M_PI)
			spanAngle += 2 * M_PI;
		else if (spanAngle > M_PI)
			spanAngle -= 2 * M_PI;
		
		return ArcSP(new Arc(circle, startAngle, spanAngle));
	}

private:
	VertexCircularSP mVertex1;
	VertexCircularSP mVertex2;
	ArcSP mArc;
};

#endif
