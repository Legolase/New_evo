#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class cell;

struct square;

class coordin {
private:
	int x, y;
public:
	coordin() : x(0), y(0) {}
	coordin(int a, int b) : x(a), y(b) {}
	int getx() const { return x; }
	int gety() const { return y; }
	void setcord(int a, int b) { x = a; y = b; };
	bool operator==(coordin a) const { return ((this->x == a.x) && (this->y == a.y)) ? true : false; }
	void operator=(coordin a) { this->x = a.x; this->y = a.y; }
	coordin operator+(coordin a) const {
		coordin b(this->x + a.x, this->y + a.y);
		return b;
	}
	coordin operator-(coordin a) const {
		coordin b(this->x - a.x, this->y - a.y);
		return b;
	}
	const bool operator<(const vector<vector<square>>& a) const {
		if (((this->x < (int)a.size()) && (this->y < a[0].size()) && (this->x > -1) && (this->y > -1)))
			return true;
		else
			return false;
	}
};

const coordin neighboor[] = { {1, 0}, {1,1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
const int variants_of_move = sizeof(neighboor) / sizeof(neighboor[0]);

const int cell_colour[] = { 1, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const int quan_cell_colour = sizeof(cell_colour) / sizeof(cell_colour[0]);

int year_cycle = 10, plants_year = 20, putrefaction_year = 40, mutation = 5, meat_energy = 60, plant_energy = 20;
const int type_of_reaction = 6, fill_start = 5;


int reproduct_energy = 70, max_age = 100;
const int act_price[] = { 3, 2, 5, 2, 1, reproduct_energy };
//int act_price[] = { 1, 2, 3, 4, 5, 6 };
const int _move[] = { 0, 1, 2, 3 }, meet[] = { 0, 1, 2, 5 }, food[] = { 0, 1, 2, 4 };
const int reaction_gens = 10, action_gens = 100;
const int eat_colour[] = { 10, 12, 6 };

const char meat = 177, bush = 206, empt = ' ', alive = 219;
const int col_meat = 4, col_plants = 2;

//===============================
//Files constants

const string name_file = "\\Best_genome_of_last_cell.txt";

//===============================

int rand(int left, int right) {
	mt19937 gen{ random_device()() };
	uniform_int_distribution<int> dist(left, right);
	return dist(gen);
}

void SetColour(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

void setcur(int x, int y) // вывод без мерцания
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct square {
	char c;
	int colour;

	square() : c(empt), colour(7) {}
	square(const char c1) : c(c1), colour(7) {}
	square(const char c1, const int colour1) : c(c1), colour(colour1) {}
	void set(const char c1, const int colour1) {
		c = c1;
		colour = colour1;
	}
};

class cell {
private:
	const int  actgens{ action_gens }, regen{ reaction_gens };
	int age, colour, previous_gen, previous_gen2, eat;
	coordin course, hit_from;
	bool bited;
	vector<vector<int>> TOAR;
	bool revative(cell& met) {
		int counter{};
		for (int i = 0; i < type_of_reaction; ++i)
			for (int j = 0; j < TOAR[0].size(); ++j)
				if (TOAR[i][j] != met.TOAR[i][j])
					++counter;
		if ((double(counter) / (type_of_reaction * TOAR[0].size())) * 100 < double(mutation) * 2)
			return true;
		else
			return false;
	}
public:
	int energy;
	coordin position;

	//cell& operator=(const cell& a);
	cell() : energy(100), age(0)/*, colour(cell_colour[rand(0, quan_cell_colour - 1)])*/, TOAR(type_of_reaction, vector<int>((variants_of_move* regen) + actgens, -1)),
		course(neighboor[rand(0, variants_of_move - 1)]), hit_from(0, 0), position(0, 0), bited(false), previous_gen(-1), previous_gen2(-1), eat(rand(0, 2)) {
		//colour = eat_colour[eat];
		for (int i = 0; i < type_of_reaction; ++i)
			for (int j = 0; j < TOAR[0].size(); ++j) {
				if (i < 2)
					TOAR[i][j] = _move[rand(0, (sizeof(_move) / sizeof(_move[0])) - i - 1)];
				else if (i < 4)
					TOAR[i][j] = meet[rand(0, (sizeof(meet) / sizeof(meet[0])) - 1)];
				else if (eat == 2)
					TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 1)];
				else if (i == 4)
					TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 1 - abs(eat - 1))];
				else if (i == 5)
					TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 1 - eat)];
				else {
					cout << "Error: genome building wasn\'t right";
					system("pause >nul");
					exit(0);
				}
					
			}
	}
	cell(cell *parent) : energy((*parent).energy / 2), age(0), course(neighboor[rand(0, variants_of_move - 1)]), hit_from( 0, 0 ), position(0, 0), bited(false), previous_gen(-1), previous_gen2(-1) {
		mt19937 gen{ random_device()() };
		uniform_int_distribution<int> dist(0, 99);
		
		/*if (dist(gen) < mutation)
			colour = cell_colour[rand(0, quan_cell_colour - 1)];
		else
			colour = parent.colour;*/

		if (dist(gen) < mutation)
			eat = rand(0, 2);
		else
			eat = (*parent).eat;

		colour = eat_colour[eat];

		TOAR.resize(type_of_reaction, vector<int>((variants_of_move * regen) + actgens, -1));
		for (int i = 0; i < type_of_reaction; ++i)
			for (int j = 0; j < TOAR[0].size(); ++j) {
				if (dist(gen) < mutation) {
					if (i < 2)
						TOAR[i][j] = _move[rand(0, (sizeof(_move) / sizeof(_move[0])) - i - 1)];
					else if (i < 4)
						TOAR[i][j] = meet[rand(0, (sizeof(meet) / sizeof(meet[0])) - 1)];
					else if (eat == 2)
						TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 1)];
					else if (i == 4)
						TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 1 - abs(eat - 1))];
					else if (i == 5)
						TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 1 - eat)];
					else {
						cout << "Error: genome building wasn\'t right";
						system("pause >nul");
						exit(0);
					}
				}
				else {
					if (i == 4 && eat == 0 && (*parent).TOAR[i][j] == 4)
						TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 2)];
					else if (i == 5 && eat == 1 && (*parent).TOAR[i][j] == 4)
						TOAR[i][j] = food[rand(0, (sizeof(food) / sizeof(food[0])) - 2)];
					else
						TOAR[i][j] = (*parent).TOAR[i][j];
				}
			}
		(*parent).energy = energy;
	}
	cell& operator=(const cell& a) {
		energy = a.energy;
		age = a.age;
		colour = a.colour;
		previous_gen = a.previous_gen;
		previous_gen2 = a.previous_gen2;
		course = a.course;
		hit_from = a.hit_from;
		bited = a.bited;
		TOAR = a.TOAR;
		position = a.position;
		eat = a.eat;
		return *this;
	}
	int ahead(const vector<vector<square>>& sqar, const vector<cell>& cells) {
		coordin step = position + course;
		if (step < sqar) {
			if (sqar[step.getx()][step.gety()].c == empt)
				return 0;
			else if (sqar[step.getx()][step.gety()].c == meat)
				return 4;
			else if (sqar[step.getx()][step.gety()].c == bush)
				return 5;
			else if (sqar[step.getx()][step.gety()].c == alive) {
				int i{};
				bool found{ false };
				for (i = 0; i < cells.size(); ++i)
					if (cells[i].position == step) {
						found = true;
						break;
					}
				if (!found) {
					cout << "Error: cell[" << step.getx() << "][" << step.gety() << "] wasn\'t found\n";
					system("pause >nul");
					exit(0);
				}
				if (this->relative(cells[i]))
					return 3;
				else
					return 2;
			}
		}
		return 1;
	}
	void turn_right() {
		int l = 0;
		for (size_t i = 0; i < variants_of_move; ++i) {
			if (neighboor[i] == this->course) {
				l = i;
				break;
			}
		}
		--l;
		if (l < 0)
			l += variants_of_move;
		course = neighboor[l];
	}
	void turn_left() {
		int l = 0;
		for (int i = 0; i < variants_of_move; ++i) {
			if (neighboor[i] == this->course) {
				l = i;
				break;
			}
		}
		++l;
		if (l >= variants_of_move)
			l -= variants_of_move;
		course = neighboor[l];
	}
	void increase_age() { ++age; }
	void bit(const cell& a) {
		hit_from = a.position - position;
		bited = true;
	}
	int gage() const { return age; }
	int gcolour() const { return colour; }
	int ggen() {
		if (++previous_gen > actgens - 1) {
			previous_gen = -1;
			return 0;
		}
		return previous_gen;
	}
	int ggen2() {
		if (++previous_gen2 > regen - 1) {
			previous_gen2 = -1;
			return 0;
		}
		return previous_gen2;
	}
	int geat() { return eat; }
	vector<vector<int>> gTOAR() const { return TOAR; }
	coordin gcourse() { return course; }
	coordin ghit_from() { return hit_from; }
	bool is_bited() {
		if (bited) {
			bited = false;
			return true;
		}
		return false;
	}
	bool relative(const cell& a) const {
		int counter{};
		for (int i = 0; i < type_of_reaction; ++i)
			for (int j = 0; j < TOAR[0].size(); ++j)
				if (this->TOAR[i][j] != a.TOAR[i][j])
					++counter;
		if ((double(counter) / ((int)TOAR.size() * TOAR[0].size())) * 100 <= double(mutation)) {
			return true;
		}
		return false;
	}
};

