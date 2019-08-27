/*
svm_type –
指定SVM的类型，下面是可能的取值：
CvSVM::C_SVC C类支持向量分类机。 n类分组  (n \geq 2)，允许用异常值惩罚因子C进行不完全分类。
CvSVM::NU_SVC \nu类支持向量分类机。n类似然不完全分类的分类器。参数为 \nu 取代C（其值在区间【0，1】中，nu越大，决策边界越平滑）。
CvSVM::ONE_CLASS 单分类器，所有的训练数据提取自同一个类里，然后SVM建立了一个分界线以分割该类在特征空间中所占区域和其它类在特征空间中所占区域。
CvSVM::EPS_SVR \epsilon类支持向量回归机。训练集中的特征向量和拟合出来的超平面的距离需要小于p。异常值惩罚因子C被采用。
CvSVM::NU_SVR \nu类支持向量回归机。 \nu 代替了 p。

可从 [LibSVM] 获取更多细节。

kernel_type –
SVM的内核类型，下面是可能的取值：
CvSVM::LINEAR 线性内核。没有任何向映射至高维空间，线性区分（或回归）在原始特征空间中被完成，这是最快的选择。K(x_i, x_j) = x_i^T x_j.
CvSVM::POLY 多项式内核： K(x_i, x_j) = (\gamma x_i^T x_j + coef0)^{degree}, \gamma > 0.
CvSVM::RBF 基于径向的函数，对于大多数情况都是一个较好的选择： K(x_i, x_j) = e^{-\gamma ||x_i - x_j||^2}, \gamma > 0.
CvSVM::SIGMOID Sigmoid函数内核：K(x_i, x_j) = \tanh(\gamma x_i^T x_j + coef0).

degree – 内核函数（POLY）的参数degree。

gamma – 内核函数（POLY/ RBF/ SIGMOID）的参数\gamma。

coef0 – 内核函数（POLY/ SIGMOID）的参数coef0。

Cvalue – SVM类型（C_SVC/ EPS_SVR/ NU_SVR）的参数C。

nu – SVM类型（NU_SVC/ ONE_CLASS/ NU_SVR）的参数 \nu。

p – SVM类型（EPS_SVR）的参数 \epsilon。

class_weights – C_SVC中的可选权重，赋给指定的类，乘以C以后变成 class\_weights_i * C。所以这些权重影响不同类别的错误分类惩罚项。权重越大，某一类别的误分类数据的惩罚项就越大。

term_crit – SVM的迭代训练过程的中止条件，解决部分受约束二次最优问题。您可以指定的公差和/或最大迭代次数。

*/
#ifndef TRAINING_HPP
#define TRAINING_HPP

#include <QObject>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QObject>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::ml;

static string trainImagesWin = "C:\\Users\\xuhua\\GitHub\\OpenCV-MNIST\\data\\train-images.idx3-ubyte";
static string trainImageUnix = "/home/xuhua/GitHub/OpenCV-MNIST/data/train-images.idx3-ubyte";
static string trainLabelsWin = "C:\\Users\\xuhua\\GitHub\\OpenCV-MNIST\\data\\train-labels.idx3-ubyte";
static string trainLabelsUnix = "/home/jackey/GitHub/OpenCV-MNIST/data/train-labels.idx1-ubyte";
static string testImagesWin = "C:\\Users\\xuhua\\GitHub\\OpenCV-MNIST\\data\\t10k-images.idx3-ubyte";
static string testImagesUnix = "/home/xuhua/GitHub/OpenCV-MNIST/data/t10k-images.idx3-ubyte";
static string testLabelsWin = "C:\\Users\\xuhua\\GitHub\\OpenCV-MNIST\\data\\t10k-labels.idx3-ubyte";
static string testLabelsUnix = "/home/jackey/GitHub/OpenCV-MNIST/data/t10k-labels.idx1-ubyte";
static string knnSaveWin = "C:\\Users\\xuhua\\GitHub\\OpenCV-MNIST\\data\\knn.yml";
static string knnSaveUnix = "/home/jackey/GitHub/OpenCV-MNIST/data/knn.yml";
static string svmSaveWin = "C:\\Users\\xuhua\\GitHub\\OpenCV-MNIST\\data\\svm.xml";
static string svmSaveUnix = "/home/jackey/GitHub/OpenCV-MNIST/data/svm.xml";

