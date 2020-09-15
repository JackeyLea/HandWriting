#include "svmtt.h"

SVMTT::SVMTT()
{
    _run_mode = STOP;
}

void SVMTT::setRunMode(RUNMODE mode)
{
    _run_mode = mode;
}

float SVMTT::getRate()
{
    return _rate;
}

void SVMTT::reset()
{
    _run_mode = STOP;
    _rate = 0;
}
//读取图像数据集
Mat SVMTT::readImagesData()
{
    ifstream f;
    qDebug()<<"读取训练图像数据。。。";

    switch(_run_mode){
    case TRAIN:
        f.open("train-images.idx3-ubyte",ios::binary);
        qDebug()<<"Reading train images";
        break;
    case TEST:
        f.open("t10k-images.idx3-ubyte",ios::binary);
        qDebug()<<"Reading testing images";
        break;
    default:
        break;
    }

    if(!f.is_open()){
        qDebug()<<"无法读取图像数据";
        return Mat();
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
    for(int i=0;i<number_of_images;i++){//第几张图
        for(int r=0;r<height;++r){
            for(int c=0;c<width;++c){
                unsigned char temp =0;
                f.read((char*)&temp,sizeof(temp));
                train_images.at<uchar>(i,r*width+c)=(int)temp;
                if(i==0){
                    Mat digit=Mat(height,width,CV_8UC1);
                    digit.at<uchar>(r,c)=(int)temp;
                    imwrite("digit.png",digit);//输出第一张图片
                }
            }
        }
    }
    train_images.convertTo(train_images,CV_32F);
    f.close();
    return train_images;
}
//读取标记数据集
Mat SVMTT::readLabelsData()
{
    ifstream f;
    qDebug()<<"读取训练标签数据中。。。";

    switch(_run_mode){
    case TRAIN:
        f.open("train-labels.idx1-ubyte");
        qDebug()<<"Reading train labels";
        break;
    case TEST:
        f.open("t10k-labels.idx1-ubyte");
        qDebug()<<"Reading testing labels";
        break;
    default:
        break;
    }

    if(!f.is_open()){
        qDebug()<<"无法读取标签数据";
        exit(-1);
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

void SVMTT::run()
{
    time_start = (double)clock();

    if(_run_mode==TRAIN){
        Mat train_images = readImagesData();
        if(train_images.size==0) return;

        Mat train_labels = readLabelsData();
        if(train_labels.size ==0) return;
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
        time_end = (double)clock();
        qDebug()<<"SVM训练数据已保存";
        qDebug()<<"SVM training spend "<<(time_end-time_start)/1000.0<<"ms";
    }else if(_run_mode==TEST){
        QFile file("svm.xml");
        if(!file.exists()){
            qDebug()<<"没有训练结果文件";
            return;
        }

        qDebug()<<"开始导入SVM训练结果文件";
        Ptr<ml::SVM> svm = StatModel::load<ml::SVM>("svm.xml");
        qDebug()<<"已成功导入SVM训练结果文件";

        qDebug()<<"开始导入测试数据";
        Mat tData = readImagesData();
        Mat tLabel = readLabelsData();
        qDebug()<<"已成功导入测试数据";

        float count =0;
        for(int i=0;i<tData.rows;i++){
            Mat sample = tData.row(i);
            float res = svm->predict(sample);
            res = std::abs(res-tLabel.at<unsigned int>(i,0))<=FLT_EPSILON?1.0f:0.0f;
            count += res;
        }
        qDebug()<<"准确识别的个数是："<<count;
        _rate = (count + 0.0)/10000*100.0;
        qDebug()<<"准确率为："<<_rate<<"%...";
    }else if(_run_mode==STOP){
        qDebug()<<"Current run mode is STOP, please change the mode first.";
    }else{
        qDebug()<<"Something was wrong!!!";
    }
}
