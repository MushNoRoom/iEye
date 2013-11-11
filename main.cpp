#include <QApplication>
#include <QThread>
#include <QScopedPointer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <stdio.h>
#include "highgui.h"
#include "highgui/highgui.hpp"
#include "opencv.hpp"
#include "iEyeAppMainWindow.h"
#include "iEyeDotWindow.h"
#include "ieyeimgproc.h"
#include "iEyeProcessor.h"
#include "iEyeCalibrator.h"
#include "iEyePosMonitor.h"
#include "ieye3by3quantizedmonitor.h"
#include "iEyeMappingPosMonitor.h"
#include "PortListener.h"

//#include "ieyeimgproc.h"

int MAX_KERNEL_LENGTH = 7;
//
//int FRAME_WIDTH = 160;
//int FRAME_HEIGHT = 120;


bool iEye::iEyePosMonitor::kb_en;
bool iEye::iEyePosMonitor::tux_flag;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    iEye::iEyeAppMainWindow w("D:/McMaster_WS/4OI6_repo/Qt_App/iEye_Qt_App/LANDS_UHD.jpg");


//    iEye::iEyeAppMainWindow w();
	

	
//	iEye::iEyeDotWindow dotWinow(0);
	

	
	using namespace cv;

	//cv::VideoCapture cap(0); // open the default camera
    //cap.open(0);

//	Mat test_img(imread("13_sw.bmp", -1));
//	iEye::windowScanMin(test_img, cv::Size(8, 8));
//	cv::namedWindow("Eye_Grey");
    iEye::iEyePosMonitor::kb_en = true;
    iEye::iEyePosMonitor::tux_flag = true;


    QScopedPointer<QThread> thread(new QThread());

    QScopedPointer<iEye::iEyeProcessor> my_processor(new iEye::iEyeProcessor);
	iEye::Calibrator cal(my_processor.data(), 0);
    QScopedPointer<iEye::iEyePosMonitor> monitor(new iEye::iEye3by3QuantizedMonitor(my_processor.data()));

//    QScopedPointer<iEye::iEyePosMonitor> monitor(new iEye::iEyeMappingPosMonitor(my_processor.data()));
//    QObject::connect(&cal, SIGNAL(newRegionAvailable(const QRect&)), monitor.data(), SLOT(setNewRegion(const QRect&)));
    QObject::connect(&cal, SIGNAL(newRegionAvailable(const QRect&, const QPoint&)), monitor.data(), SLOT(setNewRegion(const QRect&, const QPoint&)));
    QObject::connect(my_processor->portListener(), SIGNAL(blinkDetected(int)), &w, SLOT(swapBackground()));
    //   QObject::connect(&cal, SIGNAL(newRegionAvailable(const QRect&)), &dotWinow, SLOT(updateScale(const QRect&)));
    QObject::connect(monitor.data(), SIGNAL(moveScene(iEye::SceneMovement, int)), &w, SLOT(moveScene(iEye::SceneMovement, int)));
//    QObject::connect(my_processor->portListener(), SIGNAL(blinkDetected(int)), &dotWinow, SLOT(onBlink(int)));
//    QObject::connect(monitor.data(), SIGNAL(moveCursor(iEye::SceneMovement, iEye::SceneMovement)), &dotWinow, SLOT(moveDot(iEye::SceneMovement, iEye::SceneMovement)));
	//! Connect the calibrator's signal to the app window for region information update
	//QObject::connect(&cal, SIGNAL(newRegionAvailable(const QRect&)), &w, SLOT(setNewRegion(const QRect&)));
	my_processor->setWindow("Eye_Grey");
    //my_processor->startHW();
    my_processor->moveToThread(thread.data());
    thread->start();
    //PortListener portListener( QLatin1String("COM9"));
    //QObject::connect(my_processor->portListener(), SIGNAL(eyeChanged(int, int)), &dotWinow, SLOT(setDotPos(int, int)));
    //QObject::connect(&portListener, SIGNAL(eyeChanged(int, int)), &dotWinow, SLOT(setDotPos(int, int)));
//    QMetaObject::invokeMethod(my_processor.data(), "startCam", Qt::QueuedConnection);
    QMetaObject::invokeMethod(my_processor.data(), "startHW", Qt::QueuedConnection);


	cal.show();
    //dotWinow.show();
    w.show();

    return a.exec();
	//return 1;
}
