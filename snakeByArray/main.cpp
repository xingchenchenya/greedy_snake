#include<iostream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
using namespace std;

//ȫ�ֱ���
const int High = 20, Width = 30;
int moveDirection;
int food_x, food_y;
int canvas[High + 2][Width + 2] = { 0 }; //��ά����洢������Ϸ����
int con = 0;  //con ���ڼ�¼����
int flag = 1; //flag ����ʹ�������е�whileѭ������
int last = 0; //last ���ڴ洢��һ���ƶ��ķ��� �Ӷ�����ԭ�˶�����
int score = 0;

//��������
void show();   //��ά����Ĵ�ӡ����
void initsnake(); //���߼�ʳ��ĳ�ʼ������
void update();    //�˶���λ�ĸ��º���
void moveSnake();  //�ߵ��ƶ�����
void autoMove();   //�޲���ʱ���ߵ��Զ��˶�����
void HideCursor(); //���ع�꺯��
void gotoxy(int x, int y);//�����ת��x,y������ĺ���

//������
int main() {
	int time = 0, re = 0;
	initsnake();
	//�Ѷ�ѡ�� 
	int hard = 0;
	cout << "�������֣�10������Ϊ���Ѷȣ�10��20Ϊ�е��Ѷȣ�20������Ϊ���Ѷ�" << endl
		<< "�Ѷ�ѡ��";
	cin >> hard;
	if (hard <= 10)cout << "ѡ��ɹ�����ѡ����ǵ��Ѷȣ���Ϸ������ʼ" << endl;
	else if (hard >= 20)cout << "ѡ��ɹ�����ѡ����Ǹ��Ѷȣ���Ϸ������ʼ" << endl;
	else cout << "ѡ��ɹ�����ѡ������е��Ѷȣ���Ϸ������ʼ" << endl;
	//������Ϊ1������breakʱ��������
	while (1) {
		//���ݿ�ʼʱhard������ֵ��ͨ������ͣ��ʱ�䣬���ѶȽ����𲽽���
		if (hard < 20) {
			Sleep(200);
			if (hard <= 10)
				Sleep(300);
		}
		//���ع��
		HideCursor();
		//��flagһֱΪ�㣬��ѭ�������������ﵽ����ˢ�µ�Ч��
		if (!flag) break;
		else gotoxy(0, 4);
		show();
		//time��¼��ʼʱ��
		time = clock();
		cout << "����������a��s��d��w���ƣ�a����d���ң�w���ϣ�s���¡�" << endl;
		cout << "�м� ͬʱ�������¼������Ҽ���ֱ������" << endl;
		cout << "��ǰ�ɼ���" << con << endl;
		update();
		if (last != 0) {
			int time1 = 0;  //time1���ڼ�¼��ʽ��ʼʱ��
			if (re == 0) {
				cout << "��Ϸ��ʼ";
				re = 1;
				if (last != 0) {
					time1 = clock();
					cout << "��Ϸʱ�䣺" << (time - time1) / 1000 << "��                  " << endl;
				}
			}
			else {
				cout << "��Ϸʱ�䣺" << (time - time1) / 1000 << "��                  " << endl;
			}
		}
		else {
			//��ʾ��ҽ������룬��û�����룬lastʼ��0������Ϸ����ʼ
			cout << "����a s d w ���һ����ʼ��Ϸ" << endl;
		}
	}
	return 0;
}


//�������ߵĺ���
void initsnake() {
	int i, j;
	//����߿� ��ֵ-1���б��
	for (i = 0; i < High; i++) {
		canvas[i][0] = -1;
		canvas[i][Width - 1] = -1;
	}
	for (j = 0; j < Width; j++) {
		canvas[0][j] = -1;
		canvas[High - 1][j] = -1;
	}
	//ȡ�����ĵ�Ϊ��ͷ��ʼλ��
	canvas[High / 2][Width / 2] = 1;
	//��ʼ���߳���Ϊ4
	for (i = 1; i <= 4; i++) canvas[High / 2][Width / 2 - i] = i + 1;
	moveDirection = 4;
	//�����ʼ��ʳ��λ��
	food_x = rand() % (High - 4) + 2;
	food_y = rand() % (Width - 4) + 2;
	//��ʳ��λ�ø�ֵ-2���б��
	canvas[food_x][food_y] = -2;
}

