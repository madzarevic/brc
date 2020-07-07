#include <iostream>
#include <QHash>
#include <QSharedPointer>
#include <QObject>
#include "Vertex.h"
#include "Edge.h"
#include "Map.h"
#include "Brc2014.h"

using namespace std;

Circles2014::Circles2014()
{
	addCircle(CircleSP(new Circle("Esplanade", 2520)), 0.50);
	addCircle(CircleSP(new Circle("A", 3000)), 0.50);
	addCircle(CircleSP(new Circle("B", 3240)), 0.50);
	addCircle(CircleSP(new Circle("C", 3480)), 0.50);
	addCircle(CircleSP(new Circle("D", 3720)), 0.50);
	addCircle(CircleSP(new Circle("E", 3960)), 0.50);
	addCircle(CircleSP(new Circle("F", 4200)), 0.50);
	addCircle(CircleSP(new Circle("G", 4420)), 0.25);
	addCircle(CircleSP(new Circle("H", 4680)), 0.25);
	addCircle(CircleSP(new Circle("I", 4920)), 0.25);
	addCircle(CircleSP(new Circle("J", 5160)), 0.25);
	addCircle(CircleSP(new Circle("K", 5400)), 0.25);
	addCircle(CircleSP(new Circle("L", 5640)), 0.25);
}

const Circles2014 Brc2014::kCircles;
const double Brc2014::kStreetWidth = 40;
const double Brc2014::kFootpathWidth = 5;

bool Brc2014::build()
{
	try
	{
		if (!buildBasicBlocks())
			throw -1;
		
		if (!buildBigBlocks())
			throw -1;
		
		if (!buildCenterCamp())
			throw -1;
		
		if (!buildPlaya())
			throw -1;
	}
	catch (int code)
	{
		clear();
		return false;
	}
	
	return true;
}

double Brc2014::circleIntersection(double radius1, double radius2, double distance)
{
	return acos((-radius1*radius1 + radius2*radius2 + distance*distance)/(2*radius2*distance));
}

bool Brc2014::buildBasicBlocks()
{
	QList<QString> circles = Brc2014::circles().names();
	
	bool firstCircle = true;
	for (int i = 0; i < circles.size(); i++)
	{
		QString circle = circles.at(i);
		double timeIncrement = Brc2014::circles().timeIncrement(circle); 
		bool firstTime = true;
		VertexCircularSP previousVertex = VertexCircularSP(NULL);
		for (double time = 2.0; time <= 10.0; time += timeIncrement)
		{
			VertexCircularSP vertex = VertexCircularSP(new VertexCircular(BrcCoordinates2014(time, circle)));
			addVertex(vertex.dynamicCast<IVertex>());
			if (!firstTime)
			{
				IEdgeSP edge = IEdgeSP(new EdgeArc(previousVertex, vertex, Brc2014::kStreetWidth));
				addEdge(edge);
			}
			firstTime = false;

			if (!firstCircle)
			{
				BrcCoordinates2014 inner(time, circles.at(i - 1));
				IVertexSP innerVertex = getVertex(inner.toString());
				if (innerVertex)
				{
					IEdgeSP edge = IEdgeSP(new EdgeLine(vertex, innerVertex, Brc2014::kStreetWidth));
					addEdge(edge);
				}
			}

			previousVertex = vertex;
		}

		firstCircle = false;
	}
	return true;
}

