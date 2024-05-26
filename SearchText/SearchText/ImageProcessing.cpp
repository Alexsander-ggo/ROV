#include "ImageProcessing.h"

std::vector<cv::Mat> FindInfo(const std::string& imageName) {
    std::vector<cv::Mat> info;
    cv::Mat image;
    image = imread(imageName, cv::IMREAD_COLOR);
    cv::Mat resultImage = FindRedSquarea(image);
    Points points = SelectPoints(FindContour(ClearImage(resultImage)));
    Figures figures = FindFigures(points);
    if (figures.size() >= 3) {
        std::sort(figures.begin(), figures.end(),
        [] (const Figure& left, const Figure& right) {
            return left.area > right.area;
        });
        Figure codeFigure = figures.front();
        Figure rowFigure = figures.at(1);
        Figure placeFigure = figures.at(2);
        if (codeFigure.minY < rowFigure.minY
            && codeFigure.minY < placeFigure.minY) {
            if (rowFigure.minX > placeFigure.minX) {
                rowFigure = figures.at(2);
                placeFigure = figures.at(1);
            }
        } else {
            if (rowFigure.minX < placeFigure.minX) {
                rowFigure = figures.at(2);
                placeFigure = figures.at(1);
            }
        }
        if (codeFigure.width > codeFigure.height) {
            float kWidth = 3.1f;
            float kHeight = 1.f;
            uint16_t minWidth = kWidth * rowFigure.width;
            uint16_t minHeight = kHeight * rowFigure.height;
            if (codeFigure.width < minWidth) {
                codeFigure.width = minWidth;
                codeFigure.maxX = codeFigure.minX + codeFigure.width;
            }
            if (codeFigure.height < minHeight) {
                codeFigure.height = minHeight;
                codeFigure.maxY = codeFigure.minY + codeFigure.height;
            }
        }
        info.push_back(ShowFigure(resultImage, codeFigure, "Code Image " + imageName));
        info.push_back(ShowFigure(resultImage, rowFigure, "Row Image " + imageName));
        info.push_back(ShowFigure(resultImage, placeFigure, "Place Image " + imageName));
    }
    return info;
}

cv::Mat FindRedSquarea(const cv::Mat& image)
{
    cv::Mat newImage;
    image.copyTo(newImage);
    cv::Mat hsvImage;
    image.copyTo(hsvImage);
    cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            uint8_t h = cv::saturate_cast<uchar>(hsvImage.at<cv::Vec3b>(y,x)[0]);
            uint8_t s = cv::saturate_cast<uchar>(hsvImage.at<cv::Vec3b>(y,x)[1]);
            uint8_t v = cv::saturate_cast<uchar>(hsvImage.at<cv::Vec3b>(y,x)[2]);
            for (int c = 0; c < image.channels(); c++) {
                if (h > 6 || s < 150 || v < 75) {
                    newImage.at<cv::Vec3b>(y,x)[c] = 0;
                }
            }
        }
    }
    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat filterImage;
    newImage.copyTo(filterImage);
    cvtColor(filterImage, filterImage, cv::COLOR_BGR2GRAY);
    erode(filterImage, filterImage, element);
    dilate(filterImage, filterImage, element);
    medianBlur(filterImage, filterImage, 3);
    Points points = SelectPoints(FindContour(filterImage));
    Figures figures = FindFigures(points);
    float scale = 1.2f;
    Figure maxFigure;
    for (const auto& figure : figures) {
        if ((figure.width < scale * figure.height
             && figure.height < scale * figure.width)) {
            if (maxFigure.area < figure.area) {
                maxFigure = figure;
            }
        }
    }
    float p = 0.2;
    int offsetX = p * maxFigure.width;
    int offsetY = p * maxFigure.height;
    int startX = maxFigure.minX + offsetX;
    int startY = maxFigure.minY + offsetY;
    int width = maxFigure.width - 2 * offsetX;
    int height = maxFigure.height - 2 * offsetY;
    return cv::Mat(image, cv::Rect(startX, startY, width, height));
}

cv::Mat ClearImage(const cv::Mat& image)
{
    cv::Mat result;
    image.copyTo(result);
    cvtColor(result, result, cv::COLOR_BGRA2GRAY);
    DeleteGlare(result).copyTo(result);
    int widthImage = result.size().width;
    int count = (widthImage > 150) ? 3 : 2;
    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    for (int i = 0; i < count; ++i) {
        dilate(result, result, element);
    }
    for (int i = 0; i < count; ++i) {
        erode(result, result, element);
    }
    return result;
}


cv::Mat DeleteGlare(const cv::Mat& image, uint8_t max)
{
    cv::Mat result;
    image.copyTo(result);
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            if (image.at<uchar>(y, x) > max) {
                result.at<uchar>(y, x) = max;
            }
        }
    }
    return result;
}

cv::Mat FindContour(const cv::Mat& image)
{
    cv::Mat contour;
    Canny(image, contour, 45, 200, 3);
    return contour;
}

Points SelectPoints(const cv::Mat& image, uint8_t min)
{
    Points points;
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            if (image.at<uchar>(y, x) >= min) {
                points[x][y] = true;
            }
        }
    }
    return points;
}

void FindFigure(uint16_t x, uint16_t y, Figure& figure, Points& points)
{
    points[x][y] = false;
    if (x < figure.minX) {
        figure.minX = x;
    }
    if (x > figure.maxX) {
        figure.maxX = x;
    }
    if (y < figure.minY) {
        figure.minY = y;
    }
    if (y > figure.maxY) {
        figure.maxY = y;
    }
    ++figure.size;
    int8_t dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int8_t dy[8] = {0, -1, -1, -1, 0, 1, 1, 1};
    for (uint8_t k = 0; k < 8; ++k) {
        uint16_t posX = x + dx[k];
        uint16_t posY = y + dy[k];
        if (points.count(posX)) {
            if (points.at(posX).count(posY)) {
                if (points.at(posX).at(posY)) {
                    FindFigure(posX, posY, figure, points);
                }
            }
        }
    }
}

Figures FindFigures(Points& points)
{
    Figures figures;
    for (auto& [x, col] : points) {
        for (auto& [y, flag] : col) {
            if (flag) {
                Figure figure;
                FindFigure(x, y, figure, points);
                figures.push_back(figure);
            }
        }
    }
    for (Figure& figure : figures)  {
        figure.width = figure.maxX - figure.minX;
        figure.height = figure.maxY - figure.minY;
        figure.area = figure.width * figure.height;
    }
    return figures;
}

cv::Mat ShowFigure(const cv::Mat& image, const Figure& figure,
                   const cv::String& imageName) {
    int startX = figure.minX;
    int startY = figure.minY;
    int width = figure.width;
    int height = figure.height;
    cv::Mat copyImage;
    image.copyTo(copyImage);
    cv::Mat resultImage(copyImage, cv::Rect(startX, startY, width, height));
    imshow(imageName, resultImage);
    return resultImage;
}
