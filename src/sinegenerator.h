class sinegenerator
{

private:
	double sampling_rate;
	double inv_sampling_rate;
	double frequency;


public:
	sinegenerator();
    ~sinegenerator();
	int size_single_channel;
	int size_dual_channel;

	double *tone_dual_channel;
	double *tone_single_channel;

	sinegenerator(double sampling_rate, double frequency);

	void generate_tone();
};
