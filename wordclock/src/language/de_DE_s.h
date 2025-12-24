#ifndef WORDCLOCK_GRID_LANG_de_DE_s_H
#define WORDCLOCK_GRID_LANG_de_DE_s_H

class Grid_de_DE_s {
    private:
    // These track the last time that was actually displayed on the LEDs
    static int last_displayed_minute_block; // Represents the 0-11 index for the word clock
    static int last_displayed_single_minute; // Represents the 0-4 single minute dots
	public:
	static int time_it_is[5];
	static int time_minutes[12][12];
	static int time_hours[12][6];

	static void setTime(int hour, int minute, bool force);

};

typedef class Grid_de_DE_s Grid;

#endif
