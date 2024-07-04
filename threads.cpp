#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

void lesson1()
{
	const char str[] = "Hello World!\b\t\nhello-1998.";
	size_t len = sizeof(str);
	for (size_t i = 0; i < len; i++)
	{
		std::cout << str[i] << ":";
		if (isalnum(str[i])) std::cout << "A"; //检测字符是否是英文字母或数字
		if (isalpha(str[i])) std::cout << "B"; //检测字符是否为英文字母
		if (isascii(str[i])) std::cout << "C"; //是否为ASCII
		if (isblank(str[i])) std::cout << "D"; //是否为空格字符
		if (iscntrl(str[i])) std::cout << "E"; //是否为ASCII控制字符
		if (isdigit(str[i])) std::cout << "F"; //是否为阿拉伯数字
		if (isgraph(str[i])) std::cout << "G"; //是否是可打印字符(图形字符，比如空格就不行)
		if (islower(str[i])) std::cout << "H"; //是否为小写字母
		if (isupper(str[i])) std::cout << "I"; //是否是大写字母
		if (isprint(str[i])) std::cout << "J"; //是否是可打印字符
		if (isspace(str[i])) std::cout << "K"; //是否为空格字符(任何，包括\n \r等)
		if (ispunct(str[i])) std::cout << "O"; //是否为标点或者特殊符号
		if (isxdigit(str[i])) std::cout << "P";//是否为16进制
		std::cout << std::endl;
	}
}

void lesson2()
{

}

void lesson3()
{
	uid_t x_user = geteuid();
	uid_t now_user = getuid();
	std::cout << x_user << std::endl;
	std::cout << now_user << std::endl;
}

#include <setjmp.h>
#include <signal.h>
//jmp_buf jmpbuf;
//void test002()
//{
//	longjmp(jmpbuf, 2);
//}
//void test001()
//{
//	longjmp(jmpbuf, 1);
//}
//void signal_deal(int sig)
//{
//	if (sig == SIGSEGV)
//		longjmp(jmpbuf, SIGSEGV);//自由跳转
//}
//void lesson4()
//{
//	signal(SIGSEGV, signal_deal);
//	//跳转后直接还原寄存器，若中间有计算，不会保存。
//	int ret = setjmp(jmpbuf);//异常捕获
//	if (ret == 0)
//	{
//		test001();
//		*(int*)(NULL) = 0;
//	}
//	else if (ret == 1)
//		printf("error 1\n");
//	else if (ret == 2)
//		printf("error 2\n");
//	else if (ret == SIGSEGV)
//		printf("error SIGSEGV\n");
//}

#include <sys/resource.h>
void lesson5()
{
	std::cout << "#getpid:" << getpid() << std::endl;
	std::cout << "getpid-group:" << getpgid(getpid()) << std::endl;
	std::cout << "#getgrp:" << getpgrp() << std::endl;
	std::cout << "getppid:" << getppid() << std::endl;
	std::cout << "#getpriority:" << getpriority(PRIO_PROCESS, getpid()) << std::endl;//获取优先级，越小越牛逼
	
	std::cout << "setpid:" << setpgid(getpid(), 1) << std::endl; //只能设置子进程，否则返回-1
	std::cout << "setpgrp:" << setpgrp() << std::endl;
	std::cout << "nice:" << nice(3) << std::endl;
	std::cout << "$getpriority:" << getpriority(PRIO_PROCESS, getpid()) << std::endl;
	std::cout << "setpriority:" << setpriority(PRIO_PROCESS, getpid(), 1) << std::endl; //如果没权限，可以降级，不可以升级
	std::cout << "$getpid:" << getpid() << std::endl;
	std::cout << "$getgrp:" << getpgrp() << std::endl;
	std::cout << "$getpriority:" << getpriority(PRIO_PROCESS, getpid()) << std::endl; 
	sleep(10);
}

void lesson6()
{
	int ret = system("ls -l");
	printf("system return : %d\n", ret);
	char buffer[256] = "";
	snprintf(buffer, sizeof(buffer), "echo \"%s\" >> file.txt", __FUNCTION__);
	printf("%s\n", buffer);
	ret = system(buffer);
	printf("system return:%d\n", ret);
}

