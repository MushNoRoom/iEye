#include "ieye3by3quantizedmonitor.h"
#include "iEyeProcessor.h"
#include <QDebug>



namespace iEye
{
    iEye3by3QuantizedMonitor::iEye3by3QuantizedMonitor(iEyeProcessor *proc, QObject *parent)
        :iEyePosMonitor(proc, parent),
          m_hQL(3),
          m_vQL(3),
          m_hSampleCount(0),
          m_vSampleCount(0),
          m_hIntervals(3),
          m_vIntervals(3),
          m_hMove(iEye::NoMove),
          m_vMove(iEye::NoMove)
    {

    }


    iEye3by3QuantizedMonitor::~iEye3by3QuantizedMonitor()
    {

    }


    void iEye3by3QuantizedMonitor::onNewRegionAvailable()
    {

        // Update the horizontal intervals
//        calcInterval(QPoint(region().left(), region().right()),  // Span of the interval
//                     region().width() / m_hQL, // Size of each interval
//                     m_hIntervals // Vector containnig all the interval information
//                     );

//        // Update the vertical intervals
//        calcInterval(QPoint(region().top(), region().bottom()),  // Span of the interval
//                     region().height() / m_vQL, // Size of each interval
//                     m_vIntervals // Vector containnig all the interval information
//                     );


        // With center calibrated
        calcInterval(QPoint(region().left(), region().right()),  // Span of the interval
                     center().x(), // X of the center
                     region().width() / m_hQL, // Size of each interval
                     m_hIntervals // Vector containnig all the interval information
                     );

        // Update the vertical intervals
        calcInterval(QPoint(region().top(), region().bottom()),  // Span of the interval
                     center().y(), // Y of center;
                     region().height() / m_vQL, // Size of each interval
                     m_vIntervals // Vector containnig all the interval information
                     );

        QObject::connect(iEyePosMonitor::posSampler().data(), SIGNAL(timeout()), SLOT(hMove()), Qt::AutoConnection);
        QObject::connect(iEyePosMonitor::posSampler().data(), SIGNAL(timeout()), SLOT(vMove()), Qt::AutoConnection);

//        QObject::connect(proc(), SIGNAL(eyeChanged(int,int)), SLOT(hMove()), Qt::AutoConnection);
//        QObject::connect(proc(), SIGNAL(eyeChanged(int,int)), SLOT(vMove()), Qt::AutoConnection);

        qDebug() << m_hIntervals << m_vIntervals << center();

        qDebug() << "hi";
    }

    void iEye3by3QuantizedMonitor::calcInterval(QPoint span, int interval_size, QVector<QPoint>& intervals)
    {

//        int mid_interval_size = 2 * interval_size;
//        int end_interval_size = (span.y() - span.x() + 1 - mid_interval_size) / 2;
//        intervals[0].setX(span.x());
//        intervals[0].setY(intervals.at(0).x() + end_interval_size - 1);

//        intervals[1].setX(intervals.at(0).y() + 1);
//        intervals[1].setY(intervals.at(1).x() + mid_interval_size - 1);

//        intervals[2].setX(intervals.at(1).y() + 1);
//        intervals[2].setY(intervals.at(2).x() + end_interval_size - 1);
        for (int i = 0; i < intervals.size(); ++i)
        {
            int interval_start = i * interval_size + span.x();
            intervals[i].setX(interval_start);
            intervals[i].setY(interval_start + interval_size - 1);
        }
    }


