
import numpy as np

# Answer of Question1
def generate_dataset(mean, cov, n, Y):
    X = []
    for i in range(n):
        if Y[i] == 0:
            X.append(np.random.multivariate_normal(mean[0], cov[0]))
        elif Y[i] == 1:
            X.append(np.random.multivariate_normal(mean[1], cov[1]))
            
    return np.array(X)


mean0 = list(map(float, input("Enter the mean vector0 (comma separated): ").split(',')))

mean1 = list(map(float, input("Enter the mean vector1 (comma separated): ").split(',')))

cov0 = np.random.rand(len(mean0), len(mean0))
cov0 = np.dot(cov0, cov0.transpose())

cov1 = np.random.rand(len(mean1), len(mean1))
cov1 = np.dot(cov1, cov1.transpose())

n = int(input("Enter the number of points: "))

Y = np.random.choice([0, 1], n, p=[0.5, 0.5])

mean = [mean0, mean1]
cov = [cov0, cov1]

dataset = generate_dataset(mean, cov, n, Y)
print("Generated dataset:")
print(dataset)

#Answer of Question2
def bayes_classifier_single(dataset, Y, cov):
    X1 = dataset[:, 0]

    # Calculate the mean and variance for each class
    mean_0 = np.mean(X1[Y == 0])
    var_0 = cov[0][0, 0]   
    mean_1 = np.mean(X1[Y == 1])
    var_1 = cov[1][0, 0]

    # Calculate the prior probabilities
    prior_0 = 0.5
    prior_1 = 0.5

    # Function to calculate the Gaussian probability density function
    def gaussian_pdf(x, mean, var):
        if var > 0:
            return (1.0 / np.sqrt(2 * np.pi * var)) * np.exp(-((x - mean) ** 2) / (2 * var))
        else:
            return 0

    # Make predictions
    Y_pred = []
    for x in X1:
        prob_0 = gaussian_pdf(x, mean_0, var_0) * prior_0
        prob_1 = gaussian_pdf(x, mean_1, var_1) * prior_1
        Y_pred.append(0 if prob_0 > prob_1 else 1)

    Y_pred = np.array(Y_pred)
    # Compute the empirical classification error (0-1 loss)
    error = np.mean(Y_pred != Y)
    print("Empirical classification error (0-1 loss):", error)

#Answer of Question3
def bayes_classifier_double(dataset, Y, cov):
    X = dataset[:, :2]  # Use the first two features

    # Slice the covariance manp.trices to match the used features
    cov_0 = cov[0][:2, :2]
    # print(cov_0)
    cov_1 = cov[1][:2, :2]


    mean_0 = np.mean(X[Y == 0], axis=0)
    mean_1 = np.mean(X[Y == 1], axis=0)

    # Calculate the prior probabilities
    prior_0 = 0.5
    prior_1 = 0.5

    # Function to calculate the multivariate Gaussian PDF
    def multivariate_gaussian_pdf(x, mean, cov):
        size = len(x)
        det = np.linalg.det(cov)
        norm_const = 1.0 / (np.power((2 * np.pi), float(size) / 2) * np.sqrt(det)) if det > 0 else 0
        x_mu = x - mean
        inv = np.linalg.inv(cov)
        result = np.exp(-0.5 * (np.dot(np.dot(x_mu, inv), x_mu.T)))
        return norm_const * result

    # Make predictions
    Y_pred = []
    for x in X:
        prob_0 = multivariate_gaussian_pdf(x, mean_0, cov_0) * prior_0
        prob_1 = multivariate_gaussian_pdf(x, mean_1, cov_1) * prior_1
        Y_pred.append(0 if prob_0 > prob_1 else 1)

    # Compute the empirical classification error (0-1 loss)
    error = np.mean(Y_pred != Y)
    print("Empirical classification error (0-1 loss):", error)

