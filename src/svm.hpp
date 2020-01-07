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
#ifndef SVM_HPP
#define SVM_HPP

#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::ml;

class SVMTT{
public:
    SVMTT(){}
    ~SVMTT(){}

    int reverseDigit(int num){
        unsigned char c1,c2,c3,c4;
        c1=num&255;
        c2=(num>>8)&255;
        c3=(num>>16)&255;
        c4=(num>>24)&255;

        return ((int)c1<<24)+((int)c2<<16)+((int)c3<<8)+c4;
    }
    //读取图像数据集
    Mat readImages(int opt){
        ifstream f;
        Mat img;
        qDebug()<<"读取训练图像数据。。。";
        if(opt==0){
            f.open("train-images.idx3-ubyte",ios::binary);
        }
        else{
            f.open("t10k-images.idx3-ubyte",ios::binary);
        }

        if(!f.is_open()){
            qDebug()<<"无法读取图像数据";
            exit(1);
        }
        /*
         byte 0 - 3 : Magic Number(Not to be used)
         byte 4 - 7 : Total number of images in the dataset
         byte 8 - 11 : rows of each image in the dataset
         byte 12 - 15 : cols of each image in the dataset
        */
        int magic_number = 0;
        int number_of_images = 0;
        int height=0;
        int width = 0;

        f.read((char*)&magic_number,sizeof(magic_number));
        magic_number = reverseDigit(magic_number);

        f.read((char*)&number_of_images,sizeof (number_of_images));
        number_of_images = reverseDigit(number_of_images);
        qDebug()<<"图像数量是："<<number_of_images;

        f.read((char*)&height,sizeof (height));
        height = reverseDigit(height);

        f.read((char*)&width,sizeof (width));
        width = reverseDigit(width);

        Mat train_images = Mat(number_of_images,height*width,CV_8UC1);
        Mat digitImg = Mat::zeros(height,width,CV_8UC1);
        for(int i=0;i<number_of_images;i++){
            int index=0;
            for(int r=0;r<height;++r){
                for(int c=0;c<width;++c){
                    unsigned char temp =0;
                    f.read((char*)&temp,sizeof(temp));
                    index = r*width+c;
                    train_images.at<uchar>(i,index)=(int)temp;
                    digitImg.at<uchar>(r,c)=(int)temp;
                }
            }
            //       if (i < 100) {
            //           qDebug()<<"Reading image: "<<i<<" done";
            //            imwrite(format("/home/jackey/GitHub/OpenCV-MNIST/data/images/digit_%d.png", i), digitImg);
            //       }
        }
        train_images.convertTo(train_images,CV_32F);
        f.close();
        return train_images;
    }
//读取标记数据集
    Mat readLabels(int opt){
        ifstream f;
        Mat img;
        qDebug()<<"读取训练标签数据中。。。";
        if(opt == 0){
            f.open("train-labels.idx1-ubyte");
        }
        else{
            f.open("t10k-labels.idx1-ubyte");
        }
        if(!f.is_open()){
            qDebug()<<"无法读取标签数据";
            exit(0);
        }
        /*
         byte 0 - 3 : Magic Number(Not to be used)
         byte 4 - 7 : Total number of labels in the dataset
        */
        int magic_number = 0;
        int number_of_labels = 0;
        f.read((char*)&magic_number,sizeof(magic_number));
        magic_number = reverseDigit(magic_number);

        f.read((char*)&number_of_labels,sizeof(number_of_labels));
        number_of_labels = reverseDigit(number_of_labels);
        qDebug()<<"标签数量为："<<number_of_labels;

        Mat labels = Mat(number_of_labels,1,CV_8UC1);
        for(long int i=0;i<number_of_labels;i++){
            unsigned char temp =0;
            f.read((char*)&temp,sizeof (temp));
            labels.at<uchar>(i,0)=temp;
        }
        labels.convertTo(labels,CV_32S);
        f.close();
        return labels;
    }

    bool svmTrain(){
        Mat train_images = readImages(0);
        if(train_images.size==0) return false;

        Mat train_labels = readLabels(0);
        if(train_labels.size ==0) return false;
        qDebug()<<"成功读取图像和标签";

        Ptr<ml::SVM> svm = ml::SVM::create();
        svm->setType(ml::SVM::C_SVC);
        svm->setKernel(ml::SVM::LINEAR);
        svm->setDegree(5);
        svm->setGamma(0.01);
        svm->setCoef0(1.0);
        svm->setC(10.0);
        svm->setNu(0.5);
        svm->setP(0.1);
        svm->setTermCriteria(TermCriteria(TermCriteria::EPS,1000,FLT_EPSILON));

        qDebug()<<"SVM训练开始。。。";
        Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images,ml::ROW_SAMPLE,train_labels);
        svm->train(tdata);
        svm->save("svm.xml");
        qDebug()<<"SVM训练数据已保存";
        return true;
    }

    float testSVM(){
        QFile file("svm.xml");
        if(!file.exists()){
            qDebug()<<"没有训练结果文件";
            return 0.0;
        }

        qDebug()<<"开始导入SVM训练结果文件";
        Ptr<ml::SVM> svm = StatModel::load<ml::SVM>("svm.xml");
        qDebug()<<"已成功导入SVM训练结果文件";

        qDebug()<<"开始导入测试数据";
        Mat tData = readImages(1);
        Mat tLabel =  readLabels(1);
        qDebug()<<"已成功导入测试数据";

        float count =0;
        for(int i=0;i<tData.rows;i++){
            Mat sample = tData.row(i);
            float res = svm->predict(sample);
            res = std::abs(res-tLabel.at<unsigned int>(i,0))<=FLT_EPSILON?1.0f:0.0f;
            count += res;
        }
        qDebug()<<"准确识别的个数是："<<count;
        float rate = (count + 0.0)/10000*100.0;
        qDebug()<<"准确率为："<<rate<<"%...";
        return rate;
    }
};

#endif
