/* Quoc Lien (816097211)
*  Masc1434
*  CS570, Summer 2014
*  Assignment #2, Page Memory Management
*  A2.h 
*/

#include "A2.h"

A2::A2(string frame_size) {
	this->frame_size = string_to_int(frame_size);
	frames.reserve(this->frame_size);
	read_file();
}

/* Initializes blank frames to the stack */
void A2::init_frames() {
	pages.p_number = "";
	pages.use = 0;
	frames.clear();
	for(int i = 0; i < frame_size; i++) {
		frames.push_back(pages);
	}
}

void A2::init_linkedlists() {
	init_frames();
	for(int i = 0; i < frame_size; i++) {
		if(i != (frame_size - 1)) {
			frames[i].next = &frames[i+1]; // Establishing pointers to next frame.
		}
		else {
			frames[i].next = &frames[0]; // Point to the beginning of the list.
		}
	}
}

void A2::read_file() {
	string data;
	ifstream file;

	file.open("pages.txt");
	getline(file, data);
	file.close();

	parse_data(data);
}

void A2::parse_data(string data) {
	string token;
	stringstream s(data);

	/* Parse tokens using space as delimiter, then add the token to a vector */
	while(!s.eof()) {
		getline(s, token, ' ');
		pages.p_number = token;
		pages.use = 0;

		v_pages.push_back(pages);
	}
}

void A2::sort(int n) {
	if(n != 0) {
		/* Swaps the element at position n to the front of the list. */
		for(unsigned i = n - 1; i > 0; i--) {
			Page tmp = frames.at(i);
			frames.at(i) = frames.at(i - 1);
			frames.at(i - 1) = tmp;
		}
	}
}

int A2::string_to_int(string n) {
	stringstream s(n);
	int i;
	s >> i;
	return i;
}

/* Algorithms */
void A2::opt() {
	faults = 0;
	init_frames();
	int index = 0;
	int max = 0;
	cout << "Running Optimal Algorithm--" << endl;
	for(unsigned int i = 0; i < v_pages.size(); i++) {
		int m = search(v_pages[i].p_number);
		if(m == -1) {
			for(int n = 0; n < frame_size; n++) {
				int disp = calculate_disp(i, frames[n].p_number);
				if(max < disp) {
					max = disp; // Looking for highest displacement for page replacement.
					index = n; // Position of the highest displacement.
				}
			}
			frames[index] = v_pages[i];
			max = 0;
			faults++;
		}
	}
	cout << "Faults: " << faults << endl;
}

void A2::lru() {
	faults = 0;
	init_frames();
	cout << "Running Least Recent Used Algorithm--" << endl;
	for(unsigned int i = 0; i < v_pages.size(); i++) {
		int m = search(v_pages[i].p_number);
		if(m != -1) {
			Page tmp = frames.at(m);
			sort(m+1); // Bubble sort, moves the element to the front of the list if found.
			frames.at(0) = tmp;
		}
		else {
			sort(frame_size);
			frames.at(0) = v_pages[i];
			faults++;
		}
	}
	cout << "Faults: " << faults << endl;
}

void A2::clock() {
	faults = 0;
	init_linkedlists();
	cout << "Running Clock Algorithm--" << endl;
	Page * current = &frames[0];
	for(unsigned int i = 0; i < v_pages.size(); i++) {
		int m = search(v_pages[i].p_number); // Searches for the String in the data structure.
		// if search string is in the list, set its use bit to 1.
		if(m != -1) {
			while(current->p_number != v_pages[i].p_number) {
				current = current->next;
			}
			current->use = 1;
		}
		// Set use bit to 0, point to the next frame until the pointer to the frame's use bit is 0.
		// Swap the page, and point to next frame.
		else {
			while(current->use == 1) {
				current->use = 0;
				current = current->next;
			}
			current->p_number = v_pages[i].p_number;
			current->use = 1;
			current = current->next;
			faults++;
		}
	}
	cout << "Faults: " << faults << endl;
}

void A2::print_frame() {
	for(f_iter = frames.begin(); f_iter != frames.end(); ++f_iter) {
 		cout << f_iter->p_number << endl;
 	}
}

void A2::print_list() {
	for(unsigned int i = 0; i < v_pages.size(); i++) {
		cout << v_pages[i].p_number << " " << v_pages[i].use << endl;
	}
}

int A2::search(string n) {
	bool found = false;
	int index = 0;
	/* Iterate through the data structure looking for string n */
	for(f_iter = frames.begin(); f_iter != frames.end(); ++f_iter) {
		if(n == f_iter->p_number) {
			found = true;
			break;
		}
		index++;
	}
	if(found) {
		return index; // Returns the position of the found string. 
	}
	else {
		return -1;
	}
}

int A2::calculate_disp(int current, string n) {
	int disp = 0;
	for(unsigned int i = current; i < v_pages.size(); i++) {
		if(n != v_pages[i].p_number) {
			disp++;
		}
		else {
			break;
		}
	}
	return disp;
}

// Creates a file with the filename.
void A2::create() {
	int random;
	srand(time(NULL));

	if(!ofs.is_open()) {
		ofs.open("pages.txt");
		for(int i = 0; i < DATA_SIZE -1; i++) {
			random = rand() % RANGE;
			ofs << random << " ";
		}
		random = rand() % RANGE;
		ofs << random;
		ofs.close();
	}
	else {
		perror("Unable to open file.");
		exit(-3);
	}
}

int main(int argc, char* argv[]) {

	/* Checks for valid number of arguments passed in. */
	if(argc < 2) {
		perror("Invalid command line arguments! <Frame Size> ");
	}
	string frame_size = argv[1];

	A2 A2(frame_size);
	A2.lru();
	A2.clock();
	A2.opt();
}