void work(vector<vector<square>>& place, vector<cell>& cells, int& i, int& act, int& way_b) {
	if (act) {
		coordin step = cells[i].position + cells[i].gcourse();
		if (act == 1) {
			cells[i].energy -= act_price[1];
			cells[i].turn_left();
			way_b = 1;
			coordin cor = cells[i].gcourse();
			if (abs(cor.getx()) > 1 || abs(cor.gety()) > 1) {
				cout << "Eeee";
				system("pause >nul");
			}
		}
		else if (act == 2) {
			cells[i].energy -= act_price[1];
			cells[i].turn_right();
			way_b = 2;
			coordin cor = cells[i].gcourse();
			if (abs(cor.getx()) > 1 || abs(cor.gety()) > 1) {
				cout << "Eeee";
				system("pause >nul");
			}
		}
		else if (act == 3) {
			if (step < place) {
				place[cells[i].position.getx()][cells[i].position.gety()].set(empt, 7);
				place[step.getx()][step.gety()].set(alive, cells[i].gcolour());
				cells[i].position = step;
			}
			cells[i].energy = cells[i].energy - act_price[0];
			way_b = 3;
		}
		else if (act == 4) {
			if (step < place) {
				place[cells[i].position.getx()][cells[i].position.gety()].set(empt, 7);
				if (place[step.getx()][step.gety()].c == bush)
					cells[i].energy = cells[i].energy + plant_energy;
				else if (place[step.getx()][step.gety()].c == meat)
					cells[i].energy = cells[i].energy + meat_energy;
				else {
					cout << "Error: unknown thing which is wanted to be eaten by cell!!!\n";
					system("pause >nul");
					exit(0);
				}
				place[step.getx()][step.gety()].set(alive, cells[i].gcolour());
				cells[i].position = step;
			}
			cells[i].energy = cells[i].energy - act_price[3];
			way_b = 4;
		}
		else if (act == 5) {
			int j;
			{
				bool found{ false };
				for (j = 0; j < cells.size(); ++j)
					if (step == cells[j].position) {
						found = true;
						break;
					}
				if (!found) {
					cout << "Error: not found bited cell!!!\n";
					system("pause >nul");
					exit(0);
				}
			}
			cells[i].energy = cells[i].energy - act_price[2];
			cells[j].energy = cells[j].energy - act_price[5];
			cells[j].bit(cells[i]);
			way_b = 5;
		}
		return;
	}
	cells[i].energy -= act_price[4];
	way_b = 6;
	return;
}

