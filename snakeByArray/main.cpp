#include<iostream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
using namespace std;

//全局变量
const int High = 20, Width = 30;
int moveDirection;
int food_x, food_y;
int canvas[High + 2][Width + 2] = { 0 }; //二维数组存储整个游戏数据
int con = 0;  //con 用于记录积分
int flag = 1; //flag 用于使主函数中的while循环结束
int last = 0; //last 用于存储上一步移动的方向 从而保持原运动趋势
int score = 0;

//函数声明
void show();   //二维数组的打印函数
void initsnake(); //新蛇及食物的初始化函数
void update();    //运动方位的更新函数
void moveSnake();  //蛇的移动函数
void autoMove();   //无操作时，蛇的自动运动函数
void HideCursor(); //隐藏光标函数
void gotoxy(int x, int y);//将光标转向（x,y）坐标的函数

//主函数
int main() {
	int time = 0, re = 0;
	initsnake();
	//难度选择 
	int hard = 0;
	cout << "输入数字，10及以下为低难度，10到20为中等难度，20及以上为高难度" << endl
		<< "难度选择：";
	cin >> hard;
	if (hard <= 10)cout << "选择成功，你选择的是低难度，游戏即将开始" << endl;
	else if (hard >= 20)cout << "选择成功，你选择的是高难度，游戏即将开始" << endl;
	else cout << "选择成功，你选择的是中等难度，游戏即将开始" << endl;
	//条件设为1，仅当break时才能跳出
	while (1) {
		//根据开始时hard的输入值，通过增加停顿时间，对难度进行逐步降低
		if (hard < 20) {
			Sleep(200);
			if (hard <= 10)
				Sleep(300);
		}
		//隐藏光标
		HideCursor();
		//若flag一直为零，则循环不会跳出，达到反复刷新的效果
		if (!flag) break;
		else gotoxy(0, 4);
		show();
		//time记录初始时间
		time = clock();
		cout << "操作方法：a，s，d，w控制，a向左，d向右，w向上，s向下。" << endl;
		cout << "切记 同时按下上下键或左右键会直接死亡" << endl;
		cout << "当前成绩：" << con << endl;
		update();
		if (last != 0) {
			int time1 = 0;  //time1用于记录正式开始时间
			if (re == 0) {
				cout << "游戏开始";
				re = 1;
				if (last != 0) {
					time1 = clock();
					cout << "游戏时间：" << (time - time1) / 1000 << "秒                  " << endl;
				}
			}
			else {
				cout << "游戏时间：" << (time - time1) / 1000 << "秒                  " << endl;
			}
		}
		else {
			//提示玩家进行输入，若没有输入，last始终0，则游戏不开始
			cout << "输入a s d w 随机一个开始游戏" << endl;
		}
	}
	return 0;
}


//定义新蛇的函数
void initsnake() {
	int i, j;
	//定义边框 赋值-1进行标记
	for (i = 0; i < High; i++) {
		canvas[i][0] = -1;
		canvas[i][Width - 1] = -1;
	}
	for (j = 0; j < Width; j++) {
		canvas[0][j] = -1;
		canvas[High - 1][j] = -1;
	}
	//取正中心点为蛇头初始位置
	canvas[High / 2][Width / 2] = 1;
	//初始化蛇长度为4
	for (i = 1; i <= 4; i++) canvas[High / 2][Width / 2 - i] = i + 1;
	moveDirection = 4;
	//随机初始化食物位置
	food_x = rand() % (High - 4) + 2;
	food_y = rand() % (Width - 4) + 2;
	//将食物位置赋值-2进行标记
	canvas[food_x][food_y] = -2;
}

//二维数组的打印函数
void show() {
	int i = 0;
	for (; i < High; i++) {
		for (int j = 0; j < Width; j++) {
			//边框用#标记
			if (canvas[i][j] == -1) cout << setw(2) << "#";
			//蛇头用●标记
			else if (canvas[i][j] == 1) cout << setw(2) << "●";
			//食物用■标记
			else if (canvas[i][j] == -2) cout << setw(2) << "■";
			//空白场地用空格  标记
			else if (canvas[i][j] == 0) cout << setw(2) << " ";
			//其余值说明此处为蛇身，用□标记
			else cout << setw(2) << "□";
		}
		cout << endl;
	}
}

//运动方位的更新函数
void update() {
	char input;
	if (_kbhit()) {   //判断是否有输入
		input = _getch();    //根据输入值判断是否变更移动方向
		if (input == 'w' || input == 'W') {   //输入w/W 将向上移动，last和moveDirection都赋值为1，再调用moveSnake函数进行移动
			moveDirection = 1;
			last = 1;
			moveSnake();
		}
		else if (input == 's' || input == 'S') {//规则同上，向下移动
			moveDirection = 2;
			last = 2;
			moveSnake();
		}
		else if (input == 'a' || input == 'A') {//规则同上，向左移动
			moveDirection = 3;
			last = 3;
			moveSnake();
		}
		else if (input == 'd' || input == 'D') {//规则同上，向右移动
			moveDirection = 4;
			last = 4;
			moveSnake();
		}
		else autoMove();//输入的值不是a、s、d、w的 不改变方向 调用自动移动函数autoMove 继续运动
	}
	else  autoMove();//没有输入 也不改变运动方向 调用自动移动函数autoMove 继续运动
}
void AfterEatFood()
{
	gotoxy(Width + 16, High - 18);
	printf("%d", score);

}

