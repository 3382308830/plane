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


// ��ʼ����Ϸ����
void InitGame();
// ����Ϸ����
void DrawGame();
void DrawEnemy(struct EnemyPlane enplane);
//���õз�ս��
void SetEnemy(struct EnemyPlane enplane[], int index);
void UpdateGame();	// �޸���Ϸ��������

// ����BGM
void loadBGM()
{
	mciSendString("open level.mp3", NULL, 0, NULL);
	mciSendString("play level.mp3", NULL, 0, NULL);
}
//�л�����
typedef struct EnemyPlane {
	int x;
	int y;
	int speed;
};
//�ṹ������
EnemyPlane enemyplane[MAX];


// Pos:Position λ��
typedef struct Pos
{
	int x, y;
};



IMAGE BackgroundImg;	     // ���屳��ͼƬ
int BackgroundImg0Y, BackgroundImg1Y;	// ���屳��ͼƬλ��
IMAGE EnemyImg[2];	         //����з�ս��ͼƬ
IMAGE MyPlaneImg0, MyPlaneImg1;	// �����ҷ�ս��ͼƬ
Pos MyPlanePos;	             // ����ɻ�����

IMAGE MyBulletImg0, MyBulletImg1;	// �����ӵ�ͼƬ

std::list<Pos> MyBulletList;	// �����ӵ�����



int main()
{
	initgraph(WIDTH, HEIGHT);
	InitGame();

	loadBGM();
	//��Ϸ��ʼǰ����ʼ�����ез�ս��
	for (int i = 0; i < MAX; i++)
	{
		SetEnemy(enemyplane, i);
	}

	BeginBatchDraw();
	while (true)
	{
		DrawGame();
		//�����з�ս������
		//DrawEnemy({200,200,10});
		for (int i = 0; i < MAX; i++)
		{
			DrawEnemy(enemyplane[i]);
		}
		for (int i = 0; i < MAX; i++)
		{
			enemyplane[i].y = enemyplane[i].y + enemyplane[i].speed;
			//�γ�ѭ������
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

// ��ʼ����Ϸ
void InitGame()
{
	/* ����ͼƬ */
	// ���ر���ͼƬ
	loadimage(&BackgroundImg, "bg.jpg");
	// ���طɻ�ͼƬ
	loadimage(&MyPlaneImg0, "uiPlane0.jpg", 100, 100);
	loadimage(&MyPlaneImg1, "uiPlane1.jpg", 100, 100);
	loadimage(&EnemyImg[0], "enplane0.jpg", 100, 100);
	loadimage(&EnemyImg[1], "enplane1.jpg", 100, 100);
	// �����ӵ�ͼƬ
	loadimage(&MyBulletImg0, "bullet0.jpg");
	loadimage(&MyBulletImg1, "bullet1.jpg");

	/* ��ʼ����Ϸ���� */
	// ��ʼ������ͼƬλ��
	BackgroundImg0Y = 0;
	BackgroundImg1Y = -HEIGHT;
	// ��ʼ���ɻ���λ��
	MyPlanePos.x = (WIDTH - 60) * 0.5;			        // (WIDTH - 60) >> 1
	MyPlanePos.y = (HEIGHT - 62) * 0.5 + 170;	// (HEIGHT - 62) >> 1
	// ��ʼ���ӵ�����
	MyBulletList.clear();
}

// ����Ϸ
void DrawGame()
{
	// ˫������ͼ
	BeginBatchDraw();

	/* ��ʾ��Ϸ���� */
	// �������ͼƬ
	putimage(0, BackgroundImg0Y, &BackgroundImg);
	putimage(0, BackgroundImg1Y, &BackgroundImg);

	// ����ɻ�ͼƬ
	putimage(MyPlanePos.x, MyPlanePos.y, &MyPlaneImg0, SRCPAINT);
	putimage(MyPlanePos.x, MyPlanePos.y, &MyPlaneImg1, SRCAND);


	// ����ӵ�ͼƬ
	for (Pos pos : MyBulletList)
	{
		putimage(pos.x, pos.y, &MyBulletImg0, SRCPAINT);
		putimage(pos.x, pos.y, &MyBulletImg1, SRCAND);
	}

	EndBatchDraw();
}

//�����з�ս��
void DrawEnemy(EnemyPlane enplane)
{
	//��ֹ����������˫����
	BeginBatchDraw();
	putimage(enplane.x, enplane.y, &EnemyImg[1], SRCPAINT);
	putimage(enplane.x, enplane.y, &EnemyImg[0], SRCAND);
	FlushBatchDraw();
	//����˫����
	EndBatchDraw();

}
//���õз�ս������
void SetEnemy(struct EnemyPlane enplane[], int index)
{
	enplane[index].x = rand() % 451;    //[0,500]
	enplane[index].y = rand() % 401 - 500;   //[-500,-100]
	enplane[index].speed = rand() % 8 + 3;   //[3,10]
}

// �޸���Ϸ����
void UpdateGame()
{
	/* �޸���Ϸ���� */
	// �޸ı���λ��
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

	// �ӵ��ƶ�
	for (Pos& pos : MyBulletList)
	{
		pos.y -= 5;
	}
}