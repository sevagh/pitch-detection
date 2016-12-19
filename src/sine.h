#include <string>

std::tuple<std::vector<double>, std::vector<double>> generate_sine(double frequency, int sample_rate);

void validate_pitch(std::string algo, double frequency, int sample_rate);
