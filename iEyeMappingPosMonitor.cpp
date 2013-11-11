#include "iEyeMappingPosMonitor.h"
#include "iEyeProcessor.h"
#include <QDebug>

namespace iEye
{
    iEyeMappingPosMonitor::iEyeMappingPosMonitor(iEyeProcessor* p, QObject* parent)
        :iEyePosMonitor(p, parent),
          m_mappedArea(0, 0, 800, 600),
          m_blocks(9) // Separate the area into 9 blocks
    {
        m_hMapScale = m_mappedArea.width() / 160;
        m_vMapScale = m_mappedArea.height() / 120;
    }

    iEyeMappingPosMonitor::~iEyeMappingPosMonitor()
    {

    }

    void iEyeMappingPosMonitor::updateNewBlocks()
    {
        QPoint mid = region().center(); // The center of the calibration area
        qDebug() << mid;
        int xdelta = region().width()/16;
        int ydelta = region().height()/6;

        m_blocks[0] = QRect((mid - QPoint(xdelta, ydelta)), mid + QPoint(xdelta, ydelta)); //The middle block
        m_blocks[1] = QRect(region().topLeft(), m_blocks.at(0).topLeft() - QPoint(1, 1)); // Top left block
        m_blocks[2] = QRect(m_blocks.at(1).topRight() + QPoint(1, 0), m_blocks.at(0).topRight() - QPoint(0, 1)); // Middle top block
        m_blocks[3] = QRect(m_blocks.at(2).topRight() + QPoint(1, 0),
                            QSize(region().width() - m_blocks.at(1).width() - m_blocks.at(2).width(), m_blocks.at(1).height())); // Top right block

        m_blocks[4] = QRect(m_blocks.at(1).bottomLeft() + QPoint(0,1), m_blocks.at(0).bottomLeft() - QPoint(1, 0)); // Middle left block
        m_blocks[5] = QRect(m_blocks.at(0).topRight() + QPoint(1, 0),
                            QSize(m_blocks.at(3).width(), m_blocks.at(0).height())); // Middle right

        m_blocks[6] = QRect(m_blocks.at(4).bottomLeft() + QPoint(0,1),
                            QSize(m_blocks.at(4).width(), region().height() - m_blocks.at(1).height() - m_blocks.at(4).height())); // Bottom left

        m_blocks[7] = QRect(m_blocks.at(6).topRight() + QPoint(0,1),
                            QSize(m_blocks.at(0).width(), m_blocks.at(6).height())); // Middle bottom
        m_blocks[8] = QRect(m_blocks.at(7).topRight() + QPoint(1,0), QSize(m_blocks.at(5).width(), m_blocks.at(7).height())); // Bottom right


        qDebug() << m_blocks;

    }

    int iEyeMappingPosMonitor::determineRegion(int x, int y) const
    {
       QPoint currentPos(x, y); // Current eye position
       qDebug() << m_blocks;
       qDebug() << currentPos;
        int i = 0;
        for (; i < m_blocks.size() + 1; ++i)
        {
            if (i == m_blocks.size())
                break;
            if (m_blocks.at(i).contains(currentPos))
                break;
        }
        SceneMovement hMove(iEye::NoMove);
        SceneMovement vMove(iEye::NoMove);

        switch(i)
        {
        case 0:
            qDebug() << "Center";
            break;
        case 1:
            qDebug() << "Top right";
            hMove = MoveRight;
            vMove = MoveUp;
            break;
        case 2:
            qDebug() << "Top middle top";
            vMove = MoveUp;
            break;
        case 3:
            qDebug() << "Top left";
            hMove = MoveLeft;
            vMove = MoveUp;
            break;

        case 4:
            qDebug() << "middle right";
            hMove = MoveRight;
            break;

        case 5:
            qDebug() << "middle left";
            hMove = MoveLeft;
            break;

        case 6:
            hMove = MoveRight;
            vMove = MoveDown;
            qDebug() << "bottom right";
            break;

        case 7:
            qDebug() << "middle bottom";
            vMove = MoveDown;
            break;
        case 8:
            qDebug() << "bottom left";
            hMove = MoveLeft;
            vMove = MoveDown;
            break;
        default:
            break;
//            qDebug() << "None found";

        }
//        qDebug() << "Region" << i;
        emit moveCursor(hMove, vMove);

        return i;
    }

    void iEyeMappingPosMonitor::onNewRegionAvailable()
    {
        QRect reg = region();

        //! Resets the mapping scales on the region setup
        m_hMapScale = m_mappedArea.width() / reg.width();
        m_vMapScale = m_mappedArea.height() / reg.width();

        updateNewBlocks();
        QObject::connect(iEyePosMonitor::posSampler().data(), SIGNAL(timeout()), SLOT(determineRegion()), Qt::AutoConnection);
    }

    int iEyeMappingPosMonitor::determineRegion() const
    {
        return determineRegion(iEyePosMonitor::proc()->currentX(), proc()->currentY());
    }
}
