#include "rtc.h"
#include "../x86/asm.h"

int read_cmos(int reg) {
	outb(0x70,reg);
	return inb(0x71);
}
time tim;

void updtime() {
	int second = read_cmos(0x00);
	int minute = read_cmos(0x02);
	int hour = read_cmos(0x04);
	int weekday = read_cmos(0x06);
	int day = read_cmos(0x07);
	int month = read_cmos(0x08);
	int year = read_cmos(0x09);
	int century = read_cmos(0x32);

	if(!(read_cmos(0x0B)&0x04)) {            
		second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
        century = (century & 0x0F) + ((century / 16) * 10);
	}

	if (!(read_cmos(0x0B) & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }

    tim.seconds = second;
    tim.minutes = minute;
    tim.hours = hour;
    tim.weekday = weekday;
    tim.day_of_month = day;
    tim.month = month;
    tim.year = year;
    tim.century = century;
}

time gettime() {
	return tim;
}