#include <vector>

double loop(std::vector<double> data, int data_size, double sampling_rate, double (*fp)(double, std::vector<double>, int, double));
