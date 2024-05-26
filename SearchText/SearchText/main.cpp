#include "ImageProcessing.h"

int main()
{
    FindInfo("Test.jpg");
    FindInfo("test0.png");
    FindInfo("test1.png");
    FindInfo("test2.png");
    FindInfo("test3.png");
    FindInfo("test4.png");
    cv::waitKey(0);
    return 0;
}
