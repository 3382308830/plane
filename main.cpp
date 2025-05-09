#include <stdio.h>
#include <graphics.h>
#include <list>
#include <mmsystem.h>
#include<time.h>
#include<stdlib.h>
#pragma comment(lib,"winmm.lib")

#define WIDTH 520
#define HEIGHT 800
#define MAX 8


// 初始化游戏声明
void InitGame();
// 画游戏声明
void DrawGame();
void DrawEnemy(struct EnemyPlane enplane);
//重置敌方战机
void SetEnemy(struct EnemyPlane enplane[], int index);
void UpdateGame();	// 修改游戏数据声明

// 播放BGM
void loadBGM()
{
	mciSendString("open level.mp3", NULL, 0, NULL);
	mciSendString("play level.mp3", NULL, 0, NULL);
}
//敌机属性
typedef struct EnemyPlane {
	int x;
	int y;
	int speed;
};
//结构体数组
EnemyPlane enemyplane[MAX];


// Pos:Position 位置
typedef struct Pos
{
	int x, y;
};



IMAGE BackgroundImg;	     // 定义背景图片
int BackgroundImg0Y, BackgroundImg1Y;	// 定义背景图片位置
IMAGE EnemyImg[2];	         //定义敌方战机图片
IMAGE MyPlaneImg0, MyPlaneImg1;	// 定义我方战机图片
Pos MyPlanePos;	             // 定义飞机坐标

IMAGE MyBulletImg0, MyBulletImg1;	// 定义子弹图片

std::list<Pos> MyBulletList;	// 定义子弹链表



int main()
{
	initgraph(WIDTH, HEIGHT);
	InitGame();

	loadBGM();
	//游戏开始前，初始化所有敌方战机
	for (int i = 0; i < MAX; i++)
	{
		SetEnemy(enemyplane, i);
	}

	BeginBatchDraw();
	while (true)
	{
		DrawGame();
		//单个敌方战机数据
		//DrawEnemy({200,200,10});
		for (int i = 0; i < MAX; i++)
		{
			DrawEnemy(enemyplane[i]);
		}
		for (int i = 0; i < MAX; i++)
		{
			enemyplane[i].y = enemyplane[i].y + enemyplane[i].speed;
			//形成循环掉落
			if (enemyplane[i].y >= HEIGHT)
			{
				SetEnemy(enemyplane, i);
			}
		}

		UpdateGame();
		Sleep(10);
	}
	FlushBatchDraw();
	EndBatchDraw();
	return 0;
}

// 初始化游戏
void InitGame()
{
	/* 加载图片 */
	// 加载背景图片
	loadimage(&BackgroundImg, "bg.jpg");
	// 加载飞机图片
	loadimage(&MyPlaneImg0, "uiPlane0.jpg", 100, 100);
	loadimage(&MyPlaneImg1, "uiPlane1.jpg", 100, 100);
	loadimage(&EnemyImg[0], "enplane0.jpg", 100, 100);
	loadimage(&EnemyImg[1], "enplane1.jpg", 100, 100);
	// 加载子弹图片
	loadimage(&MyBulletImg0, "bullet0.jpg");
	loadimage(&MyBulletImg1, "bullet1.jpg");

	/* 初始化游戏数据 */
	// 初始化背景图片位置
	BackgroundImg0Y = 0;
	BackgroundImg1Y = -HEIGHT;
	// 初始化飞机的位置
	MyPlanePos.x = (WIDTH - 60) * 0.5;			        // (WIDTH - 60) >> 1
	MyPlanePos.y = (HEIGHT - 62) * 0.5 + 170;	// (HEIGHT - 62) >> 1
	// 初始化子弹链表
	MyBulletList.clear();
}

// 画游戏
void DrawGame()
{
	// 双缓冲贴图
	BeginBatchDraw();

	/* 显示游戏画面 */
	// 输出背景图片
	putimage(0, BackgroundImg0Y, &BackgroundImg);
	putimage(0, BackgroundImg1Y, &BackgroundImg);

	// 输出飞机图片
	putimage(MyPlanePos.x, MyPlanePos.y, &MyPlaneImg0, SRCPAINT);
	putimage(MyPlanePos.x, MyPlanePos.y, &MyPlaneImg1, SRCAND);


	// 输出子弹图片
	for (Pos pos : MyBulletList)
	{
		putimage(pos.x, pos.y, &MyBulletImg0, SRCPAINT);
		putimage(pos.x, pos.y, &MyBulletImg1, SRCAND);
	}

	EndBatchDraw();
}

//画出敌方战机
void DrawEnemy(EnemyPlane enplane)
{
	//防止闪屏，开启双缓冲
	BeginBatchDraw();
	putimage(enplane.x, enplane.y, &EnemyImg[1], SRCPAINT);
	putimage(enplane.x, enplane.y, &EnemyImg[0], SRCAND);
	FlushBatchDraw();
	//结束双缓冲
	EndBatchDraw();

}
//重置敌方战机数据
void SetEnemy(struct EnemyPlane enplane[], int index)
{
	enplane[index].x = rand() % 451;    //[0,500]
	enplane[index].y = rand() % 401 - 500;   //[-500,-100]
	enplane[index].speed = rand() % 8 + 3;   //[3,10]
}

// 修改游戏数据
void UpdateGame()
{
	/* 修改游戏数据 */
	// 修改背景位置
	BackgroundImg0Y++;
	if (BackgroundImg0Y >= HEIGHT)
		BackgroundImg0Y = -HEIGHT;
	BackgroundImg1Y++;
	if (BackgroundImg1Y >= HEIGHT)
		BackgroundImg1Y = -HEIGHT;

	if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))
	{
		if (MyPlanePos.y > 0)
			MyPlanePos.y -= 5;
	}
	if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))
	{
		if (MyPlanePos.y < HEIGHT - 62)
			MyPlanePos.y += 3;
	}
	if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
	{
		if (MyPlanePos.x > 0)
			MyPlanePos.x -= 5;
	}
	if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
	{
		if (MyPlanePos.x < WIDTH - 60)
			MyPlanePos.x += 5;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		Pos pos;
		pos.x = MyPlanePos.x + (100 - 9) * 0.5;
		pos.y = MyPlanePos.y - 21;
		MyBulletList.push_back(pos);
	}

	// 子弹移动
	for (Pos& pos : MyBulletList)
	{
		pos.y -= 5;
	}
}