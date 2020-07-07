#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <QSharedPointer>
#include <QHash>
#include <QColor>
#include "BrcCoordinates.h"
#include "Circle.h"

class IVertex
{
public:
	virtual ~IVertex() {}
	virtual QString name() const = 0;
	virtual QPointF position() const = 0;
	virtual QString toString() const = 0;
};

typedef QSharedPointer<IVertex> IVertexSP;

class VertexPolar : public IVertex
{
public:
	VertexPolar(QString name, PolarCoordinates polarCoordinates) :
		mName(name),
		mPolarCoordinates(polarCoordinates)
	{}

	QString name() const { return mName; }
	PolarCoordinates polarCoordinates() const { return mPolarCoordinates; }
	QPointF position() const { return polarCoordinates().cartesianCoordinates(); }
	QString toString() const { return QString("%1 %2,%3").arg(name()).arg(position().x()).arg(position().y()); }
	
private:
	QString mName;
	PolarCoordinates mPolarCoordinates;
};

class VertexCircular : public IVertex
{
public:
	VertexCircular(BrcCoordinates brc) : mName(brc.toString()) { addAngle(brc.circle(), brc.angle()); }
	VertexCircular(QString name, CircleSP circle, double angle) : mName(name) { addAngle(circle, angle); }

	QString name() const { return mName; }
	void addAngle(CircleSP circle, double angle) { mAngles.insert(circle, angle); }
	CircleSP circle() const { return mAngles.keys().at(0); }
	double angle() const { return mAngles.value(circle()); }
	double angle(CircleSP circle, bool* ok = NULL) const
	{
		if (ok)
			*ok = mAngles.contains(circle);
		
		return mAngles.value(circle, 0);
	}
	QPointF position() const { return circle()->edgePoint(angle()); }
	QString toString() const { return QString("%1 %2,%3").arg(name()).arg(position().x()).arg(position().y()); }
	
private:
	QString mName;
	QHash<CircleSP, double> mAngles;
};

typedef QSharedPointer<VertexCircular> VertexCircularSP;

#endif
