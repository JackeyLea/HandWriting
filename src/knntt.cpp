#include "knntt.h"

KNNTT::KNNTT()
{
    _run_mode = STOP;
}

void KNNTT::setRunMode(RUNMODE mode)
{
    _run_mode = mode;
}

float KNNTT::getRate()
{
    return _rate;
}

int KNNTT::getPredictResult()
{
    return _predictResult;
}

void KNNTT::reset()
{
    _run_mode = STOP;
    _rate = 0;
}

Mat KNNTT::readImagesData()
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

Mat KNNTT::readLabelsData()
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
    qDebug()<<"->数据集标签数据读取完毕。";

    return labels;
}

int KNNTT::predict(Mat img)
{
    qDebug()<<"knn预测开始";
    QFile file("knn.xml");
    if(!file.exists()){
        qDebug()<<"->KNN训练结果文件knn.xml不存在。";
        return -1;
    }

    qDebug()<<"->开始加载knn模型";
    Ptr<ml::KNearest> knn=Algorithm::load<ml::KNearest>("knn.xml");
    qDebug()<<"->knn模型加载完毕";

    cv::Mat result;//没有用，用于占位置
    float predicted = knn->predict(img,result);
    _predictResult = static_cast<int>(predicted);
    qDebug()<<"knn预测结束";

    return 0;
}

void KNNTT::run()
{
    if(_run_mode==TRAIN){
        qDebug()<<"KNN方式训练数据开始";
        Mat train_images = readImagesData();
        if(train_images.size==0) return;

        Mat train_labels = readLabelsData();
        if(train_labels.size ==0) return;
        qDebug()<<"->成功读取图像和标签";

        qDebug() << "->KNN训练开始";
        time_start = (double)clock();
        Ptr<ml::KNearest>  knn = ml::KNearest::create();
        Ptr<ml::TrainData> tdata =
            ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
        knn->train(tdata);
        knn->setDefaultK(5);
        knn->setIsClassifier(true);
        knn->save("knn.xml");
        time_end = (double)clock();
        qDebug() << "->KNN训练数据已成功保存";
        qDebug()<<"->KNN训练耗时："<<(time_end-time_start)/1000.0<<"ms";
        qDebug()<<"KNN训练结束。";
    }else if(_run_mode==TEST){
        qDebug()<<"KNN测试开始";

        QFile file("knn.xml");
        if(!file.exists()){
            qDebug()<<"->没有训练结果文件";
            return;
        }

        qDebug() << "->开始导入KNN训练结果文件";
        Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("knn.xml");
        qDebug() << "->已成功导入KNN训练结果文件";

        qDebug()<<"->开始导入测试数据";
        Mat tData = readImagesData();
        if(tData.size==0) return;
        Mat tLabel = readLabelsData();
        if(tLabel.size==0) return;
        qDebug()<<"->已成功导入测试数据";

        float total   = tData.rows;
        float correct = 0;

        qDebug()<<"->KNN测试开始";
        time_start = (double)clock();
        Rect rect;
        rect.x      = 0;
        rect.height = 1;
        rect.width  = (28 * 28);
        for (int i = 0; i < total; i++) {
            int actual     = tLabel.at<int>(i);
            rect.y         = i;
            Mat   oneImage = tData(rect);
            Mat   result;
            float predicted = knn->predict(oneImage, result);
            int   digit     = static_cast<int>(predicted);
            if (digit == actual) {
                correct++;
            }
        }
        time_end = (double)clock();
        _rate = correct / total * 100.0;
        qDebug()<<"->识别准确率是：" << _rate;
        qDebug()<<"->KNN训练耗时："<<(time_end-time_start)/1000.0<<"ms";
        qDebug()<<"KNN测试结束。";
    }else if(_run_mode==STOP){
        qDebug()<<"Current run mode is STOP, please change the mode first.";
    }else{
        qDebug()<<"Something was wrong!!!";
    }
}
