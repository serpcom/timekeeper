// level01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <ctime>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <windows.h>


#define ANSWERS_NUMBER 4

#define SEC_PER_DAY 24 * 60 * 60
#define SEC_PER_HALF_DAY 24 * 60 * 60 / 2

#define ONE_HOUR_SEC 60 * 60

typedef void(*LevelHandler)(unsigned int &start_time, unsigned int &end_time);

struct level {
	unsigned int round_time; // sec
	unsigned int qst_num;
	unsigned int qst_time; // ms
	unsigned int answ_time; // ms
	std::string name;
	unsigned int user_correct_answ_count;
};

level s_game_levels[]{
	{ 60 * 60,  3, 5000, 8000, "level 01", 0 },
	{ 30 * 60,  3, 5000, 8000, "level 02", 0 },
	{ 15 * 60,  3, 5000, 8000, "level 03", 0 },
	{  5 * 60,  3, 5000, 8000, "level 04", 0 },
	{  1 * 60,  3, 5000, 8000, "level 05", 0 },
	{ 0, 0, 0, 0, "", 0 }
};

std::string SecToString(unsigned int time) {
	unsigned int hours = time / 60 / 60;
	unsigned int min = (time - (hours * 60 * 60)) / 60;
	unsigned int sec = time - (hours * 60 * 60) - (min * 60);
	std::string str;

	if (hours < 10) {
		str += "0";
	}
	str += std::to_string(hours);
	str += ":";
	if (min < 10) {
		str += "0";
	}
	str += std::to_string(min);
	//str += ":";
	//if (sec < 10) {
	//	str += "0";
	//}
	//str += std::to_string(sec);
	return str;
}

unsigned int RoundTime(unsigned int time, unsigned int round) {
	return  time - (time % round);
}

bool AnswerCheck(unsigned int *answers, const unsigned int to_check) {
	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		if (answers[i] == to_check) {
			return true;
		}
	}
	return false;
}

unsigned int FakeAnswer(const unsigned int time, unsigned int strategi, unsigned int *answers, level *l) {
	switch (strategi) {
	case 0:
	case 1: {
		unsigned int t = 0;
		do {
			t = RoundTime(rand() % SEC_PER_DAY, l->round_time);
		} while (AnswerCheck(answers, t));
		return t;
	}
	case 2: { // + 1 hour strategi
		int k = 0;
		k = time < SEC_PER_DAY - ONE_HOUR_SEC ? k += ONE_HOUR_SEC : k -= ONE_HOUR_SEC;
		return time + k;
	}
	case 3: { // - 1 hour strategi
		int k = 0;
		k = time < ONE_HOUR_SEC ? k += ONE_HOUR_SEC : k -= ONE_HOUR_SEC;
		return time + k;
	}
	default: {
		printf("ERROR, wrong strategi for fake answer");
		exit(-1);
	}
	}
}

void PrepareAnsw(const unsigned int correct_answ, unsigned int correct_answ_indx, unsigned int *answers, level *l) {
	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		if (correct_answ_indx == i) {
			answers[i] = correct_answ;
		}
		else {
			answers[i] = FakeAnswer(correct_answ, i, answers, l);
		}
	}
}

unsigned int PrintAnsw(const unsigned int start_time, const unsigned int end_time, level *l) {
	unsigned int answers[ANSWERS_NUMBER] = {};
	unsigned int correct_answ_indx = rand() % ANSWERS_NUMBER;
	PrepareAnsw(end_time - start_time, correct_answ_indx, answers, l);

	std::string str;

	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		str += std::to_string(i + 1);
		str += ") ";
		str += SecToString(answers[i]);
		//if (i == correct_answ_indx) {
		//	str += "<";
		//}
		str += "\n";
	}
	std::cout << str << "\n";
	return correct_answ_indx;
}

void Level(unsigned int &start_time, unsigned int &end_time, level *l) {
	start_time = rand() % (SEC_PER_DAY / 2);
	start_time = RoundTime(start_time, l->round_time);
	do {
		end_time = start_time + rand() % (SEC_PER_DAY - start_time);
		end_time = RoundTime(end_time, l->round_time);
	} while (end_time == start_time);
}

void PrintLine() {
	std::cout << "==========================================\n";
}

void PrintQst(const unsigned int start_time, const unsigned int end_time) {
	std::cout << SecToString(start_time) << "\n";
	std::cout << SecToString(end_time) << "\n";
	std::cout << "\n";
	std::cout << "Press SPACE to speedup\n";
}

void PrintHead(std::string level_name) {
	std::cout << "================Time Keeper===============\n";
	std::cout << "================ " << level_name << " ================\n";
}

void SystemPlatformInit() {
	// windows
	srand((unsigned int)time(0));
}

int SystemGetKey()
{
	for (int i = 49; i <= 53; i++)
	{
		if (GetAsyncKeyState(i) & 0x7FFF)
		{
			return i;
		}
	}
	return 0;
}

int SystemWaitSpaceKey() {
	if (GetAsyncKeyState(0x20) & 0x7FFF)
	{
		return true;
	}
	return false;
}

// ms
void SystemSleep(unsigned int timeout) {
	std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

void PrintStatistic(struct level *l) {
	PrintHead(l->name);
	printf("Level questons num: %d\n", l->qst_num);
	printf("Level correct answers: %d\n", l->user_correct_answ_count);
	printf("Wrong answers: %d\n", l->qst_num - l->user_correct_answ_count);
	PrintLine();
}

void SystemClearKeyboardBuffer() {
	LONG start_time_ms = GetTickCount();
	do {
		SystemGetKey();
	} while (GetTickCount() - start_time_ms < 100);
}

int main()
{
	SystemPlatformInit();

	unsigned int start_time = 0;
	unsigned int end_time = 0;

	auto level = s_game_levels;
	unsigned int correct_answ = 0;
	do {
		for (unsigned int i = 0; i < level->qst_num; i++) {
			Level(start_time, end_time, level);
			SystemClearKeyboardBuffer();
			PrintHead(level->name);
			PrintQst(start_time, end_time);
			LONG start_time_ms = GetTickCount();
			int key = 0;
			do {
				if (SystemWaitSpaceKey()) {
					break;
				}
			} while (GetTickCount() - start_time_ms < (LONG)level->qst_time);
			system("CLS");

			SystemClearKeyboardBuffer();
			PrintHead(level->name);
			correct_answ = PrintAnsw(start_time, end_time, level);

			start_time_ms = GetTickCount();
			key = 0;
			do {
				key = SystemGetKey();
				if (key) {
					break;
				}
			} while (GetTickCount() - start_time_ms < (LONG)level->answ_time);

			int user_answ = key - 49;
			if (correct_answ == user_answ) {
				level->user_correct_answ_count++;
				std::cout << "Yes!\n";
			}
			else { std::cout << "No!\n"; }
			PrintLine();
			SystemSleep(500);
			system("CLS");
		}
		system("CLS");
		level++;
	} while (level->round_time != 0);

	// Total Statistics
	system("CLS");
	PrintLine();
	level = s_game_levels;
	do {
		PrintStatistic(level);
		level++;
	} while (level->round_time != 0);
	PrintLine();
}