#Answer of Question4
def bayes_classifier_triple(dataset, Y, cov):
    X = dataset[:, :3]  # Use the first three features

    # Slice the covariance matrices to match the used features
    cov_0 = cov[0][:3, :3]
    cov_1 = cov[1][:3, :3]

    # Calculate the mean and covariance matrix for each class
    mean_0 = np.mean(X[Y == 0], axis=0)
    mean_1 = np.mean(X[Y == 1], axis=0)

    # Calculate the prior probabilities
    prior_0 = 0.5
    prior_1 = 0.5

    # Function to calculate the multivariate Gaussian probability density function
    def multivariate_gaussian_pdf(x, mean, cov):
        size = len(x)
        det = np.linalg.det(cov)
        norm_const = 1.0 / (np.power((2 * np.pi), float(size) / 2) * np.sqrt(det))
        x_mu = x - mean
        inv = np.linalg.inv(cov)
        result = np.exp(-0.5 * (np.dot(np.dot(x_mu, inv), x_mu.T)))
        return norm_const * result

    # Make predictions
    Y_pred = []
    for x in X:
        prob_0 = multivariate_gaussian_pdf(x, mean_0, cov_0) * prior_0
        prob_1 = multivariate_gaussian_pdf(x, mean_1, cov_1) * prior_1
        Y_pred.append(0 if prob_0 > prob_1 else 1)

    # Compute the empirical classification error (0-1 loss)
    error = np.mean(Y_pred != Y)
    print("Empirical classification error (0-1 loss):", error)


print("Bayes classifier for single feature:")
bayes_classifier_single(dataset, Y, cov)

print("Bayes classifier for double feature:")
bayes_classifier_double(dataset, Y, cov)

print("Bayes classifier for triple feature:")
bayes_classifier_triple(dataset, Y, cov)

listp = [500,1000,1500,2000,2500,3000,3500,4000,4500,5000]
for i in listp:
    Y = np.random.choice([0, 1], i, p=[0.5, 0.5])
    dataset = generate_dataset(mean, cov, i, Y)
    print("Number of points: ", i)
    print("Bayes classifier for single feature:")
    bayes_classifier_single(dataset, Y, cov)

    print("Bayes classifier for double feature:")
    bayes_classifier_double(dataset, Y, cov)

    print("Bayes classifier for triple feature:")
    bayes_classifier_triple(dataset, Y, cov)
    print("\n")
    if i%1000 == 0:
        print("---------------------------------------------------")
        print("\n")
        
#Answer of Question5

# The error decreases as the number of features is increased, in most of the cases. Adding more feature increases the accuracy if the classifier and makes it more probable to assign it to the correct class. Similarly, increasing the value of n also decreases the error as the larger dataset evens out any possible irregularities(deviations). We may have a case when the error might increase as the number of features increase, for large number of features, but here, this is apparently not the case.

#Answer of Question6

def classifier_with_closest_mean_feature(mean, dataset, cov):
    X1 = dataset[:, :1]
    mean1 = np.mean(X1)

    X2 = dataset[:, 1:2]
    mean2 = np.mean(X2)

    X3 = dataset[:, 2:3]
    mean3 = np.mean(X3)

    # compare mean[0] with mean1, mean2 and mean3 and assign the closest mean to mean[0]
    dist1 = np.linalg.norm(mean[0] - mean1)
    dist2 = np.linalg.norm(mean[0] - mean2)
    dist3 = np.linalg.norm(mean[0] - mean3)

    # Select the features with the closest mean
    if dist1 < dist2 and dist1 < dist3:
        X = dataset[:, :1]
        cov = cov[:1, :1]
    elif dist2 < dist1 and dist2 < dist3:
        X = dataset[:, 1:2]
        cov = cov[1:2, 1:2]
    else:
        X = dataset[:, 2:3]
        cov = cov[2:3, 2:3]

    errir = bayes_classifier_single(X, Y, cov)
    print("Classification error for closest mean feature:", errir)