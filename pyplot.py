#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
import csv

# Extract data
with open("philosophes.csv", "r") as file:
    reader = csv.reader(file)
    data = []
    row_index = 0
    for row in reader:
        if row_index == 0: row_index += 1
        else: data.append(row)
    file.close()

# Refactor data
for i in range(len(data)):
    data[i] = data[i][1::]
    data[i] = [float(x) for x in data[i]]

def mean(data):
    return sum(data) / len(data)

def variance(data):
    moy = mean(data)
    dev = [(x - moy)**2 for x in data]
    return sum(dev) / len(data)

def standard_dev(data):
    var = variance(data)
    return np.sqrt(var)

# Prepare arrays for plotting
X = np.arange(1, 13)
Mean = [mean(data[0]), mean(data[1]), mean(data[2]), mean(data[3]), mean(data[4]), mean(data[5]), mean(data[6]), mean(data[7]), mean(data[8]), mean(data[9]), mean(data[10]), mean(data[11])]
StDev = [standard_dev(data[0]), standard_dev(data[1]), standard_dev(data[2]), standard_dev(data[3]), standard_dev(data[4]), standard_dev(data[5]), standard_dev(data[6]), standard_dev(data[7]), standard_dev(data[8]), standard_dev(data[9]), standard_dev(data[10]), standard_dev(data[11])]

fig1 = plt.figure()
plt.plot(X, Mean, color="blue", linewidth=1.0, linestyle="solid", marker="o")
plt.plot(X, StDev, color="red", linewidth=1.0, linestyle="solid", marker="o")

plt.xlim(1, 5)
plt.xticks(np.linspace(1, 5, 5))
plt.xlabel('Nombre d\'exécutions')

plt.ylim(0, 40)
plt.yticks(np.linspace(0, 40, 6))
plt.ylabel('Temps (sec)')

plt.title('Temps moyen de compilation et écart-type avec X threads pour le problème des philosophes')
plt.grid(True)
plt.legend(['Moyenne', 'Écart-type'], loc='upper right')

plt.savefig("Performances_Philosophes.png")

plt.show()

plt.close()
