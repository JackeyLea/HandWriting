# OpenCV-MNIST

基于Qt、OpenCV、mnist的测试软件

# Base

Windows 10 Pro 1903

Qt 5.13.1

GCC 7.3.0

OpenCV 4.1.1

https://blog.csdn.net/qq_26056015/article/details/100176334

这是我编译的，也是我现在用的

直接用Qt编译就可以了，注意库文件位置

MNIST数据集与exe处于同一位置

# Result

## KNN部分

KNN train starting

Reading training data ...

Number of images is:  60000

Reading training data ...

No. of labels: 60000

read mnist train dataset successfully...

KNN train data has been saved successfully

KNN train done

开始KNN测试

Reading test data ...

Number of images is:  10000

Reading test data ...

No. of labels: 10000

Read mnist test dataset successfully...

Recognize rate is:  96.88

All done

## SVM部分

SVM training starting

Reading training data ...

Number of images is:  60000

Reading training data ...

No. of labels: 60000

Read mnist test dataset successfully...

SVM train file has been saved

SVM train done

开始导入SVM文件...

成功导入SVM文件...

开始导入测试数据...

Reading test data ...

Number of images is:  10000

Reading test data ...

No. of labels: 10000

成功导入测试数据！！！

正确的识别个数 count =  1009 

错误率为... 89.91 %....

All done


# Reference

https://www.cnblogs.com/cheermyang/p/5624333.html