#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include "base.hpp"

class Processing {
public:
private:
    int count =0;
public:
    Processing() {
    }

    ~Processing() {
    }

    //在标签中显示图片
    void displayImage(QLabel *label, Mat srcImage) {
        label->clear();
        cvtColor(srcImage, srcImage, COLOR_RGB2BGR);
        QImage img = QImage((const unsigned char *)(srcImage.data), srcImage.cols, srcImage.rows,
                            srcImage.cols * srcImage.channels(), QImage::Format_RGB888);
        // img = img.scaled(label->size());
        label->setPixmap(QPixmap::fromImage(img));
    }

    //计算列像素值
    int getColSum(Mat inputImage, int col) {
        int sum    = 0;
        int height = inputImage.rows;
        for (int j = 0; j < height; j++) {
            sum += inputImage.at<uchar>(j, col);
        }
        return sum;
    }

    //计算行像素值
    int getRowSum(Mat inputImage, int row) {
        int sum   = 0;
        int width = inputImage.cols;
        for (int j = 0; j < width; j++) {
            sum += inputImage.at<uchar>(row, j);
        }
        return sum;
    }

    //灰度化图像
    Mat toGrayImg(Mat srcImg) {
        Mat grayImg;
        if (srcImg.channels() == 3) {
            cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
        } else {
            grayImg = srcImg;
        }
        return grayImg;
    }

    //输入必须是灰度图像，同时规定结果图片字为黑色
    Mat toBinaryImg(Mat grayImg, int threholdValue = -1) {
        Mat binary; //用于存储结果
        int middle = 0;

        if (threholdValue == -1) {
            uchar *       p;
            int           nrows = grayImg.rows; //灰度图行像素值
            int           ncols = grayImg.cols;
            long long int count = 0;
            int           mid   = 0;
            //获取并计算灰度值的平均值
            for (int i = 0; i < nrows; i++) {
                p = grayImg.ptr<uchar>(i);
                for (int j = 0; j < ncols; j++) {
                    mid += p[ j ];
                    count++;
                }
            }
            middle = int((mid / count) * 2 / 3); //
        } else {
            middle = threholdValue;
        }
        qDebug() << "Average pixel is: " << middle;
        //以平均值二值化图片
        threshold(grayImg, binary, middle, 255, THRESH_BINARY);
        return binary;
    }

    //腐蚀图片,扩大文字区域，扩大黑色区域，缩小白色区域
    Mat toErodeImg(Mat inputImg, int erosion_type = MORPH_RECT, int erosion_size = 3) {
        Mat tempImage;
        Mat element =
            getStructuringElement(erosion_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                  Point(erosion_size, erosion_size));
        erode(inputImg, tempImage, element); //腐蚀函数

        return tempImage;
    }

    //膨胀图片，扩大非文字区域
    /*
     * @inputImage input image
     * @expand_type 膨胀核类型
     * @expand_size 膨胀核大小
     */
    Mat dilateImage(Mat inputImage, int expand_type = MORPH_RECT, int expand_size = 1) {
        Mat tempImage;
        Mat element =
            getStructuringElement(expand_type, Size(expand_size, expand_size), Point(-1, -1));
        dilate(inputImage, tempImage, element);

        return tempImage;
    }

