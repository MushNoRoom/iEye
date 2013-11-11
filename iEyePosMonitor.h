#ifndef IEYEPOSMONITOR_H_
#define IEYEPOSMONITOR_H_

#include <QObject>
#include <QScopedPointer>
#include <QTimer>
#include <QRect>
#include <QVector>
#include <QPoint>
#include "iEyeTypes.h"

namespace iEye
{
	class iEyeProcessor;
	/*!
	 * Class designed to monitor the position of eye.
	 */
	class iEyePosMonitor : public QObject
	{
		Q_OBJECT
	public:
		/*!
		 * \brief Constructor for the iEyePosMonitor
		 * \param hQL horizontal quantization levels.
		 * \param vQL vertical quantization levels.
		 */
        iEyePosMonitor(iEyeProcessor* p, QObject* parent = 0);
        virtual ~iEyePosMonitor();

    public :
        //! Keyboard event enable flag
        static bool kb_en;


        static void ieye_keybd_event(Qt::Key k);
	 

		void setOnDuty(bool onDuty)
		{
			m_onDuty = onDuty;
		}

		bool isOnDuty()
		{
			return m_onDuty;
		}

        /*!
         * \brief posSampler return the QScopedpointer reference to the timer used to sample the position of the eye
         */
        QScopedPointer<QTimer>& posSampler()
        {
            return m_timer;
        }

		//! Starts sampling the position of the eye every at frequency of 1/mseconds
		void startSampling();
		void stopSampling();

        /*!
         * \return the center of the eye given by calibration result.
         */
        const QPoint& center() const
        {
            return m_eyeCenter;
        }

        /*!
         * \brief region obtained after calibration in the 160x120 domain
         * \return const reference to the rectangular area
         */
        const QRect& region()
        {
            return m_eyeRegion;
        }

	public slots:
		/*!
		 * \brief function that sets the new region for eye's movement.
		 * \param newRegion the new rectangle specifying the eye region of movement
		 */
		void setNewRegion(const QRect& newRegion);


        void setNewRegion(const QRect& newRegion, const QPoint &newCenter);

    protected:
        iEyeProcessor* proc() const
        {
            return m_proc;
        }


    private:
        virtual void onNewRegionAvailable() = 0;

    private:
        iEyeProcessor* m_proc;
        bool m_onDuty; // The duty flag.
        QPoint m_eyeCenter; // Center of the eye
        QRect m_eyeRegion; // The eye's region of movement.
        QScopedPointer<QTimer> m_timer;
    };


}

#endif
