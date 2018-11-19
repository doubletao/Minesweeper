#include "stdio.h"
#include "stdlib.h"

//srand()和rand()函数介绍
//计算机里边的“随机”其实是个伪概念，因为计算机是不存在真正的随机的，随机是通过一系列复杂计算模拟出来一个随机，有点像数学上的随机数表法查找随机数
//随机数“种子”的概念有点像指定一个随机数表的起始位置，一般用当前时间来作为随机种子
//这两个是随机数产生函数，其中srand是rand的设置函数，用来给rand一个随机数种子，后续计算机会根据这个种子来给rand函数生成随机数
//time(0)和clock()函数分别是获取本机时间（拿到的是个整数，是从1970年1月1日0点至今的秒数）和获取程序启动以来的毫秒数
//用time(0)+clock()可以获得一个几乎不会重复的整数，将这个整数作为随机数种子，可以基本保证每次rand的数字足够“随机”

//默认游戏规模为10*10，10颗雷，可以修改
#define MAP_SIZE 10

//地图数组（-1为未探索区域，-2为未探索的地雷，0为已探索且附近无地雷，1~8为已探索且附近有n颗地雷，9为已探索的地雷）
int g_nAryMineMap[MAP_SIZE][MAP_SIZE] = {0};

//显示菜单并获取用户选项
int MenuSelector()
{
	system("cls");
	printf("请选择:\n");
	printf("0.退出\n");
	printf("1.开始游戏\n");
	int nUserCommand = 0;
	scanf("%d", &nUserCommand);
	return nUserCommand;
};

//初始化地雷，在MAP_SIZE * MAP_SIZE的位置上填入MAP_SIZE个雷，其实就是从0到MAP_SIZE * MAP_SIZE - 1中选出MAP_SIZE个不重复的数字
void InitialMineMap()
{
	//思路，通过随机数来生成0~MAP_SIZE * MAP_SIZE - 1范围内的数字，如果重复则丢弃，如果不重复则记下来，直到凑够MAP_SIZE个不重复的随机数结束
	//地图初始化为-1未探索
	for (size_t y = 0; y < MAP_SIZE; y++)
	{
		for (size_t x = 0; x < MAP_SIZE; x++)
		{
			g_nAryMineMap[x][y] = -1;
		}
	}
	//选中的结果
	int nAryRet[MAP_SIZE] = { -1 };
	//已选中的地雷数
	int nMineCount = 0;
	while (nMineCount < MAP_SIZE)
	{
		//随机生成一个0~MAP_SIZE * MAP_SIZE - 1范围内的数
		int nRet = rand() % (MAP_SIZE * MAP_SIZE);
		//判定是否与已选中的地雷位置重复
		int bRepeat = 0;
		for (size_t i = 0; i < nMineCount; i++)
		{
			if (nAryRet[i] == nRet)
			{
				bRepeat = 1;
				break;
			}
		}
		if (!bRepeat)
		{
			//不重复则记下来，重复了就继续循环
			nAryRet[nMineCount++] = nRet;
		}
	}
	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		//将选中的地雷位置记录到地雷地图中，其中，十位数作为列，个位数作为行
		g_nAryMineMap[nAryRet[i] / 10][nAryRet[i] % 10] = -2;//整数计算情况下，56除以10等于5，56对10求余等于6，这种办法可以方便的取得数字的十位和个位
	}
	//至此，地图初始化完成
}

//显示当前情况
void ShowCurrentStatus()
{
	system("cls");
	//这就是个打印函数，规则很简单，小于0，为未探索区域，打印个□，0到8直接打印相应数据，代表该位置附近的地雷数，9打印个X，代表踩到雷了
	//打印个列号
	printf("   ");
	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		printf("%d ", i);
	}
	printf("\n");
	for (size_t y = 0; y < MAP_SIZE; y++)
	{
		//打印个行号
		printf("%d :", y);
		for (size_t x = 0; x < MAP_SIZE; x++)
		{
			if (g_nAryMineMap[x][y] < 0)
			{
				printf("□");
			}
			else if (9 == g_nAryMineMap[x][y])//注意这种写法，常数要放在双等号左边，防止笔误将双等号写成单等号
			{
				printf("X ");
			}
			else
			{
				printf("%d ", g_nAryMineMap[x][y]);
			}
		}
		printf("\n");//一列结束，换个行
	}
}

