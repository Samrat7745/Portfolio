import numpy as np
from sklearn.datasets import fetch_openml
from collections import Counter
from scipy.spatial import distance

mnist = fetch_openml('mnist_784')

X, y = np.array(mnist["data"]), np.array(mnist["target"])

#Question 1

# Convert the data to binary
def convert_to_binary(data):
  data[data>0] = 1
  return data

print(X[0])
X_binary = convert_to_binary(X)
print(X_binary[0])

# Question 2

# Split the data into training and test sets
def datasplit(X,Y):
  x_size , n = X.shape
  X_train, X_test = X[:int(0.8*x_size),:], X[int(0.8*x_size):,:]
  Y_train, Y_test = Y[:int(0.8*x_size)], Y[int(0.8*x_size):]
  return X_train, X_test, Y_train, Y_test

D_train, D_test, Y_train, Y_test = datasplit(X_binary,y)
print(D_train.shape)
print(D_test.shape)
print(Y_train.shape)
print(Y_test.shape)


# Question 3

# Create and train the Bernoulli Naive Bayes classifier
def fit_bernoulli_nb(X, y, alpha=1.0, beta=1.0):
    unique_classes = np.unique(y)  # Array of unique class labels
    class_counts = np.zeros(len(unique_classes))  # Array to count instances of each class
    feature_counts = np.zeros((len(unique_classes), X.shape[1]))  # Array to count feature occurrences for each class
    log_class_priors_prob = np.zeros(len(unique_classes))  # Array to store log prior probabilities of each class
    log_feature_probs = np.zeros((len(unique_classes), X.shape[1]))  # Array to store log probabilities of features given class

    for idx, cls in enumerate(unique_classes):
        X_cls = X[y == cls]
        class_counts[idx] = X_cls.shape[0]
        feature_counts[idx] = X_cls.sum(axis=0) + alpha
        log_class_priors_prob[idx] = np.log(class_counts[idx] / X.shape[0])
        log_feature_probs[idx] = np.log(feature_counts[idx] / (class_counts[idx] + alpha + beta))

    return unique_classes, log_class_priors_prob, log_feature_probs

def predict_bernoulli_nb(X, unique_classes, log_class_priors_prob, log_feature_probs):

    joint_log_likelihood = np.dot(X, log_feature_probs.T) + np.dot(1 - X, np.log(1 - np.exp(log_feature_probs)).T)
    joint_log_likelihood += log_class_priors_prob
    return unique_classes[np.argmax(joint_log_likelihood, axis=1)]

unique_classes, log_class_priors_prob, log_feature_probs = fit_bernoulli_nb(D_train, Y_train, alpha=1.0)

# Question 4

# Predict on the test set
y_pred = predict_bernoulli_nb(D_test, unique_classes, log_class_priors_prob, log_feature_probs)

# Calculate the empirical 0-1 loss
def Empirical_zero_one_loss(y_true, y_pred):
    return np.mean(y_true != y_pred)

emprirical_loss = Empirical_zero_one_loss(Y_test, y_pred)
print(f'Empirical 0-1 loss of Dtest : {emprirical_loss}')

#Question 5

# Define the set of values for a and b
a_values = [1, 5, 10, 20, 80, 100, 1000]
b_values = [1, 5, 10, 20, 80, 100, 1000]

# Initialize variables to store the best parameters and the corresponding loss
best_a = None
best_b = None
best_loss = float('inf')


# Implement KFold cross-validation from scratch
def custom_kfold_split(X, y, n_splits=5, shuffle=True, random_state=None):
    if shuffle:
        np.random.seed(random_state)
        indices = np.random.permutation(len(X))
    else:
        indices = np.arange(len(X))
    
    fold_sizes = np.full(n_splits, len(X) // n_splits, dtype=int)
    fold_sizes[:len(X) % n_splits] += 1
    current = 0
    folds = []
    for fold_size in fold_sizes:
        start, stop = current, current + fold_size
        folds.append(indices[start:stop])
        current = stop
    return folds

# Perform 5-fold cross-validation using custom KFold
folds = custom_kfold_split(D_train, Y_train, n_splits=5, shuffle=True, random_state=42)

for a in a_values:
    for b in b_values:
        fold_losses = []
        for i in range(5):
            val_index = folds[i]
            train_index = np.hstack([folds[j] for j in range(5) if j != i])
            
            X_train_fold, X_val_fold = D_train[train_index], D_train[val_index]
            y_train_fold, y_val_fold = Y_train[train_index], Y_train[val_index]
            
            # Train the Bernoulli Naive Bayes classifier with current a and b
            unique_classes, log_class_priors_prob, log_feature_probs = fit_bernoulli_nb(X_train_fold, y_train_fold, alpha=a, beta=b) 
            
            # Predict on the validation set
            y_val_pred = predict_bernoulli_nb(X_val_fold, unique_classes, log_class_priors_prob, log_feature_probs)
            
            # Calculate the empirical 0-1 loss for the current fold
            fold_loss = Empirical_zero_one_loss(y_val_fold, y_val_pred)
            fold_losses.append(fold_loss)
        
        # Calculate the average loss across all folds
        avg_loss = np.mean(fold_losses)
        
        # Update the best parameters if the current average loss is lower than the best loss
        if avg_loss < best_loss:
            best_loss = avg_loss
            best_a = a
            best_b = b

print(f'Best a: {best_a}, Best b: {best_b}, Best empirical 0-1 loss: {best_loss}')

# Train the classifier with the best parameters on the entire training set
unique_classes, log_class_priors_prob, log_feature_probs = fit_bernoulli_nb(D_train, Y_train, alpha=best_a)

# Predict on the test set with the best parameters
y_pred_best = predict_bernoulli_nb(D_test, unique_classes, log_class_priors_prob, log_feature_probs)

# Calculate the empirical 0-1 loss on the test set with the best parameters
empirical_loss_best = Empirical_zero_one_loss(Y_test, y_pred_best)
print(f'Empirical 0-1 loss on D_test with best parameters: {empirical_loss_best}')

#Question 6

def knn_predict(X_train, y_train, X_test, k):
    y_pred = []
    for x in X_test:
        distances = [distance.euclidean(x, x_train) for x_train in X_train]
        k_indices = np.argsort(distances)[:k]
        k_nearest_labels = [y_train[i] for i in k_indices]
        most_common = Counter(k_nearest_labels).most_common(1)
        y_pred.append(most_common[0][0])
    return np.array(y_pred)

# Evaluate KNN for K=3
k = 3
y_pred_knn_3 = knn_predict(D_train[:8000], Y_train[:8000], D_test[:2000], k)
empirical_loss_knn_3 = Empirical_zero_one_loss(Y_test[:2000], y_pred_knn_3)
print(f'Empirical 0-1 loss on D_test with K={k}: {empirical_loss_knn_3}')

# Evaluate KNN for K=5
k = 5
y_pred_knn_5 = knn_predict(D_train[:8000], Y_train[:8000], D_test[:2000], k)
empirical_loss_knn_5 = Empirical_zero_one_loss(Y_test[:2000], y_pred_knn_5)
print(f'Empirical 0-1 loss on D_test with K={k}: {empirical_loss_knn_5}')