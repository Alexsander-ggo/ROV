#include "GraphHomography.h"
#include "ui_GraphHomography.h"

#include <cmath>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QPen>
#include <QString>

GraphHomography::GraphHomography(QWidget* parent)
    : QWidget(parent)
    , mUI(new Ui::GraphHomography())
{
    mUI->setupUi(this);
    mTimer = new QTimer();
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    makeGraph(mUI->customPlotAngle, "Точка", "Угол,°", Qt::green);
    makeGraph(mUI->customPlotScale, "Точка", "Масштаб", Qt::black);
    makeGraph(mUI->customPlotDeltaX, "Точка", "X", Qt::red);
    makeGraph(mUI->customPlotDeltaY, "Точка", "Y", Qt::blue);
}

GraphHomography::~GraphHomography()
{
    delete mUI;
    delete mTimer;
}

void GraphHomography::makeGraph(QCustomPlot* plot,
                                const QString& labelX,
                                const QString& labelY,
                                const QColor& color)
{
    plot->addGraph();
    plot->graph(0)->setPen(QPen(color));
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->xAxis->setLabel(labelX);
    plot->yAxis->setLabel(labelY);
    clearGraph(plot);
}

void GraphHomography::addPoint(QCustomPlot* plot, double x, double y)
{
    plot->graph(0)->addData(x, y);
    double minX = plot->xAxis->range().lower;
    double maxX = plot->xAxis->range().upper;
    double minY = plot->yAxis->range().lower;
    double maxY = plot->yAxis->range().upper;
    plot->xAxis->setRange(std::min(x, minX), std::max(x, maxX));
    plot->yAxis->setRange(std::min(y, minY), std::max(y, maxY));
    plot->replot();
}

void GraphHomography::clearGraph(QCustomPlot* plot)
{
    plot->graph(0)->clearData();
    plot->xAxis->setRange(0, 1);
    plot->yAxis->setRange(0, 1);
}

void GraphHomography::update()
{
    try {
        mFramePrev = mFrame.clone();
        if (mCapture.grab()) {
            mCapture >> mFrame;
            if (mFilterIsActive) {
                cvtColor(mFrame, mFrame, cv::COLOR_BGR2HSV);
                cv::Mat channels[3];
                split(mFrame, channels);
                cv::Ptr<cv::CLAHE> clahe = createCLAHE(mFilterLimit, cv::Size(mFilterSize, mFilterSize));
                clahe->apply(channels[2], channels[2]);
                merge(channels, 3, mFrame);
                cvtColor(mFrame, mFrame, cv::COLOR_HSV2BGR);
            }
        } else {
            mTimer->stop();
        }
        cv::Ptr<cv::FeatureDetector> detector;
        cv::Ptr<cv::DescriptorExtractor> extractor;
        cv::BFMatcher matcher;
        detector = cv::ORB::create();
        extractor = cv::ORB::create();
        std::vector<cv::KeyPoint> keysPrev, keysCur;
        cv::Mat descriptorPrev, descriptorCur, imageMatches;
        std::vector<cv::DMatch> matches;
        std::vector<std::vector<cv::DMatch>> knnMatches;
        detector->detect(mFramePrev, keysPrev);
        detector->detect(mFrame, keysCur);
        extractor->compute(mFramePrev, keysPrev, descriptorPrev);
        extractor->compute(mFrame, keysCur, descriptorCur);
        matcher.knnMatch(descriptorPrev, descriptorCur, knnMatches, 2);
        std::vector<cv::DMatch> goodMatches;
        double k = 0.7;
        for (auto item : knnMatches) {
            if (item[0].distance < k * item[1].distance) {
                goodMatches.push_back(item[0]);
            }
        }
        if (!keysPrev.empty() && !keysCur.empty() && !goodMatches.empty()) {
            cv::drawMatches(mFramePrev, keysPrev, mFrame, keysCur, goodMatches, imageMatches);
            cv::imshow("Result", imageMatches);
        }
        std::vector<cv::Point2f> prev;
        std::vector<cv::Point2f> cur;
        for (size_t i = 0; i < goodMatches.size(); i++) {
            prev.push_back(keysPrev[goodMatches[i].queryIdx].pt);
            cur.push_back(keysCur[goodMatches[i].trainIdx].pt);
        }
        cv::Mat H = findHomography(prev, cur, cv::RANSAC);
        double dx = H.at<double>(0, 2);
        double dy = H.at<double>(1, 2);
        double scaleCos = H.at<double>(0, 0);
        double scaleSin = H.at<double>(1, 0);
        double angle = 180 * atan2(scaleSin, scaleCos) / (4 * atan(1));
        double scale = scaleCos / cos(angle);
        ++mFrameIndex;
        mDeltaX += dx;
        mDeltaY += dy;
        mAngle += angle;
        addPoint(mUI->customPlotAngle, mFrameIndex, mAngle);
        addPoint(mUI->customPlotScale, mFrameIndex, scale);
        addPoint(mUI->customPlotDeltaX, mFrameIndex, mDeltaX);
        addPoint(mUI->customPlotDeltaY, mFrameIndex, mDeltaY);
    } catch (...) {
        mTimer->stop();
    }
}


void GraphHomography::on_pushButtonFileOpen_clicked()
{
    mFilterSize = mUI->spinBoxFilterSize->value();
    mFilterLimit = mUI->spinBoxFilterLimit->value();
    mFilterIsActive = mUI->checkBoxFilter->isChecked();
    mFrameIndex = 0;
    mDeltaX = 0;
    mDeltaY = 0;
    mAngle = 0;
    clearGraph(mUI->customPlotAngle);
    clearGraph(mUI->customPlotScale);
    clearGraph(mUI->customPlotDeltaX);
    clearGraph(mUI->customPlotDeltaY);
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Выбрать файл", QDir::currentPath(),
                                                    "Files (*)", nullptr, QFileDialog::DontUseNativeDialog);
    QFileInfo fileInfo(fileName);
    mUI->lineEditFileName->setText(fileInfo.fileName());
    mCapture.open(0/*fileName.toStdString()*/);
    if (mCapture.grab()) {
        mCapture >> mFrame;
    }
    double rate = mCapture.get(cv::CAP_PROP_FPS);
    int delay = 1000 / rate;
    mTimer->start(delay);
}

void GraphHomography::on_spinBoxFilterSize_valueChanged(int value)
{
    mFilterSize = value;
}

void GraphHomography::on_spinBoxFilterLimit_valueChanged(int value)
{
    mFilterLimit = value;
}


void GraphHomography::on_checkBoxFilter_clicked()
{
    mFilterIsActive = mUI->checkBoxFilter->isChecked();
}
