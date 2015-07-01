#include "rfmpi.h"

MPI并行化实现：

MPI_INITIALIZED(flag) 
	OUT flag 	Flag is true if MPI_INIT has been called and false otherwise.

int MPI_Initialized(int *flag)

MPI_Initialized(flag, ierror)
    LOGICAL, INTENT(OUT) ::  flag
    INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_INITIALIZED(FLAG, IERROR)
    LOGICAL FLAG
    INTEGER IERROR

判断函数MPI_INI()是否已经被调用，即MPI的初始化是否完成。

//------------------------

int MPI_Init(int *argc, char ***argv)

MPI_Init(ierror)
    INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_INIT(IERROR)
    INTEGER IERROR

MPI_INIT()是MPI程序开始时调用的函数，用于完成MPI程序所有的初始化工作。

//-------------------
int MPI_Comm_rank(MPI_Comm comm, int *rank)

MPI_Comm_rank(comm, rank, ierror)
    TYPE(MPI_Comm), INTENT(IN) ::  comm
    INTEGER, INTENT(OUT) ::  rank
    INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_COMM_RANK(COMM, RANK, IERROR)
    INTEGER COMM, RANK, IERROR

MPI_COMM_RANK(comm, rank) 
	IN comm 	communicator (handle)
	OUT rank 	rank of the calling process in group of comm (integer)


comm 表示该进程所在的通讯域（句柄）
rank 调用该进程在comm中的表识号

这一调用返回调用进程在给定的通信域中的进程标识号,有了这一标识号,不同的进程就可以将自身和其它的进程区别开来,实现各进程的并行和协作。

//-----------------------------
MPI_COMM_SIZE(comm, size) 
	IN comm 	communicator (handle)
	OUT size 	number of processes in the group of comm (integer)

int MPI_Comm_size(MPI_Comm comm, int *size)

MPI_Comm_size(comm, size, ierror)
 	TYPE(MPI_Comm), INTENT(IN) ::  comm
    INTEGER, INTENT(OUT) ::  size
    INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_COMM_SIZE(COMM, SIZE, IERROR)
    INTEGER COMM, SIZE, IERROR

comm 表示该进程所在的通讯域（句柄）
size 通信域comm内包括的进程数

这一调用返回给定的通信域中所包括的进程的个数,不同的进程通过这一调用得知在给定的通信域中一共有多少个进程在并行执行

// -----------
MPI_GET_PROCESSOR_NAME( name, resultlen )
	OUT name  		A unique specifier for the actual (as opposed to vir- tual) node.
	OUT resultlen 	Length (in printable characters) of the result returned in name

int MPI_Get_processor_name(char *name, int *resultlen)

MPI_Get_processor_name(name, resultlen, ierror)
    CHARACTER(LEN=MPI_MAX_PROCESSOR_NAME), INTENT(OUT) ::  name
    INTEGER, INTENT(OUT) ::  resultlen
    INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_GET_PROCESSOR_NAME( NAME, RESULTLEN, IERROR)
    CHARACTER*(*) NAME
    INTEGER RESULTLEN,IERROR

//----------------------------
int MPI_Send(const void* buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm)

MPI_SEND(buf, count, datatype, dest, tag, comm)
	IN 	buf 		initial address of send buffer (choice)
	IN 	count 		number of elements in send buffer (non-negative inte- ger)
	IN 	datatype 	datatype of each send buffer element (handle) 
	IN 	dest 		rank of destination (integer)
	IN 	tag 		message tag (integer)
	IN 	comm 		communicator (handle)

MPI_Send(buf, count, datatype, dest, tag, comm, ierror)
		TYPE(*), DIMENSION(..), INTENT(IN) ::  buf
		INTEGER, INTENT(IN) ::  count, dest, tag
		TYPE(MPI_Datatype), INTENT(IN) ::  datatype
		TYPE(MPI_Comm), INTENT(IN) ::  comm
		INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_SEND(BUF, COUNT, DATATYPE, DEST, TAG, COMM, IERROR)
		<type> BUF(*)
		INTEGER COUNT, DATATYPE, DEST, TAG, COMM, IERROR

