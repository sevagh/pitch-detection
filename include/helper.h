#define FREQ_MIN 80
#define FREQ_MAX 10000
#define MIN_SNR 15

double* zero_pad(double *original_signal, int size);

void hanning_window(double *signal, int size);

double looper(double *data, int data_size, double sampling_rate,
	      double (*fp)(double, double*, int, double));
