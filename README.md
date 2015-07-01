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














* The input files describe a tabular format for the data.
* Values of the different attributes of a single instance/row are delimited by commas.
* The first line (row) of the input file (table) must contain the names of the columns/attributes. Each column name must be unique.
* The last column of the table must contain the class labels.
* Every consequent line (after the first one) corresponds to values of the attributes specified in the first line for a particular instance.

For example if we want to predict whether someone is cool or not, based on their choice of device, social network and laptop, we can have the following data table:

    --------------------------------------
    | Device | Network  | Laptop | Cool? |
    --------------------------------------
    | ipod   | twitter  | mac    | no    |
    | iphone | facebook | pc     | yes   |
    | ipod   | facebook | mac    | yes   |
    | iphone | facebook | mac    | yes   |
    | iphone | facebook | pc     | yes   |
    | iphone | twitter  | pc     | yes   |
    | ipod   | twitter  | mac    | no    |
    | iphone | twitter  | mac    | yes   |
    | none   | twitter  | mac    | no    |
    | none   | facebook | mac    | yes   |
    | ipod   | twitter  | mac    | no    |
    | iphone | facebook | pc     | yes   |
    | iphone | twitter  | mac    | yes   |
    | iphone | twitter  | mac    | yes   |
    | ipod   | twitter  | pc     | no    |
    | none   | twitter  | mac    | no    |
    --------------------------------------

This table will be represented in the training and testing files as:

    Device,Network,Laptop,Cool?
    ipod,twitter,mac,no
    iphone,facebook,pc,yes
    ipod,facebook,mac,yes
    iphone,facebook,mac,yes
    iphone,facebook,pc ,yes
    iphone,twitter,pc,yes
    ipod,twitter,mac,no
    iphone,twitter,mac,yes
    none,twitter,mac,no
    none,facebook,mac,yes
    ipod,twitter,mac,no
    iphone,facebook,pc,yes
    iphone,twitter,mac,yes
    iphone,twitter,mac,yes
    ipod,twitter,pc,no
    none,twitter,mac,no

Sample training (`train.dat`) and testing (`test.dat`) files have been included.  
In the sample files, we're trying to predict which restaurant (*Buck-I-Mart*/*Oxley's*/*Brennan's*/*Oxley's Cafe*) a customer will go to based on:

* How quickly a customer wants the meal (Column name: *speed*. Values that *speed* can take: *quick*, *leisurely*, *medium*).
* Meal preference (Column name: *meal*. Values that *meal* can take: *soup*, *sandwich*, *other*).
* Whether the customer wants coffee or not (Column name: *coffee*. Values that *coffee* can take: *yes*, *no*).
* How many patrons are already eating at *Oxley's* (Column name: *oxley*. Values that *oxley* can take: *0*, *1-3*, *4-8*, *>8*).

And so on.

The column we want to predict is *restaurant*. The class labels are:   

* *Buck-I-Mart*
* *Oxley's*
* *Brennan's*
* *Oxley's Cafe*

### To run the program, execute the following commands in the terminal:

    make clean
    make
    ./dtree train.dat test.dat

After the program finishes execution, open `decisionTreeOutput.txt` to view the output.

### Technical Notes:

The algorithm that builds the decision tree is a recursive algorithm and is implemented in the function `buildDecisionTree()` which is defined in `functions.cpp`. The algorithm works as follows:

    if the sub-table passed to the algorithm is empty
        return NULL;    // since there is no data in the table
    if the sub-table passed to the algorithm is homogeneous (if all the rows have the same value for the last column)
        mark this node as a leaf node;
        label this node with the value of the last column
        return a pointer to this node
    else
        decide a column to split the table on based on information gain
        set the node's splitOn value to this column's name
        for all the values that the splitting column can take:
            create a new node
            set the new node as the current node's child node
            prune the sub-table so that all the rows with this value of the last column are removed
            recursively call the function by passing it the new pruned table and the new node

The splitting column is decided based on information gain. This information gain is calculated using entropy which is a measure of randomness of information. The less random a split leaves the table in, the more information we can gain from it. So we split the table on the column which gives us the most information gain (least entropy).

### Tags

`decision`, `tree`, `decision tree`, `decision tree code`, `decision tree C++`, `decision tree C`, `decision tree C++ code`.
