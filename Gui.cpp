#include <iostream>
#include <math.h>
#include <QPainter>
#include "util.h"
#include "Gui.h"

using namespace std;

static const double kBorder = 0.1;
//static const double kDistanceThreshold = 100;
static const double kDistanceThreshold = 100000;

static const double kStreetWidth = 30;
static const double kMinWidth = 5;
static const double kPathWidth = 30;
static const double kIntersectionRadius = 15;
static const double kSelectedRadius = 60;


Gui::Gui()
{
	setMouseTracking(true);
	setAutoFillBackground(true);
	setPalette(Qt::darkGray);
	setBackgroundRole(QPalette::Window);
}

void Gui::mouseMoveEvent(QMouseEvent* event)
{
	QPointF cursorPosition = QPointF(event->x()/mScaleX - mTranslateX, event->y()/mScaleY - mTranslateY);
	//cout << cursorPosition.x() << ", " << cursorPosition.y() << endl;
	QList<IVertexSP> vertexList = mMap->vertexList();
	double closestDistance = kDistanceThreshold;
	IVertexSP closestVertex;
	for (int i = 0; i < vertexList.size(); i++)
	{
		IVertexSP vertex = vertexList.at(i);
		double distance = euclidianDistance(cursorPosition, vertex->position());
		if (distance < kDistanceThreshold && distance < closestDistance)
		{
			closestVertex = vertex;
			closestDistance = distance;
		}
	}
	
	if (mHoveredVertex != closestVertex)
	{
		mHoveredVertex = closestVertex;
		if (mHoveredVertex && mHoveredVertex != mStartVertex && mStartVertex && !mDestinationVertex)
			emit shortestPathRequested(mStartVertex->name(), mHoveredVertex->name());
		update();
	}
}

void Gui::selectVertex()
{
	if (!mHoveredVertex)
		return;
	
	if (mStartVertex)
	{
		if (mHoveredVertex == mStartVertex && !mDestinationVertex)
		{
			// If the start vertex is selected, clear it
			mStartVertex = IVertexSP();
			mPath.clear();
			return;
		}
	
		if (mDestinationVertex)
		{
			// if a path is already set, start a new path
			mPath.clear();
			mStartVertex = mHoveredVertex;
			mDestinationVertex = IVertexSP();
			update();
		}
		else
		{
			// if no path exists, set this one
			mDestinationVertex = mHoveredVertex;
			update();
			emit shortestPathRequested(mStartVertex->name(), mDestinationVertex->name());
		}
	}
	else
	{
		mStartVertex = mHoveredVertex;
		update();
	}
}

void Gui::mouseReleaseEvent(QMouseEvent* event)
{
	selectVertex();
}

void Gui::paintEvent(QPaintEvent* event)
{
	QRectF rect = mMap->rect();
	
	double scaleX = width()/rect.width();
	double scaleY = height()/rect.height();
	double minScale = qMin(scaleX, scaleY) * (1 - kBorder);
	
	mScaleX = minScale;
	mScaleY = -minScale;
	
	mTranslateX = -rect.left() + rect.width() * (scaleX/minScale - 1) / 2;
	mTranslateY = -rect.bottom() - rect.height() * (scaleY/minScale - 1) / 2;
	
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.scale(mScaleX, mScaleY);
	painter.translate(mTranslateX, mTranslateY);

	QList<IEdgeSP> edgeList = mMap->edgeList();
	
	// Draw "invisible" paths
	painter.setPen(QPen(Qt::lightGray, kMinWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.setBrush(Qt::NoBrush);
	for (int i = 0; i < edgeList.size(); i++)
	{
		IEdgeSP edge = edgeList.at(i);
		if (!edge->visible())
			painter.drawPath(edge->painterPath());
	}
	
	// Draw "visible" paths
	painter.setPen(QPen(Qt::white, kStreetWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.setBrush(Qt::NoBrush);
	for (int i = 0; i < edgeList.size(); i++)
	{
		IEdgeSP edge = edgeList.at(i);
		if (edge->visible())
			painter.drawPath(edge->painterPath());
	}
	
	// Draw intersections
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	QList<IVertexSP> vertexList = mMap->vertexList();
	for (int i = 0; i < vertexList.size(); i++)
	{
		IVertexSP vertex = vertexList.at(i);
		painter.drawEllipse(vertex->position(), kIntersectionRadius, kIntersectionRadius);
	}
	
	// Draw current path
	painter.setPen(QPen(Qt::blue, kPathWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.setBrush(Qt::NoBrush);
	QString previous;
	for (int i = 0; i < mPath.size(); i++)
	{
		QString next = mPath.at(i);
		if (i > 0)
		{
			IEdgeSP edge = mMap->getEdge(previous, next);
			if (edge)
				painter.drawPath(edge->painterPath());
				
		}
		previous =  next;
	}
	
	// Draw starting vertex
	if (mStartVertex)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::green);
		painter.drawEllipse(mStartVertex->position(), kSelectedRadius, kSelectedRadius);
	}
	
	// Draw destination vertex
	if (mDestinationVertex)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::red);
		painter.drawEllipse(mDestinationVertex->position(), kSelectedRadius, kSelectedRadius);
	}
	
	// Draw selected vertex
	if (mHoveredVertex)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush((mStartVertex && !mDestinationVertex) ? Qt::red : Qt::green);
		painter.drawEllipse(mHoveredVertex->position(), kSelectedRadius, kSelectedRadius);
	}
}