    void iEye3by3QuantizedMonitor::calcInterval(QPoint span, int center, int interval_size, QVector<QPoint> &intervals)
    {
        Q_UNUSED(interval_size);

        int span_size = span.y() - span.x() + 1;

        int mid_interval_size = (span.y() - span.x() + 1)/3;


        // Proportional mapping
        int mid_left_size = ((center - span.x()) * mid_interval_size) / span_size;
        int mid_right_size = ((span.y() - center) * mid_interval_size) / span_size;


        intervals[0].setX(span.x());
        intervals[1].setX(center - mid_left_size - 1);
        intervals[1].setY(center + mid_right_size);
        intervals[0].setY(intervals.at(1).x() - 1);
        intervals[2].setX(intervals.at(1).y() + 1);
        intervals[2].setY(span.y());
//        intervals[1].setX(intervals.at(0).y() + 1);
//        intervals[1].setY(intervals.at(1).x() + mid_interval_size - 1);

//        intervals[2].setX(intervals.at(1).y() + 1);
//        intervals[2].setY(intervals.at(2).x() + end_interval_size - 1);

//        intervals[0].setY(intervals[0].y() - diff_lr);
//        intervals[1].setX(intervals[1].x() - diff_lr);
//        intervals[1].setY(intervals[1].y() - diff_lr);
//        intervals[2].setX(intervals[2].x() - diff_lr);


//        int mid2left = center - span.x() + 1;
//        int mid2right = span.y() - center;

//        int diff_lr = mid2right - mid2left;

//        intervals[0].setX(span.x());
//        intervals[0].setY(intervals.at(0).x() + end_interval_size - 1);

//        intervals[1].setX(intervals.at(0).y() + 1);
//        intervals[1].setY(intervals.at(1).x() + mid_interval_size - 1);

//        intervals[2].setX(intervals.at(1).y() + 1);
//        intervals[2].setY(intervals.at(2).x() + end_interval_size - 1);

//        intervals[0].setY(intervals[0].y() - diff_lr);
//        intervals[1].setX(intervals[1].x() - diff_lr);
//        intervals[1].setY(intervals[1].y() - diff_lr);
//        intervals[2].setX(intervals[2].x() - diff_lr);

//        qreal left_portion = qreal(center - span.x() + 1) / qreal(span_size);
//        qreal right_portion = qreal(span.y() - center ) / qreal(span_size);

//        int mid_left = mid_interval_size *

    }

    SceneMovement iEye3by3QuantizedMonitor::thisHMove(int x)
    {
        int i = 0;

//        if (m_hMove == NoMove)  // Looking at constant region
//        {
//            if (x <= (m_hIntervals.at(0).y() - (m_hIntervals.at(0).y() - m_hIntervals.at(0).x() + 1) / 3))
//                return MoveLeft;

//            if (x >= (m_hIntervals.at(2).x() + (m_hIntervals.at(2).y() - m_hIntervals.at(2).x() + 1) / 3))
//                return MoveRight;
//        }

//        if (m_hMove == MoveRight)
//        {
//            if (x <= (m_hIntervals.at(0).y()))
//                return MoveLeft;
//            if (x > m_hIntervals.at(0).y()
//                && x <= (m_hIntervals.at(1).y() - (m_hIntervals.at(1).y() - m_hIntervals.at(1).x() + 1) / 3))
//                return NoMove;
//        }

//        if (m_hMove == MoveLeft)
//        {
//            if (x >= (m_hIntervals.at(2).x()))
//                return MoveRight;

//            if (x >= (m_hIntervals.at(1).x() + (m_hIntervals.at(1).y() - m_hIntervals.at(1).x() + 1) / 3)
//                    && x < m_hIntervals.at(2).x())
//                return NoMove;
//        }

        if (x > 0 && x <= m_hIntervals.at(0).x()) // Compensate some head movement so it still moves the image even outside of calibraiton range
//            return MoveRight;
            return MoveLeft;

        if (x > m_hIntervals.at(2).y())
//            return MoveLeft;
            return MoveRight;



        for (; i < m_hIntervals.size(); ++i)
        {
            if (x >= m_hIntervals.at(i).x() && x <= m_hIntervals.at(i).y())
                break;
        }
        switch(i)
        {
        case 0:
            return MoveLeft;
//            return MoveRight; //SW
        case 2:
            return MoveRight;
//            return MoveLeft;// SW
        case 1:
        default:
            return NoMove;

        }

    }

