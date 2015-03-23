#include "image.h"

Image::Image()
{

    isPaintingRed = false;
    isPaintingBlue = false;

    if(!palmRecognizer.load("/Users/jasondebottis/Desktop/QTProjects/490FinalProj/cascades/peaceSymbol.xml"))
    {
        qDebug("failed to load cascade file");
    }
    if(!fistRecognizer.load("/Users/jasondebottis/Desktop/QTProjects/490FinalProj/cascades/fist.xml"))
    {
        qDebug("failed to load cascade file");
    }

    if(!faceRecognizer.load("/Users/jasondebottis/Desktop/QTProjects/490FinalProj/cascades/haarcascade_frontalface_alt2.xml"))
    {
        qDebug("failed to load cascade file");
    }
    detector = cv::FeatureDetector::create("FAST");
    sampleCounter = 0;
    cropRect = Rect(10,20,130,215);
    getSkinValRect = Rect(100,100,200,150);
    colorTotal = 0;
    int history = 1000;
    int nmixtures = 16;
    int bShadowDetection = 0;

    bg = new BackgroundSubtractorMOG2(history,nmixtures,bShadowDetection);
}
void Image::sobelMy(Image *rawIm){
    int total;
    if (!src.data){
        src = rawIm->src.clone();
    }
    src=Mat::zeros(rawIm->src.size(), rawIm->src.type());

    for (int y = 1; y < src.rows-1; y++)
    {

        for (int x = 1; x < src.cols-1; x++)
        {
            total=0;
            for (int c = 0; c<3; c++)
            {
                //total=0;
                total+=-2*rawIm->src.at<Vec3b>(y-1,x-1)[c];
                total+=-1*rawIm->src.at<Vec3b>(y-1,x)[c];
                //total+=rawIm->src.at<Vec3b>(y-1,x+1)[c];

                total+=-1*rawIm->src.at<Vec3b>(y,x-1)[c];
                //total+=rawIm->src.at<Vec3b>(y,x)[c];
                total+=1*rawIm->src.at<Vec3b>(y,x+1)[c];

                //total+=rawIm->src.at<Vec3b>(y+1,x-1)[c];
                total+=1*rawIm->src.at<Vec3b>(y+1,x)[c];
                total+=2*rawIm->src.at<Vec3b>(y+1,x+1)[c];

                total+=abs(total);
            }

            total*=4;
            if (total>255)
                total=0;//255;
            for (int c = 0; c<3; c++)
            {
                src.at<Vec3b>(y,x)[c]=total;
            }
        }
    }
}


void Image::setCameraNum(int i)
{
    cam = VideoCapture(i);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT,600);
    cam.set(CV_CAP_PROP_FRAME_WIDTH,600);
}


