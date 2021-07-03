#ifndef TIME_H
#define TIME_H

typedef struct {
	int seconds;
	int minutes;
	int hours;

	int weekday;
	int day_of_month;
	int month;
	int year;
	int century;
} time;

char* getday(int day);
char* getmonth(int month);
time gettime();

#endif