class Training
{
public:
    Training(){}
    //读取图像数据集
    Mat readImages(int opt)
    {
        ifstream f;
        Mat img;
        if(opt == 0){
            qDebug() << "Reading training data ...";
#ifdef Q_OS_WIN
        f.open("train-images.idx3-ubyte",ios::binary);
#elif define(Q_OS_UNIX)
        f.open(trainImagesUnix,ios::binary);
#endif
        }
        else{
            qDebug() << "Reading test data ...";
#ifdef Q_OS_WIN
        f.open("t10k-images.idx3-ubyte",ios::binary);
#elif define(Q_OS_UNIX)
        f.open(testImagesUnix,ios::binary);
#endif
        }
        // check file
        if (!f.is_open()){
            qDebug() << "Images File Not Found!";
            return img;
        }
        /*
         byte 0 - 3 : Magic Number(Not to be used)
         byte 4 - 7 : Total number of images in the dataset
         byte 8 - 11 : rows of each image in the dataset
         byte 12 - 15 : cols of each image in the dataset
        */
        int magic_number = 0;
        int number_of_images = 0;
        int height = 0;
        int width = 0;

        f.read((char*)&magic_number, sizeof(magic_number));

        magic_number = reverseDigit(magic_number);
        f.read((char*)&number_of_images, sizeof(number_of_images));

        number_of_images = reverseDigit(number_of_images);
        qDebug()<<"Number of images is: "<<number_of_images;

        f.read((char*)&height, sizeof(height));
        height = reverseDigit(height);

        f.read((char*)&width, sizeof(width));
        width = reverseDigit(width);

        Mat train_images = Mat(number_of_images, height*width, CV_8UC1);

        Mat digitImg = Mat::zeros(height, width, CV_8UC1);

        for (int i = 0; i < number_of_images; i++) {
            int index = 0;
            for (int r = 0; r<height; ++r) {
               for (int c = 0; c<width; ++c) {
                   unsigned char temp = 0;
                   f.read((char*)&temp, sizeof(temp));
                   index = r*width + c;
                   train_images.at<uchar>(i, index) = (int)temp;
                   digitImg.at<uchar>(r, c) = (int)temp;
               }
           }
        //       if (i < 100) {
        //           qDebug()<<"Reading image: "<<i<<" done";
        //            imwrite(format("/home/jackey/GitHub/OpenCV-MNIST/data/images/digit_%d.png", i), digitImg);
        //       }
        }
        train_images.convertTo(train_images, CV_32FC1);

        f.close();
        return train_images;
    }
    //读取标记数据集
    Mat readLabels(int opt)
    {
        ifstream f;
        Mat img;
        if(opt == 0){
            qDebug() << "Reading training data ...";
#ifdef Q_OS_WIN
        f.open("train-labels.idx1-ubyte");
#elif define(Q_OS_UNIX)
        f.open(trainLabelsUnix);
#endif
        }
        else{
            qDebug() << "Reading test data ...";
#ifdef Q_OS_WIN
        f.open("t10k-labels.idx1-ubyte");
#elif define(Q_OS_UNIX)
        f.open(testLabelsUnix);
#endif
        }
         // check file
        if (!f.is_open()){
            qDebug() << "Labels File Not Found!";
            return img;
        }
        /*
         byte 0 - 3 : Magic Number(Not to be used)
         byte 4 - 7 : Total number of labels in the dataset
        */
        int magic_number = 0;
        int number_of_labels = 0;
        f.read((char*)&magic_number, sizeof(magic_number));
        magic_number = reverseDigit(magic_number);

        f.read((char*)&number_of_labels, sizeof(number_of_labels));
        number_of_labels = reverseDigit(number_of_labels);
        qDebug() << "No. of labels:" << number_of_labels ;

        Mat labels = Mat(number_of_labels, 1, CV_8UC1);
        for (long int i = 0; i<number_of_labels; ++i){
            unsigned char temp = 0;
            f.read((char*)&temp, sizeof(temp));
            //printf("temp : %d\n ", temp);
            labels.at<uchar>(i, 0) = temp;
        }
        labels.convertTo(labels, CV_32SC1);

        f.close();
        return labels;
    }
    /*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们的正确率
      KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
    */
    void knnTrain()
    {
        qDebug()<<"KNN train starting";
        Mat train_images = readImages(0);
        if(train_images.size==0) return;
        Mat train_labels = readLabels(0);
        if(train_labels.size ==0) return;
        qDebug()<<"read mnist train dataset successfully...";

        Ptr<ml::KNearest> knn = ml::KNearest::create();
        knn->setDefaultK(5);
        knn->setIsClassifier(true);
        Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
        knn->train(tdata);
#ifdef Q_OS_WIN
        knn->save("knn.xml");
#elif define(Q_OS_UNIX)
        knn->save("knn.xml");
#endif
        qDebug()<<"KNN train data has been saved successfully";
    }

