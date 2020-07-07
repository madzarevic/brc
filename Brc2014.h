#ifndef _BRC2014_H_
#define _BRC2014_H_

#include <iostream>
#include <QHash>
#include <QSharedPointer>
#include "Circles.h"
#include "Map.h"
#include "BrcCoordinates.h"

class Circles2014 : public Circles
{
public:
	Circles2014();
};

class Brc2014 : public Map
{
public:
	Brc2014() { build(); }
	
private:
	bool build();
	bool buildBasicBlocks();
	bool buildBigBlocks();
	bool buildCenterCamp();
	bool buildRodsRoadStuff(QString circleName, CircleSP rodsRoad);
	bool buildPlaya();

public:
	static double circleIntersection(double radius1, double radius2, double distance);
	static const Circles2014 circles() { return kCircles; }
	
private:
	static const Circles2014 kCircles;
	static const double kStreetWidth;
	static const double kFootpathWidth;
};

class BrcCoordinates2014: public BrcCoordinates
{
public:
	BrcCoordinates2014(double time, QString circleName) : BrcCoordinates(time, Brc2014::circles().circle(circleName)) {}
};

#endif