vector<vector<square>> fill(const int& xmax, const int& ymax, vector<cell>& cells) {
	mt19937 gen{ random_device()() };
	uniform_int_distribution<int> dist(0, 99);
	vector<vector<square>> place(xmax, vector<square>(ymax, empt));
	cells.clear();
	cells.shrink_to_fit();

	for (int i = 0; i < xmax; ++i) {
		for (int j = 0; j < ymax; ++j) {
			if (dist(gen) < fill_start) {
				cell a;
				a.position = { i, j };
				cells.push_back(a);
				place[i][j].set(alive, a.gcolour());
			}
		}
	}
	return place;
}

void show(const vector<vector<square>> & sqar, const int w, const int statistic[]) {
	const size_t outsize = (size_t)sqar.size(), insize = sqar[0].size();
	cout << char(201);
	for (size_t i = 0; i < outsize; ++i)
		cout << char(205);
	cout << char(187) << endl;

	for (int j = insize - 1; j > -1; --j) {
		SetColour(7, 0);
		cout << char(186);
		for (size_t i = 0; i < outsize; ++i) {
			SetColour(sqar[i][j].colour, 0);
			cout << sqar[i][j].c;
		}
		SetColour(7, 0);
		cout << char(186) << j << '\n';
	}
	cout << char(200);
	for (size_t i = 0; i < outsize; ++i)
		cout << char(205);
	cout << char(188) << "\n ";

	for (int i = 0; i < outsize; ++i)
		cout << (i / 10) % 10;
	cout << "\n ";

	for (int i = 0; i < outsize; ++i)
		cout << i % 10;
	cout << "\n " << w << " \n\n Herbivorous == " << statistic[0] << "       \n"
		<< " Predator == " << statistic[1] << "       \n"
		<< " Omnivorous == " << statistic[2] << "       \n";
}

