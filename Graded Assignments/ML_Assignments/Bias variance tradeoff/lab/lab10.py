import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

def generate_dataset(n, sigma=0.1, seed=None):
    if seed is not None:
        np.random.seed(seed)
    x = np.random.uniform(-1, 1, n)
    epsilon = np.random.normal(0, sigma, n)
    y = np.sin(np.pi * x) + epsilon
    return x, y

def compute_average_predictors(n_datasets=100, n_samples=30, sigma=0.1, x_grid=None):
    if x_grid is None:
        x_grid = np.linspace(-1, 1, 100)

    predictions_h1 = np.zeros((n_datasets, len(x_grid)))
    predictions_h2 = np.zeros((n_datasets, len(x_grid)))

    for i in range(n_datasets):
        x_train, y_train = generate_dataset(n_samples, sigma)
        
        # H1: Constant predictor
        constant_prediction = np.mean(y_train)
        predictions_h1[i, :] = constant_prediction  # same for all x

        # H2: Linear regression
        model = LinearRegression()
        model.fit(x_train.reshape(-1, 1), y_train)
        predictions_h2[i, :] = model.predict(x_grid.reshape(-1, 1))

    # Average predictor function
    avg_g_h1 = np.mean(predictions_h1, axis=0)
    avg_g_h2 = np.mean(predictions_h2, axis=0)

    return x_grid, avg_g_h1, avg_g_h2

x_grid, avg_g_h1, avg_g_h2 = compute_average_predictors(n_datasets=100, n_samples=30, sigma=0.2)

plt.plot(x_grid, np.sin(np.pi * x_grid), label='True function', color='black')
plt.plot(x_grid, avg_g_h1, label='Avg Predictor H1 (Constant)', linestyle='--')
plt.plot(x_grid, avg_g_h2, label='Avg Predictor H2 (Linear)', linestyle='-.')

plt.title('Average Predictors from 100 Datasets')
plt.xlabel('x')
plt.ylabel('ḡ(x)')
plt.legend()
plt.grid(True)
plt.show()


def plot_hypothesis_class(n_datasets=100, n_samples=2, sigma=0.0, x_grid=None, title_suffix=""):
    if x_grid is None:
        x_grid = np.linspace(-1, 1, 100)

    predictions_h1 = np.zeros((n_datasets, len(x_grid)))
    predictions_h2 = np.zeros((n_datasets, len(x_grid)))

    plt.figure(figsize=(12, 5))

    # Plot individual learnt functions
    for i in range(n_datasets):
        x_train, y_train = generate_dataset(n_samples, sigma)

        # H1
        constant_prediction = np.mean(y_train)
        predictions_h1[i, :] = constant_prediction
        plt.subplot(1, 2, 1)
        plt.plot(x_grid, [constant_prediction]*len(x_grid), color='lightblue', alpha=0.3)

        # H2
        model = LinearRegression()
        model.fit(x_train.reshape(-1, 1), y_train)
        pred = model.predict(x_grid.reshape(-1, 1))
        predictions_h2[i, :] = pred
        plt.subplot(1, 2, 2)
        plt.plot(x_grid, pred, color='lightgreen', alpha=0.3)

    # Plot average predictor functions
    avg_g_h1 = np.mean(predictions_h1, axis=0)
    avg_g_h2 = np.mean(predictions_h2, axis=0)

    true_func = np.sin(np.pi * x_grid)

    # H1 plot
    plt.subplot(1, 2, 1)
    plt.plot(x_grid, avg_g_h1, color='blue', label='Avg Predictor')
    plt.plot(x_grid, true_func, color='black', label='True function')
    plt.title(f"H1 (Constant) {title_suffix}")
    plt.xlabel("x")
    plt.ylabel("g_D(x)")
    plt.legend()
    plt.grid(True)

    # H2 plot
    plt.subplot(1, 2, 2)
    plt.plot(x_grid, avg_g_h2, color='green', label='Avg Predictor')
    plt.plot(x_grid, true_func, color='black', label='True function')
    plt.title(f"H2 (Linear) {title_suffix}")
    plt.xlabel("x")
    plt.ylabel("g_D(x)")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

plot_hypothesis_class(n_datasets=100, n_samples=2, sigma=0.0, title_suffix="(Noiseless)")
plot_hypothesis_class(n_datasets=100, n_samples=2, sigma=0.2, title_suffix="(Noisy, sigma=0.2)")

