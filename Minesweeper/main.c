#include "stdio.h"
#include "stdlib.h"

//srand()��rand()��������
//�������ߵġ��������ʵ�Ǹ�α�����Ϊ������ǲ���������������ģ������ͨ��һϵ�и��Ӽ���ģ�����һ��������е�����ѧ�ϵ�����������������
//����������ӡ��ĸ����е���ָ��һ������������ʼλ�ã�һ���õ�ǰʱ������Ϊ�������
//���������������������������srand��rand�����ú�����������randһ����������ӣ��������������������������rand�������������
//time(0)��clock()�����ֱ��ǻ�ȡ����ʱ�䣨�õ����Ǹ��������Ǵ�1970��1��1��0��������������ͻ�ȡ�������������ĺ�����
//��time(0)+clock()���Ի��һ�����������ظ��������������������Ϊ��������ӣ����Ի�����֤ÿ��rand�������㹻�������

//Ĭ����Ϸ��ģΪ10*10��10���ף������޸�
#define MAP_SIZE 10

//��ͼ���飨-1Ϊδ̽������-2Ϊδ̽���ĵ��ף�0Ϊ��̽���Ҹ����޵��ף�1~8Ϊ��̽���Ҹ�����n�ŵ��ף�9Ϊ��̽���ĵ��ף�
int g_nAryMineMap[MAP_SIZE][MAP_SIZE] = {0};

//��ʾ�˵�����ȡ�û�ѡ��
int MenuSelector()
{
	system("cls");
	printf("��ѡ��:\n");
	printf("0.�˳�\n");
	printf("1.��ʼ��Ϸ\n");
	int nUserCommand = 0;
	scanf("%d", &nUserCommand);
	return nUserCommand;
};

//��ʼ�����ף���MAP_SIZE * MAP_SIZE��λ��������MAP_SIZE���ף���ʵ���Ǵ�0��MAP_SIZE * MAP_SIZE - 1��ѡ��MAP_SIZE�����ظ�������
void InitialMineMap()
{
	//˼·��ͨ�������������0~MAP_SIZE * MAP_SIZE - 1��Χ�ڵ����֣�����ظ�������������ظ����������ֱ���չ�MAP_SIZE�����ظ������������
	//��ͼ��ʼ��Ϊ-1δ̽��
	for (size_t y = 0; y < MAP_SIZE; y++)
	{
		for (size_t x = 0; x < MAP_SIZE; x++)
		{
			g_nAryMineMap[x][y] = -1;
		}
	}
	//ѡ�еĽ��
	int nAryRet[MAP_SIZE] = { -1 };
	//��ѡ�еĵ�����
	int nMineCount = 0;
	while (nMineCount < MAP_SIZE)
	{
		//�������һ��0~MAP_SIZE * MAP_SIZE - 1��Χ�ڵ���
		int nRet = rand() % (MAP_SIZE * MAP_SIZE);
		//�ж��Ƿ�����ѡ�еĵ���λ���ظ�
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
			//���ظ�����������ظ��˾ͼ���ѭ��
			nAryRet[nMineCount++] = nRet;
		}
	}
	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		//��ѡ�еĵ���λ�ü�¼�����׵�ͼ�У����У�ʮλ����Ϊ�У���λ����Ϊ��
		g_nAryMineMap[nAryRet[i] / 10][nAryRet[i] % 10] = -2;//������������£�56����10����5��56��10�������6�����ְ취���Է����ȡ�����ֵ�ʮλ�͸�λ
	}
	//���ˣ���ͼ��ʼ�����
}

//��ʾ��ǰ���
void ShowCurrentStatus()
{
	system("cls");
	//����Ǹ���ӡ����������ܼ򵥣�С��0��Ϊδ̽�����򣬴�ӡ������0��8ֱ�Ӵ�ӡ��Ӧ���ݣ������λ�ø����ĵ�������9��ӡ��X������ȵ�����
	//��ӡ���к�
	printf("   ");
	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		printf("%d ", i);
	}
	printf("\n");
	for (size_t y = 0; y < MAP_SIZE; y++)
	{
		//��ӡ���к�
		printf("%d :", y);
		for (size_t x = 0; x < MAP_SIZE; x++)
		{
			if (g_nAryMineMap[x][y] < 0)
			{
				printf("��");
			}
			else if (9 == g_nAryMineMap[x][y])//ע������д��������Ҫ����˫�Ⱥ���ߣ���ֹ����˫�Ⱥ�д�ɵ��Ⱥ�
			{
				printf("X ");
			}
			else
			{
				printf("%d ", g_nAryMineMap[x][y]);
			}
		}
		printf("\n");//һ�н�����������
	}
}