void Image::handDetect(bool collectPoints,Image *canvasImage,int paintColor){
    Mat img_grey, canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> heiarchy;
    double area = 0.0, areamax = 0.0;

    RNG rng(12345);

    cvtColor(src,img_grey,CV_BGR2GRAY);
    blur(img_grey,img_grey,Size(3,3));

    Canny(img_grey,canny_output,100,200,3);
    findContours(canny_output,contours,heiarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    vector<vector<Point> > hull (contours.size());

    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius.at(i) );

    }

    for(int i=0; i < contours.size(); i ++){
        convexHull(Mat(contours[i]),hull[i],false);
    }

    //draw contours + hull results
    for(int i=0; i < contours.size(); i++){
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( src, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );

        if(radius.at(i)>40.0){
          circle( src, center[i], (int)radius[i], color, 2, 8, 0 );
        }
        cv::Rect boundingRectangle(boundRect[i].x,boundRect[i].y,boundRect[i].width,boundRect[i].height);

        if(collectPoints){
            collectedPoints.push_back(boundingRectangle);
            paintIndexs.push_back(paintColor);

        }


        area = contourArea(contours[i]);
        if(area>areamax){
            areamax=area;
        }
        if(areamax > 400){
        }
    }
    for(int totalPts=0;totalPts<collectedPoints.size();totalPts++){
        Scalar color;
        switch(paintIndexs.at(totalPts)){
        case 0:
            color = Scalar(0,0,255);//Red
            break;
        case 1:
            color = Scalar(255,0,0);//Blue
            break;
         case 2:
            color = Scalar(0,255,0);//Green
            break;
         case 3:
            color = Scalar(255,255,255);//white
            break;
         case 4:
            color = Scalar(0,0,0);//Black
            break;
         case 5:
            color = Scalar(0,255,255);//Yellow
            break;
         case 6:
            color = Scalar(150,150,150);//Grey
            break;
         default:
            break;

        }

        rectangle(canvasImage->src,collectedPoints.at(totalPts),color,-1,8,0);
    }

}
void Image::handDetect2(Image *otherImg,Image *otherBinImage,int thresh){
    cvtColor(src,otherImg->src,CV_BGR2HSV);
    otherBinImage->src=Mat::zeros(src.size(),src.type());
    for(int x=0;x<src.rows;x++){
        for(int y=0;y<src.cols;y++){
            double color = otherImg->src.at<Vec3b>(x,y)[0]+otherImg->src.at<Vec3b>(x,y)[1];
            double aveColor = color/2;
            if(aveColor > (colorTotal-thresh) && aveColor < (colorTotal + thresh)){
                for(int i=0;i < 2;i++){
                    otherBinImage->src.at<Vec3b>(x,y)[i]=src.at<Vec3b>(x,y)[i];
                }
            }
        }
    }
}
void Image::handDetect3(Image *HSVImage, Image *result,double hVal, double sVal){
    result->src=Mat::zeros(src.size(),src.type());
    cvtColor(src,HSVImage->src,CV_BGR2HSV);

    vector<double> HueVals,SatVals;
    cv::Rect CanvasRect(50,50,500,500);
    rectangle(src,CanvasRect,Scalar(0,0,255),3,8,0);
    for (int x=CanvasRect.x;x<CanvasRect.x+CanvasRect.width;x++){
        for (int y=CanvasRect.y;y<CanvasRect.y+CanvasRect.height;y++){
            //HSVImage->src.at<Vec3b>(x,y)[3]=0;
            HueVals.push_back(HSVImage->src.at<Vec3b>(x,y)[0]);
            SatVals.push_back(HSVImage->src.at<Vec3b>(x,y)[1]);
            if((HSVImage->src.at<Vec3b>(x,y)[0] > hVal && HSVImage->src.at<Vec3b>(x,y)[1] >sVal ) ){
                result->src.at<Vec3b>(x,y)[0]=255;
                result->src.at<Vec3b>(x,y)[1]=255;
                result->src.at<Vec3b>(x,y)[2]=255;
            }
        }
    }
    double test;
    for(int i=0;i<HueVals.size();i++){
        test = test + HueVals.at(i);
    }
    test = test / HueVals.size();
    //qDebug("%f",test);
}


