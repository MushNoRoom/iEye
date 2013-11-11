
#include "iEyeAppMainWindow.h"
#include "iEyeToolBar.h"
#include "iEyeTypes.h"
#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QScrollBar>

namespace iEye
{
	iEyeAppMainWindow::iEyeAppMainWindow(QWidget* parent)
		:QMainWindow(parent),
		
        m_centralSA(new QScrollArea(this))
	{
        QMainWindow::setCentralWidget(m_centralSA);
        iEyeToolBar* toolBar = new iEyeToolBar(this);
        addToolBar(toolBar);
        connectToolBarSignals(toolBar);
        view_label = new QLabel(this);
        buildBGList();
        view_label->setPixmap((bg_vec.at(0)));
        m_centralSA->setWidget(view_label);
        m_centralSA->setAlignment(Qt::AlignCenter);

        this->setSceneCenter(); // Put the scene in the center before showing
	} 

    iEyeAppMainWindow::iEyeAppMainWindow(const char* image, QWidget* parent)
        :QMainWindow(parent),
          m_centralSA(new QScrollArea(this)),
          img1(image),
          img2(""),
          bgflag(1)
    {
        m_centralSA->setBackgroundRole(QPalette::Dark);

        view_label = (new QLabel(this));
        label_buf = 0;
        QImage view_image(image);
        if (view_image.isNull())
        {
            qDebug() << "Load image failed. Exit program";
          //  exit(0);
        }
        view_label->setPixmap(QPixmap::fromImage(view_image));

        m_centralSA->setWidget(view_label);
        m_centralSA->setAlignment(Qt::AlignCenter);

        this->setSceneCenter(); // Put the scene in the center before showing

        QMainWindow::setCentralWidget(m_centralSA);

        iEyeToolBar* toolBar = new iEyeToolBar(this);
        addToolBar(toolBar);
        connectToolBarSignals(toolBar);
        buildBGList();

    }

    iEyeAppMainWindow::iEyeAppMainWindow(const char* image, const char* secondary_image, QWidget* parent)
        :QMainWindow(parent),
          m_centralSA(new QScrollArea(this)),
          img1(image),
          img2(secondary_image),
          bgflag(1)
    {
        m_centralSA->setBackgroundRole(QPalette::Dark);

        view_label = (new QLabel(this));
        QImage view_image(img1);
        view_label->setPixmap(QPixmap::fromImage(view_image));
        m_centralSA->setWidget(view_label);
        m_centralSA->setAlignment(Qt::AlignCenter);

        this->setSceneCenter(); // Put the scene in the center before showing

        QMainWindow::setCentralWidget(m_centralSA);

        iEyeToolBar* toolBar = new iEyeToolBar(this);
        addToolBar(toolBar);
        connectToolBarSignals(toolBar);

    }



    void iEyeAppMainWindow::swapBackground()
    {

        if (bg_vec.isEmpty()) // When no bg vector is was built
        {
            if (bgflag == 1)
            {
                qDebug() << img2;
                QImage view_image(img2);
                Q_ASSERT(!view_image.isNull());
                view_label = new QLabel;
                view_label->setPixmap(QPixmap::fromImage(view_image));
                m_centralSA->setWidget(view_label);
                m_centralSA->setAlignment(Qt::AlignCenter);
                setSceneCenter();
                bgflag = 2;
            }
            else if (bgflag == 2)
            {
                QImage view_image(img1);
                view_label = new QLabel;
                view_label->setPixmap(QPixmap::fromImage(view_image));
                m_centralSA->setWidget(view_label);
                m_centralSA->setAlignment(Qt::AlignCenter);
                setSceneCenter();
                bgflag = 1;
            }
        }
        else
        {
            //QImage view_image(*it);
            view_label = new QLabel;
            view_label->setPixmap(*it);
            m_centralSA->setWidget(view_label);
            m_centralSA->setAlignment(Qt::AlignCenter);
            setSceneCenter();
            if (it == bg_vec.end()-1)
                 it = bg_vec.begin();
            else
                it++;
            setSceneCenter();

        }
    }

    void iEyeAppMainWindow::setSceneCenter()
    {
        QScrollBar* hScrollBar = m_centralSA->horizontalScrollBar();
        QScrollBar* vScrollBar = m_centralSA->verticalScrollBar();

        int hRange(hScrollBar->maximum() - hScrollBar->minimum());  // Horizontal range of scrollbar
        int vRange(vScrollBar->maximum() - vScrollBar->minimum()); // Vertical range of scrollbar

        // Set the scrollbar's postions to the middle of the ranges
        hScrollBar->setSliderPosition(hRange / 2);
        vScrollBar->setSliderPosition(vRange / 2);
    }

    void iEyeAppMainWindow::resizeEvent(QResizeEvent *evt)
    {
        QMainWindow::resizeEvent(evt);
        setSceneCenter();

    }

	

    //! Negative value of i will result in the effect of moving to the right
    void iEyeAppMainWindow::moveScene(iEye::SceneMovement mv, int i)
    {
        if (mv == iEye::MoveLeft)
        {
            swapBackground();
            return;
        }
        if (mv == iEye::MoveCenter)
        {
            setSceneCenter();
            return;
        }
        QScrollBar* scrollBar(0);
        if (mv == iEye::MoveLeft || mv == iEye::MoveRight)
        {
           scrollBar = m_centralSA->horizontalScrollBar(); // Get the horizontal scroll bar
           if (mv == iEye::MoveLeft)
           {
               int value2set = std::max(scrollBar->minimum(), scrollBar->value() - i);
               scrollBar->setValue(value2set);
           }

           if (mv == iEye::MoveRight)
           {
               int value2set = std::min(scrollBar->maximum(), scrollBar->value() + i);
               scrollBar->setValue(value2set);
           }
        }
        else if (mv == iEye::MoveDown || mv == iEye::MoveUp)
        {
           scrollBar = m_centralSA->verticalScrollBar(); // Get the vertical scroll bar
           if (mv == iEye::MoveUp)
           {
               int value2set = std::max(scrollBar->minimum(), scrollBar->value() - i);
               scrollBar->setValue(value2set);
           }

           if (mv == iEye::MoveDown)
           {
               int value2set = std::min(scrollBar->maximum(), scrollBar->value() + i);
               scrollBar->setValue(value2set);
           }
        }
        else
           return;

    }


    void iEyeAppMainWindow::iEyeMove(SceneMovement mv)
    {
        moveScene(mv, 1);
    }

    /*!
     * \brief iEyeAppMainWindow::connectToolBarSignals
     *
     * This function will connect the toolbar's signals to the corresponding slots
     */
    void iEyeAppMainWindow::connectToolBarSignals(iEyeToolBar* tb)
    {
        QObject::connect(tb, SIGNAL(need2MoveScene(iEye::SceneMovement)), this, SLOT(iEyeMove(iEye::SceneMovement)));
    }

    void iEyeAppMainWindow::buildBGList()
    {
        QString base_dir("D:/McMaster_WS/4OI6_repo/Qt_App/iEye_Qt_App/");
        QString name[6];
        name[0] = base_dir + "LANDS_UHD.jpg";
        name[1] = base_dir + "chair.jpg";
        name[2] = base_dir + "hotel";
        name[3] = base_dir + "coast.jpg";
        name[4] = base_dir + "panda.jpg";
        name[5] = base_dir + "city.jpg";

        for (int i = 0; i < 6; ++i)
        {
            bg_vec.push_back((QPixmap::fromImage(QImage(name[i]))));
        }
        it = bg_vec.begin()+1;


    }

}
