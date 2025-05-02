# Samrat Moon 2203320 MnC
# Ritesh Prakashrao Phulari 2203125 CSE
# Shreeraj Deshmukh 2203130 CSE

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import multivariate_normal
from sklearn.preprocessing import StandardScaler

def likelihood(X,pi,mu,sigma):

    N,D=X.shape
    K = len(pi)
    likelihood=np.zeros(N,K)
    for k in range(K):
        likelihood[:,k] = pi[k]* multivariate_normal.pdf(X[i],mean=mu[k],cov = sigma[k])
    likelihood = np.prod(likelihood,axis=1)
    return likelihood

# Log likelihood function
def log_likelihood(X, pi, mu, sigma):

    N, D = X.shape
    K = len(pi)
    log_likelihood = 0
    
    for i in range(N):
        likelihood_i = 0
        for k in range(K):
            likelihood_i += pi[k] * multivariate_normal.pdf(X[i], mean=mu[k], cov=sigma[k])
        log_likelihood += np.log(likelihood_i)
    
    return log_likelihood

# EM Algorithm for GMM
def EM_GMM(X, K, max_iter=100, tol=1e-6):
    N, D = X.shape
    pi = np.ones(K) / K  # Initial mixing coefficients (uniform distribution)
    mu = np.random.randn(K, D)  # Randomly initialize means
    sigma = np.array([np.eye(D) for _ in range(K)])  # Initial covariance matrices as identity matrices
    
    log_likelihoods = []
    
    for iteration in range(max_iter):
        # E-step: Calculate responsibilities
        gamma = np.zeros((N, K))
        for i in range(N):
            denom = np.sum([pi[k] * multivariate_normal.pdf(X[i], mean=mu[k], cov=sigma[k]) for k in range(K)])
            for k in range(K):
                gamma[i, k] = (pi[k] * multivariate_normal.pdf(X[i], mean=mu[k], cov=sigma[k])) / denom
        
        # M-step: Update parameters
        N_k = np.sum(gamma, axis=0)
        pi = N_k / N
        
        for k in range(K):
            mu[k] = np.sum(gamma[:, k].reshape(-1, 1) * X, axis=0) / N_k[k]
            diff = X - mu[k]
            sigma[k] = np.dot(gamma[:, k] * diff.T, diff) / N_k[k]
        
        # Compute the log likelihood
        log_likelihood = log_likelihood(X, pi, mu, sigma)
        log_likelihoods.append(log_likelihood)
        
        # Check for convergence (if log likelihood change is small)
        if iteration > 0 and abs(log_likelihood - log_likelihoods[-2]) < tol:
            break
    
    return pi, mu, sigma, log_likelihoods, gamma

# Load and standardize datasets
def load_and_standardize_data():
    # Load Old Faithful dataset
    old_faithful = np.array([[76, 79], [80, 81], [85, 89], [90, 92], [95, 98], [98, 101], [103, 107], [110, 115]])  # replace with actual data
    scaler = StandardScaler()
    old_faithful_standardized = scaler.fit_transform(old_faithful)

    # Load dataset.csv
    data = pd.read_csv('dataset.csv')
    data_standardized = scaler.fit_transform(data)
    
    return old_faithful_standardized, data_standardized

# Plotting the results
def plot_results(X, mu, gamma, log_likelihoods, title="EM Clustering"):
    N, D = X.shape
    cluster_assignments = np.argmax(gamma, axis=1)
    
    # Log Likelihood Plot
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    plt.plot(log_likelihoods)
    plt.title("Log Likelihood vs Iteration")
    plt.xlabel("Iteration")
    plt.ylabel("Log Likelihood")
    
    # Initial and Final Assignment Plot
    plt.subplot(1, 2, 2)
    plt.scatter(X[:, 0], X[:, 1], c=cluster_assignments, cmap='viridis')
    plt.scatter(mu[:, 0], mu[:, 1], color='red', marker='x')
    plt.title(title)
    plt.xlabel("Feature 1")
    plt.ylabel("Feature 2")
    plt.show()