//返回指定位置是不是地雷
int IsMine(int nX, int nY)
{
	if (nX < 0
		|| nY < 0
		|| nX > MAP_SIZE - 1
		|| nY > MAP_SIZE - 1)//越界，直接返回不是
	{
		return 0;
	}
	else if (-2 == g_nAryMineMap[nX][nY])//等于-2，是地雷
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//统计指定位置附近的地雷数
int CountMine(int nX, int nY)
{
	if (IsMine(nX, nY))
	{
		//踩到雷了
		return -1;
	}
	else
	{
		//没有踩到雷，那就数当前格子周围的8个格子中地雷的数量
		int nCount = 0;
		if (IsMine(nX - 1, nY - 1))//左上
		{
			nCount++;
		}
		if (IsMine(nX - 1, nY))//左中
		{
			nCount++;
		}
		if (IsMine(nX - 1, nY + 1))//左下
		{
			nCount++;
		}
		if (IsMine(nX, nY + 1))//中下
		{
			nCount++;
		}
		if (IsMine(nX + 1, nY + 1))//右下
		{
			nCount++;
		}
		if (IsMine(nX + 1, nY))//右中
		{
			nCount++;
		}
		if (IsMine(nX + 1, nY - 1))//右上
		{
			nCount++;
		}
		if (IsMine(nX, nY - 1))//中上
		{
			nCount++;
		}
		return nCount;
	}
}

//点击某个坐标
//ret : 0游戏失败 1游戏继续 2游戏胜利
int ClickCoordinate(int nX, int nY)
{
	if (nX < 0
		|| nY < 0
		|| nX > MAP_SIZE - 1
		|| nY > MAP_SIZE - 1)
	{
		return 1;//越界，直接返回1，游戏继续
	}
	//格子已经被探索，直接返回
	if (g_nAryMineMap[nX][nY] > -1)
	{
		return 1;
	}
	//点击动作思路，点下去如果是个地雷，设置该位置值为9，并则返回0，游戏结束，
	//如果不是地雷，点下去的格子如果附近有地雷，设置该位置值为附近地雷数，并返回1，游戏继续
	//如果不是地雷且附近地雷数为0，则将附近的格子统统点开，并返回1，游戏继续
	//如果已经胜利，返回2，游戏胜利
	if (IsMine(nX, nY))
	{
		g_nAryMineMap[nX][nY] = 9;//点中了地雷，游戏结束
		return 0;
	}
	int nCount = CountMine(nX, nY);
	g_nAryMineMap[nX][nY] = nCount;
	//设定完做一次胜利判定
	if (IsVictory())
	{
		return 2;
	}
	//如果没有胜利，则继续对count做判定
	if (0 == nCount)
	{
		//对周围的8个格子继续执行点击动作，点击过程中如果游戏胜利，就不用继续了
		if (2 == ClickCoordinate(nX - 1, nY - 1))//左上
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX - 1, nY))//左中
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX - 1, nY + 1))//左下
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX, nY + 1))//中下
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX + 1, nY + 1))//右下
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX + 1, nY))//右中
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX + 1, nY - 1))//右上
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX, nY - 1))//中上
		{
			return 2;
		}
		return 1;
	}
	else
	{
		return 1;
	}
}

//胜利判定
int IsVictory()
{
	//胜利判定思路很简单，地图上没有-1（未探索的非地雷格子）就是胜利
	for (size_t y = 0; y < MAP_SIZE; y++)
	{
		for (size_t x = 0; x < MAP_SIZE; x++)
		{
			if (-1 == g_nAryMineMap[x][y])
			{
				return 0;
			}
		}
	}
	return 1;
}

int main()
{
	//将当前时间及程序启动的毫秒数的和作为随机种子
	srand(time(0) + clock());
	do
	{
		int nUserCommand = MenuSelector();
		if (nUserCommand == 0)
		{
			break;
		}
		else//只要不等于0，就视为开始游戏
		{
			InitialMineMap();//初始化地图
			int nGameStatus = 1;
			while (1 == nGameStatus)
			{
				ShowCurrentStatus();//显示当前情况
				//获取用户输入
				printf("\n请点击（格式：x,y）:");
				int nX = -1;
				int nY = -1;
				scanf("%d,%d", &nX, &nY);
				//点击用户格子
				nGameStatus = ClickCoordinate(nX, nY);
			}
			//游戏结束，最后显示一下局势，进入下一次循环
			ShowCurrentStatus();
			if (0 == nGameStatus)
			{
				printf("\n游戏失败!");
			}
			else if (2 == nGameStatus)
			{
				printf("\n游戏胜利!");
			}
			getch();
		}
	} while (1);
}