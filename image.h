#ifndef IMAGE_H
#define IMAGE_H
//copy values of drawing to a mat and then recopy the color values back to main frame
//morphorilogical close

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>

#include <QLabel>
#include <QtCore>
#include <QString>

#include <string>

using namespace cv;
using namespace std;

class Image
{
public:
    Image();
    ~Image();
    void setCameraNum(int i);
    void takePicture();
    void copyImg(Image *imgToCpy);
    QImage convertToQImage(bool isBin);
    void displayImage(QLabel &location,bool isBin);
    QImage Mat2Qimage(cv::Mat mat);
    void detectObjects();
    void loadImage(string path);
    void saveImage(QString filename);
    void handDetect(bool collectPoints,Image *canvasImage,int paintColor);
    void handDetect2(Image *otherImg,Image *otherBinImage, int thresh);
    void handDetect3(Image *HSVImage,Image *result,double hVal, double sVal);

    void createSamples();
    void sobelCV(Image *rawImg);
    void sobelMy(Image *rawIm);
    void getAve(Image *img);

    float calcMahal(double pixAve1,double pixAve2,double stdDev1,double stdDev2);
    float stdDev(cv::Mat image, int x1, int y1,float avg);
    float average(cv::Mat image, int x1, int y1);

    void normCrossCorr(Image *templateImg);
    //background subtractor stuff
    void applyBackgroundSub(Image *img);
private:
    Mat src;
    Mat display;
    VideoCapture cam;
    CascadeClassifier fistRecognizer;
    CascadeClassifier palmRecognizer;
    CascadeClassifier faceRecognizer;
    bool isPaintingRed,isPaintingBlue;
    vector<Rect> paintRects;
    std::vector<Point> drawingPts;
    cv::FREAK extractor;
    Mat descriptorsA, descriptorsB;
    std::vector<KeyPoint> keypointsA, keypointsB;
    Ptr<cv::FeatureDetector> detector;
    std::vector<DMatch> matches;
    cv::Mat matchResult;
    Mat objImage;
    int sampleCounter;
    cv::Rect cropRect;
    cv::Rect getSkinValRect;
    double colorTotal;
    vector<Rect> collectedPoints;
    vector<int> paintIndexs;
    //Background subtraction stuff
    BackgroundSubtractorMOG2 *bg;
    vector<vector<Point> > contours;
};

#endif // IMAGE_H
