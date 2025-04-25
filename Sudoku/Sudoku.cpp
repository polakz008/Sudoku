#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define EMPTY 0

int** board = NULL;
int size = 4;
int block_size = 2;
int** solution_board = NULL;
time_t start_time, end_time;

// Alokacja planszy 
void allocate_board(int size) {
	board = (int**)malloc(size * sizeof(int*));
	if (board == NULL) {
		printf("Blad alokacji pamieci dla planszy.\n");
		exit(1);
	}

	for (int i = 0; i < size; i++) {
		board[i] = (int*)malloc(size * sizeof(int));
		if (board[i] == NULL) {
			printf("Blad alokacji pamieci dla wiersza %d.\n", i);
			exit(1);
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = EMPTY;
		}
	}
}

// Zwolnienie pamieci
void free_board() {
	for (int i = 0; i < size; i++) {
		free(board[i]);
	}
	free(board);
}
void free_solution_board() {
	for (int i = 0; i < size; i++) {
		free(solution_board[i]);
	}
	free(solution_board);
}

//Zapis stanu gry do pliku
void save_game(const char* filename) {
	FILE* file;
	fopen_s(&file, filename, "w");
	if (file == NULL) {
		printf("Nie mozna otworzyc pliku do zapisu.\n");
		return;
	}

	fprintf(file, "%d %d\n", size, block_size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(file, "%d ", board[i][j]);
		}
		fprintf(file, "\n");
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(file, "%d ", solution_board[i][j]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
	printf("Gra zostala zapisana do pliku '%s'.\n", filename);
}

// Sprawdzenie poprawnosci ruchow gracza
int is_valid(int row, int col, int num) {
	for (int i = 0; i < size; i++) {
		if (board[row][i] == num || board[i][col] == num)
			return 0;
	}

	int startRow = row - row % block_size;
	int startCol = col - col % block_size;

	for (int i = startRow; i < startRow + block_size; i++) {
		for (int j = startCol; j < startCol + block_size; j++) {
			if (board[i][j] == num)
				return 0;
		}
	}

	return 1;
}

//Sprawdzenie rozwiazania
int is_solution_correct() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] != solution_board[i][j]) {
				return 0;  
			}
		}
	}
	return 1;  
}

// Tasowanie liczb
void shuffle(int* array, int n) {
	for (int i = 0; i < n - 1; i++) {
		int j = i + rand() / (RAND_MAX / (n - i) + 1);
		int tmp = array[j];
		array[j] = array[i];
		array[i] = tmp;
	}
}

// Wypelnianie planszy rekurencyjnie (backtracking)
int fill_board(int row, int col) {
	if (row == size)
		return 1;

	int nextRow = (col == size - 1) ? row + 1 : row;
	int nextCol = (col + 1) % size;

	int* nums = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++) nums[i] = i + 1;
	shuffle(nums, size);

	for (int i = 0; i < size; i++) {
		if (is_valid(row, col, nums[i])) {
			board[row][col] = nums[i];
			if (fill_board(nextRow, nextCol)) {
				free(nums);
				return 1;
			}
			board[row][col] = EMPTY;
		}
	}

	free(nums);
	return 0;
}

// Wypisanie planszy
void print_board() {
	printf("    ");
	for (int j = 0; j < size; j++) {
		if (j % block_size == 0 && j != 0)
			printf("|");
		printf("%2d ", j + 1);
	}
	printf("\n");

	printf("   ");
	for (int j = 0; j < size + (size - 1) / block_size; j++) {
		printf("---");
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		if (i % block_size == 0 && i != 0) {
			printf("   ");
			for (int k = 0; k < size + (size - 1) / block_size; k++) {
				printf("---");
			}
			printf("\n");
		}

		printf("%2d |", i + 1);

		for (int j = 0; j < size; j++) {
			if (j % block_size == 0 && j != 0)
				printf("|");

			if (board[i][j] == EMPTY)
				printf(" . ");
			else if (size == 16 && board[i][j] < 10)
				printf(" %d ", board[i][j]);
			else
				printf("%2d ", board[i][j]);
		}
		printf("\n");
	}
}