buf 发送缓冲区的起始地址(可选类型)
count 将发送的数据的个数(非负整数)
datatype 发送数据的数据类型(句柄)
dest 目的进程标识号(整型)
tag 消息标志(整型)
comm 通信域(句柄)

MPI_SEND将发送缓冲区中的count个datatype数据类型的数据发送到目的进程。目的进程在通信域中的标识号是dest，本次发送的消息标志是tag，
送的消息和本进程向同一目的进程发送的其它消息区别开来。使用这一标志，就可以把本次发MPI_SEND操作指定的发送缓冲区是由count个类型为datatype的连续数据空间组成,
起始地址为buf，以数据类型为单位指定消息的长度, 其中datatype数据类型可以是MPI的预定义类型,也可以是用户自定义的类型，通过使用不同的数据类型调用MPI_SEND，可以发送不同类型数据。

//----------------------
int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source,
             int tag, MPI_Comm comm, MPI_Status *status)

MPI_RECV (buf, count, datatype, source, tag, comm, status)
	OUT buf IN count 				initial address of receive buffer (choice)
	IN datatype 				 	number of elements in receive buffer (non-negative in- teger)
	IN source  						message tag or MPI_ANY_TAG (integer) communicator (handle)
	IN tag 							rank of source or MPI_ANY_SOURCE (integer)
	IN comm  						datatype of each receive buffer element (handle)  
	OUT status 						status ob ject (Status)

MPI_Recv(buf, count, datatype, source, tag, comm, status, ierror)
	    TYPE(*), DIMENSION(..)  ::  buf
	    INTEGER, INTENT(IN) ::  count, source, tag
	    TYPE(MPI_Datatype), INTENT(IN) ::  datatype
	    TYPE(MPI_Comm), INTENT(IN) ::  comm
	    TYPE(MPI_Status) ::  status
	    INTEGER, OPTIONAL, INTENT(OUT) ::  ierror

MPI_RECV(BUF, COUNT, DATATYPE, SOURCE, TAG, COMM, STATUS, IERROR)
    <type> BUF(*)
	    INTEGER COUNT, DATATYPE, SOURCE, TAG, COMM, STATUS(MPI_STATUS_SIZE),
	    IERROR

MPI_RECV从指定的进程source接收消息，并且该消息的数据类型和消息标识和本接收。进程指定的datatype和tag相一致，接收到的消息所包含的数据元素的个数最多不能超过count。
接收缓冲区是由count个类型为datatype的连续元素空间组成,由datatype指定其类型,起始地址为buf。其中datatype数据类型可以是MPI的预定义类型，也可以是用户自定义的类型。通过count指定
的接收到消息的长度必须小于或等于接收缓冲区的长度，这是因为如果接收到接收缓冲区会发生溢出错误。因此编程者要保证接收缓冲区的，如果一个短于接收缓冲区的消息到达，那么只有相应于这个消息。不同的数据类型调用MPI_RECV
可以接收不同类型的数据。

//---------------------
int MPI_Finalize(void)

MPI_Finalize(ierror)
	INTEGER, OPTIONAL, INTENT(OUT) :: ierror

MPI_FINALLIZE(IERROR)
	INTEGER IERROR

MPI_FINALIZE是MPI程序的最后一个调用，它结束MPI程序的运行。它会清楚所有的MPI状态，如果一个MPI程序正常退出，那么每一个进程都必须在退出前调用MPI_Finalize。

//--------------------

返回状态变量status是MPI定义的一个数据类型, 使用之前需要用户为它分配空间
在C实现中 状态变量是由至少三个域组成的结构类型 这三个域分别是 MPI_SOURCE MPI_TAG和MPI_ERROR 它还可以包括其它的附加域 这样通过对status.MPI_SOURCE
所包含的发送数据进程的标识 发送数据使用的tag标识和本接收操作返回的错误代码
status.MPI_TAG和status.MPI_ERROR的引用 就可以得到返回状态中在FORTRAN实现中 status是包含MPI_STATUS_SIZE个整型的数组分别表示发送数据的进程标
发送数据使用tag标识和该接收操作返回的错误代码.
除了以上三个信息之外 对status变量执行MPI_GET_COUNT调用可以得到接收到的消 息的长度信息 这在后面的部分会对这一调用进行介绍