    void svmTrain()
    {
        qDebug()<<"SVM training starting";
        Mat train_images = readImages(0);
        if(train_images.size ==0) return;
        Mat train_labels = readLabels(0);
        if(train_labels.size ==0) return;
        qDebug()<<"Read mnist test dataset successfully...";

        Ptr<ml::SVM> svm = ml::SVM::create();
        svm->setDegree(5);
        Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images,ml::ROW_SAMPLE,train_labels);
        svm->train(tdata);
#ifdef Q_OS_WIN
        svm->save("svm.xml");
#elif define(Q_OS_UNIX)
        svm->save(svmSaveUnix);
#endif
        qDebug()<<"SVM train file has been saved";
    }

    float testMnistKNN()
    {
        QFile file(QString::fromStdString("knn.xml"));
        if(!file.exists()){
            qDebug()<<"No knn result file\nApplication exit";
            return 0;
        }
        qDebug()<<"开始KNN测试";
#ifdef Q_OS_WIN
        Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("knn.xml"); // KNN - 97%
#elif define(Q_OS_UNIX)
        Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>(knnSaveUnix);
#endif

        Mat train_images = readImages(1);
        Mat train_labels = readLabels(1);
        qDebug()<<"Read mnist test dataset successfully...";

        float total = train_images.rows;

        float correct = 0;
        Rect rect;
        rect.x = 0;
        rect.height = 1;
        rect.width = (28 * 28);
        for(int i = 0; i < total; i++){
            int actual = train_labels.at<int>(i);
            rect.y = i;
            Mat oneImage = train_images(rect);
            Mat result;
            float predicted = knn->predict(oneImage, result);
            int digit = static_cast<int>(predicted);
            if(digit == actual){
                correct++;
            }
        }

        float rate = correct /total *100.0;
        qDebug()<<"Recognize rate is: "<<rate;

        return rate;
    }

    float testMnistSVM()
    {
        QFile file(QString::fromStdString("svm.xml"));
        if(!file.exists()){
            qDebug()<<"No svm result file\nApplication exit";
            return 0.0;
        }
        qDebug() << "开始导入SVM文件...";
#ifdef Q_OS_WIN
        Ptr<SVM> svm1 = StatModel::load<SVM>("svm.xml");
#elif define(Q_OS_UNIX)
        Ptr<SVM> svm1 = StatModel::load<SVM>(svmSaveUnix);
#endif

        qDebug()<< "成功导入SVM文件...";

        qDebug()<< "开始导入测试数据...\n";
        Mat testData = readImages(1);
        Mat tLabel = readLabels(1);
        qDebug()<< "成功导入测试数据！！！";

        float count = 0;
        for (int i = 0; i < testData.rows; i++) {
            Mat sample = testData.row(i);
            float res = svm1->predict(sample);
            res = std::abs(res - tLabel.at<unsigned int>(i, 0)) <= FLT_EPSILON ? 1.f : 0.f;
            count += res;
        }
        qDebug() << "正确的识别个数 count = " << count << endl;
        float rate = (10000-count +0.0)/10000*100.0;
        qDebug() << "错误率为..." << rate<< "%....";

        return rate;
    }

    int reverseDigit(int num)
    {
        unsigned char c1,c2,c3,c4;
        c1=num&255;
        c2=(num>>8)&255;
        c3=(num>>16)&255;
        c4=(num>>24)&255;

        return ((int)c1<<24)+((int)c2<<16)+((int)c3<<8)+c4;
    }
};

#endif // TRAINING_HPP
