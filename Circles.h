#ifndef _CIRCLES_H_
#define _CIRCLES_H_

#include <iostream>
#include <QString>
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <QPainterPath>
#include <QRectF>
#include "PolarCoordinates.h"
#include "Circle.h"

class Circles
{
public:
	CircleSP circle(QString name) const { return mCircles.value(name); }
	double timeIncrement(QString name) const { return mTimeIncrements.value(name); }
	QList<QString> names() const { return mNames; }
	
protected:
	bool addCircle(CircleSP circle, double timeIncrement)
	{
		if (!circle)
			return false;

		QString name = circle->name();

		if (mCircles.contains(name))
			return false;
		
		mCircles.insert(name, circle);
		mTimeIncrements.insert(name, timeIncrement);
		mNames.append(name);
		return true;
	}
	
private:
	QHash<QString, double> mTimeIncrements;
	QHash<QString, CircleSP> mCircles;
	QList<QString> mNames;
};

#endif
