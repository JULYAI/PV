#include <iostream>
#include "p5.h"
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define random() (rand()%2)

enum State       //进程状态
{
	running,     //运行
	ready,       //就绪
	waiting,     //阻塞
	done         //终止
};
enum WaitFor{    //等待
	null, sema
};

struct PCB
{
	char pName;         //进程名
	State pState;		//状态
	WaitFor pWaitFor;	//是否等待信号量
	int PC;				//指令计数器
};

PCB p1, p2;
PCB * pro, *proc;

static char BufferPool[10];//大小为10的缓冲池

int s1;//空闲信号量
int s2;//占用信号量

int in = 0;
int out = 0;

int PC;
int PA[5];
int PB[5];

char c1,c2;

//P,V原语
int P(int s){
	s--;
	if (s < 0){
		pro->pState = waiting;
		pro->pWaitFor = sema;	//等待信号量s
	}
	else
		pro->pState = ready;
	return s;
}
int V(int s){
	s++;
	if (s <= 0){
		proc->pState = ready;
	}
	pro->pState = ready;
	return s;
}

//模拟一组指令
void Put(char c){
	BufferPool[in] = c;
	in = (in + 1) % 10;
}
char Get(){
	char c = BufferPool[out];
	out = (out + 1) % 10;
	return c;
}
char produce(){
	char c;
	cout << "请输入一个字符：" << endl;
	cin >> c;
	return c;
}
void consume(char c){
	cout << "！！！"<<c << endl;
}
void GOTO(int L){
	pro->PC = L;
}
void NOP(){

}


//初始化程序
void init(){
	s1 = 10;	//空闲信号量赋初值10
	s2 = 0;		//占用信号量赋初值0
	p1 = {'P',ready,null,0};//生产者进程
	p2 = {'C',ready,null,0};//消费者进程
	p1.pState = running;
	pro = &p1;
	proc = &p2;
	//生产者处理器指令执行程序
	for (int i = 0; i < 5; i++){
		PA[i] = i;
	}
	//消费者处理器指令执行程序
	for (int i = 0; i < 5; i++){
		PB[i] = i;
	}
	schedule();
}

//处理器调度程序
void schedule(){
	if (pro->pState == running){
		pro->pState = ready;
	}
	if (p1.pState == ready||p2.pState == ready){//生产者和消费者同时就绪
		if (random() == 0)
		{ 
			pro = &p1; 
			proc = &p2; 
		}
		else
		{ 
			pro = &p2;
			proc = &p1; 
		}
		if (pro->pState == ready)
		{
			PC = pro->PC;
			pro->pState = running;
			pcDoing();
		}
	}

}

//处理器指令调度程序
void pcDoing(){
	int i = PC;
	showPCB(i);
	if (p1.pState == running){
		int j = PA[i];
		switch (j)
		{
		case 0:c1 = produce(); pro->PC++; break;
		case 1:s1 = P(s1); pro->PC++; break;
		case 2:Put(c1); pro->PC++; break;
		case 3:s2 = V(s2); pro->PC++; break;
		case 4:GOTO(0); break;	
		default:
			break;
		}
	}
	if (p2.pState == running){
		int j = PB[i];
		switch (j)
		{
		case 0:s2 = P(s2); pro->PC++; break;
		case 1:c2 = Get(); pro->PC++; break;
		case 2:s1 = V(s1); pro->PC++; break;
		case 3:consume(c2); pro->PC++; break;
		case 4:GOTO(0); break;
		default:
			break;
		}
	}
}

//显示进程
void showPCB(int i){
	cout << "---现在进行的进程是" << pro->pName << "第" << i << "步" << "---------" << endl;
}


int main(){
	int m = 500;
	init();
	while (m>0){
		schedule();
		m--;
	}	
	return 0;
}
