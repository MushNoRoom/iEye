#include "iEyeDotWindow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
namespace iEye
{
	iEyeDotWindow::iEyeDotWindow(QWidget* parent)
        :QGraphicsView(parent),
          m_hscale(1),
          m_vscale(1)
	{
        setAutoFillBackground(false);
        m_viewScene = new QGraphicsScene(this);
        m_viewScene->setSceneRect(0, 0, qreal(WINDOW_WIDTH), qreal(WINDOW_HEIGHT));
        m_viewScene->setBackgroundBrush(QBrush(Qt::red));
        m_dot = new QGraphicsEllipseItem(0, 0, 20, 20);
		m_dot->setPen(QPen(Qt::blue));
        m_dot->setBrush(QBrush(Qt::blue));
//        m_dot->setFlag(QGraphicsItem::ItemIsMovable);
		m_viewScene->addItem(m_dot); // Add the dot to the scene
//		m_dotView = new  QGraphicsView(m_viewScene, this); // Assign the scene to view so it can be shown

        this->setScene(m_viewScene);
        this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        qDebug() << m_viewScene->sceneRect().center();
        m_dot->setPos(m_viewScene->sceneRect().center());
        resize(100+ WINDOW_WIDTH, 100 + WINDOW_HEIGHT);
        QPointF initialPos =  m_dot->scenePos();
                qDebug() << initialPos.x() << initialPos.y();
    }


	iEyeDotWindow::~iEyeDotWindow()
	{

	}

    int iEyeDotWindow::updateScale(const QRect& region)
    {
        m_region = region;
        m_hscale = WINDOW_WIDTH / region.width();
        m_vscale = WINDOW_HEIGHT / region.height();
        return m_hscale;
    }

    void iEyeDotWindow::setDotPos(int x, int y)
	{
        qreal xPos((x - m_region.left()) * m_hscale);
        qDebug() << xPos;
        m_dot->setPos(QPointF(std::max(qreal(0), qreal(xPos)), qreal(y)));
	}

    void iEyeDotWindow::moveDot(SceneMovement hMove ,SceneMovement vMove)
    {
        qreal horizontal_move(0);
        qreal vertical_move(0);
        QPointF oldpos = m_dot->scenePos();
        if (hMove == iEye::MoveLeft)
        {
            horizontal_move = -10;
        }
        else if (hMove == iEye::MoveRight)
        {
            horizontal_move = 10;
        }

        if (vMove == iEye::MoveUp)
        {
            vertical_move = -10;
        }
        else if (vMove == iEye::MoveDown)
        {
            vertical_move = 10;
        }
        QPointF newpos = oldpos + QPointF(horizontal_move, vertical_move);
        if (m_viewScene->sceneRect().contains(newpos))
        {

            m_dot->setPos(newpos);
        }
        else
        {
            return; // Do nothing if outside of the scene
        }


    }

    void iEyeDotWindow::onBlink(int blink)
    {
        if (blink == 1)
            m_viewScene->setBackgroundBrush(Qt::green);
        if (blink == 2)
            m_viewScene->setBackgroundBrush(Qt::red);
        update();
    }

	
}
