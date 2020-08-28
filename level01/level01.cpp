// level01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <ctime>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <Windows.h>


#define ANSWERS_NUMBER 4

// sec
#define LEVEL_01_ROUND_TIME 60 * 60
#define LEVEL_02_ROUND_TIME 30 * 60
#define LEVEL_03_ROUND_TIME 15 * 60
#define LEVEL_04_ROUND_TIME 30
#define LEVEL_05_ROUND_TIME 15

#define SEC_PER_DAY 24 * 60 * 60
#define SEC_PER_HALF_DAY 24 * 60 * 60 / 2

#define ONE_HOUR_SEC 60 * 60

typedef void(*LevelHandler)(unsigned int &start_time, unsigned int &end_time);

struct level {
	LevelHandler handler;
	unsigned int qst_num;
	unsigned int qst_time; // ms
	unsigned int answ_time; // ms
	std::string name;
	unsigned int user_correct_answ;
};

// Levels 
void Level01(unsigned int &start_time, unsigned int &end_time);
void Level02(unsigned int &start_time, unsigned int &end_time);
void Level03(unsigned int &start_time, unsigned int &end_time);
void Level04(unsigned int &start_time, unsigned int &end_time);
void Level05(unsigned int &start_time, unsigned int &end_time);

level s_game_levels[]{
	{ Level01,  3, 5000, 10000, "level 01", 0 },
	{ Level02,  4, 4000,  8000, "level 02", 0 },
	{ Level03,  6, 3000,  4000, "level 03", 0 },
	{ Level04,  6, 2000,  4000, "level 04", 0 },
	{ Level05,  7, 1000,  2000, "level 05", 0 },
	{ nullptr, 0, 0, 0, "", 0 }
};

// Windows
HANDLE s_keyboard_handler;

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
	str += ":";
	if (sec < 10) {
		str += "0";
	}
	str += std::to_string(sec);
	return str;
}

void RoundTime(unsigned int &time, unsigned int round) {
	time = time - (time % round);
}

unsigned int FakeAnswer(const unsigned int time, unsigned int strategi) {
	switch (strategi) {
	case 0:
	case 1:
	case 2: {

	}
	case 3: { // +/- 1 hour strategi
		int k = 0;
		if (rand() % 2) {
			if (time < SEC_PER_DAY - ONE_HOUR_SEC) {
				k = ONE_HOUR_SEC;
			}
			else {
				k = -ONE_HOUR_SEC;
			}
		}
		else {
			if (time > ONE_HOUR_SEC) {
				k = -ONE_HOUR_SEC;
			}
			else {
				k = ONE_HOUR_SEC;
			}
		}
		return time + k;
	}
	default: {
		printf("ERROR, wrong strategi for fake answer");
		exit(-1);
	}
	}
}

unsigned int PrintAnsw(const unsigned int start_time, const unsigned int end_time) {
	unsigned int correct_answ_indx = rand() % ANSWERS_NUMBER;
	unsigned int correct_answ = end_time - start_time;
	std::string str;

	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		//if (i) {
		//	str += " ";
		//}
		str += std::to_string(i + 1);
		str += ") ";
		if (i == correct_answ_indx) {
			// mark for correct answer
			//str += ">";
			str += SecToString(correct_answ);
			str += "\n";
		}
		else {
			str += SecToString(FakeAnswer(correct_answ, rand() % ANSWERS_NUMBER));
			str += "\n";
		}
	}

	std::cout << str << "\n";
	return correct_answ_indx;
}

void Level05(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (SEC_PER_DAY / 2);
	RoundTime(start_time, LEVEL_05_ROUND_TIME);
	do {
		end_time = start_time + rand() % (SEC_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_05_ROUND_TIME);
	} while (end_time == start_time);
}

void Level04(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (SEC_PER_DAY / 2);
	RoundTime(start_time, LEVEL_04_ROUND_TIME);
	do {
		end_time = start_time + rand() % (SEC_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_04_ROUND_TIME);
	} while (end_time == start_time);
}

void Level03(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (SEC_PER_DAY / 2);
	RoundTime(start_time, LEVEL_03_ROUND_TIME);
	do {
		end_time = start_time + rand() % (SEC_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_03_ROUND_TIME);
	} while (end_time == start_time);
}

void Level02(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (SEC_PER_HALF_DAY / 2);
	RoundTime(start_time, LEVEL_02_ROUND_TIME);
	do {
		end_time = start_time + rand() % (SEC_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_02_ROUND_TIME);
	} while (end_time == start_time);
}

void Level01(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (SEC_PER_HALF_DAY / 2);
	RoundTime(start_time, LEVEL_01_ROUND_TIME);
	do {
		end_time = start_time + rand() % (SEC_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_01_ROUND_TIME);
	} while (end_time == start_time);
}

void PrintLine() {
	std::cout << "==========================================\n";
}

void PrintQst(const unsigned int start_time, const unsigned int end_time) {
	std::cout << SecToString(start_time) << "\n";
	std::cout << SecToString(end_time) << "\n";
	std::cout << "\n";
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

// ms
void SystemSleep(unsigned int timeout) {
	std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

void PrintStatistic(struct level *l) {
	PrintHead(l->name);
	printf("Level questons num: %d\n", l->qst_num);
	printf("Level correct answers: %d\n", l->user_correct_answ);
	printf("Wrong answers: %d\n", l->qst_num - l->user_correct_answ);
	PrintLine();
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
			level->handler(start_time, end_time);
			PrintHead(level->name);
			PrintQst(start_time, end_time);
			SystemSleep(level->qst_time);
			system("CLS");
			
			PrintHead(level->name);
			correct_answ = PrintAnsw(start_time, end_time);

			LONG start_time_ms = GetTickCount();
			LONG end_time = 0;
			int key = 0;
			do
			{
				key = SystemGetKey();
				if (key) {
					break;
				}
			} while (GetTickCount() - start_time_ms < (LONG)level->answ_time);

			int user_answ = key - 49;
			if (correct_answ == user_answ) {
				level->user_correct_answ++;
				std::cout << "Yes!\n";
			}
			else { std::cout << "No!\n"; }
			PrintLine();
			SystemSleep(500);
			system("CLS");
		}
		// Levasl Statistics
		PrintStatistic(level);
		system("pause");
		system("CLS");
		level++;
	} while (level->handler != nullptr);

	// Total Statistics
	system("CLS");
	PrintLine();
	level = s_game_levels;
	do {
		PrintStatistic(level);
		level++;
	} while (level->handler != nullptr);
	PrintLine();
}