bool Brc2014::buildBigBlocks()
{
	removeEdge("5:00 & F", "5:30 & F");
	removeEdge("5:30 & F", "6:00 & F");
	removeEdge("6:00 & F", "6:30 & F");
	removeEdge("6:30 & F", "7:00 & F");

	removeVertex("5:30 & F");
	removeVertex("6:00 & F");
	removeVertex("6:30 & F");

	addEdge(IEdgeSP(new EdgeLine(getVertex("5:30 & E"), getVertex("5:30 & G"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("6:00 & E"), getVertex("6:00 & G"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("6:30 & E"), getVertex("6:30 & G"), Brc2014::kStreetWidth)));

	removeVertex("2:00 & F");
	addEdge(IEdgeSP(new EdgeLine(getVertex("2:00 & E"), getVertex("2:00 & G"), Brc2014::kStreetWidth)));
	removeEdge("3:30 & F", "4:00 & F");
	removeEdge("8:00 & F", "8:30 & F");
	removeVertex("10:00 & F");
	addEdge(IEdgeSP(new EdgeLine(getVertex("10:00 & E"), getVertex("10:00 & G"), Brc2014::kStreetWidth)));
	
	return true;
}

bool Brc2014::buildCenterCamp()
{
	removeVertex("6:00 & Esplanade");
	removeVertex("6:00 & A");
	removeVertex("6:00 & B");
	removeVertex("6:00 & C");
	removeEdge("5:30 & Esplanade", "5:30 & A");
	removeEdge("6:30 & Esplanade", "6:30 & A");
	
	double theta = M_PI / 12.0;
	double dRadius = Brc2014::circles().circle("D")->radius();
	double rodsRoadRadius = dRadius * sin(theta) / (1 + sin(theta));
	double centerCampRadius = rodsRoadRadius - 480;
	double centerCampY = dRadius - rodsRoadRadius;
	CircleSP rodsRoad = CircleSP(new Circle("Rod's Road", PolarCoordinates(3*M_PI/2, centerCampY), rodsRoadRadius));
	CircleSP centerCamp = CircleSP(new Circle("Center Camp", PolarCoordinates(3*M_PI/2, centerCampY), centerCampRadius));
	
	getVertex("6:00 & D").dynamicCast<VertexCircular>()->addAngle(rodsRoad, 3.0 * M_PI / 2.0);
	addVertex(IVertexSP(new VertexCircular("Rod's Road East & 5:30", rodsRoad, M_PI / 12.0)));
	addVertex(IVertexSP(new VertexCircular("Rod's Road West & 6:30", rodsRoad, 11.0 * M_PI / 12.0)));
	addVertex(IVertexSP(new VertexCircular("Rod's Road North", rodsRoad, M_PI / 2.0)));
	
	addVertex(IVertexSP(new VertexCircular("Center Camp East", centerCamp, 0)));
	addVertex(IVertexSP(new VertexCircular("Center Camp North", centerCamp, M_PI / 2.0)));
	addVertex(IVertexSP(new VertexCircular("Center Camp West", centerCamp, M_PI)));
	addVertex(IVertexSP(new VertexCircular("Center Camp South", centerCamp, 3.0 * M_PI / 2.0)));

	buildRodsRoadStuff("Esplanade", rodsRoad);
	buildRodsRoadStuff("A", rodsRoad);
	buildRodsRoadStuff("B", rodsRoad);
	buildRodsRoadStuff("C", rodsRoad);
	
	addEdge(IEdgeSP(new EdgeLine(getVertex("5:30 & A"), getVertex("Rod's Road East & 5:30"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("Rod's Road East & 5:30"), getVertex("5:30 & Esplanade"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("6:30 & A"), getVertex("Rod's Road West & 6:30"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("Rod's Road West & 6:30"), getVertex("6:30 & Esplanade"), Brc2014::kStreetWidth)));
	
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("6:00 & D").dynamicCast<VertexCircular>(), getVertex("Rod's Road East & C").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("6:00 & D").dynamicCast<VertexCircular>(), getVertex("Rod's Road West & C").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road East & C").dynamicCast<VertexCircular>(), getVertex("Rod's Road East & B").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road West & C").dynamicCast<VertexCircular>(), getVertex("Rod's Road West & B").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road East & B").dynamicCast<VertexCircular>(), getVertex("Rod's Road East & A").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road West & B").dynamicCast<VertexCircular>(), getVertex("Rod's Road West & A").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road East & A").dynamicCast<VertexCircular>(), getVertex("Rod's Road East & 5:30").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road West & A").dynamicCast<VertexCircular>(), getVertex("Rod's Road West & 6:30").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road East & 5:30").dynamicCast<VertexCircular>(), getVertex("Rod's Road East & Esplanade").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road West & 6:30").dynamicCast<VertexCircular>(), getVertex("Rod's Road West & Esplanade").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road East & Esplanade").dynamicCast<VertexCircular>(), getVertex("Rod's Road North").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(rodsRoad, getVertex("Rod's Road West & Esplanade").dynamicCast<VertexCircular>(), getVertex("Rod's Road North").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));

	addEdge(IEdgeSP(new EdgeLine(getVertex("Center Camp East"), getVertex("Rod's Road East & A"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("Center Camp North"), getVertex("Rod's Road North"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("Center Camp West"), getVertex("Rod's Road West & A"), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeLine(getVertex("Center Camp South"), getVertex("6:00 & D"), Brc2014::kStreetWidth)));

	addEdge(IEdgeSP(new EdgeArc(getVertex("Center Camp East").dynamicCast<VertexCircular>(), getVertex("Center Camp South").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(getVertex("Center Camp North").dynamicCast<VertexCircular>(), getVertex("Center Camp East").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(getVertex("Center Camp West").dynamicCast<VertexCircular>(), getVertex("Center Camp North").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));
	addEdge(IEdgeSP(new EdgeArc(getVertex("Center Camp South").dynamicCast<VertexCircular>(), getVertex("Center Camp West").dynamicCast<VertexCircular>(), Brc2014::kStreetWidth)));

	return true;
}

bool Brc2014::buildRodsRoadStuff(QString circleName, CircleSP rodsRoad)
{
	CircleSP circle = Brc2014::circles().circle(circleName);
	double angle1 = circleIntersection(rodsRoad->radius(), circle->radius(), rodsRoad->center().radius());
	double angle2 = circleIntersection(circle->radius(), rodsRoad->radius(), rodsRoad->center().radius());
	VertexCircularSP vertexE(new VertexCircular(QString("Rod's Road East & %1").arg(circleName), circle, 3.0 *M_PI / 2.0 + angle1));
	VertexCircularSP vertexW(new VertexCircular(QString("Rod's Road West & %1").arg(circleName), circle, 3.0 *M_PI / 2.0 - angle1));
	vertexE->addAngle(rodsRoad, M_PI / 2.0 - angle2);
	vertexW->addAngle(rodsRoad, M_PI / 2.0 + angle2);
	addVertex(vertexE.dynamicCast<IVertex>());
	addVertex(vertexW.dynamicCast<IVertex>());
	
	addEdge(IEdgeSP(new EdgeArc(getVertex(QString("5:30 & %1").arg(circleName)).dynamicCast<VertexCircular>(), vertexE, Brc2014::kStreetWidth))); 
	addEdge(IEdgeSP(new EdgeArc(getVertex(QString("6:30 & %1").arg(circleName)).dynamicCast<VertexCircular>(), vertexW, Brc2014::kStreetWidth)));
	return true;
}

bool Brc2014::buildPlaya()
{
	QList<QString> circles = Brc2014::circles().names();
	
	for (int i = 0; i < circles.size(); i++)
	{
		QString circleI = circles.at(i);
		if (circleI == "F" || circleI == "Esplanade")
			continue;
		
		for (int j = 0; j < circles.size(); j++)
		{
			QString circleJ = circles.at(j);
			if (circleJ == "F" || circleJ == "Esplanade")
				continue;

			addEdge(IEdgeSP(new EdgeLine(getVertex(QString("2:00 & %1").arg(circleI)), getVertex(QString("10:00 & %1").arg(circleJ)), Brc2014::kFootpathWidth, false)));
		}
	}
	
	for (int i = 0; i < circles.size(); i++)
	{
		QString circle = circles.at(i);
		for (double time = 2.0; time < 4.0; time += 0.5)
		{
			if (circle == "F" || circle == "Esplanade")
				continue;
			
			addEdge(IEdgeSP(new EdgeLine(getVertex(QString("10:00 & %1").arg(circle)), getVertex(BrcCoordinates2014(time, "Esplanade").toString()), Brc2014::kFootpathWidth, false)));
		}
		
		for (double time = 10.0; time > 8.0; time -= 0.5)
		{
			if (circle == "F" || circle == "Esplanade")
				continue;
			
			addEdge(IEdgeSP(new EdgeLine(getVertex(QString("2:00 & %1").arg(circle)), getVertex(BrcCoordinates2014(time, "Esplanade").toString()), Brc2014::kFootpathWidth, false)));
		}
	}
	
	QList<IVertexSP> vertexList;
	for (double time = 2.0; time <= 10.0; time += 0.5)
	{
		if (time >= 5.5 && time <= 6.5)
		{
			/*
			vertexList.append(getVertex("Rod's Road East & Esplanade"));
			vertexList.append(getVertex("Rod's Road North"));
			vertexList.append(getVertex("Rod's Road West & Esplanade"));
			*/
		}
		else
			vertexList.append(getVertex(BrcCoordinates2014(time, "Esplanade").toString()));
	}
	for (int i = 0; i < vertexList.size(); i++)
	{
		for (int j = i + 1; j < vertexList.size(); j++)
			addEdge(IEdgeSP(new EdgeLine(vertexList.at(i), vertexList.at(j), Brc2014::kFootpathWidth, false)));
	}
	
	for (double time = 2.0; time <= 10.0; time += 0.5)
	{
		if (time < 5.5 || time > 6.5)
			addEdge(IEdgeSP(new EdgeLine(getVertex("Rod's Road North"), getVertex(BrcCoordinates2014(time, "Esplanade").toString()), Brc2014::kFootpathWidth, false)));
	}
	
	for (double time = 2.0; time <= 10.0; time += 0.5)
	{
		if (time < 5.5 || time > 8.0)
			addEdge(IEdgeSP(new EdgeLine(getVertex("5:30 & Esplanade"), getVertex(BrcCoordinates2014(time, "Esplanade").toString()), Brc2014::kFootpathWidth, false)));
	}
	
	for (double time = 2.0; time <= 10.0; time += 0.5)
	{
		if (time > 6.5 || time < 4.0)
			addEdge(IEdgeSP(new EdgeLine(getVertex("6:30 & Esplanade"), getVertex(BrcCoordinates2014(time, "Esplanade").toString()), Brc2014::kFootpathWidth, false)));
	}
	
	return true;
}