#include <sys/wait.h>
void handle_child_status(int status)
{
	// 判断子进程是否正常退出
	if (WIFEXITED(status)) {
		// 打印子进程的退出状态码
		printf("Child exited with status %d\n", WEXITSTATUS(status));
	}
	// 判断子进程是否被信号终止
	else if (WIFSIGNALED(status)) {
		// 打印导致子进程终止的信号编号
		printf("Child killed by signal %d\n", WTERMSIG(status));
	}
	// 判断子进程是否被信号停止（通常用于调试）
	else if (WIFSTOPPED(status)) {
		// 打印导致子进程停止的信号编号
		printf("Child stopped by signal %d\n", WSTOPSIG(status));
	}
	// 判断子进程是否继续执行（在之前被停止的情况下）
	else if (WIFCONTINUED(status)) {
		// 打印子进程继续执行的消息
		printf("Child continued\n");
	}
}
void lesson7()
{
	pid_t pid;
	int status;
	pid = fork();
	if (pid == -1) {
		printf("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		//进入子进程
		printf("Child process (PID: %d) started\n", getpid());
		std::cout << "Child getgrp:" << getpgrp() << std::endl;
	}
	else{
		//进入父进程
		printf("Parent process (PID: %d) waiting for child process (PID: %d)\n", getpid(), pid);
		std::cout << "Parent getgrp:" << getpgrp() << std::endl;
		while (waitpid(pid, &status, WNOHANG) == 0) {
			printf("Still waiting for child process to finish...\n");
			sleep(4); // 模拟父进程的其他工作
		}
		pid_t wpid = wait(&status);
		if (wpid == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		printf("Parent process (PID: %d) finished waiting for child process (PID: %d)\n", getpid(), wpid);
		handle_child_status(status);
	}
}

void lesson_file()
{
	FILE* pf = freopen("./ttt.txt", "rw+t", stdout);//文件重定向：把输出写到文件中
	if (pf != NULL)
	{
		printf("xxxxx\n");
		fclose(pf);
	}
	else
	{
		printf("open file failure!\n");
	}
}

void lesson_file_3()
{
	int ret = 0;
	/*ret = mkdir(__FUNCTION__, 0755);
	printf("%s(%d):%s ret = %d\n", __FILE__, __LINE__, __FUNCTION__, ret);
	system("echo \"test\" > lesson_file_3/test.txt");*/
	ret = remove(__FUNCTION__);
	printf("%s(%d):%s remove = %d\n", __FILE__, __LINE__, __FUNCTION__, ret);
}

#include <dirent.h>
void lesson_file_4()
{
	DIR* proot = opendir("/root/");
	if (proot == NULL)
	{
		printf("%s(%d):%s is not exist!\n", __FILE__, __LINE__, __FUNCTION__);
		return;
	}
	dirent* pCur = NULL;
	do {
		pCur = readdir(proot);
		if (pCur != NULL) {
			if (strcmp(pCur->d_name, ".") == 0) continue;
			if (pCur->d_type & DT_DIR) {
				printf("%s(%d):%s unlink = %s\n", __FILE__, __LINE__, __FUNCTION__, pCur->d_name);
			}
		}
	} while (pCur != NULL);
	closedir(proot);
}

void lesson_IO()
{
	int fd = open("/root/file/test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	if (fd >= 0)
	{
		printf("flock:%d\n", flock(fd, LOCK_EX));
		printf("write:%d\n", write(fd, "hello", 5));
		sleep(6);
		flock(fd, LOCK_UN);
		close(fd);
	}
	else
	{
		std::cout << strerror(errno) << std::endl;
	}
}

void lesson_exec()
{
	pid_t pid = fork();
	if (pid > 0)
	{
		sleep(3);
		std::cout << "parent." << pid << std::endl;
	}
	else
	{
		execl("/bin/ls", "ls", "-l", NULL);
	}
}

void lesson_sock()
{
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock != -1)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("192.168.0.1");
		addr.sin_port = htons(9527);
		int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
		if (ret != 0)
		{
			std::cout << "192.168.0.1接收失败！\n";
		}
		else 
		{
			std::cout << "192.168.0.1接收成功！\n";
		}
	}
}

void run_client()
{
	int client = socket(PF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		perror("socket creation failed");
		return;
	}
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(9527);
	int ret = connect(client, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (ret == 0)
	{
		char buffer[256] = "hello, here is client.\n";
		write(client, buffer, strlen(buffer));
		memset(buffer, 0, sizeof(buffer));
		read(client, buffer, sizeof(buffer));
		std::cout << buffer << std::endl;
	}
	else
	{
		perror("connect failed");
	}
	close(client);
	std::cout << "client done!" << std::endl;
}
void run_server()
{
	int server, client;
	struct sockaddr_in seraddr, cliaddr;
	socklen_t cliaddrlen = sizeof(cliaddr); //没有初始化
	//const char* message = "hello, here is server.\n";
	server = socket(PF_INET, SOCK_STREAM, 0);
	if (server < 0)
	{
		std::cout << "create socket failed!" << std::endl;
		return;
	}
	memset(&seraddr, 0, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	seraddr.sin_port = htons(9527);
	int ret = bind(server, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if (ret == -1)
	{
		std::cout << "bind failed!" << std::endl;
		close(server);
		return;
	}
	ret = listen(server, 3);
	if (ret == -1)
	{
		std::cout << "listen failed" << std::endl;
		close(server);
		return;
	}
	char buffer[1024];
	while (1)
	{
		memset(buffer, 0, sizeof(buffer));
		client = accept(server, (struct sockaddr*)&cliaddr, &cliaddrlen);
		if (client == -1)
		{
			std::cout << "accept failed!" << std::endl;
			close(server);
			return;
		}
		read(client, buffer, sizeof(buffer));
		std::cout << "Received from client: " << buffer << std::endl;
		ssize_t len = write(client, buffer, strlen(buffer));
		if (len != (ssize_t)strlen(buffer))
		{
			std::cout << "write failed!" << std::endl;
			close(server);
			close(server);
			return;
		}
		close(client);
	}
	close(server);
}
void lesson_sock_2()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		sleep(2);
		run_client();
		run_client();
	}
	else if (pid > 0)
	{
		run_server();
		int status = 0;
		wait(&status);
	}
	else
	{
		std::cout << "fork failed! " << pid << std::endl;
	}
}

#define BUF_SIZE 30
void lesson_udp(int argc, char* argv[])
{
	int serv_sock;
	char message[BUF_SIZE] = "";
	int str_len;
	socklen_t clnt_adr_sz;

	struct sockaddr_in serv_adr, clnt_adr;
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
	{
		printf("UDP socket creation error\n");
	}
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		printf("bind() error\n");
	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
	}
	close(serv_sock);
}
void lesson_udp_client(int argc, char* argv[])
{
	int client_sock;
	struct sockaddr_in serv_adr;
	socklen_t serv_len = sizeof(serv_adr);

	if (argc != 3)
	{
		printf("Usage: %s ip <port>\n", argv[0]);
		exit(1);
	}
	client_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (client_sock == -1)
	{
		//
	}
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));


	close(client_sock);

}

