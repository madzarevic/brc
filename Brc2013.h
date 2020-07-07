#ifndef _BRC2013_H_
#define _BRC2013_H_

#include <iostream>
#include <QHash>
#include <QSharedPointer>
#include "Circles.h"
#include "Map.h"
#include "BrcCoordinates.h"

class Circles2013 : public Circles
{
public:
	Circles2013();
};

class Brc2013 : public Map
{
public:
	Brc2013() { build(); }
	
private:
	bool build();
	bool buildBasicBlocks();
	bool buildBigBlocks();
	bool buildCenterCamp();
	bool buildRodsRoadStuff(QString circleName, CircleSP rodsRoad);
	bool buildPlaya();

public:
	static double circleIntersection(double radius1, double radius2, double distance);
	static const Circles2013 circles() { return kCircles; }
	
private:
	static const Circles2013 kCircles;
	static const double kStreetWidth;
	static const double kFootpathWidth;
};

class BrcCoordinates2013: public BrcCoordinates
{
public:
	BrcCoordinates2013(double time, QString circleName) : BrcCoordinates(time, Brc2013::circles().circle(circleName)) {}
};

#endif

