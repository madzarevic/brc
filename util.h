#ifndef _UTIL_H_
#define _UTIL_H_

#include <math.h>
#include <QPointF>

inline double arcDistance(double radius, double spanAngle)
{
	return fabs(radius * spanAngle);
}

inline double arcDistance(double radius, double startAngle, double endAngle)
{
	return arcDistance(radius, endAngle - startAngle);
}

inline double euclidianDistance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx  + dy * dy);
}

inline double euclidianDistance(QPointF p1, QPointF p2)
{
	return euclidianDistance(p1.x(), p1.y(), p2.x(), p2.y());
}

#endif // #indef _UTIL_H_