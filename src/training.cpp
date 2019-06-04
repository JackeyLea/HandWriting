#include "training.h"

Training::Training()
{
}
//读取图像数据集
Mat Training::readImages(int opt)
{
    ifstream file;
    Mat img;
    if(opt == 0){
        qDebug() << "Reading training data ...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/train-images.idx3-ubyte", ios::binary);
    }
    else{
        qDebug() << "Reading test data ...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/t10k-images.idx3-ubyte", ios::binary);
    }
    // check file
    if (!file.is_open()){
        qDebug() << "File Not Found!";
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

    file.read((char*)&magic_number, sizeof(magic_number));

    magic_number = reverseDigit(magic_number);
    file.read((char*)&number_of_images, sizeof(number_of_images));

    number_of_images = reverseDigit(number_of_images);
    qDebug()<<"Number of images is: "<<number_of_images;

    file.read((char*)&height, sizeof(height));
    height = reverseDigit(height);

    file.read((char*)&width, sizeof(width));
    width = reverseDigit(width);

    Mat train_images = Mat(number_of_images, height*width, CV_8UC1);

    Mat digitImg = Mat::zeros(height, width, CV_8UC1);

    for (int i = 0; i < number_of_images; i++) {
        int index = 0;
        for (int r = 0; r<height; ++r) {
           for (int c = 0; c<width; ++c) {
               unsigned char temp = 0;
               file.read((char*)&temp, sizeof(temp));
               index = r*width + c;
               train_images.at<uchar>(i, index) = (int)temp;
               digitImg.at<uchar>(r, c) = (int)temp;
           }
       }
       if (i < 100) {
           qDebug()<<"Reading image: "<<i<<" done";
            imwrite(format("/home/jackey/GitHub/OpenCV-MNIST/data/images/digit_%d.png", i), digitImg);
       }
    }
    train_images.convertTo(train_images, CV_32FC1);

    file.close();
    return train_images;
}
//读取标记数据集
Mat Training::readLabels(int opt)
{
    ifstream file;
    Mat img;
    if(opt == 0){
        qDebug() << "Reading training data ...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/train-labels.idx1-ubyte");
    }
    else{
        qDebug() << "Reading test data ...";
        file.open("/home/jackey/GitHub/OpenCV-MNIST/data/t10k-labels.idx1-ubyte");
    }
     // check file
    if (!file.is_open()){
        qDebug() << "File Not Found!";
        return img;
    }
    /*
     byte 0 - 3 : Magic Number(Not to be used)
     byte 4 - 7 : Total number of labels in the dataset
    */
    int magic_number = 0;
    int number_of_labels = 0;
    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);

    file.read((char*)&number_of_labels, sizeof(number_of_labels));
    number_of_labels = reverseDigit(number_of_labels);
    qDebug() << "No. of labels:" << number_of_labels ;

    Mat labels = Mat(number_of_labels, 1, CV_8UC1);
    for (long int i = 0; i<number_of_labels; ++i){
        unsigned char temp = 0;
        file.read((char*)&temp, sizeof(temp));
        //printf("temp : %d\n ", temp);
        labels.at<uchar>(i, 0) = temp;
    }
    labels.convertTo(labels, CV_32SC1);

    file.close();
    return labels;
}

/*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们
  率
  KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
*/
void Training::knnTrain()
{
    Mat train_images = readImages(0);
    Mat train_labels = readLabels(0);
    qDebug()<<"read mnist train dataset successfully...";

    Ptr<ml::KNearest> knn = ml::KNearest::create();
    knn->setDefaultK(5);
    knn->setIsClassifier(true);
    Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
    knn->train(tdata);
    knn->save("/home/jackey/GitHub/OpenCV-MNIST/data/knn_knowledge.yml");
    qDebug()<<"KNN train data has been saved successfully";
}

void Training::svmTrain()
{
    /*
    //--------------------- 1. Set up training data ---------------------------------------
    Mat trainData = readImages(0);
    Mat trainLabels = readLabels(0);

    qDebug()<<"Train data rows is: "<<trainData.rows
           <<"Train data cols is: "<<trainData.cols;
    qDebug()<<"Train label rows is: "<<trainLabels.rows
           <<"Train label cols is: "<<trainLabels.cols;

    //------------------------ 2. Set up the support vector machines parameters --------------------
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::RBF);
    //svm->setDegree(10.0);
    svm->setGamma(0.01);
    //svm->setCoef0(1.0);
    svm->setC(10.0);
    //svm->setNu(0.5);
    //svm->setP(0.1);
    //svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON));
    svm->setTermCriteria(TermCriteria(cv::TermCriteria::Type::EPS,1000,FLT_EPSILON));

    //------------------------ 3. Train the svm ----------------------------------------------------
    qDebug()<< "Starting training process";
    svm->train(trainData, ROW_SAMPLE, labels);

    //------------------------ 4. save the svm ----------------------------------------------------
    svm->save("/home/jackey/GitHub/OpenCV-MNIST/data/mnist_svm.xml");
    qDebug()<< "save as mnist_svm.xml";

    */

    Mat train_images = readImages(1);
    Mat train_labels = readLabels(1);
    qDebug()<<"Read mnist test dataset successfully...";

        Ptr<ml::SVM> svm = ml::SVM::create();
        svm->setDegree(5);
        Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images,ml::ROW_SAMPLE,train_labels);
        svm->train(tdata);
        svm->save("/home/jackey/GitHub/OpenCV-MNIST/data/mnist_svm.xml");
        qDebug()<<"SVM train file has been saved";
}

void Training::testMnistKNN()
{
    Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("/home/jackey/GitHub/OpenCV-MNIST/data/knn_knowledge.yml"); // KNN - 97%
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

    float rate = correct /total;
    qDebug()<<"Recognize rate is: "<<rate;
}

void Training::testMnistSVM()
{
    //------------------------ 5. load the svm ----------------------------------------------------
    qDebug() << "开始导入SVM文件...";
    Ptr<SVM> svm1 = StatModel::load<SVM>("/home/jackey/GitHub/OpenCV-MNIST/data/mnist_svm.xml");
    qDebug()<< "成功导入SVM文件...";

    //------------------------ 6. read the test dataset -------------------------------------------
    qDebug()<< "开始导入测试数据...\n";
    Mat testData;
    Mat tLabel;
    testData = readImages(1);
    tLabel = readLabels(1);
    qDebug()<< "成功导入测试数据！！！";

    float count = 0;
    for (int i = 0; i < testData.rows; i++) {
        Mat sample = testData.row(i);
        float res = svm1->predict(sample);
        res = std::abs(res - tLabel.at<unsigned int>(i, 0)) <= FLT_EPSILON ? 1.f : 0.f;
        count += res;
    }
    qDebug() << "正确的识别个数 count = " << count << endl;
    qDebug() << "错误率为..." << (10000 - count + 0.0) / 10000 * 100.0 << "%....";

}

int Training::reverseDigit(int num)
{
    unsigned char c1,c2,c3,c4;
    c1=num&255;
    c2=(num>>8)&255;
    c3=(num>>16)&255;
    c4=(num>>24)&255;

    return ((int)c1<<24)+((int)c2<<16)+((int)c3<<8)+c4;
}
