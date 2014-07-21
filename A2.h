/* Quoc Lien (816097211)
*  Masc1434
*  CS570, Summer 2014
*  Assignment #2, Page Memory Management
*  A2.h 
*/

#ifndef A2_H
#define A2_H

#define DATA_SIZE 100 // Defined size for creating file of random numbers.
#define RANGE 100

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h> 
#include <time.h>
#include <vector>

using namespace std;

class A2 {    
public:
    A2(string frame_size); /*Default Constructor*/
    /* Optimal Page Replacement Algorithm */
    void opt();
    /* Least Recently Used Page Replacement Algorithm */
    void lru();
    /* Clock Page Replacement Algorithm */
    void clock();
    /* This function prints the current frame if the page stack */
    void print_frame();
    /* This prints the list of Random numbers in the file. */
    void print_list();
    /* Creates a file of randomly generated numbers between 0-99 */
    void create();

private:
	int frame_size, faults;
	string f_name;
	ofstream ofs;

	/* Initializes frames with default values */
	void init_frames();
	/* Builds a linked list using the current data structure */
	/* Instantiates pointers to next positions */
	void init_linkedlists();
	/* Reads the text file, and parses the data from the text in to an dynamic array */
	void read_file();
	/* Parses the a given string into a vector string array */
	void parse_data(string data);
	/* Sorting for LRU stack, bubbles down */
	void sort(int n);
	/* Converts string to int. */
	int string_to_int(string n);
	/* Searches data structure for string, Returns current position if located, else return -1 if its not in the array */
	int search(string n);
	/* Calculates displacement for OPT algorithm. */
	int calculate_disp(int current, string n);

	struct Page {
		string p_number;
		int use;
		Page *next;
	};

	Page pages;
	vector<Page> v_pages;
	vector<Page> frames;
	vector<Page>::iterator f_iter;
};

#endif
