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

int SVMTT::getPredictResult()
{
    return _predictResult;
}

void SVMTT::reset()
{
    _run_mode = STOP;
    _rate = 0;
}
//读取图像数据集
Mat SVMTT::readImagesData()
{
    switch(_run_mode){
    case TRAIN:
        f.open("train-images.idx3-ubyte",ios::binary);
        qDebug()<<"->读取训练用的图像数据。";
        break;
    case TEST:
        f.open("t10k-images.idx3-ubyte",ios::binary);
        qDebug()<<"->读取测试用的图像数据。";
        break;
    default:
        break;
    }

    if(!f.is_open()){
        qDebug()<<"->无法读取图像数据";
        exit(-1);
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
    qDebug()<<"->图像数量是："<<number_of_images;

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
//                if(i==0){
//                    Mat digit=Mat(height,width,CV_8UC1);
//                    digit.at<uchar>(r,c)=(int)temp;
//                    imwrite("digit.png",digit);//输出第一张图片
//                }
            }
        }
    }
    train_images.convertTo(train_images,CV_32F);
    f.close();
    qDebug()<<"->数据集图像数据读取完毕。";

    return train_images;
}
//读取标记数据集
Mat SVMTT::readLabelsData()
{
    switch(_run_mode){
    case TRAIN:
        f.open("train-labels.idx1-ubyte");
        qDebug()<<"->读取训练标签。";
        break;
    case TEST:
        f.open("t10k-labels.idx1-ubyte");
        qDebug()<<"->读取测试标签。";
        break;
    default:
        break;
    }

    if(!f.is_open()){
        qDebug()<<"->无法读取标签数据";
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
    qDebug()<<"->标签数量为："<<number_of_labels;

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

int SVMTT::predict(Mat &img)
{
    qDebug()<<"svm预测开始";

    QFile file("svm.xml");
    if(!file.exists()){
        qDebug()<<"->SVM训练结果文件svm.xml不存在。";
        return -1;
    }

    qDebug()<<"->开始导入svm模型";
    Ptr<ml::SVM> svm =Algorithm::load<ml::SVM>("svm.xml");
    qDebug()<<"->svm模型导入成功";
    float res = svm->predict(img);
    _predictResult = static_cast<int>(res);
    qDebug()<<"svm预测结束。\n";
    return 0;
}

void SVMTT::run()
{
    if(_run_mode==TRAIN){
        qDebug()<<"SVM训练开始";
        Mat train_images = readImagesData();
        if(train_images.size==0) return;

        Mat train_labels = readLabelsData();
        if(train_labels.size ==0) return;
        qDebug()<<"->成功读取数据集中的图像和标签。";

        Ptr<ml::SVM> svm = ml::SVM::create();
        //设置的参数越多，训练时间越长
        svm->setType(ml::SVM::C_SVC);
        svm->setKernel(ml::SVM::LINEAR);
        svm->setDegree(5);
        svm->setGamma(0.01);
//        svm->setCoef0(1.0);
//        svm->setC(10.0);
//        svm->setNu(0.5);
//        svm->setP(0.1);
//        svm->setTermCriteria(TermCriteria(TermCriteria::EPS,1000,FLT_EPSILON));

        qDebug()<<"->SVM类型数据训练正式开始，请稍等。";
        time_start = (double)clock();
        Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images,ml::ROW_SAMPLE,train_labels);
        svm->train(tdata);
        svm->save("svm.xml");
        time_end = (double)clock();
        qDebug()<<"->SVM训练耗时："<<(time_end-time_start)/1000.0<<"ms。";
        qDebug()<<"->SVM训练数据已保存";
        qDebug()<<"SVM训练结束。\n";
    }else if(_run_mode==TEST){
        qDebug()<<"SVM方式识别测试开始。";

        QFile file("svm.xml");
        if(!file.exists()){
            qDebug()<<"->没有训练结果文件！！！";
            return;
        }

        qDebug()<<"->开始导入SVM训练结果文件";
        Ptr<ml::SVM> svm = StatModel::load<ml::SVM>("svm.xml");
        qDebug()<<"->已成功导入SVM训练结果文件";

        qDebug()<<"->开始导入测试数据";
        Mat tData = readImagesData();
        if(tData.size==0) return;
        Mat tLabel = readLabelsData();
        if(tLabel.size==0) return;
        qDebug()<<"->已成功导入测试数据";

        qDebug()<<"->测试正式开始。";
        time_start = (double)clock();
        float count =0;
        for(int i=0;i<tData.rows;i++){
            Mat sample = tData.row(i);
            float res = svm->predict(sample);
            res = std::abs(res-tLabel.at<unsigned int>(i,0))<=FLT_EPSILON?1.0f:0.0f;
            count += res;
        }
        _rate = (count + 0.0)/10000*100.0;
        time_end = (double)clock();
        qDebug()<<"->SVM测试耗时："<<(time_end-time_start)/1000.0<<"ms。";
        qDebug()<<"->SVM测试准确率为："<<_rate<<"%。";
        qDebug()<<"SVM训练结果测试结束。\n";
    }else if(_run_mode==STOP){
        qDebug()<<"Current run mode is STOP, please change the mode first.";
    }else{
        qDebug()<<"Something was wrong!!!";
    }
}
