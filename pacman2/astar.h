#pragma once
#include "define.h"

class CAStarNode
{
public:
	int father;
	int G;
	int H;
	bool close;
	bool open;

	CAStarNode() : father(0), G(0), H(0), close(0), open(0) {}
};


class CAStar
{
private:
	CAStarNode ns[static_cast<int>(Map::CNT)];	// 点集

	vector<int> open_list;		// 开放列表

	int s;	// 开始结点（怪物）
	int e;	// 结束结点（玩家）

	int w_inst;	// 距离权重，此值越大，monster越注重距离控制
	int w_path;	// 路径权重，此值越大，monster越注重短路径

	COLORREF color;
	bool path_show;
	int cnt;

public:
	CAStar() :s(0), e(0), w_inst(1), w_path(1), path_show(0), color(RED), cnt(0) {}

	void SetNodeWall(int r,int c)
	{
		if (r < 0 || r >= static_cast<int>(Map::ROW) || c < 0 || c >= static_cast<int>(Map::COLUMN))
			return;
		int k = r * static_cast<int>(Map::COLUMN) + c;
		ns[k].close = 1;
	}

	void SwitchPathShow(COLORREF c)
	{
		if (path_show)
			path_show = 0;
		else
			path_show = 1;

		color = c;
	}

	void SetStyle(int instance, int path)
	{
		w_inst = instance;
		w_path = path;
	}

	// 初始化
	void init(int* map, CPoint sp, CPoint ep)
	{
		open_list.clear();
		
		e = ep.x * static_cast<int>(Map::COLUMN) + ep.y;
		s = sp.x * static_cast<int>(Map::COLUMN) + sp.y;

		for (int i = 0; i < static_cast<int>(Map::CNT); i++)
		{
			ns[i].close = (map[i] == 3) ? 1 : 0;
			ns[i].open = 0;
			ns[i].G = 0;
			
			int dk = abs(i - e);
			ns[i].H = (dk / static_cast<int>(Map::COLUMN) + dk % static_cast<int>(Map::COLUMN)) * w_inst;

			ns[i].father = 0;
		}
	}

	int FindMinF()
	{
		int i_min = 0;
		int k_min = 0;
		int F_min = 1000000;

		int n = open_list.size();
		for (int i = 0; i < n; i++)
		{
			int k = open_list[i];
			int F = ns[k].G + ns[k].H;
			if (F < F_min)
			{
				F_min = F;
				k_min = k;
				i_min = i;
			}
		}

		if (k_min != 0)
		{
			open_list[i_min] = open_list[n - 1];
			open_list.pop_back();
		}

		return k_min;
	}

	void BuildAWay()
	{
		open_list.push_back(s); // 添加第一个结点
		ns[s].open = 1;

		int k_current = s;	// 设置起点

		while (true)
		{
			if (k_current == e)
				break;

			if (open_list.size() == 0)
				break;

			// 查询下一个结点
			k_current = FindMinF();
			ns[k_current].close = 1;


			// 搜寻周围结点
			for (int i = 0; i < 4; i++)
			{
				int ni = k_current / static_cast<int>(Map::COLUMN);
				int nj = k_current % static_cast<int>(Map::COLUMN);

				switch (i)
				{
				case 0: ni--; break;
				case 1: ni++; break;
				case 2: nj--; break;
				case 3: nj++; break;
				}

				if (ni < 0 || ni >= static_cast<int>(Map::ROW) || nj < 0 || nj >= static_cast<int>(Map::COLUMN))
					continue;

				int k_next = ni * static_cast<int>(Map::COLUMN) + nj;

				if (!ns[k_next].close)			// 没加入close （ 墙，或者探查完毕的结点 ）
				{
					int G_old = ns[k_next].G;
					int G_new = ns[k_current].G + w_path;

					if (ns[k_next].open)		// 已经加入了open
					{
						// 优化路径
						if (G_old > G_new )	
						{
							ns[k_next].G = G_new;
							ns[k_next].father = k_current;
						}
					}
					else
					{
						ns[k_next].G = G_new;
						ns[k_next].father = k_current;

						open_list.push_back(k_next);			// 添加进open_list
						ns[k_next].open = 1;
					}
				}

			}
		}
	}


	// 获得方向
	Dir GetDir()
	{
		if (path_show)
		{
			cnt++;
			if (cnt > 5)
				cnt = 0;

			if (cnt == 0)
				setlinecolor(BLACK);
			else
				setlinecolor(color);
		}
		

		int k[2] = { 0 };
		
		k[0] = e;

		while (1)
		{
			k[1] = ns[k[0]].father;

			if (k[1] == 0 || k[1] == s)
				break;


			if (path_show)
			{
				int xi = k[0] / static_cast<int>(Map::COLUMN) * static_cast<int>(Size::BLOCK) + static_cast<int>(Size::BLOCK) / 2;
				int xj = k[0] % static_cast<int>(Map::COLUMN) * static_cast<int>(Size::BLOCK) + static_cast<int>(Size::BLOCK) / 2;
				int yi = k[1] / static_cast<int>(Map::COLUMN) * static_cast<int>(Size::BLOCK) + static_cast<int>(Size::BLOCK) / 2;
				int yj = k[1] % static_cast<int>(Map::COLUMN) * static_cast<int>(Size::BLOCK) + static_cast<int>(Size::BLOCK) / 2;
				line(xj, xi, yj, yi);
			}


			k[0] = k[1];
	
		}

		if (k[0] - k[1] == static_cast<int>(Map::COLUMN))	return Dir::DOWN;
		if (k[0] - k[1] == -static_cast<int>(Map::COLUMN))	return Dir::UP;
		if (k[0] - k[1] == -1)			return Dir::LEFT;
		if (k[0] - k[1] == 1)			return Dir::RIGHT;

		return Dir::NOME;
	}

};