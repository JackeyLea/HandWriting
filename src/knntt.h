/*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们的正确率
  KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
*/

#ifndef KNNTT_H
#define KNNTT_H

#include "global.hpp"

class KNNTT :public QThread
{
public:
    KNNTT();

    void setRunMode(RUNMODE mode);
    float getRate();
    int getPredictResult();

    void reset();

    Mat readImagesData();//读取图像数据集
    Mat readLabelsData();//读取标记数据集

    int predict(Mat &img);

protected:
    void run();

private:
    RUNMODE _run_mode;
    ifstream f;

    float _rate;
    int _predictResult;
    double time_start;
    double time_end;
};

#endif // KNNTT_H