#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h>
typedef struct {
	int id;
	char name[128];
	int age;
	bool sex;
}STUDENT, * PSTUDENT;
void lesson_shared_memory()
{
	pid_t pid = fork();
	if (pid > 0)
	{
		key_t key = ftok(".", 2);
		int sem_id = semget(key, 2, IPC_CREAT);
		semctl(sem_id, 0, SETVAL, 0);
		semctl(sem_id, 1, SETVAL, 0);
		int shm_id = shmget(ftok(".",1), sizeof(STUDENT), IPC_CREAT | 0666);
		if (shm_id == -1)
		{
			return;
		}
		PSTUDENT pStu = (PSTUDENT)shmat(shm_id, NULL, 0);
		pStu->id = 130111;
		strcpy(pStu->name, "iuk");
		pStu->age = 29;
		pStu->sex = true;
		sembuf sop = {
			.sem_num = 0,
			.sem_op = 1
		};
		semop(sem_id, &sop, 1);//V操作
		sop.sem_num = 1;
		sop.sem_op = -1;
		semop(sem_id, &sop, 1);//P操作
		shmdt(pStu);
		shmctl(shm_id, IPC_RMID, NULL);
		semctl(sem_id, 0, IPC_RMID);
		semctl(sem_id, 1, IPC_RMID);
	}
	else
	{
		key_t key = ftok(".", 2);
		int sem_id = semget(key, 2, IPC_CREAT);
		int shm_id = shmget(ftok(".", 1), sizeof(STUDENT), IPC_CREAT | 0666);
		if (shm_id == -1)
		{
			return;
		}
		sembuf sop = {
			.sem_num = 0,
			.sem_op = 1
		};
		semop(sem_id, &sop, 1);//P操作
		PSTUDENT pStu = (PSTUDENT)shmat(shm_id, NULL, 0);
		printf("id:%d\tname:%s\n", pStu->id, pStu->name);
		sop.sem_num = 1;
		sop.sem_op = 1;
		semop(sem_id, &sop, 1);//V操作
		shmdt(pStu);
		shmctl(shm_id, IPC_RMID, NULL);
	}
}

