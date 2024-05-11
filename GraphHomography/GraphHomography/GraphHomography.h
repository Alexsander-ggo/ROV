#ifndef GRAPHHOMOGRAPHY_H
#define GRAPHHOMOGRAPHY_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

#include <QColor>
#include <QTimer>
#include <QWidget>

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
    namespace Ui { class GraphHomography; }
QT_END_NAMESPACE

class GraphHomography : public QWidget
{
    Q_OBJECT
public:
    GraphHomography(QWidget* parent = nullptr);

    ~GraphHomography();

private:
    void makeGraph(QCustomPlot* plot,
                   const QString& labelX,
                   const QString& labelY,
                   const QColor& color = Qt::red);

    void addPoint(QCustomPlot* plot, double x, double y);

    void clearGraph(QCustomPlot* plot);

public slots:
    void update();

private slots:
    void on_pushButtonFileOpen_clicked();

    void on_spinBoxFilterSize_valueChanged(int value);

    void on_spinBoxFilterLimit_valueChanged(int value);

    void on_checkBoxFilter_clicked();

private:
    Ui::GraphHomography* mUI;
    QTimer* mTimer;
    cv::Mat mFramePrev;
    cv::Mat mFrame;
    cv::VideoCapture mCapture;
    int mFilterSize;
    int mFilterLimit;
    bool mFilterIsActive;
    int mFrameIndex;
    double mDeltaX;
    double mDeltaY;
    double mAngle;
};
#endif // GRAPHHOMOGRAPHY_H