//贪吃蛇的移动函数
void moveSnake() {
	for (int i = 1; i < High - 1; i++) {
		for (int j = 1; j < Width - 1; j++) {
			if (canvas[i][j] > 0) canvas[i % High][j % Width]++;  	//找到正数元素就+1
		}
	}
	int oldTail_x, oldTail_y, oldHead_i, oldHead_j;
	int max = 0;
	for (int i = 1; i < High - 1; i++) {
		for (int j = 1; j < Width - 1; j++)
			if (canvas[i][j] > 0) {
				if (max < canvas[i][j]) {  //找到数组中存储的最大整数，即为旧蛇的蛇尾
					max = canvas[i][j];
					//标记旧蛇尾
					oldTail_x = i;
					oldTail_y = j;
				}
				if (canvas[i][j] == 2) {   //旧蛇头的值原来为1 在前面执行+1操作后 存储的数值为2的就是旧蛇头
					//标记旧蛇头
					oldHead_i = i;
					oldHead_j = j;
				}
			}
	}
	int newHead_i = 0;
	int	newHead_j = 0;
	//贪吃蛇需有穿墙的功能
	//只需监测旧蛇蛇头运动后的位置是否是围墙即可
	switch (moveDirection) {
	case 1:
		//向上运动 
		// 当旧蛇头oldHead_i - 1的值为0，说明到达上边界
		//新头需要从下边界穿出，即 将newHead_i赋值为 High-2（High-1为围墙）
		if (oldHead_i - 1 == 0) newHead_i = High - 2;
		//若没到边界 直接将oldHead_i - 1赋值给newHead 就能够使新蛇头在旧蛇头上方位置
		else newHead_i = (oldHead_i - 1) % (High);
		//j值无需改变
		newHead_j = oldHead_j % (Width);
		break;
	case 2:
		//向下运动 
		// 当旧蛇头oldHead_i + 1的值为High-1，说明到达下边界
		//新头需要从上边界穿出，即 将newHead_i赋值为 1 （0为围墙）
		if (oldHead_i + 1 == High - 1) newHead_i = 1;
		//若没到边界 直接对oldHead_i + 1 赋值给 newHead_i 就能使新头在旧头的下方位置
		else  newHead_i = (oldHead_i + 1) % (High);
		//j值无需改变
		newHead_j = oldHead_j % (Width);
		break;
	case 3:
		//向左运动 
		//当旧蛇头oldHead_j - 1的值为0，说明到达左边界
		//新头需要从右边界穿出，即 将newHead_j赋值为 Width-2 （Width-1 为围墙）
		if (oldHead_j - 1 == 0) newHead_j = Width - 2;
		//若没到边界 直接对oldHead_j - 1 赋值给 newHead_j 就能使新头在旧头的左边位置
		else newHead_j = (oldHead_j - 1) % (Width);
		//i值无需改变
		newHead_i = oldHead_i % (High);
		break;
	case 4:
		//向右运动 
		// 当旧蛇头oldHead_j + 1的值为Width-1，说明到达右边界
		//新头需要从左边界穿出，即 将newHead_j赋值为 1 （0 为围墙）
		if (oldHead_j + 1 == Width - 1) newHead_j = 1;
		//若没到边界 直接对oldHead_j + 1 赋值给 newHead_j 就能使新头在旧头的右边位置
		else newHead_j = (oldHead_j + 1) % (Width);
		//i值无需改变
		newHead_i = oldHead_i % (High);
		break;
	default:
		break;
	}
	//若新头位置值为正数，说明是蛇身，则游戏结束
	if (canvas[newHead_i][newHead_j] > 0) {
		flag = 0;
		cout << endl;
		cout << "因蛇头碰到蛇身，游戏结束" << endl;
		Sleep(2000);
		cout << "**************************" << endl;
		cout << "你的最终成绩为：" << con << endl;
		exit(0);
	}
	//若新蛇头位置值是-2 说明是食物
	else if (canvas[newHead_i][newHead_j] == -2)
	{   //吃到食物 蛇长度增加1 
		//故保留原蛇尾 仅增加新蛇头
		canvas[newHead_i][newHead_j] = 1;  //将新蛇头位置赋值为1
		//随机生成新食物
		food_x = rand() % (High - 3) + 2;
		food_y = rand() % (Width - 3) + 2;
		canvas[food_x][food_y] = -2;
		//得分增加1
		con++;
	}
	else {
		//未吃到食物 旧蛇尾值变0 新蛇头值变1 即蛇向前运动了一步
		canvas[oldTail_x][oldTail_y] = 0;
		canvas[newHead_i][newHead_j] = 1;
	}
}
//自动移动函数
void autoMove() {
	//全局变量last的初值为0，用于存储上步的操作
	//当last非零时，每过200ms，函数会调用moveSnake（）重复之前的运动
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

//光标隐藏函数
void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo; //定义光标信息的结构体变量
	curInfo.dwSize = 1; //如果没赋值的话，光标隐藏无效
	curInfo.bVisible = FALSE; //将光标设置为不可见
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorInfo(handle, &curInfo); //设置光标信息
}

//光标移动到（x，y）位置的函数
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