#include <sys/msg.h>
typedef struct
{
	int type;
	char data[1024];
}MSG,*PMSG;
void lesson_message_queue()
{
	pid_t pid = fork();
	if (pid > 0)
	{
		int msg_id = msgget(ftok(".", 3), IPC_CREAT | 0666);
		MSG msg;
		memset(&msg, 0, sizeof(msg));
		msgrcv(msg_id, &msg, sizeof(msg), 0, 0);
		printf("%d\t%s\n", msg.type, msg.data);
		getchar();
		msgctl(msg_id, IPC_RMID, 0);
	}
	else
	{
		int msg_id = msgget(ftok(".", 3), IPC_CREAT | 0666);
		MSG msg;
		memset(&msg, 0, sizeof(msg));
		msg.type = 1;
		strcpy(msg.data, "bingo");
		msgsnd(msg_id, &msg, sizeof(msg), 0);
		msgctl(msg_id, IPC_RMID, 0);
	}
}

#include <pthread.h>
void* threadEntry(void* arg)
{
	printf("%s(%d):%s thread begin:%d\n", __FILE__, __LINE__, __FUNCTION__, *(int*)arg);
	const char* msg = "I'm a thread.";
	return (void*)msg;
}
void lesson_thread()
{
	pthread_t tid;
	//const char* pInfo = "hello world";
	const int a = 12345;
	const int* pInfo = &a;
	int ret = pthread_create(&tid, NULL, threadEntry, (void*)pInfo);
	//↑最好不要用局部变量
	// -lpthread
	if (ret != -1)
	{
		void* result = NULL;
		pthread_join(tid, &result);
		printf("from thread:%s\n", result);
	}
}

int num = 0;
pthread_mutex_t mutex;
//互斥量不允许多个进程同时访问
void* thread_inc(void* arg)
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 100000; i++)
		num++;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
void* thread_dec(void* arg)
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 100000; i++)
		num--;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
void lesson_thread_hc()
{
	
	pthread_mutex_init(&mutex, NULL);
	pthread_t thread_id[50];
	for (int i = 0; i < 50; i++)
	{
		if (i<=30)
			pthread_create(thread_id + i, NULL, thread_inc, NULL);
		else
			pthread_create(thread_id + i, NULL, thread_dec, NULL);
	}
	for (int i = 0; i < 50; i++)
	{
		pthread_join(thread_id[i], NULL);
	}
	printf("num = %d\n", num);
	pthread_mutex_destroy(&mutex);
}

#include <semaphore.h>
sem_t sem1;
sem_t sem2;
void* input_num(void* arg)
{
	pthread_detach(pthread_self());
	int count = *(int*)arg;
	for(int i =0;i<count;i++)
	{
		printf("Input num : ");
		sem_wait(&sem2);
		scanf("%d", &num);
		sem_post(&sem1);
	}
	pthread_exit(NULL);
	//return NULL;
}
void lesson_thread_signal()
{
	int sum = 0;
	const int count = 5;
	sem_init(&sem1, 0, 0);//是否完成输入
	sem_init(&sem2, 0, 1);//是否完成计算
	pthread_t thread;
	pthread_create(&thread, NULL, input_num, (void*)&count);
	for (int i = 0; i < count; i++) 
	{
		sem_wait(&sem1);
		sum += num;
		sem_post(&sem2);
	}
	printf("sum = %d\n", sum);
	sem_destroy(&sem2);
	sem_destroy(&sem1);
}

