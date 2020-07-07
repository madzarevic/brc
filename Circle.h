#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include <QString>
#include <QSharedPointer>
#include <QRectF>
#include "PolarCoordinates.h"

class Circle
{
public:
	Circle(QString name, double radius) : mName(name), mCenter(PolarCoordinates(0, 0)), mRadius(radius) {}
	Circle(QString name, PolarCoordinates center, double radius) : mName(name), mCenter(center), mRadius(radius) {}
	QString name() const { return mName; }
	PolarCoordinates center() const { return mCenter; }
	double radius() const { return mRadius; }
	QPointF edgePoint(double angle) const { return PolarCoordinates(angle, radius()).cartesianCoordinates() + center().cartesianCoordinates(); }
	QRectF rectangle() const { return QRectF(center().cartesianCoordinates() + QPointF(-radius(), radius()), QSizeF(2*radius(), -2*radius())); }
	//bool operator==(const Circle& other) const { return mCenter == other.mCenter && mRadius == other.mRadius; }
	//bool operator!=(const Circle& other) const { return !operator==(other); }

private:
	QString mName;
	PolarCoordinates mCenter;
	double mRadius;
};

typedef QSharedPointer<Circle> CircleSP;

inline uint qHash(const CircleSP& key) { return qHash(reinterpret_cast<quint64>(key.data())); }  

#endif