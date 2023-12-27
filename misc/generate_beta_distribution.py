import numpy as np
from scipy.stats import beta

# Define the parameters for the Beta distributions
# Example: mean = 0.1, beta_param = 18 (alpha is calculated from these)
means = [0.1, 0.15, 0.2]
beta_params = [18, 11 + 1/3, 8]  # Adjust these values as needed

# Number of points in the distribution
n_points = 100

for i, (mean, beta_param) in enumerate(zip(means, beta_params)):
    alpha = mean * beta_param / (1 - mean)
    x = np.linspace(0, 1, n_points)
    y = beta.pdf(x, alpha, beta_param)/100.0

    # Format as a C++ array
    cpp_array = ', '.join(f'{val:.6f}' for val in y)
    print(f"// Beta distribution with mean {mean} and beta {beta_param}\n"
          f"static const float Beta_Distribution_{i}[{n_points}] = "
          f"{{ {cpp_array} }};\n")
