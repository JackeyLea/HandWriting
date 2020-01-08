/*
 * @author jackey lea
 * @date 2019-6-18
 * @function image process
 * @description including any actions ,any methods aims to process image like gray binary rgb2bgr and ect.
 */

#ifndef BASE_HPP
#define BASE_HPP

//Qt标准库
#include <QDebug>
#include <QString>
#include <QImage>
#include <QLabel>
#include <QFileDialog>
#include <QMainWindow>
#include <QResizeEvent>
#include <QTextCodec>
#include <QMessageBox>
#include <QPixmap>

//opencv头文件
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

//标准库
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

namespace Silkworm{
//在标签中显示图片
inline void displayImage(QLabel *label, Mat srcImage)
{
    label->clear();
    cvtColor(srcImage,srcImage,COLOR_RGB2BGR);
    QImage img = QImage(static_cast<const unsigned char*>(srcImage.data),
                        srcImage.cols,srcImage.rows,srcImage.cols*srcImage.channels(),
                        QImage::Format_RGB888);
    //img = img.scaled(label->size());
    label->setPixmap(QPixmap::fromImage(img));
}
//计算列像素值
inline int getColSum(Mat inputImage,int col)
{
    int sum=0;
    int height = inputImage.rows;
    for (int j = 0; j < height; j++)
    {
        sum += inputImage.at<uchar>(j, col);
    }
    return sum;
}
//计算行像素值
inline int getRowSum(Mat inputImage,int row)
{
    int sum = 0;
    int width = inputImage.cols;
    for (int j = 0; j < width; j++)
    {
        sum += inputImage.at<uchar>(row, j);
    }
    return sum;
}
//灰度化图像
inline Mat grayImage(Mat inputImage)
{
    Mat grayImage, tempImage;
    cvtColor(inputImage, tempImage, COLOR_RGB2GRAY);
    tempImage.copyTo(grayImage);
    tempImage.release();
    return grayImage;
}
//输入必须是灰度图像，同时规定结果图片字为黑色
inline Mat binaryImage(Mat graySrc,int threholdValue=-1)
{
    Mat binary; //用于存储结果
    int middle=0;

    if(threholdValue ==-1){
        uchar *p;
        int nrows = graySrc.rows; //灰度图行像素值
        int ncols = graySrc.cols;
        long long int count = 0;
        int mid = 0;
        //获取并计算灰度值的平均值
        for (int i = 0; i < nrows; i++)
        {
            p = graySrc.ptr<uchar>(i);
            for (int j = 0; j < ncols; j++)
            {
                mid += p[j];
                count++;
            }
        }
        middle = int((mid / count) * 2/3);//
    }
    else{
        middle = threholdValue;
    }
    qDebug()<<"Average pixel is: "<<middle;
    //以平均值二值化图片
    threshold(graySrc, binary, middle, 255, THRESH_BINARY);
    return binary;
}
//腐蚀图片,扩大文字区域
inline Mat erodeImage(Mat inputImage, int erosion_type = MORPH_RECT, int erosion_size=3)
{
    cv::Mat tempImage;
    Mat element = getStructuringElement(erosion_type,
                                        Size(2*erosion_size+1,2*erosion_size+1),
                                        Point(erosion_size,erosion_size));
    erode(inputImage,tempImage,element);

    return tempImage;
}
//膨胀图片，扩大非文字区域
/*
 * @inputImage input image
 * @expand_type 膨胀核类型
 * @expand_size 膨胀核大小
 */
inline Mat dilateImage(Mat inputImage,int expand_type=MORPH_RECT,int expand_size=1)
{
    Mat tempImage;
    Mat element = getStructuringElement(expand_type, Size(expand_size, expand_size),
                                        Point(-1, -1));
    dilate(inputImage, tempImage, element);

    return tempImage;
}
//切下左右两端图片，保存中间的值,0为正常，1为错误
inline int cutVertical(Mat &srcImage, Mat &leftImage, Mat &rightImage)
{
    int left = 0, right = srcImage.cols, b;
     qDebug() << "image width is: " << right;
     for (b = 0; b < srcImage.cols; b++)
     {
         int colValue = getColSum(srcImage, b);
         //qDebug()<<"Col: "<<b<<" value is: "<< colValue << endl;
         if (colValue > 0)
         {
             left = b;
             break;
         }
     }
     qDebug() << "left is:" << left;
     for (; b < srcImage.cols; b++)
     {
         int colValue = getColSum(srcImage, b);
         //qDebug()<<"right col is: "<< b << " value is: " << colValue << endl;
         if (colValue == 0)
         {
             right = b;
             break;
         }
     }
     qDebug() << "right is: " << right;
     //计算图片宽度，为0表示图像为空白
     int width = right - left;
     if (width == 0)
         return 1;
     Rect rect(left, 0, width, srcImage.rows);
     //切割后的图片
     leftImage = srcImage(rect).clone();
     Rect rectRight(right, 0, srcImage.cols - right, srcImage.rows);
     //切割后原图剩余的图片
     rightImage = srcImage(rectRight).clone();
     return 0;
}
//切割上下的空白值，保留中间的部分
inline void cutHorizon(Mat &srcImage, Mat &dstImage)
{
    int top = 0, bottom = srcImage.rows, b, rowValue;
    qDebug() << "image height is :" << bottom;
    //获取顶部
    for (b = 0; b < srcImage.rows; b++)
    {
        rowValue = getRowSum(srcImage, b);
        //qDebug()<<"top row is "<<b<<" value is: "<< rowValue << endl;
        if (rowValue > 0)
        {
            top = b;
            break;
        }
    }
    //获取底部
    for (; b < srcImage.rows; b++)
    {
        rowValue = getRowSum(srcImage, b);
        //qDebug()<<"bottom row is "<< b << " value is: " << rowValue << endl;
        if (rowValue == 0)
        {
            bottom = b;
            break;
        }
    }
    //计算高度
    int height = bottom - top;
    //左端 顶端 宽度 高度
    Rect rect(0, top, srcImage.cols, height);
    //dstImage 切割结果图片
    dstImage = srcImage(rect).clone();
    //调用函数匹配
}
//计算特征值
inline void getFeature(Mat m,float a[])
{
    int M,N;//存储图像m的宽高
    int i,j;
    M=m.cols;
    N=m.rows;
    for(i=0;i<25;i++)
        a[i]=0;
    //转换为灰度图像
    cv::cvtColor(m,m,COLOR_RGB2GRAY);
    //转换为二值图像
    cv::threshold(m,m,100,255,THRESH_BINARY);
    //将图像划分为5-5个子块，计算每块像素值的和
    for(i=0;i<M;i++){
        for(j=0;j<N;j++){
            if(m.at<uchar>(i,j)==255){
                a[i/(M/5)*5+j/(N/5)]++;
            }
        }
    }
    //计算当前像素块的平均值
    for(i=0;i<25;i++)
        a[i]=a[i]/((M/5)*(N/5));
}
//计算欧氏距离
inline float ouDistance(float a[],float b[])
{
    float distance =0;
    //不要忘记初始化，否则会出错
    //根据欧氏距离计算公式,计算距离的平方
    for(int i=0;i<25;i++)
        distance += (a[i]-b[i])*(a[i]-b[i]);
    //对上述结果开根号
    distance=sqrt(distance);
    return distance;
}
//显示RGB空间值
inline void showRGBSpace(Mat inputImage, int n, QLabel *label)
{
    Mat bgr_res[3];
    Mat bgr[3];
    for(int j=0;j<3;j++){
        split(inputImage,bgr);
        for(int i=0;i<3;++i){
            if(i!=j)
                bgr[i]=Mat::zeros(inputImage.size(),bgr[0].type());
        }
        merge(bgr,3,bgr_res[j]);
    }
    QImage img = QImage((const unsigned char*)(bgr_res[n].data),
                 bgr_res[n].cols,bgr_res[n].rows,bgr_res[n].cols*bgr_res[n].channels(),
                 QImage::Format_RGB888);
    label->clear();
    img = img.scaled(label->size());
    label->setPixmap(QPixmap::fromImage(img));
}


/*
 * @function find all text region within input image
 * @input input image in erode format
 * @output detected text region in vector format
 */
inline vector<RotatedRect> findTextRegion(Mat inputImage)
{
    double originArea = inputImage.size().area();
    qDebug() << "Original area is: " << originArea;
    //RotatedRect类表示平面上的旋转矩形，有三个属性：矩形中心点(质心)、矩形大小、旋转角度
    vector<RotatedRect> rects;      //结果
    vector<vector<Point>> contours; //轮廓
    vector<Vec4i> hierarchy;
    //第一个参数是膨胀后的图像，第二个参数是用于检测轮廓向量
    //findContours(OpenedImage,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    //findContours(inputImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
    cv::findContours(inputImage,contours,RETR_CCOMP,CHAIN_APPROX_SIMPLE,Point(100,100));
    qDebug() << "Contours size is: " << contours.size();
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        //计算当前轮廓的面积
        double area = contourArea(contours[i]);
        qDebug() << "current area size is:" << area;
        //面积小于100或者大于原图的全部筛选掉
        if (area < 100)
            continue;
        if (area >= originArea*0.75)
            continue;
        //轮廓近似，作用较小，approxPolyDP函数有待研究
        double epsilon = 0.001 * arcLength(contours[i], true);
        Mat approx;
        approxPolyDP(contours[i], approx, epsilon, true);
        //找到最小矩形，该矩形可能有方向
        RotatedRect rect = minAreaRect(contours[i]);
        //计算高和宽
        int m_width = rect.boundingRect().width;
        int m_height = rect.boundingRect().height;
        //筛选那些太细的矩形，留下扁的
        if (m_height > m_width * 1.2)
            continue;
        //符合条件的rect添加到rects集合中
        rects.push_back(rect);
    }
    qDebug() << "Rects size is: " << rects.size();