// Liczenie rozwiazan
int count_solutions(int row, int col, int* count) {
	if (row == size) {
		(*count)++;
		return *count > 1 ? 1 : 0;
	}

	int nextRow = (col == size - 1) ? row + 1 : row;
	int nextCol = (col + 1) % size;

	if (board[row][col] != EMPTY)
		return count_solutions(nextRow, nextCol, count);

	for (int num = 1; num <= size; num++) {
		if (is_valid(row, col, num)) {
			board[row][col] = num;
			if (count_solutions(nextRow, nextCol, count))
				return 1;
			board[row][col] = EMPTY;
		}
	}

	return 0;
}

// Usuwanie cyfr przy zachowaniu jednego rozwiazania
void remove_numbers(int clues) {
	solution_board = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		solution_board[i] = (int*)malloc(size * sizeof(int));
		for (int j = 0; j < size; j++) {
			solution_board[i][j] = board[i][j];
		}
	}

	int total = size * size;
	int attempts = total - clues;

	while (attempts > 0) {
		int row = rand() % size;
		int col = rand() % size;

		if (board[row][col] == EMPTY)
			continue;

		int backup = board[row][col];
		board[row][col] = EMPTY;

		int count = 0;
		count_solutions(0, 0, &count);

		if (count != 1) {
			board[row][col] = backup;
		}
		else {
			attempts--;
		}
	}
}

//Wyswietlenie instrukcji
void print_instructions() {
	printf("Instrukcja: Gdy wejdziesz w tryb gry, wpisz np. 23 8 aby wstawic wartosc 8 do rzedu 2 i kolumny 3.\n");
	printf("Aby usunac liczbe, wpisz 23 0.\n");
	printf("Aby zakonczyc tryb gry,wpisz 0 0.\n");
}

