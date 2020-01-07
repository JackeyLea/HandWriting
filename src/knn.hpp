#ifndef KNN_HPP
#define KNN_HPP


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

class KNNTT{
public:
    KNNTT(){}
    ~KNNTT(){}

    int reverseDigit(int num){
        unsigned char c1,c2,c3,c4;
        c1=num&255;
        c2=(num>>8)&255;
        c3=(num>>16)&255;
        c4=(num>>24)&255;

        return ((int)c1<<24)+((int)c2<<16)+((int)c3<<8)+c4;
    }

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
        }
        train_images.convertTo(train_images,CV_32F);
        f.close();
        return train_images;
    }

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
    /*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们的正确率
      KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
    */
    bool KNNTrain(){
        Mat train_images = readImages(0);
        if(train_images.size==0) return false;
        Mat train_labels = readLabels(0);
        if(train_labels.size==0) return false;
        qDebug()<<"已成功读取图片图标数据。。。";

        qDebug()<<"KNN训练开始。。。";
        Ptr<ml::KNearest> knn = ml::KNearest::create();
        Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images,ml::ROW_SAMPLE,train_labels);
        knn->train(tdata);
        knn->setDefaultK(5);
        knn->setIsClassifier(true);
        knn->save("knn.xml");
        qDebug()<<"KNN训练数据已成功保存";

        return true;
    }

    float testKNN(){
        QFile file("knn.xml");
        if(!file.exists()){
            qDebug()<<"没有训练结果文件";
            return 0.0;
        }

        qDebug()<<"开始导入KNN训练结果文件";
        Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("knn.xml");
        qDebug()<<"已成功导入KNN训练结果文件";

        qDebug()<<"开始导入测试数据";
        Mat tData = readImages(1);
        Mat tLabel =  readLabels(1);
        qDebug()<<"已成功导入测试数据";

        float total = tData.rows;
        float correct = 0;

        Rect rect;
        rect.x=0;
        rect.height=1;
        rect.width=(28*28);
        for(int i=0;i<total;i++){
            int actual = tLabel.at<int>(i);
            rect.y=i;
            Mat oneImage = tData(rect);
            Mat result;
            float predicted = knn->predict(oneImage,result);
            int digit = static_cast<int>(predicted);
            if(digit == actual){
                correct++;
            }
        }
        qDebug()<<"已成功识别："<<correct;
        float rate = correct/total *100.0;
        qDebug()<<"识别准确率是："<<rate;
        return rate;
    }
};

#endif // KNN_HPP
