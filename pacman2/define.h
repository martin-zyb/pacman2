#pragma once
// 定义了二维空间的 point类 和 用于标注区域的 rect类
#include <graphics.h>
#include <conio.h>

#include <ctime>
#include <vector>
#include <string>

using namespace std;


enum class Dir : short	// 方向定义
{
	NOME = 0, UP = -1, DOWN = 1, LEFT = -2, RIGHT = 2
};

enum class Size : short
{
	POINT = 1,		// 豆子大小
	BIG_POINT = 3,	// 大力丸大小
	BLOCK = 20,		// 方格大小
	PERSON = 30		// 人物大小
};

enum class Status : short
{
	GAMING, END, MENU
};

enum class Tolerence : short
{
	MOVE = 5,		// 转弯容限
	EAT = 8,		// 吃豆容限
	FIGHT = 12		// 战斗容限
};

enum class Speed : short	// 速度
{
	MONSTER_SLOW = 15,	// 怪物慢速度 * 10
	MONSTER_FAST = 25,	// 怪物快速度 * 10
	PACMAN = 30			// 人物速度 * 10
};

enum class Map : short
{
	COLUMN = 21,		// 地图方格列数
	ROW = 27,			// 地图方格行数
	CNT = COLUMN * ROW	// 地图方格总数
};

enum class Time : short
{
	EAT_FLASH = 5,
	MONSTER_FEAR = 600		// 怪物恐惧时长
};

constexpr auto BEAN_NUM = 195;	// 豆子总数(含大力丸)

constexpr auto FPS = 60;

#define GAME_WIDTH  (static_cast<int>(Map::COLUMN) * static_cast<int>(Size::BLOCK))	// 游戏窗口宽度
#define GAME_HEIGHT (static_cast<int>(Map::ROW) * static_cast<int>(Size::BLOCK))	// 游戏窗口高度


class CPoint
{
public:
	int x;
	int y;
	CPoint() :x(0), y(0) {};
	CPoint(int x, int y) :x(x), y(y) {}
};

class CRect
{
public:
	CPoint site;		// 左上角坐标 = [行，列]
	CPoint shape;		// 区域大小   = [高，宽]
	CRect() :site(CPoint()), shape(CPoint()) {}
	CRect(int x, int y, int h, int w) :site(CPoint(x, y)), shape(CPoint(h, w)) {}
};