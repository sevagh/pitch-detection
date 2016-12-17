#include <string>

class sinegenerator
{

private:
    int size_single_channel, size_dual_channel;
	double sample_rate, inv_sample_rate, frequency;
	double *tone_dual_channel, *tone_single_channel;

public:
	sinegenerator(double sample_rate, double frequency);
    ~sinegenerator();

    void validate_pitch(std::string algo);
};
