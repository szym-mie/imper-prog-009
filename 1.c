#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
	return *(A+(cols*row)+col);
}

void set(int cols, int row, int col, int *A, int value) {
	*(A+(cols*row)+col) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
	for (int i = 0; i < rowsA; i++)
	{
		for (int j = 0; j < colsB; j++)
		{
			int sum = 0;
			for (int k = 0; k < colsA; k++)
			{
				sum += get(colsA, i, k, A) * get(colsB, k, j, B);
			}
			set(colsB, i, j, AB, sum);
		}
	}
}

void read_mat(int rows, int cols, int *t) {
	for (int i = 0; i < rows * cols; i++) scanf("%d", t+i);
}

void print_mat(int rows, int cols, int *t) {
	for (int i = 0; i < rows * cols; i++) printf("%d%c", *(t+i), i % cols == cols - 1 ? '\n' : ' ');
}

int read_char_lines(char *array[]) {
	char buf[BUF_SIZE] = { 0 };
	int i = 0;
	int offset = 0;
	int c;
	while ((c = getchar()) != EOF)
	{
		*(buf+offset++) = c != '\n' ? c : '\0';
		if (c == '\n')
		{
			if (offset == 1) return i;
			char *str = malloc(offset);
			if (str == NULL) return -1;
			memcpy(str, buf, offset);
			array[i++] = str;
			offset = 0;
		}
	}

	return i;
}

void write_char_line(char *array[], int n) {
	puts(array[n]);
}

void delete_lines(char *array[]) {
	for (int i = 0; i < TAB_SIZE; i++) free(array[i]);
}

int read_int_lines_cont(int *ptr_array[]) {
	int *array = ptr_array[0]; // NEVER do anything like this
	char buf[BUF_SIZE] = { 0 };
	int i = 0; // lines read
	int ni = 0; // total numbers read
	int *line_start = array;
	int offset = 0;
	int num_count = 0;

	int c;
	while ((c = getchar()) != EOF)
	{
		*(buf+offset++) = c != '\n' && c != ' ' ? c : '\0';
		if (offset != 1 && (c == ' ' || c == '\n')) 
		{
			sscanf(buf, "%d", array+ni++);
			offset = 0;
			num_count++;
		}
		if (c == '\n')
		{
			if (num_count == 0 && offset == 1) return i;
			ptr_array[++i] = line_start;
			line_start = array+ni;
			num_count = 0;
		}
	}

	return i;
}

void write_int_line_cont(int *ptr_array[], int n) {
	for (int *ptr = ptr_array[n+1]; ptr < ptr_array[n+2]; ptr++) 
		printf("%d ", *ptr);
	printf("\n");
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

double average(int *values, int len)
{
	int sum = 0;
	for (int i = 0; i < len; i++) sum += *(values+i);
	return (double)sum / len;
}

char *parse_int_buf(char *buf, int *dest)
{
	int num = 0;
	int is_num_start = 0;
	int is_minus = 0;

	if (*buf == '\0') return NULL;

	for (;;)
	{
		if (*buf == '-') is_minus = 1;
		if ((*buf >= '0' && *buf <= '9' || *buf == '-') && !is_num_start) is_num_start = 1;
		if ((*buf == ' ' || *buf == '\0') && is_num_start) 
		{
			*dest = !is_minus ? num : -num;
			return buf;
		}
		if (is_num_start && *buf != '-')
		{
			num *= 10;
			num += *buf - '0';
		}
		buf++;
	}
}

int read_int_lines(line_type lines_array[]) {
	char buf[BUF_SIZE] = { 0 };
	int i = 0;
	int offset = 0;
	int num_count = 0;
	int is_blank = 1;
	int c;
	while ((c = getchar()) != EOF)
	{
		*(buf+offset++) = c != '\n' ? c : '\0';
		if ((c >= '0' && c <= '9' || c == '-') && is_blank)
		{
			num_count++;
			is_blank = 0;
		}
		if (c == ' ' || c == '\n') is_blank = 1;
		if (c == '\n')
		{
			if (offset == 1) return i;
			int *series = malloc(num_count);
			if (series == NULL) return -1;
			char *buf_ptr = buf;
			for (int si = 0; buf_ptr != NULL; si++)
				buf_ptr = parse_int_buf(buf_ptr, series+si);
			lines_array[i].values = series;
			lines_array[i].len = num_count;
			lines_array[i].average = average(series, num_count);
			offset = 0;
			num_count = 0;
			is_blank = 1;
			i++;
		}
	}

	return i;

}

void write_int_line(line_type lines_array[], int n) {
	line_type line = lines_array[n];
	for (int i = 0; i < line.len; i++) printf("%d ", *(line.values+i));
	printf("\n%f\n", line.average);
}

void delete_int_lines(line_type array[], int line_count) {
	for (int i = 0; i < line_count; i++) free(array[i].values);
}

int cmp (const void *a, const void *b) {
	const line_type *pa = a;
	const line_type *pb = b;
	return pa->average - pb->average;
}

void sort_by_average(line_type lines_array[], int line_count) {
	qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
	int i;
	for (i = 0; i < n_triplets; i++) 
	{
		triplet *t = triplet_array+i;
		if (scanf("%d %d %d", &t->r, &t->c, &t->v) != 3) break;
	}
	return i;
}

int cmp_triplets(const void *t1, const void *t2) {
	const triplet *pt1 = t1;
	const triplet *pt2 = t2;
	if (pt1->r == pt2->r) return pt1->c - pt2->c;
	return pt1->r - pt2->r;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
	qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
	*R++ = 0;
	int i = 0;
	triplet *t = triplet_array;
	
	for (int r = 0; r < rows; r++)
	{
		while (t->r == r)
		{
			*V++ = t->v;
			*C++ = t->c;
			t++;
			i++;
		}
		*R++ = i;
	}
	*R = n_triplets;
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
	for (int i = 0; i < rows; i++)
	{
		*(y+i) = 0;
		for (int j = *(R+i); j < *(R+i+1); j++)
		{
			int c = *(C+j);
			*(y+i) += *(V+j) * *(x+c);
		}
	}
}

void read_vector(int *v, int n) {
	for (int i = 0; i < n; i++) 
	{
		*(v+i) = 0; // in case scanf fails
		scanf("%d", v+i);
	}
}

void write_vector(int *v, int n) {
	for (int i = 0; i < n; i++) printf("%d ", *(v+i));
	printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