# K-means implementation and plotting
def kmeans(X, k, max_iters, centroids=[[-1, -1], [1, -1]]):
    # Initialize history of centroids
    history = [np.array(centroids)]
    j_history = []
    e_step_indices = []
    m_step_indices = []
    
    for i in range(max_iters):
        # E-step: calculate distance between data points and centroids
        distances = np.linalg.norm(X[:, np.newaxis] - centroids, axis=2)
        
        # Assign each data point to the closest centroid
        C = np.argmin(distances, axis=1)
        
        # Calculate J value (sum of distances) before E-step
        j_before_e = np.sum([np.linalg.norm(X[j] - centroids[C[j]]) for j in range(len(X))])
        j_history.append(j_before_e)
        
        # Record E-step index
        e_step_indices.append(len(j_history) - 1)
        
        # Adding the centroids before E-step to the history
        history.append(np.array(centroids))
        
        # M-step: update centroids
        new_centroids = np.array([X[C == i].mean(axis=0) for i in range(k)])
        
        # Adding the new centroids to the history
        history.append(new_centroids)
        
        # Calculate J value (sum of distances) after E-step
        j_after_e = np.sum([np.linalg.norm(X[j] - new_centroids[C[j]]) for j in range(len(X))])
        j_history.append(j_after_e)
        
        # Record M-step index
        m_step_indices.append(len(j_history) - 1)
        
        # Check if the centroids have changed
        if np.all(centroids == new_centroids):
            break
        centroids = new_centroids
        
    return centroids, C, history, j_history, e_step_indices, m_step_indices


# Main Execution
if __name__ == "__main__":
    # Load and Standardize Datasets
    old_faithful_standardized, data_standardized = load_and_standardize_data()
    
    # Run EM Algorithm on Old Faithful dataset
    pi_old, mu_old, sigma_old, log_likelihood_old, gamma_old = EM_GMM(old_faithful_standardized, K=2)
    
    # Plot results for Old Faithful
    plot_results(old_faithful_standardized, mu_old, gamma_old, log_likelihood_old, title="EM Clustering (Old Faithful)")

    # Run EM Algorithm on dataset.csv
    pi_data, mu_data, sigma_data, log_likelihood_data, gamma_data = EM_GMM(data_standardized, K=2)
    
    # Plot results for dataset.csv
    plot_results(data_standardized, mu_data, gamma_data, log_likelihood_data, title="EM Clustering (Dataset.csv)")
    
    # Run K-means on Old Faithful dataset
    init_centers_old = np.array([[1, -1], [-1, 1]])  # Initial centers for Old Faithful
    labels_old, centers_old = kmeans(old_faithful_standardized, init_centers_old)
    
    # Plot K-means result for Old Faithful
    plt.scatter(old_faithful_standardized[:, 0], old_faithful_standardized[:, 1], c=labels_old, cmap='viridis')
    plt.scatter(centers_old[:, 0], centers_old[:, 1], color='red', marker='x')
    plt.title("K-means Clustering (Old Faithful)")
    plt.xlabel("Feature 1")
    plt.ylabel("Feature 2")
    plt.show()
    
    # Run K-means on dataset.csv
    init_centers_data = np.array([[2, -3], [2, 3]])  # Initial centers for dataset.csv
    labels_data, centers_data = run_kmeans(data_standardized, init_centers_data)
    
    # Plot K-means result for dataset.csv
    plt.scatter(data_standardized[:, 0], data_standardized[:, 1], c=labels_data, cmap='viridis')
    plt.scatter(centers_data[:, 0], centers_data[:, 1], color='red', marker='x')
    plt.title("K-means Clustering (Dataset.csv)")
    plt.xlabel("Feature 1")
    plt.ylabel("Feature 2")
    plt.show()