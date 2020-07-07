#include <iostream>
#include <QApplication>
#include <QObject>
#include "Brc2013.h"
#include "Gui.h"
#include "Controller.h"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	Controller controller;
	
	Gui gui;
	
	qRegisterMetaType< QList<QString> >("QList<QString>");
	qRegisterMetaType<MapSP>("MapSP");
	QObject::connect(&controller, SIGNAL(newMap(MapSP)), &gui, SLOT(setMap(MapSP)), Qt::QueuedConnection);
	QObject::connect(&controller, SIGNAL(newPath(QList<QString>)), &gui, SLOT(setPath(QList<QString>)), Qt::QueuedConnection);
	QObject::connect(&gui, SIGNAL(shortestPathRequested(QString, QString)), &controller, SLOT(findShortestPath(QString, QString)), Qt::QueuedConnection);
	
	gui.show();
	
	controller.init();
	
	return app.exec();
}
