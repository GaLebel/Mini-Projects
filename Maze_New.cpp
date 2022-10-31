\\added some comments
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <algorithm>

using namespace std;
//Global Variables
const int x = 57, y = 25;

struct coord {
	int x;
	int y;
};


class Point {
	coord _Location;
	int _Free;
	bool _Visited;
	double _gScore;
	double _hScore;
	double _fScore;
	coord _Parent;
	int _x;
	int _y;
	int _n; // number of neighbours
	vector<coord> _Neighbours;

public:

	void move(int y, int x) {
		_x = x;
		_y = y;
	}

	int x() { return _x;  }
	int y() { return _y;  }

	void setLocation(int i, int j) {
		_Location.y = i;
		_Location.x = j;
	}

	coord getLocation() {
		return _Location;
	}

	vector<coord> getNeighbours() {
		return _Neighbours;
	}

	void addNeighbour(coord g) {
		_Neighbours.push_back(g);
	}


	Point() {
		_Free = false;
		_Visited = false;
		_gScore = FLT_MAX;
		_hScore = FLT_MAX;
		_fScore = FLT_MAX;
	}

	bool IsVisited() { return _Visited; }

	void Visited() { _Visited = true; }

	coord getParent() {
		return _Parent;
	}

	void setParent(int i, int j) {
		_Parent.y = i;
		_Parent.x = j;
	}

	double getF() { return _fScore; }
	double getH() { return _hScore; }
	double getG() { return _gScore; }

	void setF() { _fScore = _gScore + _hScore; }
	void setH(double l) { _hScore = l; }
	void setG(double l) { _gScore = l; }
	void updateF(double l) { _fScore = l; }

	void setFree(int a) {
		_Free = a;
	}

	int IsFree() {
		return _Free;
	}
};

vector<vector<Point>> FindNeighbours(int i, int j, vector<vector<Point>> maze) {
	int k = 0;
	//Check if we're at the most top row
	if (i > 0)
		if (maze[i - 1][j].IsFree() == 0) {
			coord g;
			g.y = i - 1;
			g.x = j;
			maze[i][j].addNeighbour(g);
			k++;
		}
	//Check if we're at the most left column
	if (j > 0) {
		if (maze[i][j - 1].IsFree() == 0) {
			coord g;
			g.y = i;
			g.x = j-1;
			maze[i][j].addNeighbour(g);
			k++;
		}
	}
	//Check if we're at the most bottom row
	if (i < y - 1) {
		if (maze[i + 1][j].IsFree() == 0) {
			coord g;
			g.y = i + 1;
			g.x = j;
			maze[i][j].addNeighbour(g);
			k++;
		}
	}
	//Check if we're at the most right column
	if (j < x - 1) {
		if (maze[i][j + 1].IsFree() == 0) {
			coord g;
			g.y = i;
			g.x = j+1;
			maze[i][j].addNeighbour(g);
			k++;
		}
	}
	return maze;
}

double calculateHValue(int row, int col, coord dest)
{
	// Return using the distance formula 
	return ((double)sqrt((row - dest.y) * (row - dest.y)
		+ (col - dest.y) * (col - dest.y)));
}

vector<vector<Point>> FirstScan(vector<vector<Point>> maze, coord dest) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++)
		{
			maze[i][j].setParent(0, 0);
			maze = FindNeighbours(i, j, maze);
			maze[i][j].setH(calculateHValue(i, j, dest));
		}
	}
	return maze;
}

void PrintMaze(vector<vector<Point>> vec) {
	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < unsigned(vec[i].size()); j++) {
			k = vec[i][j].IsFree();
			SetConsoleTextAttribute(hConsole, k);
			cout << "@";
		}
		k = 1;
		SetConsoleTextAttribute(hConsole, k);
		cout << endl;
	}
	k = 1;
	SetConsoleTextAttribute(hConsole, k);
	cout << endl;
	cout << endl;
}

vector<vector<Point>> InitializeMaze() {
	vector<vector<Point>> maze(y);
	for (int i = 0; i < y; i++) {
		maze[i] = vector<Point>(x);
		for (int j = 0; j < x; j++) {
			char c;
			cin >> c;
			if (c == 'X') {
				maze[i][j].setFree(231);
				maze[i][j].setLocation(i, j);
			}
			else {
				maze[i][j].setFree(0);
				maze[i][j].setLocation(i, j);
			}
		}

	}
	return maze;
}

