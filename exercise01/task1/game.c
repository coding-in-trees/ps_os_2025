#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printUsage(const char* programName) {
	printf("usage: %s <width> <height> <density> <steps>\n", programName);
}

int**generate_empty_field(int width, int height);
void init_field(int** field, int width, int height, double density);
void free_field(int** field, int width);
int generate_PBM(const int** field, int width, int height, int step);
void simulate(const int** current, int** dest, int width, int height);
int alive_neighbors(const int** field, int x, int y, int width, int height);
int is_out_of_bounds(int x, int y, int width, int height);

int main(int argc, char* argv[]) {
	if(argc != 5) {
		printUsage(argv[0]);
		return EXIT_FAILURE;
	}

	const int width = atoi(argv[1]);
	const int height = atoi(argv[2]);
	const double density = atof(argv[3]);
	const int steps = atoi(argv[4]);

	printf("width:   %4d\n", width);
	printf("height:  %4d\n", height);
	printf("density: %4.0f%%\n", density * 100);
	printf("steps:   %4d\n", steps);

	if (steps < 0)
	{
		return EXIT_FAILURE;
	}

	// Seeding the random number generator so we get a different starting field
	// every time.
	srand(time(NULL));

	int** source = generate_empty_field(width, height);
	int** destination = generate_empty_field(width, height);
	init_field(source, width, height, density);

	int exit = generate_PBM((const int**)source, width, height, 0);
	if (steps == 0) {
		return exit;
	}

	for (int i = 1; i < steps; i++)
	{
		simulate((const int**)source, destination, width, height);
		int exit = generate_PBM((const int**)destination, width, height, i);
		if (exit == EXIT_FAILURE)
		{
			return EXIT_FAILURE;
		}
		int** temp = source;
		source = destination;
		destination = temp;
	}
	
	free_field(source, width);
	free_field(destination, width);
	return EXIT_SUCCESS;
}

int** generate_empty_field(int width, int height) {
	int** field = malloc(sizeof(int*) * width);
	for (int i = 0; i < width; i++)
	{
		field[i] = calloc(height, sizeof(int));
	}
	return field;
}

void free_field(int** field, int width) {
	for (int i = 0; i < width; i++)
	{
		free(field[i]);
	}
	free(field);
}

void init_field(int** field,int width, int height, double density) {
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			double can_live = rand() / (double)RAND_MAX;
			field[i][j] = can_live < density;
		}
	}
}

int generate_PBM(const int** field, int width, int height, int step) {
	char* fileName = NULL;
	int size = asprintf(&fileName, "gol_%05d.pbm", step);
	if (size < 0)
	{
		return EXIT_FAILURE;
	}
	FILE *pbm = fopen(fileName, "w");

	fprintf(pbm, "P1\n");
	fprintf(pbm, "%d %d\n", width, height);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			fprintf(pbm, "%d", !field[i][j]);
		}
	}

	fclose(pbm);
	free(fileName);
	return EXIT_SUCCESS;
}

void simulate(const int** current, int** dest, int width, int height) {
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int isAlive = current[x][y];
			int countAlive = alive_neighbors(current, x, y, width, height);
			if (!isAlive && countAlive == 3)
			{
				dest[x][y] = 1;
				continue;
			}
			if (isAlive && (countAlive < 2 || countAlive > 3))
			{
				dest[x][y] = 0;
				continue;
			}
			dest[x][y] = current[x][y];
		}
	}
	
}

int alive_neighbors(const int** field, int x, int y, int width, int height) {
	int sum = 0;
	for (int pad_x = -1; pad_x < 2; pad_x++)
	{
		for (int pad_y = -1; pad_y < 2; pad_y++)
		{
			int curr_x = x+pad_x;
			int curr_y = y+pad_y;
			if (
				(curr_x == x && curr_y == y)
				|| is_out_of_bounds(curr_x, curr_y, width, height)
			)
			{
				continue;
			}
			sum += field[curr_x][curr_y];
		}
	}
	return sum;
}

int is_out_of_bounds(int x, int y, int width, int height) {
	int x_out_of_bounds = x < 0 || x >= width;
	int y_out_of_bounds = y < 0 || y >= height;
	return x_out_of_bounds || y_out_of_bounds;
}