    return rects;
}
//提取文本和调整文本倾斜
inline Mat deskewAndCrop(Mat inputImage,const RotatedRect &box)
{
    //RotatedRect类型的 box的属性有：size , center , angle
     double angle = double(box.angle);
     Size2f size = box.size;
     //调整框角度
     if (angle < -45.0)
     {
         angle += 90.0; //角度小于45度，意味着文本是垂直对齐的，增加90度的旋转角度，交换宽度与高度属性
         swap(size.width, size.height);
     }
     //根据角度旋转文本，先创建一个描述旋转的二维仿射变换矩阵
     //参数：旋转中心，旋转角度，1.0表示保持框的原始比例不变
     Mat transform = getRotationMatrix2D(box.center, angle, 1.0);
     Mat rotated;
     warpAffine(inputImage, rotated, transform, inputImage.size(), INTER_CUBIC); //旋转函数
     //---------------test--------
     //imshow("【旋转后的图像】",rotated);
     Mat cropped; //裁剪结果
     //void getRectSubPix(InputArray image, Size patchSize, Point2f center, OutputArray patch, int patchType=-1 )
     getRectSubPix(rotated, size, box.center, cropped);//裁剪矩形区域的边界框
     copyMakeBorder(cropped, cropped, 10, 10, 10, 10, BORDER_CONSTANT, Scalar(0)); //增加图像边界
     return cropped;
}
/*
 * @param the image that need to be cutted
 * @function 分割图片
*/
inline void spliteImage(Mat srcImage)
{
    qDebug()<<"Start splite image ------>";
    qDebug()<<"image cols is: "<<srcImage.cols<<" rows is: "<<srcImage.rows;
    for (int i=0;i<srcImage.cols;i++) {
    }

    qDebug()<<">------end splite image";
}
//查找区域
inline void detectArea(Mat inputImage, Mat &srcImage)
{
    qDebug()<<"detectArea(): detecting now";
    vector<RotatedRect> rects = findTextRegion(inputImage); //inputImage 是腐蚀处理的结果
    qDebug() << "Starting cut image";
    qDebug() << "Currenting detect image area count is: " << rects.size();
    foreach (cv::RotatedRect rect, rects){
        //用绿线画出这些找到的轮廓
                Point2f P[4];
                rect.points(P);
                for(int j = 0; j <= 3; j++){
                    line(srcImage,P[j],P[(j+1)%4],Scalar(0,255,0),2);
                }
        //裁剪图片
        //auto cropped = deskewAndCrop(srcImage, rect);
        //spliteImage(cropped);
    }

    qDebug()<<"detectArea(): detecting done";
}