void Image::applyBackgroundSub(Image *img){
    Mat back;
    Mat fore;
    vector<Vec4i> heiarchy;

    src = img->src.clone();
    bg->operator ()(img->src,fore);
    bg->getBackgroundImage(back);
    vector<vector<Point> > contours;
    cv::erode(fore,fore,Mat());
    cv::dilate(fore,fore,Mat());
    findContours(fore,contours,heiarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    cvtColor(src,src,CV_BGR2GRAY);
    threshold(src,src,100,255,CV_THRESH_BINARY);
    src = fore.clone();
    for(int i=0; i < contours.size(); i++){
        drawContours( src, contours, i, Scalar(255,255,255), -1, 8, vector<Vec4i>(), 0, Point() );
    }

}
void Image::normCrossCorr(Image *templateImg){


    float total1, ave1;
    float total2, ave2;
    float stdDev1,stdDev2;
    float correlationScore;
    float numerator, demoninator;
    vector <float> averages1;
    vector <float> totals;

    // for entire image += by template size
    for (int x=0;x < src.rows; x+=templateImg->src.rows){
        for (int y=0;y < src.cols; y+=templateImg->src.cols){

            //get totals and averages
            total1 = src.at<Vec3b>(x,y)[0]+src.at<Vec3b>(x,y)[1]+src.at<Vec3b>(x,y)[2];
            ave1 = total1/3;
            float newTotal = total1 - ave1;
            //qDebug(newTotal);
            //averages1.push_back(ave1);
            //totals.push_back(total1);

        }
    }

    /*
    //for entire template image by each pix
    for (int x=0;x < templateImg->src.rows; x+=1){
        for (int y=0;y < templateImg->src.cols; y+=1){
            //get totals and averages
            total2 = templateImg->src.at<Vec3b>(x,y)[0]+templateImg->src.at<Vec3b>(x,y)[1]+templateImg->src.at<Vec3b>(x,y)[2];
            ave2 = total1/3;
            averages2.push_back(ave2);
        }
    }
    */
}

void Image::createSamples(){
    takePicture();
    sampleCounter += 1;
    QString fileName = QString("%1-%2.jpeg").arg("/Users/jasondebottis/Desktop/QTProjects/490FinalProj/samples/pos",QString::number(sampleCounter));
    qDebug() << fileName;
    string cStringFileName = fileName.toStdString();
    Mat croppedImage = src(cropRect);
    imwrite(cStringFileName,croppedImage);
}

void Image::saveImage(QString filename){
    QString fileName1 = QString("%1").arg("/Users/jasondebottis/Desktop/QTProjects/490FinalProj/");
    fileName1.append(QString("%1.jpeg").arg(filename));
    std::string file_str = fileName1.toUtf8().constData();
    cv::Mat output;
    src.copyTo(output);
    imwrite(file_str,output);
    qDebug() << fileName1;
}

void Image::takePicture(){

    cam >>src;
    cv::flip(src,src,1);
}
float Image::calcMahal(double pixAve1, double pixAve2, double stdDev1, double stdDev2){
    return abs(pixAve1 - pixAve2) / (sqrt((stdDev1 * stdDev1)+(stdDev2*stdDev2)));
}

void Image::copyImg(Image *imgToCpy){
    src = imgToCpy->src.clone();
}
float Image::stdDev(cv::Mat image, int x1, int y1,float avg){
    float sum=0;
    float sub =0;
    int count =0;
    //float avg = average(image,x1,y1);
    for (int x=x1; x<=x1; x++)
        for (int y=y1; y<=y1; y++){
            sub=image.at<uchar>(y,x)-avg;
            sub*=sub;
            sum+=sub;
            count++;
        }
    sum/=count;
    return sqrt(sum);
}

float Image::average(cv::Mat image, int x1, int y1){
    float avg=0;
    int count =0;
    for (int x=x1; x<=x1; x++){
        for (int y=y1; y<=y1; y++){
            avg+=image.at<uchar>(y,x);
            count++;
        }
    }
    return avg/count;
}

QImage Image::convertToQImage(bool isBin){
    QImage img;
    if(isBin){
        cvtColor(src, display, CV_GRAY2BGR);
        cvtColor(display, display, CV_BGR2RGB);
    }else{
        cvtColor(src, display, CV_BGR2RGB);
    }
    img = QImage((const unsigned char*)(display.data), display.cols, display.rows, display.step, QImage::Format_RGB888);

    return img;
}

void Image::displayImage(QLabel &location,bool isBin){
    QImage img = convertToQImage(isBin);
    location.setPixmap(QPixmap::fromImage(img));
}

void Image::loadImage(string path){
    src = imread(path,CV_LOAD_IMAGE_COLOR);
}

QImage Image::Mat2Qimage(cv::Mat mat){
    QImage qimgConverted((const unsigned char *)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return qimgConverted;
}


void Image::detectObjects(){
    std::vector<Rect> palms;
    Mat frame_gray;
    cvtColor( src, frame_gray, CV_BGR2GRAY );

    equalizeHist( frame_gray, frame_gray );

    //-- Detect fists
    palmRecognizer.detectMultiScale(frame_gray, palms, 1.1,2,0|CV_HAAR_SCALE_IMAGE,cv::Size(30,30));

    for (size_t i= 0; i < palms.size();i++){
        Point center( palms[i].x + palms[i].width*0.5, palms[i].y + palms[i].height*0.5 );
        //qDebug("palms size %f",palms[i].width);
            if(palms[i].width > 150){
                cv::Rect ROI(palms[i].x,palms[i].y,palms[i].width,palms[i].height);
                rectangle(src,ROI,Scalar( 255, 0, 0 ),1,8,0);

            }

            if(center.x < 200 && center.y < 150){
                isPaintingRed = true;
                isPaintingBlue = false;
            }

            if(center.x > 200 && center.y > 150){
                isPaintingRed = false;
                isPaintingBlue = true;
            }


    }

    /*
    for(int i=0;i<paintRects.size();i++){
        if (collectPoints){
            rectangle(src,paintRects.at(i),Scalar(0,0,255),-1,8,0);
            src.at<Vec3b>(drawingPts.at(i).x,drawingPts.at(i).y)[2] = 255;
        }

    }
    if (isPaintingRed){

    }else if(isPaintingBlue){
        for(int i=0;i < paintRects.size();i++){
            rectangle(src,paintRects.at(i),Scalar(255,0,0),-1,8,0);
        }

    }
    */
}