//Tryb gry
void play_game() {
	int row_col, value;

	printf("\nJestes w trybie gry. Instrukcja: Wpisz np. 23 8 aby wstawic wartosc 8 do rzedu 2 i kolumny 3.\n");
	printf("Aby usunac liczbe, wpisz 23 0.\n");
	printf("Aby zakonczyc tryb gry, wpisz 0 0.\n");

	start_time = time(NULL);
	while (1) {
		printf("\n"); 
		print_board();

		printf("\nWprowadz wspolrzedne i wartosc (np. 23 8): ");
		while (scanf_s("%d %d", &row_col, &value) != 2) {
			while (getchar() != '\n');
			printf("Niepoprawne dane. Wpisz np. 23 8 aby wstawic wartosc 8 do rzedu 2 i kolumny 3. :");
		}

		if (row_col == 0 && value == 0) {
			int choice;
			end_time = time(NULL);
			double elapsed_time = difftime(end_time, start_time);
			int minutes = (int)(elapsed_time / 60);
			int seconds = (int)((int)elapsed_time % 60);
			printf("Chcesz zakonczyc gre, czy chcesz ja zapisac? 1-tak 0-nie");
			while (scanf_s("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
				while (getchar() != '\n');
				printf("Niepoprawna wartoœæ. WprowadŸ liczbê 1-tak lub 0-nie: ");
			}
			if (choice == 1) {
				save_game("save.txt");
			}
			printf("Czas gry: %d minut, %d sekund.\n", minutes, seconds);
			printf("Zakonczenie trybu gry.\n");
			break;
		}

		int row = row_col / 10;
		int col = row_col % 10;

		if (row < 1 || row > size || col < 1 || col > size || value < 0 || value > size) {
			printf("Niepoprawne dane. Sprobuj ponownie.\n");
			continue;
		}

		int i = row - 1;
		int j = col - 1;

		if (value == 0) {

			if (board[i][j] == EMPTY) {
				printf("To pole jest juz puste.\n");
				continue;
			}
			board[i][j] = EMPTY;
			printf("Usunieto wartosc z (%d, %d).\n", row, col);
		}
		else {

			if (board[i][j] != EMPTY) {
				printf("To pole jest juz zajete. Aby wstawic nowa wartosc, najpierw musisz usunac poprzednia (wpisz %d 0).\n", row_col);
				continue;
			}

			board[i][j] = value;
			printf("Wstawiono %d do (%d, %d).\n", value, row, col);

			if (board[i][j] != solution_board[i][j]) {
				printf("Uwaga: Wprowadzona wartosc jest niepoprawna w tym miejscu.\n");
				printf("Chcesz podpowiedz? 1-tak, 0-nie: ");
				int choice;
				while (scanf_s("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
					while (getchar() != '\n'); 
					printf("Niepoprawna wartosc. Wprowadz liczbe 1-tak lub 0-nie: ");
				}

				if (choice == 1) {
					printf("Podpowiedz: prawidlowa wartosc to %d\n", solution_board[i][j]);
				}
			}
		}

		if (is_solution_correct()) {
			printf("Gratulacje! Ukonczyles gre.\n");
			end_time = time(NULL);
			double elapsed_time = difftime(end_time, start_time);
			int minutes = (int)(elapsed_time / 60);
			int seconds = (int)((int)elapsed_time % 60);
			printf("Czas gry: %d minut, %d sekund.\n", minutes, seconds);
			break;
		}
	}
}


//Wczytaj zapis z pliku
void load_game_from_file(const char* filename) {
	FILE* file;
	fopen_s(&file, filename, "r");
	if (file == NULL) {
		printf("Nie mozna otworzyc pliku do wczytania.\n");
		return;
	}

	while (fscanf(file, "%d %d", &size, &block_size) != 2) {
		while (getchar() != '\n');
		printf("Niepoprawna wartosc w pliku. Sprobuj ponownie.\n");
	}
	allocate_board(size);

	solution_board = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		solution_board[i] = (int*)malloc(size * sizeof(int));
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			while (fscanf(file, "%d", &board[i][j]) != 1) {
				while (getchar() != '\n');
				printf("Niepoprawna wartosc w pliku w pozycji [%d][%d]. Sprobuj ponownie.\n", i, j);
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			while (fscanf(file, "%d", &solution_board[i][j]) != 1) {
				while (getchar() != '\n');
				printf("Niepoprawna wartosc w pliku w pozycji [%d][%d] dla rozwiazania. Sprobuj ponownie.\n", i, j);
			}
		}
	}

	fclose(file);
	printf("Gra zostala wczytana z pliku '%s'.\n", filename);
	play_game();
	free_board();
	free_solution_board();
}
//Wybor opcji
void select_board_size_and_difficulty()
{
	printf("\nWybierz rozmiar planszy:\n");
	printf("1 - 4x4\n2 - 9x9\n3 - 16x16\nTwoj wybor: ");
	int size_choice;
	while (scanf_s("%d", &size_choice) != 1 || (size_choice < 1 || size_choice > 3)) {
		while (getchar() != '\n');  
		printf("Niepoprawna wartosc. Wybierz liczbe od 1 do 3: ");
	}


	switch (size_choice) {
	case 1: size = 4; block_size = 2; break;
	case 2: size = 9; block_size = 3; break;
	case 3: size = 16; block_size = 4; break;
	default:
		printf("Niepoprawny wybor, ustawiam 4x4.\n");
	}

	int difficulty;
	printf("\nWybierz poziom trudnosci:\n");
	printf("1 - Latwy\n2 - Sredni\n3 - Trudny\nTwoj wybor: ");
	while (scanf_s("%d", &difficulty) != 1 || (difficulty < 1 || difficulty > 3)) {
		while (getchar() != '\n');  
		printf("Niepoprawna wartosc. Wybierz 1, 2 lub 3: ");
	}

	int clues;
	int total_cells = size * size;
	switch (difficulty) {
	case 1: clues = total_cells * 0.6; break;
	case 2: clues = total_cells * 0.45; break;
	case 3: clues = total_cells * 0.35; break;
	default:
		printf("Niepoprawny wybor, ustawiam latwy.\n");
		clues = total_cells * 0.6;
	}

	allocate_board(size);
	fill_board(0, 0);
	remove_numbers(clues);

	play_game();
	free_board();
}

//Menu
void show_menu() {
	int option;
	while (1) {
		printf("===== MENU SUDOKU =====\n");
		printf("1 - Nowa gra\n");
		printf("2 - Wczytaj gre\n");
		printf("3 - Instrukcja\n");
		printf("4 - Wyjscie\n");
		printf("Wybierz opcje: ");
		while (scanf_s("%d", &option) != 1 || (option < 1 || option > 4)) {
			while (getchar() != '\n'); 
			printf("Niepoprawna wartosc. Wybierz liczbe od 1 do 4: ");
		}

		switch (option) {
		case 1: {
			select_board_size_and_difficulty();
			break;
		}
		case 2:
			load_game_from_file("save.txt");
			break;
		case 3:
			print_instructions();
			break;
		case 4:
			printf("Do zobaczenia!\n");
			return;

		default:
			printf("Niepoprawna opcja, sprobuj ponownie.\n");
		}
		printf("\n");
	}
}

int main() {
	srand((unsigned int)time(NULL));
	show_menu();
	return 0;
}