//��ά����Ĵ�ӡ����
void show() {
	int i = 0;
	for (; i < High; i++) {
		for (int j = 0; j < Width; j++) {
			//�߿���#���
			if (canvas[i][j] == -1) cout << setw(2) << "#";
			//��ͷ�á���
			else if (canvas[i][j] == 1) cout << setw(2) << "��";
			//ʳ���á����
			else if (canvas[i][j] == -2) cout << setw(2) << "��";
			//�հ׳����ÿո�  ���
			else if (canvas[i][j] == 0) cout << setw(2) << " ";
			//����ֵ˵���˴�Ϊ�����á����
			else cout << setw(2) << "��";
		}
		cout << endl;
	}
}

//�˶���λ�ĸ��º���
void update() {
	char input;
	if (_kbhit()) {   //�ж��Ƿ�������
		input = _getch();    //��������ֵ�ж��Ƿ����ƶ�����
		if (input == 'w' || input == 'W') {   //����w/W �������ƶ���last��moveDirection����ֵΪ1���ٵ���moveSnake���������ƶ�
			moveDirection = 1;
			last = 1;
			moveSnake();
		}
		else if (input == 's' || input == 'S') {//����ͬ�ϣ������ƶ�
			moveDirection = 2;
			last = 2;
			moveSnake();
		}
		else if (input == 'a' || input == 'A') {//����ͬ�ϣ������ƶ�
			moveDirection = 3;
			last = 3;
			moveSnake();
		}
		else if (input == 'd' || input == 'D') {//����ͬ�ϣ������ƶ�
			moveDirection = 4;
			last = 4;
			moveSnake();
		}
		else autoMove();//�����ֵ����a��s��d��w�� ���ı䷽�� �����Զ��ƶ�����autoMove �����˶�
	}
	else  autoMove();//û������ Ҳ���ı��˶����� �����Զ��ƶ�����autoMove �����˶�
}
void AfterEatFood()
{
	gotoxy(Width + 16, High - 18);
	printf("%d", score);

}

