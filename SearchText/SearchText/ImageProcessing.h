#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <map>
#include <vector>

using Points = std::map<uint16_t, std::map<uint16_t, bool>>;

struct Figure
{
    uint16_t minX = 0xFFFF;
    uint16_t maxX = 0;
    uint16_t minY = 0xFFFF;
    uint16_t maxY = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint32_t area = 0;
    uint32_t size = 0;
};

using Figures = std::vector<Figure>;

std::vector<cv::Mat> FindInfo(const cv::String& imageName);

cv::Mat FindRedSquarea(const cv::Mat& image);

cv::Mat ClearImage(const cv::Mat& image);

cv::Mat DeleteGlare(const cv::Mat& image, uint8_t max = 90);

cv::Mat FindContour(const cv::Mat& image);

Points SelectPoints(const cv::Mat& image, uint8_t min = 30);

void FindFigure(uint16_t x, uint16_t y, Figure& contour, Points& points);

Figures FindFigures(Points& points);

cv::Mat ShowFigure(const cv::Mat& image, const Figure& figure,
                   const cv::String& imageName);

#endif // IMAGEPROCESSING_H
