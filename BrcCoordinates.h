#ifndef _BRCCOORDINATES_H_
#define _BRCCOORDINATES_H_

#include <math.h>
#include <QString>
#include "Circle.h"
#include "PolarCoordinates.h"

class BrcCoordinates
{
public:
	BrcCoordinates(double time, CircleSP circle) : mTime(time), mCircle(circle) {}
	double time() const { return mTime; }
	CircleSP circle() const { return mCircle; }
	double angle() const { return timeToAngle(time()); }
	QString toString() const { return QString("%1 & %2").arg(timeToString(time())).arg(circle()->name()); }
	QPointF position() const { return circle()->edgePoint(angle()); }

public:
	static QString timeToString(double time)
	{
		int hour = static_cast<int>(time);
		int minute = static_cast<int>((time - hour) * 60.0 + 0.5);
		return QString("%1:%2").arg(hour).arg(minute, 2, 10, QChar('0'));
	}
	
	static double timeToAngle(double time)
	{
		time = 3.0 - time;
		if (time < 0)
			time += 12.0;
		return time * M_PI / 6.0;
	}
	
private:
	double mTime;
	CircleSP mCircle;
};

#endif
