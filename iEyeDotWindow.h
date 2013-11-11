#ifndef IEYEDOTWINDOW_H
#define IEYEDOTWINDOW_H

class QGraphicsScene;
class QGraphicsEllipseItem;

#include <QGraphicsView>
#include <QRect>
#include "iEyeTypes.h"

namespace iEye
{
    /*!
     * \brief The iEyeDotWindow class provides a graphic view for window objects representing the eye movement
     */
    class iEyeDotWindow : public QGraphicsView
	{
		Q_OBJECT
	public:
		iEyeDotWindow(QWidget* parent = 0);
		//! Destructor
		~iEyeDotWindow();
        int updateScale(const QRect& region);

	public slots:
        void setDotPos(int x, int y);

        /*!
         * \brief moveDot move the dot according to the specified direction(amount moved is default)
         * \param hMove horizontal direction
         * \param vMove vertical direction
         */
        void moveDot(iEye::SceneMovement hMove, iEye::SceneMovement vMove);

        void onBlink(int flag);



	private:
		QGraphicsView *m_dotView;
		QGraphicsScene *m_viewScene;
		QGraphicsEllipseItem *m_dot;
        int m_hscale, m_vscale;
        QRect m_region;

	};
}

#endif