// 将Mat转化位QImage
inline QImage  cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() ){
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                  inMat.cols, inMat.rows,
                  static_cast<int>(inMat.step),
                  QImage::Format_ARGB32 );

        return image;
    }

// 8-bit, 3 channel
case CV_8UC3:
{
QImage image( inMat.data,
              inMat.cols, inMat.rows,
              static_cast<int>(inMat.step),
              QImage::Format_RGB888 );

return image.rgbSwapped();
}

// 8-bit, 1 channel
case CV_8UC1:
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
QImage image( inMat.data,
              inMat.cols, inMat.rows,
              static_cast<int>(inMat.step),
              QImage::Format_Grayscale8 );//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else//这里还有一种写法，最后给出
static QVector<QRgb>  sColorTable;

// only create our color table the first time
if ( sColorTable.isEmpty() )
{
    sColorTable.resize( 256 );

    for ( int i = 0; i < 256; ++i )
    {
        sColorTable[i] = qRgb( i, i, i );
    }
}

QImage image( inMat.data,
              inMat.cols, inMat.rows,
              static_cast<int>(inMat.step),
              QImage::Format_Indexed8 );

image.setColorTable( sColorTable );
#endif

return image;
}

default:
qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
break;
}

return QImage();
}

//将Mat转化为QPixmap
inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
return QPixmap::fromImage( cvMatToQImage( inMat ) );
}
//将QImage转化为Mat
inline cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true )
{
switch ( inImage.format() )
{
// 8-bit, 4 channel
case QImage::Format_ARGB32:
case QImage::Format_ARGB32_Premultiplied:
{
cv::Mat  mat( inImage.height(), inImage.width(),
              CV_8UC4,
              const_cast<uchar*>(inImage.bits()),
              static_cast<size_t>(inImage.bytesPerLine())
              );

return (inCloneImageData ? mat.clone() : mat);
}

// 8-bit, 3 channel
case QImage::Format_RGB32:
case QImage::Format_RGB888:
{
if ( !inCloneImageData )
{
    qWarning() << "CVS::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
}

QImage   swapped = inImage;

if ( inImage.format() == QImage::Format_RGB32 )
{
    swapped = swapped.convertToFormat( QImage::Format_RGB888 );
}

swapped = swapped.rgbSwapped();

return cv::Mat( swapped.height(), swapped.width(),
                CV_8UC3,
                const_cast<uchar*>(swapped.bits()),
                static_cast<size_t>(swapped.bytesPerLine())
                ).clone();
}

// 8-bit, 1 channel
case QImage::Format_Indexed8:
{
cv::Mat  mat( inImage.height(), inImage.width(),
              CV_8UC1,
              const_cast<uchar*>(inImage.bits()),
              static_cast<size_t>(inImage.bytesPerLine())
              );

return (inCloneImageData ? mat.clone() : mat);
}

default:
qWarning() << "CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
break;
}

return cv::Mat();
}

//将QPixmap转化为Mat
inline cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true )
{
    return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
}

inline QImage to8UC1(cv::Mat mat)
{
    //CV_8UC1另一种写法，不过我觉得上面的一种写法更优雅^_^
    QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
    // Set the color table (used to translate colour indexes to qRgb values)
    image.setColorCount(256);
    for(int i = 0; i < 256; i++){
    image.setColor(i, qRgb(i, i, i));
    }
    // Copy input Mat
    uchar *pSrc = mat.data;
    for(int row = 0; row < mat.rows; row ++){
        uchar *pDest = image.scanLine(row);
        memcpy(pDest, pSrc, mat.cols);
        pSrc += mat.step;
    }
    return image;
}
}

#endif // BASE_HPP