    SceneMovement iEye3by3QuantizedMonitor::thisVMove(int y)
    {

//        if (m_vMove == NoMove)  // Looking at constant region
//        {
//            if (y <= (m_vIntervals.at(0).y() - (m_vIntervals.at(0).y() - m_vIntervals.at(0).x() + 1) / 3))
//                return MoveUp;

//            if (y >= (m_vIntervals.at(2).x() + (m_vIntervals.at(2).y() - m_vIntervals.at(2).x() + 1) / 3))
//                return MoveDown;
//        }

//        if (m_vMove == MoveDown)
//        {
//            if (y <= (m_vIntervals.at(0).y()))
//                return MoveUp;
//            if (y > (m_vIntervals.at(0).y())
//                && y <= (m_vIntervals.at(1).y() - (m_vIntervals.at(1).y() - m_vIntervals.at(1).x() + 1) / 3))  // Middle range
//                return NoMove;
//        }

//        if (m_hMove == MoveUp)
//        {
//            if (y >= m_vIntervals.at(2).x())
//                return MoveDown;

//            if (y >= (m_vIntervals.at(1).x() + (m_vIntervals.at(1).y() - m_vIntervals.at(1).x() + 1) / 3)
//                    && y < m_vIntervals.at(2).x())
//                return NoMove;
//        }
        int i = 0;
        if (y > 0 && y <= m_vIntervals.at(0).x()) // Compensate some head movement so it still moves the image even outside of calibraiton range
        {

            return MoveUp;
        }
        if (y > m_vIntervals.at(2).y())
        {

            return MoveDown;
        }
        for (; i < m_vIntervals.size(); ++i)
        {

            if (y >= m_vIntervals.at(i).x() && y <= m_vIntervals.at(i).y())
                break;
        }
        switch(i)
        {
        case 0:
            return MoveUp;
        case 2:
            return MoveDown;
        case 1:
        default:
            return NoMove;

        }
    }

    void iEye3by3QuantizedMonitor::hMove()
    {
        int x = proc()->currentX(); // Sample the x position.
        SceneMovement thisMove = thisHMove(x);
        if (thisMove != m_hMove) // When a new move is detected
        {
            m_hSampleCount = 0;
            m_hMove = thisMove;
        }
        else
        {
            m_hSampleCount += 1;
            if (m_hSampleCount == 14) // Request move when collecting 60 consecutive samples
            {
                int amount(10);
                m_hSampleCount = 0; // Reset the count
                if (thisMove == iEye::MoveLeft)
                {
                    if (iEyePosMonitor::kb_en == true)
                        iEyePosMonitor::ieye_keybd_event(Qt::Key_Left);
                    else
                        emit moveScene(iEye::MoveLeft, amount); // Programmically move the scene within application.
                }
                if (thisMove == iEye::MoveRight)
                {
                    if (iEyePosMonitor::kb_en == true)
                        iEyePosMonitor::ieye_keybd_event(Qt::Key_Right);
                    else
                        emit moveScene(iEye::MoveRight, amount);
                }

            }
        }
    }

    void iEye3by3QuantizedMonitor::vMove()
    {
        int y = proc()->currentY(); // Sample the x position.
        SceneMovement thisMove = thisVMove(y);
        if (thisMove != m_vMove) // When a new move is detected
        {
            m_vSampleCount = 0;
            m_vMove = thisMove;
        }
        else
        {
            m_vSampleCount += 1;
            if (m_vSampleCount == 14) // Request move when collecting 60 consecutive samples
            { 
                int amount(10);
                m_vSampleCount = 0; // Reset the count
                if (thisMove == iEye::MoveUp)
                {
                    if (iEyePosMonitor::kb_en)
                        iEyePosMonitor::ieye_keybd_event(Qt::Key_Up);
                    else
                        emit moveScene(iEye::MoveUp, amount);
                }
                if (thisMove == iEye::MoveDown)
                {
                    if (iEyePosMonitor::kb_en)
                        iEyePosMonitor::ieye_keybd_event(Qt::Key_Down);
                    else
                        emit moveScene(iEye::MoveDown, amount);

                }
            }
        }
    }
}
