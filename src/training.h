#ifndef TRAINING_H
#define TRAINING_H

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

#include <QObject>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QDebug>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::ml;

class Training
{
public:
    Training();
    Mat readImages(int opt);
    Mat readLabels(int opt);

    void knnTrain();
    void svmTrain();

    void testMnistKNN();
    void testMnistSVM();
    int reverseDigit(int num);
};

#endif // TRAINING_H
