#include <iostream>
#include "p5.h"
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define random() (rand()%2)

enum State       //����״̬
{
	running,     //����
	ready,       //����
	waiting,     //����
	done         //��ֹ
};
enum WaitFor{    //�ȴ�
	null, sema
};

struct PCB
{
	char pName;         //������
	State pState;		//״̬
	WaitFor pWaitFor;	//�Ƿ�ȴ��ź���
	int PC;				//ָ�������
};

PCB p1, p2;
PCB * pro, *proc;

static char BufferPool[10];//��СΪ10�Ļ����

int s1;//�����ź���
int s2;//ռ���ź���

int in = 0;
int out = 0;

int PC;
int PA[5];
int PB[5];

char c1,c2;

//P,Vԭ��
int P(int s){
	s--;
	if (s < 0){
		pro->pState = waiting;
		pro->pWaitFor = sema;	//�ȴ��ź���s
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

//ģ��һ��ָ��
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
	cout << "������һ���ַ���" << endl;
	cin >> c;
	return c;
}
void consume(char c){
	cout << "������"<<c << endl;
}
void GOTO(int L){
	pro->PC = L;
}
void NOP(){

}


//��ʼ������
void init(){
	s1 = 10;	//�����ź�������ֵ10
	s2 = 0;		//ռ���ź�������ֵ0
	p1 = {'P',ready,null,0};//�����߽���
	p2 = {'C',ready,null,0};//�����߽���
	p1.pState = running;
	pro = &p1;
	proc = &p2;
	//�����ߴ�����ָ��ִ�г���
	for (int i = 0; i < 5; i++){
		PA[i] = i;
	}
	//�����ߴ�����ָ��ִ�г���
	for (int i = 0; i < 5; i++){
		PB[i] = i;
	}
	schedule();
}

//���������ȳ���
void schedule(){
	if (pro->pState == running){
		pro->pState = ready;
	}
	if (p1.pState == ready||p2.pState == ready){//�����ߺ�������ͬʱ����
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

//������ָ����ȳ���
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

//��ʾ����
void showPCB(int i){
	cout << "---���ڽ��еĽ�����" << pro->pName << "��" << i << "��" << "---------" << endl;
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