// 消息发送接收过程

先将字符串拷贝到发送缓冲区message中 然后调用MPI_Send语句将它发出 用 strlen(message)指定消息的长度 用MPI_CHAR指定消息的数据类型 1指明发往进程1 使 用的消息标识是99 MPI_COMM_WORLD是包含本进程 进程0 和接收消息的进程 进
发送方和接收方必须在同一个通信域中 由通信域来统一协调和控制消息
￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼程1
的通信域 的发送和接收
￼￼￼strcpy(message,"Hello, process 1"); MPI_Send(message, strlen(message), MPI_CHAR, 1,
99,MPI_COMM_WORLD); }
else if(myrank==1) /* 若是进程 1 */
{
进程1直接执行接收消息的操作 这里它使用message作为接收缓冲区 由此可见 对于同 一个变量 在发送进程和接收进程中的作用是不同的 它指定接收消息的最大长度为20 消 息的数据类型为MPI_CHAR字符型 接收的消息来自进程0 而接收消息携带的标识必须为
￼￼￼￼￼￼￼￼使用的通信域也是MPI_COMM_WORLD
接收完成后的各种状态信息存放在status中
接收完成后 它直接将接收到的字符串打印在屏幕上 

#include "rfmpi.h"

int MAX_BIN;
unsigned int MAX_CLASS_COUNT; // the total number of class
int MAX_DEEPTH;

int main(int argc, char ** argv) {
	int is_initialized = 0;
	unsigned int numTrees;
	vvd trainingData;
	vvd testingData;
	readTrainFile("data/Nonlinear/train.csv", trainingData);
	readTestFile("data/Nonlinear/train100.csv", testingData);							// Input data in the form of a vector of vector of strings

	// Determine if MPI has been initialized already.
	MPI_Initialized(&is_initialized);
	if (!is_initialized) {
		// Initialize MPI.
		MPI_Init(&argc, &argv);

		// Determine if this it the master process or a child.
		int rank = 0;
		int processes = 0;
		char name[MPI_MAX_PROCESSOR_NAME];
		int name_size = 0;

		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &processes);
		MPI_Get_processor_name(name, &name_size);
		cout << "Thread Process: " << processes << " Rank: " << rank << " Name: " << name << endl;
		// Check enough arguments ?
		//Extract parameters.
		numTrees = atoi(argv[1]);
		MAX_CLASS_COUNT = atoi(argv[2]); // the total number of class
		MAX_BIN = atoi(argv[3]);
		MAX_DEEPTH = atoi(argv[4]);

		if (rank == processes - 1) {
			double sample_probability = atof(argv[5]);

			if ( sample_probability < 1.0 ) {
				sample_probability = 1.0;
			}
			if ( sample_probability > 100.0 ) {
				sample_probability = 100.0;
			}
			execute_main(processes, sample_probability / 100.0, trainingData, testingData, numTrees);
		}
		else {
			execute_child(processes - 1, rank, testingData, numTrees);
		}
	}
	// Finalize MPI.
	MPI_Finalize();
	return 0;
}