bool isfile(string way_to_file) {
	ifstream f(way_to_file);
	if (!f)
		return false;
	return true;
}

int lastsign(string s, char symbol) { //index
	int index = -1;
	if (s.size() != 0) {
		for (int i = s.size() - 1; i >= 0; --i)
			if (s[i] == symbol) {
				index = i;
				break;
			}
	}
	return index;
}

void mkfile(string way_to_file, vector<string> lines) { //create a file with default lines in this
	string w = way_to_file.substr(0, lastsign(way_to_file, '\\'));
	fs::create_directories(w);
	ofstream f(way_to_file);
	if (lines.size() == 0)
		f << "";
	else {
		for (size_t i = 0; i < lines.size() - 1; ++i)
			f << lines[i] << endl;
		f << lines[lines.size() - 1];
	}
	f.close();
}

string chegin() {
	WCHAR buffer[MAX_PATH];
	string way = "";
	GetModuleFileName(NULL, buffer, sizeof(buffer) / sizeof(buffer[0]));
	for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); ++i)
		way += buffer[i];
	way = way.substr(0, lastsign(way, '\\')) + name_file;
	if (isfile(way) == false) { //creating file with standard matrix of game map
		string s = "";
		vector<string> v(1,s);
		mkfile(way, v);
	}
	else {}
	return way;
}

void show_genome_cell(const string way_to_file, cell& a) {
	const vector<vector<int>>& Toar = a.gTOAR();
	const int outsize = (int)Toar.size(), insize = Toar[0].size();
	ofstream f(way_to_file);
	int lines = variants_of_move;
	if (!f) {
		cout << "Error: can\'t open file \"" << way_to_file << "\"";
		system("pause >nul");
		f.close();
		exit(0);
	}

	f << "Eat: " << a.geat() << endl;
	for (int j = insize - 1; j > -1; --j) {
		for (int i = 0; i < outsize; ++i) {
			f << Toar[i][j] << '\t';
		}
		f << endl;
		if ((j % 10 == 0) && (lines > 0)) {
			f << "===================\n";
			--lines;
		}
	}
	f.close();
}

