#ifndef IEYETOOLBAR_H
#define IEYETOOLBAR_H

#include "iEyeTypes.h"
#include <QObject>
#include <QToolBar>


class QWidget;
class QAction;

namespace iEye
{
    class iEyeToolBar : public QToolBar
    {
        Q_OBJECT
    public:
        iEyeToolBar(QWidget* parent);
        ~iEyeToolBar();
    signals:
        /*!
         * \brief need2MoveScene signal emitted for requesting a scene movement in the MainWindow
         * \param mv the direction that the scene is moving along
         */
        void need2MoveScene(iEye::SceneMovement mv);


    private slots:
        //! Respond to keyboard enable button
        void onKeyEnPreesed();
        //! Respond to move to left signal
        void respondToML();
        //! Respond to move to right signal
        void respondToMR();
        //! Respond to move up signal
        void respondToUP();
        //! Respond to move down signal
        void respondToDOWN();
        //! Respond to the centering signal
        void respondToCenter();

    private:
        QAction* kb_en_act;

    };
}


#endif // IEYETOOLBAR_H