//̰���ߵ��ƶ�����
void moveSnake() {
	for (int i = 1; i < High - 1; i++) {
		for (int j = 1; j < Width - 1; j++) {
			if (canvas[i][j] > 0) canvas[i % High][j % Width]++;  	//�ҵ�����Ԫ�ؾ�+1
		}
	}
	int oldTail_x, oldTail_y, oldHead_i, oldHead_j;
	int max = 0;
	for (int i = 1; i < High - 1; i++) {
		for (int j = 1; j < Width - 1; j++)
			if (canvas[i][j] > 0) {
				if (max < canvas[i][j]) {  //�ҵ������д洢�������������Ϊ���ߵ���β
					max = canvas[i][j];
					//��Ǿ���β
					oldTail_x = i;
					oldTail_y = j;
				}
				if (canvas[i][j] == 2) {   //����ͷ��ֵԭ��Ϊ1 ��ǰ��ִ��+1������ �洢����ֵΪ2�ľ��Ǿ���ͷ
					//��Ǿ���ͷ
					oldHead_i = i;
					oldHead_j = j;
				}
			}
	}
	int newHead_i = 0;
	int	newHead_j = 0;
	//̰�������д�ǽ�Ĺ���
	//ֻ���������ͷ�˶����λ���Ƿ���Χǽ����
	switch (moveDirection) {
	case 1:
		//�����˶� 
		// ������ͷoldHead_i - 1��ֵΪ0��˵�������ϱ߽�
		//��ͷ��Ҫ���±߽紩������ ��newHead_i��ֵΪ High-2��High-1ΪΧǽ��
		if (oldHead_i - 1 == 0) newHead_i = High - 2;
		//��û���߽� ֱ�ӽ�oldHead_i - 1��ֵ��newHead ���ܹ�ʹ����ͷ�ھ���ͷ�Ϸ�λ��
		else newHead_i = (oldHead_i - 1) % (High);
		//jֵ����ı�
		newHead_j = oldHead_j % (Width);
		break;
	case 2:
		//�����˶� 
		// ������ͷoldHead_i + 1��ֵΪHigh-1��˵�������±߽�
		//��ͷ��Ҫ���ϱ߽紩������ ��newHead_i��ֵΪ 1 ��0ΪΧǽ��
		if (oldHead_i + 1 == High - 1) newHead_i = 1;
		//��û���߽� ֱ�Ӷ�oldHead_i + 1 ��ֵ�� newHead_i ����ʹ��ͷ�ھ�ͷ���·�λ��
		else  newHead_i = (oldHead_i + 1) % (High);
		//jֵ����ı�
		newHead_j = oldHead_j % (Width);
		break;
	case 3:
		//�����˶� 
		//������ͷoldHead_j - 1��ֵΪ0��˵��������߽�
		//��ͷ��Ҫ���ұ߽紩������ ��newHead_j��ֵΪ Width-2 ��Width-1 ΪΧǽ��
		if (oldHead_j - 1 == 0) newHead_j = Width - 2;
		//��û���߽� ֱ�Ӷ�oldHead_j - 1 ��ֵ�� newHead_j ����ʹ��ͷ�ھ�ͷ�����λ��
		else newHead_j = (oldHead_j - 1) % (Width);
		//iֵ����ı�
		newHead_i = oldHead_i % (High);
		break;
	case 4:
		//�����˶� 
		// ������ͷoldHead_j + 1��ֵΪWidth-1��˵�������ұ߽�
		//��ͷ��Ҫ����߽紩������ ��newHead_j��ֵΪ 1 ��0 ΪΧǽ��
		if (oldHead_j + 1 == Width - 1) newHead_j = 1;
		//��û���߽� ֱ�Ӷ�oldHead_j + 1 ��ֵ�� newHead_j ����ʹ��ͷ�ھ�ͷ���ұ�λ��
		else newHead_j = (oldHead_j + 1) % (Width);
		//iֵ����ı�
		newHead_i = oldHead_i % (High);
		break;
	default:
		break;
	}
	//����ͷλ��ֵΪ������˵������������Ϸ����
	if (canvas[newHead_i][newHead_j] > 0) {
		flag = 0;
		cout << endl;
		cout << "����ͷ����������Ϸ����" << endl;
		Sleep(2000);
		cout << "**************************" << endl;
		cout << "������ճɼ�Ϊ��" << con << endl;
		exit(0);
	}
	//������ͷλ��ֵ��-2 ˵����ʳ��
	else if (canvas[newHead_i][newHead_j] == -2)
	{   //�Ե�ʳ�� �߳�������1 
		//�ʱ���ԭ��β ����������ͷ
		canvas[newHead_i][newHead_j] = 1;  //������ͷλ�ø�ֵΪ1
		//���������ʳ��
		food_x = rand() % (High - 3) + 2;
		food_y = rand() % (Width - 3) + 2;
		canvas[food_x][food_y] = -2;
		//�÷�����1
		con++;
	}
	else {
		//δ�Ե�ʳ�� ����βֵ��0 ����ͷֵ��1 ������ǰ�˶���һ��
		canvas[oldTail_x][oldTail_y] = 0;
		canvas[newHead_i][newHead_j] = 1;
	}
}
//�Զ��ƶ�����
void autoMove() {
	//ȫ�ֱ���last�ĳ�ֵΪ0�����ڴ洢�ϲ��Ĳ���
	//��last����ʱ��ÿ��200ms�����������moveSnake�����ظ�֮ǰ���˶�
	if (last == 1) {
		moveDirection = 1;
		moveSnake();
		Sleep(200);
		return;
	}
	else if (last == 2) {
		moveDirection = 2;
		moveSnake();
		Sleep(200);
		return;
	}
	else if (last == 3) {
		moveDirection = 3;
		moveSnake();
		Sleep(200);
		return;
	}
	else if (last == 4) {
		moveDirection = 4;
		moveSnake();
		Sleep(200);
		return;
	}
	else {
		Sleep(200);
		return;
	}
}

//������غ���
void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo; //��������Ϣ�Ľṹ�����
	curInfo.dwSize = 1; //���û��ֵ�Ļ������������Ч
	curInfo.bVisible = FALSE; //���������Ϊ���ɼ�
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
	SetConsoleCursorInfo(handle, &curInfo); //���ù����Ϣ
}

//����ƶ�����x��y��λ�õĺ���
void gotoxy(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = sizeof(cursor);
	GetConsoleCursorInfo(handle, &cursor);
}