#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

const int inf = 1 << 30; 

map<vector<int>, int> memory;

vector<int> result(vector<int>state, vector<int> move);
int maxValue(vector<int> state, int alpha, int beta, int turn);
vector<int> alphaBetaSearch(vector<int> state);
int minValue(vector<int> state, int alpha, int beta, int turn);
bool isGameOver(vector<int> state);
bool isInvalid(vector<int> state);
void print(vector<int> state);

int main() {
	int n, temp, turn = -1;
	char c;
	vector<int> stacks;

	//初始化堆数和每堆牌数
	cout << "Please input the total number of stacks: ";
	cin >> n;
	cout << "Please input the size of each stack: ";
	for (int i = 0; i < n; i++) {
		cin >> temp;
		stacks.push_back(temp);
	}

	//初始化谁是先手
	while (turn == -1) {
		cout << "You are the first one to select ? (Y/N):";
		cin >> c;
		if (c == 'Y' || c == 'y') {
			turn = 1;
		}
		else if (c == 'N' || c == 'n') {
			turn = 0;
		}
		else {
			cout << "Invalid character. Please input again." << endl;
		}
	}

	//当游戏没有结束
	while (!isGameOver(stacks)) {
		//轮到玩家
		if (turn == 1) {
			print(stacks);
			cout << "Please input your move(stack, number): ";
			int t1, t2;
			cin >> t1 >> t2;
			vector<int> move;
			move.push_back(t1); move.push_back(t2);
			if (isInvalid(result(stacks, move))) {
				cout << "Invalid move. Please try again." << endl;
			}
			else {
				stacks = result(stacks, move);
				turn = 1 - turn; //turn =0轮到电脑
			}
		}
		//轮到电脑 turn==0
		else {
			print(stacks);
			vector<int> move = alphaBetaSearch(stacks);
			cout << "The computer's move :";
			cout << move[0] << ' ' << move[1] << endl;
			stacks = result(stacks, move);
			turn = 1 - turn; //turn =1轮到玩家
		}
	}

	//游戏结束
	//如果电脑走完到了结束状态(玩家必拿最后一张)，玩家输
	if (turn == 1) {
		cout << "Oh, you lose, Maybe next time!" << endl;
	}
	else {
		cout << "Congratulations!" << endl;
		cout << "Winner Winner Chicken Dinner!" << endl;
	}

	system("pause");
	return 0;
}

//输出当前状态
void print(vector<int> state) {
	cout << "The current state is ";
	for (int i = 0; i < state.size() - 1; i++)
		cout << state[i] << ' ';
	cout << state[state.size() - 1] << endl; 
}

//判断胜利条件
bool isGameOver(vector<int> state) {
	int sum = accumulate(state.begin(), state.end(), 0);
	if (sum == 1)
		return true;
	else
		return false;
}

//判断输入违法
bool isInvalid(vector<int> state) {
	int sum = accumulate(state.begin(), state.end(), 0);
	if (sum == 0)
		return true;
	for (int i = 0; i < state.size(); i++)
		if (state[i] < 0)
			return true;
	return false;
}

//执行玩家的移动操作
vector<int> result(vector<int>state, vector<int> move) {
	vector<int> res;
	for (int i = 0; i < state.size(); i++)
	{
		if (i == move[0])
			res.push_back(state[i] - move[1]);
		else
			res.push_back(state[i]);
	}
	return res;
}

//轮到电脑时
//α-βSearch 
vector<int> alphaBetaSearch(vector<int> state) {
	int value = maxValue(state, -inf, inf, 1);
	for (int i = 0; i < state.size(); i++)
		for (int j = state[i]; j >= 1; j--) {
			vector<int> move;
			move.push_back(i); move.push_back(j);
			vector<int> newState = result(state, move);
			if (isInvalid(newState))
				continue;
			if (minValue(newState, -inf, inf, 0) == value)
				return move;
		}
}


int maxValue(vector<int> state, int alpha, int beta, int turn) {
	vector<int> key(state);
	key.push_back(turn);

	int sum = accumulate(state.begin(), state.end(), 0);
	if (sum == 1) { memory[key] = -1; return -1; }
	if (memory.find(key) != memory.end())
		return  memory[key];

	int v = -inf;
	for (int i = 0; i < state.size(); i++) {
		for (int j = state[i]; j >= 1; j--) {
			vector<int> move;
			move.push_back(i); move.push_back(j);
			vector<int> newState = result(state, move);
			if (isInvalid(newState))
				continue;
			v = max(v, minValue(newState, alpha, beta, 1 - turn));
			if (v >= beta) {
				return v;
			}
			alpha = max(alpha, v);
		}
	}
	memory[key] = v;
	return v;
}

int minValue(vector<int> state, int alpha, int beta, int turn) {

	vector<int> key(state);
	key.push_back(turn);

	int sum = accumulate(state.begin(), state.end(), 0);
	if (sum == 1) {
		memory[key] = 1;
		return 1;
	}

	if (memory.find(key) != memory.end())
		return  memory[key];

	int v = inf;
	for (int i = 0; i < state.size(); i++)
		for (int j = state[i]; j >= 1; j--) {
			vector<int> move;
			move.push_back(i); move.push_back(j);
			vector<int> newState = result(state, move);
			if (isInvalid(newState))
				continue;
			v = min(v, maxValue(newState, alpha, beta, 1 - turn));
			if (v <= alpha) {
				return v;
			}
			beta = min(beta, v);
		}
	memory[key] = v;
	return v;
}