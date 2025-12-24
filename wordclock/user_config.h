/* Grid rows
 * 
 * Options
 * -------
 * 10: 10 rows
 * 11: 11 rows
 */
#define GRID_ROWS 10

/* Grid cols
 * 
 * Options
 * ------- 
 * 11: 11 cols
 */
#define GRID_COLS 11

/* Postition of first LED  
 *   
 * Options
 * -------
 * 0: top-left
 * 1: top-right
 * 2: buttom-left
 * 3: buttom-right
 */
#define GRID_FIRST 3

/* Grid language
 *  
 * Options
 * -------
 * 1: de_DE:     german
 * 2: de_DE_alt: german alternative
 * 3: de_DE_s: "Es ist" only at full and half hour
 * 4: de_DE_s1: "Es ist" only at full and half hour + Minutes in Last row (new front Plate De_de_s1)
 */
#define GRID_LANGUAGE de_DE

/* Position of single minute LEDs
 *  
 * Options
 * -------
 * 0: before
 * 1: after
 */
#define GRID_SINGLE_MINUTES 1

/* Data PIN, where the LED strip is connected to */
#define DATA_PIN D3
#define FASTLED_INTERNAL

/* we have ambilight like illumination around the frame*/
#define IS_HALO true

/* Data PIN, where the "HALO" led strip is connected to */
#define HALO_PIN D2

/* Number of leds in "HALO" led strip */
#define NUM_HALOLEDS 40
