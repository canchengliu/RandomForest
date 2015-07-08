Random Forest MPI (基于MPI并行化实现随机森林)
===========================================

Authoer: Charles (刘灿城)
*Student ID: 12330204*
*School of software, Sun Yat-sen university*
*Guangzhou Higher Education Mega Center*  

Contact email: [liucch3@gmail.com](mailto:liucch3@gmail.com).

--------------------------------------------------------------------------------------------

Notes:
------

### 输入文件格式:
    id,value0,value1,value2...,label
    0, -0.4394, -0.0930, 0.1718, 15
    0, 0.6376, 0.6532, 0.4472, 7
    ...

    第一列为ID， 各属性值为连续的浮点数，最后一列为类别标签，类型为整形。


### 输出文件格式:
    可根据需要输出两个输出文件：
    第一个是：kaggle_submit.txt
    输出格式为：
        id,label
    用于kaggle答案提交
    第二个是：decisionTreeOutput.txt
    用已知label的数据进行测试，并计算准确率

### 程序运行说明：
    1. 运行环境
        a) 操作系统：Mac OS / Linux， 本程序测试所用操作系统是 Mac OSX Yosemite version 10.10.3
        b) 安装配置：需进行MPI相关的配置，本程序测试配置的是 openmpi 1.8.6
    2. 运行方法
        a) 进入到程序主目录， 输入命令 make 进行编译
        b) 输入 mpirun -n [0] rfmpi [1] [2] [3] [4] [5] 运行程序
            参数说明：
            [0] - 
            [1] - Trees per forest handle by each child thread
            [2] - Number of different class label
            [3] - Split keys per node
            [4] - Maximun deepth of each tree and the deepth of root is 0
            [5] - Training set sample probability (%)


            eg: To run the program, execute the following commands in the terminal:

            make
            mpirun -n 3 ./rfmpi 2 26 2 3 70

### 实现说明:


