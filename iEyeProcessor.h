#ifndef IEYEPROCESSOR_H
#define IEYEPROCESSOR_H

#include "opencv.hpp"
#include <QThread>
#include <QScopedPointer>
#include <QThread>
class VideoCapture; 
class QPixmap;
class PortListener;

namespace iEye {

/*!
 * \brief The iEyeProcessor class the class that act as the processor for iEye app.
 *
 * The output of this class will be utilized by the Qt App to perform action
 * This is the SW side processor. The ideal output should be coming from HW.
 * However, when the HW outputs signals, this class will do processing on HW outputs too.
 */
class iEyeProcessor : public QObject
{
    Q_OBJECT
	public:
		iEyeProcessor();
		~iEyeProcessor();

	public slots:
        //! Start obtainning frames from camera and process
        void startCam();

        //! Start handling the HW outpus
        /*!
         * \todo startHW
         */
        void startHW();

	public:
		int currentX()
		{
			return m_currentX;
		}
		int currentY()
		{
			return m_currentY;
		}
		bool isFinished();

		void setWindow(std::string name)
		{
			m_window = name;
		}

        /*!
         * \brief portListener access the serial port listener that is sending the data to this processor
         * \return a pointer to the PortListener object
         */
        PortListener* portListener()
        {
            return m_pl.data();
        }

signals:
		void eyeChangedDelta(int deltaX, int deltaY);
        /*!
         * \brief eyeChanged emitted whenever the eye position has changed
         * \param x new horizontal position
         * \param y new vertical position
         */
		void eyeChanged(int x, int y);
        /*!
         * \brief blinkDetected emitted whenever a blink action is detected
         *
         * The blink action will be caught by this class itself if running software.
         * It will come from the PortListener object if running hardware.
         */
        void blinkDetected();

    private slots:
        /*!
         * \brief setEyePos sets the current eye position to (x,y)
         * \param x horizontal coordinate
         * \param y vertical coordinate
         */
		void setEyePos(int x, int y);

        /*!
         * \brief onBlinkDetect slot that reacts to the blink signal
         */
        void onBlinkDetect();
		
	private:
		bool m_finished;
        //VideoCapture* m_device; // The image capture device which provides data for processing
        QScopedPointer<PortListener> m_pl;
        QThread m_workerThread;
		int m_currentX, m_currentY;
		QPixmap* m_pixmap;
		cv::Mat m_dataMat;
		std::string m_window;
			
		
};
}
#endif