void execute_main(const int process_count, const double sample_probability, vvd& trainingData, vvd& testingData, unsigned int numTrees) {
	unsigned int child_process_count = process_count - 1;

	cout << "Master: Child Process: " << child_process_count << ", Number of Trees:"<< numTrees * child_process_count << ", Sample Probability: " << sample_probability << "%" << endl;
	int sample_probability_int = static_cast<int>(RAND_MAX * sample_probability);

	// Buffers.
	unsigned int col_count = trainingData[0].size();
	double row_buffer[col_count];
	cout << "Master: Sending training data to children..." << endl;
	for (size_t i = 0; i < trainingData.size(); i++) {
		copy(trainingData[i].begin(), trainingData[i].end(), row_buffer);
		// Send to child.
		for ( unsigned int child_rank = 0; child_rank < child_process_count; ++child_rank ) {
			if ( rand() < sample_probability_int ) {
				MPI_Send(
					&row_buffer,
					col_count,
					MPI_DOUBLE,
					child_rank,
					MessageTag::RowBuffer,
					MPI_COMM_WORLD);
			}
		}
	}
	// Stop the loading.
	// Sending stop signal for stoping sending training data to children
	row_buffer[0] = numeric_limits<double>::quiet_NaN();
	for (unsigned int child_rank = 0; child_rank < child_process_count; ++child_rank ) {
		// Send to child.
		MPI_Send(
			&row_buffer,
			col_count,
			MPI_DOUBLE,
			child_rank,
			MessageTag::RowBuffer,
			MPI_COMM_WORLD );
	}
	cout << "Master: Sending finished." << endl;

	// Grab the trees.
	MPI_Status status;
	vvi allPredictedClassLabels; // Store all the predicted class label result of the randomforest
	vi predictedClassLabels;
	unsigned int row_count = testingData.size();
	unsigned int buffer_size = row_count * numTrees;
	int label_buffer[buffer_size];
	cout << "Master: Waiting for result of predicted class labels from children." << endl;
	for ( unsigned int child_rank = 0; child_rank < child_process_count; ++child_rank ) {
		cout << "Master: Waiting on slave " << child_rank << "..." << endl;
		// Wait on child.
		MPI_Recv(
			&label_buffer,
			buffer_size,
			MPI_INT,
			child_rank,
			MessageTag::TreeFinished,
			MPI_COMM_WORLD,
			&status );

		for (size_t i = 0; i < numTrees; i++) {
			predictedClassLabels.assign(label_buffer + i * row_count, label_buffer + (i + 1) * row_count);
			allPredictedClassLabels.push_back(predictedClassLabels);
			predictedClassLabels.clear();
		}
	}
	cout << "Master: Loaded all forests ! Classifying..." << endl;
	// Load testing data.
	allRandomForest(allPredictedClassLabels, testingData);
	cout << "Master: Finished!\n" << endl;
}

void execute_child(const unsigned int parent_rank, const unsigned int rank, vvd& testingData, unsigned int numTrees) {
	cout << testingData.size() << endl;
	unsigned int col_count = testingData[0].size();

	MPI_Status status;
	//cout << "Slave [" << rank << "] online: [BD: " << bootstrap_divisor << ", SK: " << split_keys_per_node << ", TPF: " << trees_per_forest << "]" << endl;

	cout << "Slave [" << rank << "]: Receiving the trainingData" << endl;

	double buffer[col_count];
	vd data;
	buffer[0] = 0.0;
	vvd trainingData;

	while ( true ) {
		MPI_Recv(
			&buffer,
			col_count,
			MPI_DOUBLE,
			parent_rank,
			MessageTag::RowBuffer,
			MPI_COMM_WORLD,
			&status );

		// Stop on NaN.
		if ( buffer[0] != buffer[0] ) {
			break;
		}
		// Add elements to data vector.
		data.assign(buffer, buffer + col_count);
		trainingData.push_back(data);
		data.clear();
	}
	cout << "Slave [" << rank << "]: Growing tree..." << endl;
	vvi predictedClassLabels;
	randomForest(numTrees, trainingData, testingData, predictedClassLabels);
	unsigned int row_count = predictedClassLabels[0].size();
	unsigned int buffer_size = numTrees * row_count;
	int label_buffer[buffer_size];
	for (size_t i = 0; i < numTrees; i++) {
		copy(predictedClassLabels[i].begin(), predictedClassLabels[i].end(), label_buffer + i * row_count);
	}
	// Send finished signal.

	MPI_Send(
		&label_buffer,
		buffer_size,
		MPI_INT,
		parent_rank,
		MessageTag::TreeFinished,
		MPI_COMM_WORLD);

	cout << "Slave [" << rank << "]: Finished!" << endl;
}