int main(int argc, int* argv[]) {
	mt19937 gen{ random_device()() };
	uniform_int_distribution<int> dist(0, 99);

	const string way_to_file = chegin();

	int outsize{ 40 }, insize{ 40 };
	int timee = 0;
	int year{ -1 };
	if (argc > 1) {
		outsize = *argv[0];
		insize = *argv[1];
	}

	while (_getch()) {
		vector<cell> cells;
		vector<vector<square>> place = fill(outsize, insize, cells);
		vector<int> died_cells;
		int statistic[] = {0, 0, 0};

		while (cells.size() > 0) {
			//cout << "========================================================\n";
			show(place, year, statistic);
			setcur(0, 0);
			Sleep(timee);
			if (++year > year_cycle) {
				year = -1;
				for (int i = 0; i < outsize; ++i)
					for (int j = 0; j < insize; ++j) {
						if (place[i][j].c == empt) {
							if (dist(gen) < plants_year)
								place[i][j].set(bush, col_plants);
						}
						else if (place[i][j].c == meat) {
							if (dist(gen) < putrefaction_year)
								place[i][j].set(empt, 7);
						}
					}
			}
			{
				statistic[0] = 0;
				statistic[1] = 0;
				statistic[2] = 0;
			}

			const int csize = cells.size();
			for (int i = 0; i < csize; ++i) {
				if (cells[i].gage() < max_age && cells[i].energy > 0) {
					coordin temp1 = cells[i].position, r;
					int way_b = 0;

					cells[i].increase_age();

					int ahead = cells[i].ahead(place, cells), act;
					const vector<vector<int>>& Toar = cells[i].gTOAR();
					++statistic[cells[i].geat()];

					if (cells[i].is_bited()) {
						int q1, q2;
						for (q1 = 0; q1 < variants_of_move; ++q1)
							if (neighboor[q1] == cells[i].ghit_from())
								break;
						for (q2 = 0; q2 < variants_of_move; ++q2)
							if (neighboor[q2] == cells[i].gcourse())
								break;
						int from = q1 - q2;
						if (from < 0)
							from += 8;
						act = Toar[ahead][action_gens + (from * reaction_gens) + cells[i].ggen2()];
						work(place, cells, i, act, way_b);
					}
					else {
						coordin step = cells[i].position + cells[i].gcourse();
						if (cells[i].energy > reproduct_energy && step < place && ((ahead == 0) || (ahead == 4) || (ahead == 5))) {
							cell child(&cells[i]);
							child.position = { cells[i].position.getx(), cells[i].position.gety() };
							place[cells[i].position.getx()][cells[i].position.gety()].set(alive, child.gcolour());
							cells.push_back(child);
							cells[i].position = step;
							place[step.getx()][step.gety()].set(alive, cells[i].gcolour());

						}
						else {
							act = Toar[ahead][cells[i].ggen()];
							work(place, cells, i, act, way_b);
						}
					}
					r = temp1 - cells[i].position;
					if ((abs(r.getx()) > 1) || (abs(r.gety()) > 1)) {
						cout << "Upps! We found!!!\n" << way_b;
						system("pause >nul");
						system("cls");
					}
				}
				else {
					died_cells.push_back(i);
					place[cells[i].position.getx()][cells[i].position.gety()].set(meat, col_meat);
					//place[cells[i].position.getx()][cells[i].position.gety()].set(empt, 7);
				}
				//show(place, year, cells);
				//setcur(0, 0);
			}

			if (cells.size() == died_cells.size() && cells.size() != 0)
				show_genome_cell(way_to_file, cells[0]);

			for (int i = died_cells.size() - 1; i > -1; --i) {
				cells.erase(cells.cbegin() + died_cells[i]);
			}
			died_cells.clear();
			died_cells.shrink_to_fit();

			if (_kbhit()) {
				char cn = _getch();
				if (cn == 'm')
					cin >> mutation;
				else if (cn == 'y')
					cin >> year_cycle;
				else if (cn == 'p')
					cin >> plants_year;
				else if (cn == 'g')
					cin >> putrefaction_year;
				else if (cn == 't')
					cin >> timee;
				system("cls");
			}
		}
	}
}