int clnt_socks[100] = {0};
int clnt_cnt = 0;
pthread_mutex_t mutex1;
void send_msg(const char* msg, ssize_t str_len)
{
	pthread_mutex_lock(&mutex1);
	for (int i = 0; i < clnt_cnt; i++)
	{
		if (clnt_socks[i] >= 0)
		{
			write(clnt_socks[i], msg, str_len); 
		}
	}
	pthread_mutex_unlock(&mutex1);
}
void* handle_clnt(void* arg)
{
	pthread_detach(pthread_self());
	int clnt_sock = *(int*)arg;
	char msg[1024] = "";
	ssize_t str_len = 0;
	while((str_len = read(clnt_sock, msg, sizeof(msg))) > 0)
		send_msg(msg, str_len);
	pthread_mutex_lock(&mutex1);
	for (int i = 0; i < clnt_cnt; i++)
	{
		if (clnt_sock == clnt_socks[i]) 
		{
			clnt_socks[i] = -1;
			break;
		}
	}
	pthread_mutex_unlock(&mutex1);
	close(clnt_sock);
	pthread_exit(NULL);
}
sem_t semid;
char name[64] = "";
void* client_send_msg(void* arg)
{
	pthread_detach(pthread_self());
	int sock = *(int*)arg;
	char msg[256] = "";
	char buffer[1024] = "";
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		fgets(msg, sizeof(msg), stdin);
		//if ((strcmp(msg, "q\n") == 0) || (strcmp(msg, "Q\n") == 0)) break;
		snprintf(buffer, sizeof(buffer), "[%s]: %s", name, msg);
		ssize_t bytes_written = write(sock, buffer, strlen(buffer));
		printf("Bytes written: %zd\n", bytes_written);
	}
	sem_post(&semid);
	pthread_exit(NULL);
}
void* client_recv_msg(void* arg)
{
	pthread_detach(pthread_self());
	int sock = *(int*)arg;
	char msg[256] = "";
	while (true)
	{
		ssize_t str_len = read(sock, msg, sizeof(msg));
		if (str_len <= 0) break;
		fputs(msg, stdout);
		memset(msg, 0, sizeof(msg));
	}
	sem_post(&semid);
	pthread_exit(NULL);
}
void client_threads()
{
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(9527);
	fputs("input your name:", stdout);
	fflush(stdout);
	scanf("%s", name);
	getchar();
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		printf("connect() error");
		return;
	}
	pthread_t thsend, threcv;
	sem_init(&semid, 0, -1);
	pthread_create(&thsend, NULL, client_send_msg, (void*)&sock);
	pthread_create(&threcv, NULL, client_recv_msg, (void*)&sock);
	sem_wait(&semid);
	close(sock);
}
void server_threads()
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz = sizeof(clnt_adr);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9527);  
	pthread_mutex_init(&mutex1, NULL);
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		printf("bind() error\n");
		return;
	}
	if (listen(serv_sock, 5) == -1)
	{
		printf("listen() error\n");
		return;
	}
	while (true)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1) break;
		//将客户端处理交给线程
		pthread_mutex_lock(&mutex1);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutex1);
		pthread_t thread;
		pthread_create(&thread, NULL, handle_clnt, &clnt_socks[clnt_cnt-1]);
	}
	close(serv_sock);
	pthread_mutex_destroy(&mutex1);
}
void lesson_threads(const char* arg)
{
	if (strcmp(arg, "s") == 0) server_threads();
	else client_threads();
}

int main(int argc, char* argv[])
{
	//lesson1();
	//lesson7();
	//lesson_file_4();
	//lesson_IO();
	//lesson_exec();
	//lesson_sock();
	//lesson_sock_2();
	//lesson_udp(argc, argv);
	//lesson_shared_memory();
	//lesson_message_queue();
	//lesson_thread();
	//lesson_thread_hc();
	//lesson_thread_signal();
	printf("%s\n", argv[1]);
	lesson_threads(argv[1]);
	return 0;
}
