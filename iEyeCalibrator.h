#ifndef IEYECALIBRATOR_H_
#define IEYECALIBRATOR_H_

#include <QObject>
#include <QWidget>
#include <QRect>



namespace iEye
{
	class iEyeProcessor;

	class Calibrator : public QWidget
	{
		Q_OBJECT
	public:
		typedef enum CaliOrientation
		{
			NoCali,
			CaliTop,
			CaliBottom,
			CaliLeft,
			CaliRight,
			CaliCenter
		}CaliOrientation;

	public:
		Calibrator(iEyeProcessor* p = 0, QWidget* parent = 0);
		~Calibrator();

	public:
		//! Sets the current calibration orientation
		void setCaliOrientation(Calibrator::CaliOrientation orientation)
		{
			m_currentCali = orientation;
		}

        const QPoint& center() const
        {
            return m_center;
        }

        /*!
         * \return the current rectangular area given after calibration
         */
        const QRect& area() const
		{
			return m_area;
		}

	signals:
        void newRegionAvailable(const QRect&) const;
        void newRegionAvailable(const QRect&, const QPoint center) const;
	
	public slots:
		/*!
		 * \brief update the area according to the currently set orientation flag
		 * \param x the x coordinate that is being registered
		 * \param y the y coordinate that is being registered
		 */
		void updateRect(int x, int y);

	private:
		void showMessageBox(QString orientation);

	private slots:
        /*!
         * \brief startCaliC function to calibrate the center
         */
        void startCaliC();
		void startCaliL();
		void startCaliR();
		void startCaliT();
		void startCaliB();
		/*!
		 * \brief finish the calibration, typically one orientation.
		 */
		void finishCali();
		/*!
		 * \brief function that emitts signals when the done button is pressed.
		 */
		void notifyNewRegion();

	private:
		//! A pointer to the processor that emitts the (x,y) coordinates of the eye
        iEyeProcessor* m_proc;
        //! Center point of the calibration.
        QPoint m_center;
		//! The in which the eye is moving. After calibration.
		QRect m_area; 
		//! Current calibration direction.
		Calibrator::CaliOrientation m_currentCali; 

	};
}


#endif