    //切下左右两端图片，保存中间的值,0为正常，1为错误
    int cutVertical(Mat &srcImage, Mat &leftImage, Mat &rightImage) {
        int left = 0, right = srcImage.cols, b;
        qDebug() << "image width is: " << right;
        for (b = 0; b < srcImage.cols; b++) {
            int colValue = getColSum(srcImage, b);
            // qDebug()<<"Col: "<<b<<" value is: "<< colValue << endl;
            if (colValue > 0) {
                left = b;
                break;
            }
        }
        qDebug() << "left is:" << left;
        for (; b < srcImage.cols; b++) {
            int colValue = getColSum(srcImage, b);
            // qDebug()<<"right col is: "<< b << " value is: " << colValue << endl;
            if (colValue == 0) {
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
    void cutHorizon(Mat &srcImage, Mat &dstImage) {
        int top = 0, bottom = srcImage.rows, b, rowValue;
        qDebug() << "image height is :" << bottom;
        //获取顶部
        for (b = 0; b < srcImage.rows; b++) {
            rowValue = getRowSum(srcImage, b);
            // qDebug()<<"top row is "<<b<<" value is: "<< rowValue << endl;
            if (rowValue > 0) {
                top = b;
                break;
            }
        }

        //获取底部
        for (; b < srcImage.rows; b++) {
            rowValue = getRowSum(srcImage, b);
            // qDebug()<<"bottom row is "<< b << " value is: " << rowValue << endl;
            if (rowValue == 0) {
                bottom = b;
                break;
            }
        }
        //计算高度
        int height = bottom - top;
        //左端 顶端 宽度 高度
        Rect rect(0, top, srcImage.cols, height);
        // dstImage 切割结果图片
        dstImage = srcImage(rect).clone();
        //调用函数匹配
    }

    //计算特征值
    void getFeature(Mat m, float a[]) {
        int M, N; //存储图像m的宽高
        int i, j;
        M = m.cols;
        N = m.rows;
        for (i = 0; i < 25; i++)
            a[ i ] = 0;
        //转换为灰度图像
        cv::cvtColor(m, m, COLOR_RGB2GRAY);
        //转换为二值图像
        cv::threshold(m, m, 100, 255, THRESH_BINARY);
        //将图像划分为5-5个子块，计算每块像素值的和
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                if (m.at<uchar>(i, j) == 255) {
                    a[ i / (M / 5) * 5 + j / (N / 5) ]++;
                }
            }
        }
        //计算当前像素块的平均值
        for (i = 0; i < 25; i++)
            a[ i ] = a[ i ] / ((M / 5) * (N / 5));
    }

    //计算欧氏距离
    float ouDistance(float a[], float b[]) {
        float distance = 0;
        //不要忘记初始化，否则会出错
        //根据欧氏距离计算公式,计算距离的平方
        for (int i = 0; i < 25; i++)
            distance += (a[ i ] - b[ i ]) * (a[ i ] - b[ i ]);
        //对上述结果开根号
        distance = sqrt(distance);
        return distance;
    }

    //显示RGB空间值
    void showRGBSpace(Mat inputImage, int n, QLabel *label) {
        Mat bgr_res[ 3 ];
        Mat bgr[ 3 ];
        for (int j = 0; j < 3; j++) {
            split(inputImage, bgr);
            for (int i = 0; i < 3; ++i) {
                if (i != j)
                    bgr[ i ] = Mat::zeros(inputImage.size(), bgr[ 0 ].type());
            }
            merge(bgr, 3, bgr_res[ j ]);
        }
        QImage img =
            QImage((const unsigned char *)(bgr_res[ n ].data), bgr_res[ n ].cols, bgr_res[ n ].rows,
                   bgr_res[ n ].cols * bgr_res[ n ].channels(), QImage::Format_RGB888);
        label->clear();
        img = img.scaled(label->size());
        label->setPixmap(QPixmap::fromImage(img));
    }

    //积分二值化
    void thresholdIntegral(Mat inputMat, Mat &outputMat) {
        int nRows = inputMat.rows;
        int nCols = inputMat.cols;

        // create the integral image
        Mat sumMat;
        integral(inputMat, sumMat);

        int    S = MAX(nRows, nCols) / 8;
        double T = 0.15;

        // perform thresholding
        int s2 = S / 2;
        int x1, y1, x2, y2, count, sum;

        int *  p_y1, *p_y2;
        uchar *p_inputMat, *p_outputMat;

        for (int i = 0; i < nRows; ++i) {
            y1 = i - s2;
            y2 = i + s2;

            if (y1 < 0) {
                y1 = 0;
            }
            if (y2 >= nRows) {
                y2 = nRows - 1;
            }

            p_y1        = sumMat.ptr<int>(y1);
            p_y2        = sumMat.ptr<int>(y2);
            p_inputMat  = inputMat.ptr<uchar>(i);
            p_outputMat = outputMat.ptr<uchar>(i);

            for (int j = 0; j < nCols; ++j) {
                // set the SxS region
                x1 = j - s2;
                x2 = j + s2;

                if (x1 < 0) {
                    x1 = 0;
                }
                if (x2 >= nCols) {
                    x2 = nCols - 1;
                }

                count = (x2 - x1) * (y2 - y1);

                // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
                sum = p_y2[ x2 ] - p_y1[ x2 ] - p_y2[ x1 ] + p_y1[ x1 ];

                if ((int)(p_inputMat[ j ] * count) < (int)(sum * (1.0 - T))) {
                    p_outputMat[ j ] = 0;
                } else {
                    p_outputMat[ j ] = 255;
                }
            }
        }
    }

    //垂直方向投影
    void picshadowx(Mat binary) {
        Mat paintx(binary.size(), CV_8UC1, Scalar(255)); //创建一个全白图片，用作显示

        int *pointcount = new int[ binary.cols ];
        memset(pointcount, 0, binary.cols * 4);

        for (int i = 0; i < binary.rows; i++) {
            for (int j = 0; j < binary.cols; j++) {
                if (binary.at<uchar>(i, j) == 0) {
                    pointcount[ j ]++; //垂直投影按列在x轴进行投影
                }
            }
        }
        for (int i = 0; i < binary.cols; i++) {
            for (int j = 0; j < pointcount[ i ]; j++) {
                paintx.at<uchar>(binary.rows - 1 - j, i) = 0; //翻转到下面，便于观看
            }
        }

        imshow("paintx", paintx);

        vector<Mat> result;
        int         startindex = 0;
        int         endindex   = 0;
        bool        inblock    = false; //是否遍历到字符位置

        for (int i = 0; i < paintx.cols; i++) {
            if (!inblock && pointcount[ i ] != 0) //进入有字符区域
            {
                inblock    = true;
                startindex = i;
                qDebug() << "startindex:" << startindex << endl;
            }
            if (inblock && pointcount[ i ] == 0) //进入空白区
            {
                endindex = i;
                inblock  = false;
                Mat roi  = binary.colRange(
                    startindex, endindex + 1); //从而记录从开始到结束行的位置，即可进行行切分
                result.push_back(roi);
            }
        }

        for (int i = 0; i < result.size(); i++) {
            Mat tmp = result[ i ];
            imwrite("test" + to_string(count)+".png", tmp);
            count++;
        }
        delete[] pointcount;
    }

    //水平方向投影并行分割
    Mat picshadowy(Mat binary) {
        //是否为白色或者黑色根据二值图像的处理得来
        Mat painty(binary.size(), CV_8UC1, Scalar(255)); //初始化为全白

        //水平投影
        int *pointcount = new int[ binary.rows ]; //在二值图片中记录行中特征点的个数
        memset(pointcount, 0, binary.rows * 4);   //注意这里需要进行初始化

        for (int i = 0; i < binary.rows; i++) {
            for (int j = 0; j < binary.cols; j++) {
                if (binary.at<uchar>(i, j) == 0) {
                    pointcount[ i ]++; //记录每行中黑色点的个数 //水平投影按行在y轴上的投影
                }
            }
        }

        for (int i = 0; i < binary.rows; i++) {
            for (int j = 0; j < pointcount[ i ]; j++) //根据每行中黑色点的个数，进行循环
            {
                painty.at<uchar>(i, j) = 0;
            }
        }

        imshow("painty", painty);

        Mat result;
        int         startindex = 0;
        int         endindex   = 0;
        bool        inblock    = false; //是否遍历到字符位置

        for (int i = 0; i < painty.rows; i++) {
            if (!inblock && pointcount[ i ] != 0) //进入有字符区域
            {
                inblock    = true;
                startindex = i;
                cout << "startindex:" << startindex << endl;
            }
            if (inblock && pointcount[ i ] == 0) //进入空白区
            {
                endindex = i;
                inblock  = false;
                Mat roi  = binary.rowRange(
                    startindex, endindex+1); //从而记录从开始到结束行的位置，即可进行行切分
                result = roi;
            }
        }

        delete[] pointcount;

        return result;
    }
};

#endif // PROCESSING_HPP
