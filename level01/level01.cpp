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

#define LEVEL_00_ROUND_TIME 60
#define LEVEL_01_ROUND_TIME 30
#define LEVEL_02_ROUND_TIME 15

#define MINUTES_PER_DAY 24 * 60
#define MINUTES_PER_HALF_DAY 24 * 60 / 2

typedef void(*LevelHandler)(unsigned int &start_time, unsigned int &end_time);

struct level {
	LevelHandler handler;
	unsigned int repeat;
	unsigned int qst_time; // ms
	unsigned int answ_time; // ms
	std::string name;
};

// Levels 
void Level00(unsigned int &start_time, unsigned int &end_time);
void Level01(unsigned int &start_time, unsigned int &end_time);
void Level02(unsigned int &start_time, unsigned int &end_time);

level s_game_levels[]{
	{ Level00,  3, 2000, 10000, "level 00" },
	{ Level01,  3, 3000, 10000, "level 01" },
	{ Level02, 3, 2000, 10000, "level 02" },
	{ nullptr, 0, 0, 0, "" }
};

// Windows
HANDLE s_keyboard_handler;

std::string MinutesToString(unsigned int time) {
	unsigned int hours = time / 60;
	unsigned int min = time - (hours * 60);
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
			if (time < MINUTES_PER_DAY - 60) {
				k = 60;
			}
			else {
				k = -60;
			}
		}
		else {
			if (time > 60) {
				k = -60;
			}
			else {
				k = 60;
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
		if (i) {
			str += " ";
		}
		str += std::to_string(i + 1);
		str += ") ";
		if (i == correct_answ_indx) {
			// mark for correct answer
			//str += ">";
			str += MinutesToString(correct_answ);
		}
		else {
			str += MinutesToString(FakeAnswer(correct_answ, rand() % ANSWERS_NUMBER));
		}
	}

	std::cout << str << "\n";
	return correct_answ_indx;
}

void Level02(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (MINUTES_PER_DAY / 2);
	RoundTime(start_time, LEVEL_02_ROUND_TIME);
	do {
		end_time = start_time + rand() % (MINUTES_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_02_ROUND_TIME);
	} while (end_time == start_time);
}

void Level01(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (MINUTES_PER_HALF_DAY / 2);
	RoundTime(start_time, LEVEL_01_ROUND_TIME);
	do {
		end_time = start_time + rand() % (MINUTES_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_01_ROUND_TIME);
	} while (end_time == start_time);
}

void Level00(unsigned int &start_time, unsigned int &end_time) {
	start_time = rand() % (MINUTES_PER_HALF_DAY / 2);
	RoundTime(start_time, LEVEL_00_ROUND_TIME);
	do {
		end_time = start_time + rand() % (MINUTES_PER_HALF_DAY - start_time);
		RoundTime(end_time, LEVEL_00_ROUND_TIME);
	} while (end_time == start_time);
}

void PrintLine() {
	std::cout << "==========================================\n";
}

void PrintQst(const unsigned int start_time, const unsigned int end_time) {
	std::cout << MinutesToString(start_time) << "\n";
	std::cout << MinutesToString(end_time) << "\n";
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


int main()
{
	SystemPlatformInit();

	unsigned int start_time = 0;
	unsigned int end_time = 0;

	auto level = s_game_levels;
	unsigned int correct_answ = 0;
	unsigned int user_correct_answ = 0;
	unsigned int user_total_qest = 0;
	do {
		for (unsigned int i = 0; i < level->repeat; i++) {
			level->handler(start_time, end_time);
			PrintHead(level->name);
			PrintQst(start_time, end_time);
			SystemSleep(level->qst_time);
			user_total_qest++;
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
				user_correct_answ++;
				std::cout << "Yes!\n";
			}
			else { std::cout << "No!\n"; }
			PrintLine();			
			SystemSleep(500);
			system("CLS");
		}
		level++;
	} while (level->handler != nullptr);
	PrintLine();
	printf("Total questons: %d\n", user_total_qest);
	printf("Correct answers: %d\n", user_correct_answ);
	printf("Wrong answers: %d\n", user_total_qest - user_correct_answ);
	PrintLine();
}