def compute_error_metrics(n_datasets=100, n_samples=30, sigma=0.1, x_grid=None, test_size=100):
    if x_grid is None:
        x_grid = np.linspace(-1, 1, 100)
    
    
    # Generate test dataset
    x_test, y_test = generate_dataset(test_size, sigma)  # No noise in y_test for evaluation
    true_y_test = np.sin(np.pi * x_test)
    predictions_h1 = np.zeros((n_datasets, len(x_grid)))
    predictions_h2 = np.zeros((n_datasets, len(x_grid)))
    true_func = np.sin(np.pi * x_grid)

    for i in range(n_datasets):
        x_train, y_train = generate_dataset(n_samples, sigma)

        # H1 (constant) prediction
        constant_prediction = np.mean(y_train)
        predictions_h1[i, :] = constant_prediction

        # H2 (linear regression) prediction
        model = LinearRegression()
        model.fit(x_train.reshape(-1, 1), y_train)
        predictions_h2[i, :] = model.predict(x_grid.reshape(-1, 1))
    
    # Average predictor function
    avg_g_h1 = np.mean(predictions_h1, axis=0)
    avg_g_h2 = np.mean(predictions_h2, axis=0)
    
    # Calculate bias
    bias_h1 = np.mean((avg_g_h1 - true_func) ** 2)
    bias_h2 = np.mean((avg_g_h2 - true_func) ** 2)
    
    # Calculate variance
    var_h1 = np.mean(np.var(predictions_h1, axis=0))
    var_h2 = np.mean(np.var(predictions_h2, axis=0))
    
    # Calculate overall error
    error_h1 = np.mean(np.mean((predictions_h1 - true_func) ** 2))
    error_h2 = np.mean(np.mean((predictions_h2 - true_func) ** 2))
    
    # Noise (irreducible error)
    noise = np.mean((y_test - true_y_test) ** 2)
    
    return bias_h1, var_h1, error_h1, bias_h2, var_h2, error_h2, noise

n_samples_list = [2, 5, 10, 30, 100]
results = []

for n_samples in n_samples_list:
    bias_h1, var_h1, error_h1, bias_h2, var_h2, error_h2, noise = compute_error_metrics(n_datasets=100, n_samples=n_samples, sigma=0)
    results.append((n_samples, bias_h1, var_h1, error_h1, bias_h2, var_h2, error_h2, noise))

# Print the results for analysis
for result in results:
    print(f"n_samples={result[0]}: Bias(H1)={result[1]:.4f}, Var(H1)={result[2]:.4f}, Error(H1)={result[3]:.4f}, "
          f"Bias(H2)={result[4]:.4f}, Var(H2)={result[5]:.4f}, Error(H2)={result[6]:.4f}, Noise={result[7]:.4f}")
    

n_samples_list = [r[0] for r in results]
bias_h1 = np.array([r[1] for r in results])
var_h1 = np.array([r[2] for r in results])
error_h1 = np.array([r[3] for r in results])
bias_h2 = np.array([r[4] for r in results])
var_h2 = np.array([r[5] for r in results])
error_h2 = np.array([r[6] for r in results])
noise = np.array([r[7] for r in results])

# Compute theoretical error = bias + variance + noise
theoretical_error_h1 = bias_h1 + var_h1 + noise
theoretical_error_h2 = bias_h2 + var_h2 + noise

# Plot for H1
plt.figure(figsize=(12, 5))
plt.subplot(1, 2, 1)
plt.stem(n_samples_list, error_h1, linefmt='r-', markerfmt='ro', basefmt=" ", label='Measured Error')
plt.stem(n_samples_list, theoretical_error_h1, linefmt='b--', markerfmt='bo', basefmt=" ", label='Bias + Variance + Noise')
plt.title('H1: Constant Hypothesis')
plt.xlabel('n_samples')
plt.ylabel('Error')
plt.legend()
plt.grid(True)

# Plot for H2
plt.subplot(1, 2, 2)
plt.stem(n_samples_list, error_h2, linefmt='r-', markerfmt='ro', basefmt=" ", label='Measured Error')
plt.stem(n_samples_list, theoretical_error_h2, linefmt='b--', markerfmt='bo', basefmt=" ", label='Bias + Variance + Noise')
plt.title('H2: Linear Hypothesis')
plt.xlabel('n_samples')
plt.ylabel('Error')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()


