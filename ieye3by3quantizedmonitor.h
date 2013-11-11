#ifndef IEYE3BY3QUANTIZEDMONITOR_H
#define IEYE3BY3QUANTIZEDMONITOR_H

#include <QObject>
#include "iEyePosMonitor.h"
#include "iEyeTypes.h"

namespace iEye
{
    class iEyeProcessor;

    /*!
     * \brief The iEye3by3QuantizedMonitor class is the position monitor that monitoring the eye position based on 3x3 area
     *
     * The 3x3 area is the area drawn from the camera's image input which is configure at 160x120. The real area
     * is the retangular area that is determined after calibration
     */
    class iEye3by3QuantizedMonitor : public iEyePosMonitor
    {
        Q_OBJECT
    public:
        iEye3by3QuantizedMonitor(iEyeProcessor* proc, QObject* parent = 0);
        ~iEye3by3QuantizedMonitor();


    signals:
        void moveScene(iEye::SceneMovement, int);

    private:

        /*!
         * \brief function that calculates the horizontal interval
         * \param the span of all intervals
         * \param interval_size size of a interval
         * \param intervals vector containning the interval datas, modelled as QPoint
         */
        void calcInterval(QPoint span, int interval_size, QVector<QPoint>& intervals);

        void calcInterval(QPoint span, int center, int interval_size, QVector<QPoint>& intervals);

        /*!
         * \brief thisHMove function that analyse the current frame's horizontal movement according to real-time x data
         * \param x the current horizontal coordinate of the eye
         * \return the horizontal movement that is implied
         */
        SceneMovement thisHMove(int x);

        /*!
         * \brief thisVMove function that analyse the current frame's vertical movement according to real-time y data
         * \param y the current vertical coordinate of the eye
         * \return the vertical movement that is implied
         */
        SceneMovement thisVMove(int y);

    private slots:
        void hMove();
        void vMove();

    private:
        virtual void onNewRegionAvailable();

    private:

        /*!
         * \brief m_hQL the horizontal quantization levels(3).
         */
        int m_hQL;
        /*!
         * \brief m_vQL the vertical quantization levels(3).
         */
        int m_vQL;

        int m_hSampleCount, m_vSampleCount;
        //! Horizontal intervals
        QVector<QPoint> m_hIntervals, m_vIntervals;
        iEye::SceneMovement m_hMove, m_vMove;

    };
}

#endif // IEYE3BY3QUANTIZEDMONITOR_H