vector<Point> UseLowest(vector<Point> openList) {
	if (openList.size() < 2)
		return openList;
	double f = openList[0].getF();
	cout << "f:" << f << endl;
	int lowest = 0;
	for (int i = 1; i < openList.size(); i++) {
		if (f > openList[i].getF()) {
			lowest = i;
			f = openList[i].getF();
		}
	}
	Point g = openList[lowest];
	openList.erase(openList.begin() + lowest);
	openList.push_back(g);
	return openList;
}


vector<vector<Point>> TracePath(vector<vector<Point>> maze, Point current, coord start) {
	coord c = current.getLocation();
	while (c.y != start.y || c.x != start.x) {
		maze[c.y][c.x].setFree(100);
	//	cout << "\033[2J\033[1;1H";
	//	PrintMaze(maze);
		coord l;
		l = maze[c.y][c.x].getParent();
		c = l;
	}
	maze[c.y][c.x].setFree(100);
	cout << "\033[2J\033[1;1H";
	PrintMaze(maze);
	return maze;
}
bool kek(Point a, Point b) {
	if (a.getF() > b.getF())
		return true;
}
void FindingAlgorithm(vector < vector<Point>> maze, coord start, coord dest) {
	vector<Point> openList;
	maze[start.y][start.x].setG(0);
	maze[start.y][start.x].setF();
	openList.push_back(maze[start.y][start.x]);
	while (openList.empty() == false) {
		//openList = UseLowest(openList);
		sort(openList.begin(), openList.end(), kek);
		Point current = openList[openList.size() - 1];
		openList.pop_back();
		current.Visited();
		coord c = current.getLocation();
		if (c.x == dest.x and c.y == dest.y) {
			maze = TracePath(maze, current, start);
			return;
		}
		//Proccessing the f(x) of all the neighbours of the current point
		vector<coord> neighbours = maze[c.y][c.x].getNeighbours();
		double lowest_f = 999;
		for (coord l : neighbours) {
			//cout << maze[c.y][c.x].getG() + 1 << " < " << maze[l.y][l.x].getG() << endl;
			if (maze[c.y][c.x].getG() + 1 < maze[l.y][l.x].getG()) {
				maze[l.y][l.x].setG(maze[c.y][c.x].getG() + 1);
				maze[l.y][l.x].setF();
				maze[l.y][l.x].setParent(c.y, c.x);
				Point takeit = maze[l.y][l.x];
				openList.push_back(takeit);
				if (lowest_f > maze[l.y][l.x].getF())
					lowest_f = maze[l.y][l.x].getF();
			}
		}
	}
}

int main() {
	vector<vector<Point>> maze(y);
	coord dest;
	dest.x = 55;
	dest.y = 23;
	coord start;
	start.x = 0;
	start.y = 0;
	maze = InitializeMaze();
	int old = maze[0][0].IsFree();
	PrintMaze(maze);
	vector<vector<Point>> backupmaze = maze;
	bool g = false;
	coord loc;
	loc.x = 0;
	loc.y = 0;
	while (g == false) {
		maze[loc.y][loc.x].setFree(150);
		cout << "\033[2J\033[1;1H";
		PrintMaze(maze);
		char c;
		c = _getch();
		if (c == 'd') {
			if (loc.x + 1 < x - 1) {
				maze[loc.y][loc.x].setFree(old);
				loc.x++;
				old = maze[loc.y][loc.x].IsFree();
				maze[loc.y][loc.x].setFree(150);

			}
		}
		if (c == 'w') {
			if (loc.y > 0) {
				maze[loc.y][loc.x].setFree(old);
				loc.y--;
				old = maze[loc.y][loc.x].IsFree();
				maze[loc.y][loc.x].setFree(150);

			}
		}
		if (c == 's') {
			if (loc.y < y - 1) {
				maze[loc.y][loc.x].setFree(old);
				loc.y++;
				old = maze[loc.y][loc.x].IsFree();
				maze[loc.y][loc.x].setFree(150);

			}
		}
		if (c == 'a'){
			if (loc.x > 0) {
				maze[loc.y][loc.x].setFree(old);
				loc.x--;
				old = maze[loc.y][loc.x].IsFree();
				maze[loc.y][loc.x].setFree(150);

			}
		}
		if (c == 32) {
			if (old == 231) {
				old = 0;
				maze[loc.y][loc.x].setFree(old);

			}
			else {
				if (old == 0) {
					old = 231;
					maze[loc.y][loc.x].setFree(old);

				}
			}
		}
		if (c == 'r') {
			maze = backupmaze;
		}
		if (c == 'g') {
			maze = FirstScan(maze, dest);
			FindingAlgorithm(maze, loc, dest);
			c = _getch();
		}
	}
	
}
