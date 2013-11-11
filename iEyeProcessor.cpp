#include "iEyeProcessor.h"
#include "ieyeimgproc.h"
#include "highgui.h"
#include "opencv.hpp"
#include "iEyeTypes.h"
#include "PortListener.h"
#include <QDebug>



namespace iEye
{
	iEyeProcessor::iEyeProcessor()
		:m_finished(false),
		  //m_device(new VideoCapture(0)),
		  m_currentX(0),
		  m_currentY(0),
		  m_pixmap(0),
		  m_window("")
	{
         m_pl.reset(new PortListener(QLatin1String("COM2")));
	}

	iEyeProcessor::~iEyeProcessor()
	{
		//delete m_device;
	}

	void iEyeProcessor::startCam()
	{
		using namespace cv;
        cv::VideoCapture m_device(1);
		m_device.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
		m_device.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
		m_device.set(CV_CAP_PROP_BRIGHTNESS, 200);
		m_device.set(CV_CAP_PROP_GAIN, 150);
		m_finished = false;
		qDebug() << "Start processing camera.";
		while(!m_finished)
		{

			m_device.read(m_dataMat); // get a new frame from camera
			cvtColor(m_dataMat, m_dataMat, CV_BGR2GRAY); // Convert the signal to gray

            cv::Mat output = m_dataMat.clone();


            cv::Point2i center1 = iEye::windowScanMinStdDev(m_dataMat, cv::Size(8, 8));
			
            circle( output, center1, 15, Scalar( 255, 0,255 ), 1, 8, 0 );

            imshow("Eye_Grey", output);

            setEyePos(center1.x, center1.y);
			if(cv::waitKey(30) >= 0) break;
		}
		m_finished = true;

	}

    void iEyeProcessor::startHW()
    {
//        m_workerThread.quit();

       // m_pl->moveToThread(&m_workerThread);
//        QObject::connect(m_pl.data(), SIGNAL(blinkDeteced()), this, SLOT(onBlinkDeSignal()));
        QObject::connect(m_pl.data(), SIGNAL(eyeChanged(int, int)), this, SLOT(setEyePos(int, int)), Qt::AutoConnection);
       // m_workerThread.start() ; // Start the thread event handling.
    }

	bool iEyeProcessor::isFinished()
	{
		return m_finished;
	}

	void iEyeProcessor::setEyePos(int x, int y)
	{
		int deltaX = x - m_currentX; // Find out the delta x
		int deltaY = y - m_currentY;
		double xPercent = static_cast<double>(deltaX) / 160;
		double yPercent = static_cast<double>(deltaY) / 120;


        m_currentX = x;
        m_currentY = y;
        emit eyeChanged(m_currentX, m_currentY);
        emit eyeChangedDelta(deltaX, deltaY);



//		if (std::abs(xPercent) > 0.1 || std::abs(yPercent) > 0.1)
//		{
//			m_currentX = x;
//			m_currentY = y;
//			emit eyeChanged(m_currentX, m_currentY);
//			emit eyeChangedDelta(deltaX, deltaY);
//		}
	}

    void iEyeProcessor::onBlinkDetect()
    {
        emit blinkDetected(); // Emits the signal from this iEyeProcessor.
    }

}
