import numpy as np
import matplotlib.pyplot as plt

def generate_dataset(n):
    X = np.random.randn(n,2)
    Y = np.random.randint(0,2,size = n)
    return X,Y

def generate_hyperplanes(n=5):
    H = []
    for i in range(n):
        w= np.random.randn(2)
        b= np.random.randn()
        H.append((w,b))
    return H

def empirical_risk(h,X,Y):
    w,b = h
    predictions = (np.dot(X,w)+b) >=0
    predictions = predictions.astype(int)
    return np.mean(predictions != Y)

def approximating_true_risk(h,D=100,n_large=1000):
    R= []
    for i in range(D):
        X,Y = generate_dataset(n_large)
        Z=empirical_risk(h,X,Y)
        R.append(Z)
    return np.mean(R)

def Convergence_probability(H,Rh,nSet,epsilon = 0.1,D=100):
    Convergence_probs=[]
    for i in range(len(H)):
        probs = []
        for j in nSet:
            ps = []
            for k in range(D):
                X,Y = generate_dataset(j)
                e_risk = empirical_risk(H[i],X,Y)
                dif = abs(Rh[i]-e_risk)
                ps.append(dif>epsilon)
            ps = np.array(ps)
            ps.astype(int)
            probs.append(np.mean(ps))
        Convergence_probs.append(probs)
    return Convergence_probs

def uniform_convergence_probability(H, Rh, nSet, epsilon=0.1, D=100):
    convergence_probs = []
    for n in nSet:
        probs = 0
        for j in range(D):
            X, Y = generate_dataset(n)
            for i, h in enumerate(H):
                e_risk = empirical_risk(h, X, Y)
                if abs(e_risk - Rh[i]) >= epsilon:
                    probs += 1
                    break  
        convergence_probs.append(probs/D)
    return convergence_probs

def find_optimal_hypothesis(Sn, H):
    X, Y = Sn
    best_hypothesis = None
    best_risk = float('inf')
    for h in H:
        risk = empirical_risk(h, X, Y)
        if risk < best_risk:
            best_risk = risk
            best_hypothesis = h
    return best_hypothesis

# Function to simulate finding the optimal hypothesis for different dataset sizes
def compare_optimal_hypothesis(H, nSet, D=100):
    optimal_hypotheses = []
    for n in nSet:
        best_hypothesis_n = []
        for _ in range(D):
            Sn = generate_dataset(n)
            best_hypothesis_n.append(find_optimal_hypothesis(Sn, H))
        optimal_hypotheses.append(best_hypothesis_n)
    return optimal_hypotheses

nSet=[10,20,30,40,50,60,70,80,90,100]

H = generate_hyperplanes()
Rh= [approximating_true_risk(h) for h in H]

Probs = Convergence_probability(H,Rh,nSet)
Uniforms = uniform_convergence_probability(H,Rh,nSet)

Ypoints = np.array(Probs)

plt.figure(figsize=(10,6))

for i,row in enumerate(Ypoints):
    plt.plot(nSet,row,label=f'h{i+1}')
plt.plot(nSet,Uniforms,label= 'Uniform Convergence')
plt.xlabel('n (Dataset size)')
plt.ylabel('Probabilites of convergence')
plt.title('Convergence probabilities vs dataset')
plt.legend()
plt.show()
large_dataset = generate_dataset(1000)
h_star = find_optimal_hypothesis(large_dataset, H)
optimal_hypotheses = compare_optimal_hypothesis(H, nSet)
print(f"True optimal hypothesis (h*): {h_star}")
for i, best_hypotheses in enumerate(optimal_hypotheses):
    print(f"Optimal hypothesis for n = {nSet[i]} (hn*): {best_hypotheses[0]}")  # Taking the first one for simplicity
