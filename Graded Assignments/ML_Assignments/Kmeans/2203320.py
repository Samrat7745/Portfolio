import numpy as np
import matplotlib.pyplot as plt
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
filename = os.path.join(script_dir, 'faithful.csv')

# Check if the file exists
if not os.path.isfile(filename):
    raise FileNotFoundError(f"'faithful.csv' not found in the directory: {script_dir}")


# Load the dataset
data = np.loadtxt(filename, delimiter=",", skiprows=1, dtype=float, usecols=(1, 2))

# Standardize the data
def standardize_data(data):
    return (data - data.mean(axis=0)) / data.std(axis=0)

# Standardize the loaded data
Standard_data = standardize_data(data)

# Print the standardized data
print(Standard_data)

# K-means clustering algorithm
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

# Run K-means clustering
centroids, C, history, j_history, e_step_indices, m_step_indices = kmeans(Standard_data, 2, 272)

# Print the cluster assignments
print(C)

# Plot the K-means clustering process
def plot_kmeans(X, C, history, e_step_indices, m_step_indices):
    for i, centroid in enumerate(history):
        plt.figure(figsize=(10, 10))
        colors = np.array(['red', 'blue'][:len(np.unique(C))])
        plt.scatter(X[:, 0], X[:, 1], c=colors[C])
        plt.scatter(centroid[:, 0], centroid[:, 1], s=300, marker='x', c=colors)  # Color the centroids 'x' in red and blue
        
        if i not in e_step_indices:  # Draw perpendicular bisector only if not before E-step
            centroid1, centroid2 = centroid
            midpoint = (centroid1 + centroid2) / 2
            if centroid1[1] == centroid2[1]:  # Handle vertical line case
                x_vals = np.array([midpoint[0], midpoint[0]])
                y_vals = np.array(plt.gca().get_ylim())  # Use y-axis limits for vertical line
                plt.plot(x_vals, y_vals, '--', c='purple')
            else:
                slope = (centroid2[1] - centroid1[1]) / (centroid2[0] - centroid1[0])
                perp_slope = -1 / slope
                x_vals = np.array(plt.gca().get_xlim())
                y_vals = perp_slope * (x_vals - midpoint[0]) + midpoint[1]
                plt.plot(x_vals, y_vals, '--', c='purple')
        
        if i in e_step_indices:
            plt.title(f'K-means clustering iteration {i} (E-step)')
        elif i in m_step_indices:
            plt.title(f'K-means clustering iteration {i} (M-step)')
        else:
            plt.title(f'K-means clustering iteration {i}')
        
        plt.show()

# Plot the K-means clustering process
plot_kmeans(Standard_data, C, history, e_step_indices, m_step_indices)

# Plot the J value over iterations
def plot_j(j_history, e_step_indices, m_step_indices):
    plt.figure(figsize=(10, 10))
    plt.plot(j_history, label='J value')
    
    for e_step in e_step_indices:
        plt.scatter(e_step, j_history[e_step], color='r', label='E-step' if e_step == e_step_indices[0] else "")
    
    for m_step in m_step_indices:
        plt.scatter(m_step, j_history[m_step], color='b', label='M-step' if m_step == m_step_indices[0] else "")
    
    plt.xlabel('Iteration')
    plt.ylabel('J')
    plt.title('J value over iterations')
    plt.legend()
    plt.show()

# Plot the J value over iterations
plot_j(j_history, e_step_indices, m_step_indices)