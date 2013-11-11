#ifndef IEYEMAPPINGPOSMONITOR_H
#define IEYEMAPPINGPOSMONITOR_H

#include "iEyePosMonitor.h"
#include "iEyeTypes.h"
#include <QVector>
#include <QSize>
#include <QRect>

namespace iEye
{
    class iEyeProcessor;
    /*!
     * \brief The iEyeMappingPosMonitor class is the position monitor that monitors the eye movement by mapping the range to a larger scale
     */
    class iEyeMappingPosMonitor : public iEyePosMonitor
    {
        Q_OBJECT

    public:
        iEyeMappingPosMonitor(iEyeProcessor* p, QObject* parent = 0);
        ~iEyeMappingPosMonitor();


    signals:
        void moveCursor(iEye::SceneMovement hMove, iEye::SceneMovement vMove) const;

    private:
        /*!
         * \brief updateNewBlocks updates the blocks assigned to different parts the mapped area
         */
        void updateNewBlocks();
        /*!
         * \brief determineRegion determine which region is the eye at
         * \param x eye's x coordinate
         * \param y eye's y coordinate
         * \return integer that represents the blocks
         */
        int determineRegion(int x, int y) const;
        void onNewRegionAvailable();


    private slots:
        int determineRegion() const;

    private:
        //! The mapped rectangle
        const QRect m_mappedArea;
        //! Rect assigned to different blocks in the area
        QVector<QRect> m_blocks;
        int m_hMapScale, m_vMapScale; // Mapping scale

    };
}

#endif // IEYEMAPPINGPOSMONITOR_H
