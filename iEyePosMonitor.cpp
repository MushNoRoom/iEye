#include "iEyeTypes.h"
#include "iEyePosMonitor.h"
#include "iEyeProcessor.h"
#include <QDebug>
#define WINVER 0x500
#include <windows.h>


namespace iEye
{
    iEyePosMonitor::iEyePosMonitor(iEyeProcessor* p, QObject* parent)
		:QObject(parent),
		m_proc(p),
		m_onDuty(false),
        m_eyeRegion(QPoint(0, 0), QSize(iEye::FRAME_WIDTH, iEye::FRAME_HEIGHT)) // Initialize the eye region of movement
	{
        m_timer.reset(new QTimer);
	}

	iEyePosMonitor::~iEyePosMonitor()
	{
		stopSampling();
	}

    void iEyePosMonitor::ieye_keybd_event(Qt::Key k)
    {
        if (iEye::iEyePosMonitor::kb_en == false)
            return;  // Do nothing when kb is disable

        BYTE bVk;
        BYTE bScan(0);


        if (iEye::iEyePosMonitor::tux_flag) // Running tux
        {

           if(k == Qt::Key_Left)
            {
                bScan = 0x1E;
                bVk = 0x41;
            }

            if (k == Qt::Key_Right)
            {
                bScan = 0x20;
                bVk = 0x44;

            }
            if (k == Qt::Key_Up)
            {
                bScan = 0x11;
                bVk = 0x57;
            }
            if (k == Qt::Key_Down)
            {
                bScan = 0x1F;
                bVk = 0x53;
            }

            keybd_event(bVk,
                        bScan,
                        KEYEVENTF_EXTENDEDKEY | 0,
                        0);

         }
        else
        {
            if (k == Qt::Key_Left)
                       {
                           bVk = VK_LEFT;
           //                bVk = 0x41;
                       }

                       if (k == Qt::Key_Right)
                       {
                           bVk = VK_RIGHT;
           //                bVk = 0x44;

                       }
                       if (k == Qt::Key_Up)
                       {
           //                bVk = 0x57;
                           bVk = VK_UP;
                       }
                       if (k == Qt::Key_Down)
                       {
           //                bVk = 0x53;
                           bVk = VK_DOWN;
                       }

                       keybd_event(bVk,
                                   bScan,
                                   KEYEVENTF_EXTENDEDKEY | 0,
                                   0);

                       keybd_event(bVk,
                                   bScan,
                                   KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                                   0);

        }

    }


	void iEyePosMonitor::setNewRegion(const QRect& newRegion)
	{
        stopSampling(); // Stop sampling on the new region's arrival
		m_onDuty = false; // Turn off the monitoring.
		m_eyeRegion = newRegion; // Update the eye region of movement.
        m_eyeCenter = m_eyeRegion.center();

        onNewRegionAvailable();

		m_onDuty = true;
		startSampling();
	}


    void iEyePosMonitor::setNewRegion(const QRect &newRegion, const QPoint &newCenter)
    {
        stopSampling();
        m_onDuty = false;
        m_eyeRegion = newRegion;
        m_eyeCenter = newCenter;

        onNewRegionAvailable();

        m_onDuty = true;
        startSampling();
    }

	void iEyePosMonitor::startSampling()
	{

//		QObject::connect(m_timer.data(), SIGNAL(timeout()), SLOT(hMove()), Qt::DirectConnection);
//        QObject::connect(m_timer.data(), SIGNAL(timeout()), SLOT(vMove()), Qt::DirectConnection);
        m_timer->start(30);
		
	}

	void iEyePosMonitor::stopSampling()
	{
        QObject::disconnect(m_timer.data(), SIGNAL(timeout()), 0, 0);
		m_timer->stop();
	}


}
