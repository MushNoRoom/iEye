#include "iEyeCalibrator.h"
#include "iEyeProcessor.h"
#include <QPushButton>
#include <QGridlayout>
#include <QMessageBox>
#include <QDebug>

namespace iEye
{
	Calibrator::Calibrator(iEyeProcessor* p,QWidget* parent)
		:QWidget(parent),
		m_proc(p),
		m_area(0, 0, 160, 120),
		m_currentCali(NoCali) // Set to no calibration at the beginning
	{
		QGridLayout *layout = new QGridLayout(this);
		QPushButton *caliL = new QPushButton("Cali Left", this); // Left calibration button
		QPushButton *caliR = new QPushButton("Cali Right", this); // Right calibration button
		QPushButton *caliT = new QPushButton("Cali Up", this);
		QPushButton *caliB = new QPushButton("Cali Down", this);
		QPushButton *caliC = new QPushButton("Cali Center", this);
		QPushButton *done = new QPushButton("Done", this);
		layout->addWidget(caliT, 0, 1, 1, 1, Qt::AlignCenter);
		layout->addWidget(caliL, 1, 0, 1, 1, Qt::AlignCenter);
		layout->addWidget(caliC, 1, 1, 1, 1, Qt::AlignCenter);
		layout->addWidget(caliR, 1, 2, 1, 1, Qt::AlignCenter);
		layout->addWidget(caliB, 2, 1, 1, 1, Qt::AlignCenter);
		layout->addWidget(done, 3, 0, 1, 3);
		this->setLayout(layout);

		QObject::connect(caliL, SIGNAL(clicked()), SLOT(startCaliL()));
		QObject::connect(caliR, SIGNAL(clicked()), SLOT(startCaliR()));
		QObject::connect(caliT, SIGNAL(clicked()), SLOT(startCaliT()));
		QObject::connect(caliB, SIGNAL(clicked()), SLOT(startCaliB()));
        QObject::connect(caliC, SIGNAL(clicked()), SLOT(startCaliC()));
		QObject::connect(done, SIGNAL(clicked()), SLOT(notifyNewRegion())); 

		if (m_proc != NULL)
		{
			QObject::connect(m_proc, SIGNAL(eyeChanged(int, int))
				, this, SLOT(updateRect(int, int))); // The calibrator will accept the value that's calculated by the processor  to update its rectangular region
		}
	}

	Calibrator::~Calibrator()
	{
	}

	void Calibrator::updateRect(int x, int y)
	{
        //qDebug() << x << y;
		if (m_currentCali == Calibrator::NoCali)
			return; // Do nothing when it's not being calibrating.
		switch(m_currentCali)
		{
		case CaliLeft: // The left and right is inverted for image fed in from camera
//			m_area.setRight(x);
            m_area.setLeft(x);
			break;
		case CaliRight:
//			m_area.setLeft(x);
            m_area.setRight(x);
			break;
		case CaliBottom:
			m_area.setBottom(y);
			break;
		case CaliTop:
			m_area.setTop(y);
			break;
		default:
			return;

		}
//		qDebug() << "The new rect is" << area();
	}

	void Calibrator::showMessageBox(QString orientation)
	{
        QString message("Now calibrating the ");
		message.append(orientation).append(". Click ok when finished.");
		QMessageBox msgBox;
		msgBox.setText(message);
		connect(&msgBox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(finishCali())); // When test subject hits ok, the calibration for a orientation is done. Calibrator sits and waits.
		msgBox.exec();
	}

    void Calibrator::startCaliC()
    {
        m_currentCali = Calibrator::CaliCenter;
        showMessageBox("center");
    }

	void Calibrator::startCaliL()
	{
		m_currentCali = Calibrator::CaliLeft;
		showMessageBox("left");
	}

	void Calibrator::startCaliR()
	{
		m_currentCali = Calibrator::CaliRight;
		showMessageBox("right");
	}

	void Calibrator::startCaliT()
	{
		m_currentCali = Calibrator::CaliTop;
		showMessageBox("top");
	}

	void Calibrator::startCaliB()
	{
		m_currentCali = Calibrator::CaliBottom;
		showMessageBox("bottom");
	}

	void Calibrator::finishCali()
	{
        qDebug() << "The current rect" << area().left() << area().right() << area().top() << area().bottom();

        if (m_currentCali == Calibrator::CaliCenter)
        {
            m_center = QPoint(m_proc->currentX(), m_proc->currentY());
            qDebug() << "Center after calibration" << m_center;
        }
		m_currentCali = Calibrator::NoCali; // Set the calibration to NoCali so the area is not getting updated anymore.

	}

	void Calibrator::notifyNewRegion()
	{
		emit newRegionAvailable(this->area()); 
        emit newRegionAvailable(this->area(), m_center);
	}
}
