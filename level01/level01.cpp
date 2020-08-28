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

typedef void(*LevelHandler)(UINT &start_time, UINT &end_time);

struct game_tick {
	UINT hour;
	UINT minute;
	UINT second;
};

struct level {
	UINT round_time; // sec
	UINT qst_num;
	UINT qst_time; // ms
	UINT answ_time; // ms
	std::string name;
	UINT user_correct_answ_count;
};

level s_game_levels[]{
	{ 60,  3, 5000, 8000, "level 01", 0 },
	{ 30,  3, 5000, 8000, "level 02", 0 },
	{ 15,  3, 5000, 8000, "level 03", 0 },
	{  5,  3, 5000, 8000, "level 04", 0 },
	{  1,  3, 5000, 8000, "level 05", 0 },
	{ 0, 0, 0, 0, "", 0 }
};

std::string SecToString(UINT time) {
	UINT hours = time / 60 / 60;
	UINT min = (time - (hours * 60 * 60)) / 60;
	UINT sec = time - (hours * 60 * 60) - (min * 60);
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

game_tick RoundTime(game_tick &time, UINT round) {
	if (round == 60) {
		time.minute = 0;
	}
	else {
		time.minute -= time.minute % round;
		time.minute = time.minute ? time.minute : round;
	}
	return time;
}

bool AnswerCheck(const struct game_tick *answers, const game_tick &to_check) {
	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		if (answers[i].hour == to_check.hour && answers[i].minute == to_check.minute) {
			return true;
		}
	}
	return false;
}

struct game_tick FakeAnswer(const game_tick &correct_answer, UINT strategi, struct game_tick *answers, level *l) {
	game_tick ret = correct_answer;
	switch (strategi) {
	case 0:
	case 1: {
		game_tick t = {};
		do {
			t.hour = rand() % 24;
			t.minute = rand() % 60;
			t = RoundTime(t, l->round_time);
		} while (AnswerCheck(answers, t));
		return t;
	}
	case 2: { // + 1 hour strategi
		int k = 0;
		k = correct_answer.hour < 24 ? k += 1 : k -= 1;
		ret.hour += k;
		return ret;
	}
	case 3: { // - 1 hour strategi
		int k = 0;
		k = correct_answer.hour > 1 ? k -= 1 : k += 1;
		ret.hour += k;
		return ret;
	}
	default: {
		printf("ERROR, wrong strategi for fake answer");
		exit(-1);
	}
	}
}

void PrepareAnsw(const game_tick &start_time, const game_tick &end_time, UINT correct_answ_indx, struct game_tick *answers, level *l) {
	game_tick correct_answ = {};
	UINT end = end_time.hour * 60 + end_time.minute;
	UINT start = start_time.hour * 60 + start_time.minute;
	correct_answ.hour = (end - start) / 60;
	correct_answ.minute = (end - start) - (correct_answ.hour * 60);

	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		if (correct_answ_indx == i) {
			answers[i] = correct_answ;
		}
		else {
			answers[i] = FakeAnswer(correct_answ, rand() % (ANSWERS_NUMBER - i), answers, l);
		}
	}
}

UINT PrintAnsw(const game_tick &start_time, const game_tick &end_time, level *l) {
	struct game_tick answers[ANSWERS_NUMBER] = {};
	UINT correct_answ_indx = rand() % ANSWERS_NUMBER;
	PrepareAnsw(start_time, end_time, correct_answ_indx, answers, l);

	std::string str;
	for (int i = 0; i < ANSWERS_NUMBER; i++) {
		str += std::to_string(i + 1);
		str += ") ";
		str += std::to_string(answers[i].hour) + ":" + std::to_string(answers[i].minute);
		//if (i == correct_answ_indx) {
		//	str += "<";
		//}
		str += "\n";
	}
	std::cout << str << "\n";
	return correct_answ_indx;
}

void Level(game_tick &start_time, game_tick &end_time, level *l) {
	start_time.hour = rand() % (24 / 2);
	start_time.minute = rand() % 60;
	start_time = RoundTime(start_time, l->round_time);
	end_time.hour = start_time.hour + rand() % (23 - start_time.hour);
	end_time.minute = rand() % 60;
	end_time = RoundTime(end_time, l->round_time);
}

void PrintLine() {
	std::cout << "==========================================\n";
}

void PrintQst(const game_tick &start_time, const game_tick &end_time) {
	std::cout << std::to_string(start_time.hour) << ":" << std::to_string(start_time.minute) << "\n";
	std::cout << std::to_string(end_time.hour) << ":" << std::to_string(end_time.minute) << "\n";
	std::cout << "\n";
	std::cout << "Press SPACE to speedup\n";
}

void PrintHead(std::string level_name) {
	std::cout << "================Time Keeper===============\n";
	std::cout << "================ " << level_name << " ================\n";
}

void SystemPlatformInit() {
	// windows
	srand((UINT)time(0));
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
void SystemSleep(UINT timeout) {
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

	game_tick start_time = {};
	game_tick end_time = {};

	auto level = s_game_levels;
	UINT correct_answ = 0;
	do {
		for (UINT i = 0; i < level->qst_num; i++) {
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