//����ָ��λ���ǲ��ǵ���
int IsMine(int nX, int nY)
{
	if (nX < 0
		|| nY < 0
		|| nX > MAP_SIZE - 1
		|| nY > MAP_SIZE - 1)//Խ�磬ֱ�ӷ��ز���
	{
		return 0;
	}
	else if (-2 == g_nAryMineMap[nX][nY])//����-2���ǵ���
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//ͳ��ָ��λ�ø����ĵ�����
int CountMine(int nX, int nY)
{
	if (IsMine(nX, nY))
	{
		//�ȵ�����
		return -1;
	}
	else
	{
		//û�вȵ��ף��Ǿ�����ǰ������Χ��8�������е��׵�����
		int nCount = 0;
		if (IsMine(nX - 1, nY - 1))//����
		{
			nCount++;
		}
		if (IsMine(nX - 1, nY))//����
		{
			nCount++;
		}
		if (IsMine(nX - 1, nY + 1))//����
		{
			nCount++;
		}
		if (IsMine(nX, nY + 1))//����
		{
			nCount++;
		}
		if (IsMine(nX + 1, nY + 1))//����
		{
			nCount++;
		}
		if (IsMine(nX + 1, nY))//����
		{
			nCount++;
		}
		if (IsMine(nX + 1, nY - 1))//����
		{
			nCount++;
		}
		if (IsMine(nX, nY - 1))//����
		{
			nCount++;
		}
		return nCount;
	}
}

//���ĳ������
//ret : 0��Ϸʧ�� 1��Ϸ���� 2��Ϸʤ��
int ClickCoordinate(int nX, int nY)
{
	if (nX < 0
		|| nY < 0
		|| nX > MAP_SIZE - 1
		|| nY > MAP_SIZE - 1)
	{
		return 1;//Խ�磬ֱ�ӷ���1����Ϸ����
	}
	//�����Ѿ���̽����ֱ�ӷ���
	if (g_nAryMineMap[nX][nY] > -1)
	{
		return 1;
	}
	//�������˼·������ȥ����Ǹ����ף����ø�λ��ֵΪ9�����򷵻�0����Ϸ������
	//������ǵ��ף�����ȥ�ĸ�����������е��ף����ø�λ��ֵΪ������������������1����Ϸ����
	//������ǵ����Ҹ���������Ϊ0���򽫸����ĸ���ͳͳ�㿪��������1����Ϸ����
	//����Ѿ�ʤ��������2����Ϸʤ��
	if (IsMine(nX, nY))
	{
		g_nAryMineMap[nX][nY] = 9;//�����˵��ף���Ϸ����
		return 0;
	}
	int nCount = CountMine(nX, nY);
	g_nAryMineMap[nX][nY] = nCount;
	//�趨����һ��ʤ���ж�
	if (IsVictory())
	{
		return 2;
	}
	//���û��ʤ�����������count���ж�
	if (0 == nCount)
	{
		//����Χ��8�����Ӽ���ִ�е����������������������Ϸʤ�����Ͳ��ü�����
		if (2 == ClickCoordinate(nX - 1, nY - 1))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX - 1, nY))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX - 1, nY + 1))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX, nY + 1))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX + 1, nY + 1))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX + 1, nY))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX + 1, nY - 1))//����
		{
			return 2;
		}
		if (2 == ClickCoordinate(nX, nY - 1))//����
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

//ʤ���ж�
int IsVictory()
{
	//ʤ���ж�˼·�ܼ򵥣���ͼ��û��-1��δ̽���ķǵ��׸��ӣ�����ʤ��
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
	//����ǰʱ�估���������ĺ������ĺ���Ϊ�������
	srand(time(0) + clock());
	do
	{
		int nUserCommand = MenuSelector();
		if (nUserCommand == 0)
		{
			break;
		}
		else//ֻҪ������0������Ϊ��ʼ��Ϸ
		{
			InitialMineMap();//��ʼ����ͼ
			int nGameStatus = 1;
			while (1 == nGameStatus)
			{
				ShowCurrentStatus();//��ʾ��ǰ���
				//��ȡ�û�����
				printf("\n��������ʽ��x,y��:");
				int nX = -1;
				int nY = -1;
				scanf("%d,%d", &nX, &nY);
				//����û�����
				nGameStatus = ClickCoordinate(nX, nY);
			}
			//��Ϸ�����������ʾһ�¾��ƣ�������һ��ѭ��
			ShowCurrentStatus();
			if (0 == nGameStatus)
			{
				printf("\n��Ϸʧ��!");
			}
			else if (2 == nGameStatus)
			{
				printf("\n��Ϸʤ��!");
			}
			getch();
		}
	} while (1);
}