def generate_learning_curves(n_datasets=100, n_samples_list=[2, 5, 10, 30, 100], sigma=0.1, x_grid=None, test_size=100):
    if x_grid is None:
        x_grid = np.linspace(-1, 1, 100)
    
    ein_h1 = []
    eout_h1 = []
    bias_h1 = []
    
    ein_h2 = []
    eout_h2 = []
    bias_h2 = []
    
    # Generate test dataset
    x_test, y_test = generate_dataset(test_size, sigma)
    
    for n_samples in n_samples_list:
        ein_vals_h1 = []
        eout_vals_h1 = []
        ein_vals_h2 = []
        eout_vals_h2 = []
        
        bias_vals_h1 = []
        bias_vals_h2 = []
        
        for _ in range(n_datasets):
            x_train, y_train = generate_dataset(n_samples, sigma)
            
            # H1 (Constant)
            constant_prediction = np.mean(y_train)
            ein_vals_h1.append(np.mean((constant_prediction - y_train) ** 2))
            eout_vals_h1.append(np.mean((constant_prediction - y_test) ** 2))
            bias_vals_h1.append(np.mean((constant_prediction - np.sin(np.pi * x_grid)) ** 2))
            
            # H2 (Linear Regression)
            model = LinearRegression()
            model.fit(x_train.reshape(-1, 1), y_train)

            pred_train_h2 = model.predict(x_train.reshape(-1, 1))
            pred_test_h2 = model.predict(x_test.reshape(-1, 1))
            
            ein_vals_h2.append(np.mean((pred_train_h2 - y_train) ** 2))
            eout_vals_h2.append(np.mean((pred_test_h2 - y_test) ** 2))
            bias_vals_h2.append(np.mean((model.predict(x_grid.reshape(-1, 1)) - np.sin(np.pi * x_grid)) ** 2))
        
        # Average over all datasets
        ein_h1.append(np.mean(ein_vals_h1))
        eout_h1.append(np.mean(eout_vals_h1))
        bias_h1.append(np.mean(bias_vals_h1))
        
        ein_h2.append(np.mean(ein_vals_h2))
        eout_h2.append(np.mean(eout_vals_h2))
        bias_h2.append(np.mean(bias_vals_h2))
    
    # Plot Learning Curves
    plt.figure(figsize=(15, 5))

    # Ein vs n
    plt.subplot(1, 3, 1)
    plt.plot(n_samples_list, ein_h1, label="H1 (Constant)", marker='o')
    plt.plot(n_samples_list, ein_h2, label="H2 (Linear)", marker='o')
    plt.xlabel("n (Training Set Size)")
    plt.ylabel("Ein (Training Error)")
    plt.title("Ein vs n")
    plt.legend()
    plt.grid(True)

    # Eout vs n
    plt.subplot(1, 3, 2)
    plt.plot(n_samples_list, eout_h1, label="H1 (Constant)", marker='o')
    plt.plot(n_samples_list, eout_h2, label="H2 (Linear)", marker='o')
    plt.xlabel("n (Training Set Size)")
    plt.ylabel("Eout (Test Error)")
    plt.title("Eout vs n")
    plt.legend()
    plt.grid(True)

    # Average Bias vs n
    plt.subplot(1, 3, 3)
    plt.plot(n_samples_list, bias_h1, label="H1 (Constant)", marker='o')
    plt.plot(n_samples_list, bias_h2, label="H2 (Linear)", marker='o')
    plt.xlabel("n (Training Set Size)")
    plt.ylabel("Average Bias")
    plt.title("Average Bias vs n")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

generate_learning_curves(n_datasets=100, n_samples_list=[2, 5, 10, 30, 100], sigma=0.2)

bias_h1, var_h1, error_h1, bias_h2, var_h2, error_h2, noise = compute_error_metrics(n_datasets=100, n_samples=2, sigma=0.2)
theoretical_error_h11 = bias_h1 + var_h1 + noise
theoretical_error_h21 = bias_h2 + var_h2 + noise

print(f"n_samples= 2: Bias(H1)={bias_h1:.4f}, Var(H1)={var_h1:.4f}, Error(H1)={error_h1:.4f}, "f"Bias(H2)={bias_h2:.4f}, Var(H2)={var_h2:.4f}, Error(H2)={error_h2:.4f}, Noise={noise:.4f}")

bias_h1, var_h1, error_h1, bias_h2, var_h2, error_h2, noise = compute_error_metrics(n_datasets=100, n_samples=2, sigma=0)
theoretical_error_h12 = bias_h1 + var_h1 + noise
theoretical_error_h22 = bias_h2 + var_h2 + noise

print(f"n_samples= 2: Bias(H1)={bias_h1:.4f}, Var(H1)={var_h1:.4f}, Error(H1)={error_h1:.4f}, "f"Bias(H2)={bias_h2:.4f}, Var(H2)={var_h2:.4f}, Error(H2)={error_h2:.4f}, Noise={noise:.4f}")
