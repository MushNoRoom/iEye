#ifndef IEYEAPPMAINWINDOW
#define IEYEAPPMAINWINDOW

#include <QMainWindow>
#include <QWidget>
#include <string>
#include <QLabel>
#include <QVector>
#include <QImage>
#include <QScopedPointer>
#include "iEyeTypes.h"


class QScrollArea;

namespace iEye
{

    class iEyeToolBar;


	class iEyeAppMainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
        explicit iEyeAppMainWindow(QWidget* parent = 0);
		//! Construct an iEyeAppMainWindow with image being the scene
        explicit iEyeAppMainWindow(const char* image, QWidget* parent = 0);
        explicit iEyeAppMainWindow(const char* image, const char* secondary_img, QWidget* parent = 0);

        ~iEyeAppMainWindow()
        {
            bg_vec.clear();
        }



        void setimg1(const char* image)
        {
            img1 = QString(image);
        }

        void setimg2(const char* image)
        {
            img2 = QString(image);
        }



    public slots:
        /*!
         * \brief swapBackground a call to this funciton will swap the background
         */
        void swapBackground();
        //! This function sets the scene to be centred in the main window
        void setSceneCenter();
        /*!
         * \brief moveScene Move the scene in the app along a direction
         * \param mv the direction that the scene is moving along
         * \param i the unit of the movement
         */
        void moveScene(iEye::SceneMovement mv, int i = 10);


    protected:
        void resizeEvent(QResizeEvent *);

    private slots:
        /*!
         * \brief iEyeMove slot that respond to iEye's movement signal. movement is 1 unit
         * \param mv the direction that the scene is moving along
         */
        void iEyeMove(iEye::SceneMovement mv);


    private:
        void connectToolBarSignals(iEyeToolBar* tb);

        void buildBGList();

	private:
        QVector<QPixmap> bg_vec;
        QVector<QPixmap>::Iterator it;
        QScrollArea* m_centralSA; // The scroll area used as mainwindow's central widget
        QString img1, img2;
        int bgflag;
        QLabel* view_label;
        QLabel* label_buf;
    };

}
#endif	

