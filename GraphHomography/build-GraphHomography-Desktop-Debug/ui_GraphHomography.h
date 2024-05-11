/********************************************************************************
** Form generated from reading UI file 'GraphHomography.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHHOMOGRAPHY_H
#define UI_GRAPHHOMOGRAPHY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_GraphHomography
{
public:
    QPushButton *pushButtonFileOpen;
    QLabel *labelFilterSize;
    QSpinBox *spinBoxFilterSize;
    QSpinBox *spinBoxFilterLimit;
    QLabel *labelFilterLimit;
    QLineEdit *lineEditFileName;
    QCheckBox *checkBoxFilter;
    QCustomPlot *customPlotAngle;
    QCustomPlot *customPlotScale;
    QCustomPlot *customPlotDeltaX;
    QCustomPlot *customPlotDeltaY;

    void setupUi(QWidget *GraphHomography)
    {
        if (GraphHomography->objectName().isEmpty())
            GraphHomography->setObjectName(QString::fromUtf8("GraphHomography"));
        GraphHomography->resize(890, 570);
        QFont font;
        font.setPointSize(18);
        GraphHomography->setFont(font);
        pushButtonFileOpen = new QPushButton(GraphHomography);
        pushButtonFileOpen->setObjectName(QString::fromUtf8("pushButtonFileOpen"));
        pushButtonFileOpen->setGeometry(QRect(10, 10, 120, 30));
        pushButtonFileOpen->setFont(font);
        labelFilterSize = new QLabel(GraphHomography);
        labelFilterSize->setObjectName(QString::fromUtf8("labelFilterSize"));
        labelFilterSize->setGeometry(QRect(570, 10, 90, 30));
        labelFilterSize->setFont(font);
        spinBoxFilterSize = new QSpinBox(GraphHomography);
        spinBoxFilterSize->setObjectName(QString::fromUtf8("spinBoxFilterSize"));
        spinBoxFilterSize->setGeometry(QRect(660, 10, 60, 30));
        spinBoxFilterSize->setMinimum(3);
        spinBoxFilterSize->setMaximum(20);
        spinBoxFilterSize->setValue(3);
        spinBoxFilterLimit = new QSpinBox(GraphHomography);
        spinBoxFilterLimit->setObjectName(QString::fromUtf8("spinBoxFilterLimit"));
        spinBoxFilterLimit->setGeometry(QRect(810, 10, 70, 30));
        spinBoxFilterLimit->setMinimum(0);
        spinBoxFilterLimit->setMaximum(255);
        spinBoxFilterLimit->setSingleStep(10);
        spinBoxFilterLimit->setValue(100);
        labelFilterLimit = new QLabel(GraphHomography);
        labelFilterLimit->setObjectName(QString::fromUtf8("labelFilterLimit"));
        labelFilterLimit->setGeometry(QRect(730, 10, 80, 30));
        labelFilterLimit->setFont(font);
        lineEditFileName = new QLineEdit(GraphHomography);
        lineEditFileName->setObjectName(QString::fromUtf8("lineEditFileName"));
        lineEditFileName->setEnabled(false);
        lineEditFileName->setGeometry(QRect(140, 10, 300, 30));
        lineEditFileName->setStyleSheet(QString::fromUtf8("background: white;"));
        checkBoxFilter = new QCheckBox(GraphHomography);
        checkBoxFilter->setObjectName(QString::fromUtf8("checkBoxFilter"));
        checkBoxFilter->setGeometry(QRect(450, 10, 120, 30));
        customPlotAngle = new QCustomPlot(GraphHomography);
        customPlotAngle->setObjectName(QString::fromUtf8("customPlotAngle"));
        customPlotAngle->setGeometry(QRect(10, 50, 430, 250));
        customPlotScale = new QCustomPlot(GraphHomography);
        customPlotScale->setObjectName(QString::fromUtf8("customPlotScale"));
        customPlotScale->setGeometry(QRect(10, 310, 430, 250));
        customPlotDeltaX = new QCustomPlot(GraphHomography);
        customPlotDeltaX->setObjectName(QString::fromUtf8("customPlotDeltaX"));
        customPlotDeltaX->setGeometry(QRect(450, 50, 430, 250));
        customPlotDeltaY = new QCustomPlot(GraphHomography);
        customPlotDeltaY->setObjectName(QString::fromUtf8("customPlotDeltaY"));
        customPlotDeltaY->setGeometry(QRect(450, 310, 430, 250));

        retranslateUi(GraphHomography);

        QMetaObject::connectSlotsByName(GraphHomography);
    } // setupUi

    void retranslateUi(QWidget *GraphHomography)
    {
        GraphHomography->setWindowTitle(QApplication::translate("GraphHomography", "\320\223\321\200\320\260\321\204\320\270\320\272\320\270", nullptr));
        pushButtonFileOpen->setText(QApplication::translate("GraphHomography", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        labelFilterSize->setText(QApplication::translate("GraphHomography", "\320\240\320\260\320\267\320\274\320\265\321\200", nullptr));
        labelFilterLimit->setText(QApplication::translate("GraphHomography", "\320\237\320\276\321\200\320\276\320\263", nullptr));
        checkBoxFilter->setText(QApplication::translate("GraphHomography", "\320\244\320\270\320\273\321\214\321\202\321\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphHomography: public Ui_GraphHomography {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHHOMOGRAPHY_H
