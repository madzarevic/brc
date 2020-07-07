#ifndef _POLARCOORDINATES_H_
#define _POLARCOORDINATES_H_

#include <math.h>
#include <QPointF>
#include <QString>

class PolarCoordinates
{
public:
	PolarCoordinates(double angle, double radius) : mAngle(angle), mRadius(radius) {} 
	QPointF cartesianCoordinates() const { return toCartesianCoordinates(angle(), radius()); }
	double angle() const { return mAngle; }
	double radius() const { return mRadius; }
	QString toString() const { return QString("%1,%2").arg(angle()).arg(radius()); }
	//bool operator==(const PolarCoordinates& other) const { return mAngle == other.mAngle && mRadius == other.mRadius; }
	//bool operator!=(const PolarCoordinates& other) const { return !operator==(other); }

public:
	static QPointF toCartesianCoordinates(double angle, double radius) { return QPointF(radius * cos(angle), radius * sin(angle)); }

private:
	double mAngle;
	double mRadius;
};

#endif
