#ifndef _ARC_H_
#define _ARC_H_

#include <QSharedPointer>
#include <QPainterPath>
#include "PolarCoordinates.h"
#include "Circle.h"
#include "util.h"

class Arc
{
public:
	Arc(CircleSP circle, double startAngle, double spanAngle) : mCircle(circle), mStartAngle(startAngle), mSpanAngle(spanAngle)
	{
		double radius = circle->radius();
		mDistance = arcDistance(radius, spanAngle);
		double startDegrees = startAngle * 180 / M_PI;
		double spanDegrees = spanAngle * 180 / M_PI;

		mPainterPath.arcMoveTo(circle->rectangle(), startDegrees);
		mPainterPath.arcTo(circle->rectangle(), startDegrees, spanDegrees);
	}
	CircleSP circle() const { return mCircle; }
	double startAngle() const { return mStartAngle; }
	double spanAngle() const { return mSpanAngle; }
	double endAngle() const { return startAngle() + spanAngle(); }
	double distance() const { return mDistance; }
	QPainterPath painterPath() const { return mPainterPath; }

private:
	CircleSP mCircle;
	double mStartAngle;
	double mSpanAngle;
	double mDistance;
	QPainterPath mPainterPath;
};

typedef QSharedPointer<Arc> ArcSP;

#endif