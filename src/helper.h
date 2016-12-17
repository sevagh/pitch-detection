#define FREQ_MIN 0
#define FREQ_MAX 10000
#define CONSEC_FAIL_LIM 2 
#define CONSEC_FAIL_MARGIN 0.01
#define FREQ_STARTING_INCR 10

double* zero_pad(double *original_signal, int size);

void hanning_window(double *signal, int size);

double looper(double *data, int data_size, double sampling_rate,
	      double (*fp)(double, double*, int, double));
