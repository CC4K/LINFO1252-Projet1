#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
import csv

# Utils
def mean(data):
    return sum(data) / len(data)

def variance(data):
    moy = mean(data)
    dev = [(x - moy)**2 for x in data]
    return sum(dev) / len(data)

def standard_dev(data):
    var = variance(data)
    return np.sqrt(var)

def to_milli(data):
    return [x/1000 for x in data]

# Extract data
with open("philosophes.csv", "r") as file:
    reader = csv.reader(file)
    data_philo = []
    row_index = 0
    for row in reader:
        if row_index == 0: row_index += 1
        else: data_philo.append(row)
    file.close()

with open("producteurs_consommateurs.csv", "r") as file:
    reader = csv.reader(file)
    data_prod_conso = []
    row_index = 0
    for row in reader:
        if row_index == 0: row_index += 1
        else: data_prod_conso.append(row)
    file.close()

with open("lecteurs_ecrivains.csv", "r") as file:
    reader = csv.reader(file)
    data_read_write = []
    row_index = 0
    for row in reader:
        if row_index == 0: row_index += 1
        else: data_read_write.append(row)
    file.close()

# Refactor data
for i in range(7):
    data_philo[i] = data_philo[i][1::]
    data_philo[i] = [float(x) for x in data_philo[i]]
    data_prod_conso[i] = data_prod_conso[i][1::]
    data_prod_conso[i] = [float(x) for x in data_prod_conso[i]]
    data_read_write[i] = data_read_write[i][1::]
    data_read_write[i] = [float(x) for x in data_read_write[i]]

# Prepare arrays for plotting
X = np.arange(7)
Mean_philo = [mean(data_philo[0]), mean(data_philo[1]), mean(data_philo[2]), mean(data_philo[3]), mean(data_philo[4]), mean(data_philo[5]), mean(data_philo[6])]
StDev_philo = [standard_dev(data_philo[0]), standard_dev(data_philo[1]), standard_dev(data_philo[2]), standard_dev(data_philo[3]), standard_dev(data_philo[4]), standard_dev(data_philo[5]), standard_dev(data_philo[6])]
Mean_prod_conso = [mean(data_prod_conso[0]), mean(data_prod_conso[1]), mean(data_prod_conso[2]), mean(data_prod_conso[3]), mean(data_prod_conso[4]), mean(data_prod_conso[5]), mean(data_prod_conso[6])]
StDev_prod_conso = [standard_dev(data_prod_conso[0]), standard_dev(data_prod_conso[1]), standard_dev(data_prod_conso[2]), standard_dev(data_prod_conso[3]), standard_dev(data_prod_conso[4]), standard_dev(data_prod_conso[5]), standard_dev(data_prod_conso[6])]
Mean_read_write = [mean(data_read_write[0]), mean(data_read_write[1]), mean(data_read_write[2]), mean(data_read_write[3]), mean(data_read_write[4]), mean(data_read_write[5]), mean(data_read_write[6])]
StDev_read_write = [standard_dev(data_read_write[0]), standard_dev(data_read_write[1]), standard_dev(data_read_write[2]), standard_dev(data_read_write[3]), standard_dev(data_read_write[4]), standard_dev(data_read_write[5]), standard_dev(data_read_write[6])]

# Plot
fig1 = plt.figure()
plt.plot(X, to_milli(Mean_philo), "o", color="blue", markersize=5, linestyle="solid", linewidth=0.5)
plt.errorbar(X, to_milli(Mean_philo), yerr=to_milli(StDev_philo), linestyle="None", ecolor="red", elinewidth=1.5, markersize=0, capsize=10, fmt="o")
plt.legend(['Moyenne', 'Écart-type'], loc='best')
plt.xlim(1, 6)
plt.xticks(np.arange(7), [1, 2, 4, 8, 16, 32, 64])
plt.xlabel('Nombre de threads')
plt.ylim(0, round(max(to_milli(Mean_philo)) + 0.1, 1))
plt.ylabel('Temps d\'exécution (sec)')
plt.title('Moyenne et écart-type du temps d\'exécution\nde philosophes.c en fonction du nombre de threads')
plt.grid(True)
plt.savefig("philosophes_performances.png", dpi=750)

fig2 = plt.figure()
plt.plot(X, to_milli(Mean_prod_conso), "o", color="blue", markersize=5, linestyle="solid", linewidth=0.5)
plt.errorbar(X, to_milli(Mean_prod_conso), yerr=to_milli(StDev_prod_conso), linestyle="None", ecolor="red", elinewidth=1.5, markersize=0, capsize=10, fmt="o")
plt.legend(['Moyenne', 'Écart-type'], loc='best')
plt.xlim(1, 6)
plt.xticks(np.arange(7), [1, 2, 4, 8, 16, 32, 64])
plt.xlabel('Nombre de threads')
plt.ylim(0, round(max(to_milli(Mean_prod_conso)) + 0.1, 1))
plt.ylabel('Temps d\'exécution (sec)')
plt.title('Moyenne et écart-type du temps d\'exécution\nde producteurs_consommateurs.c en fonction du nombre de threads')
plt.grid(True)
plt.savefig("producteurs_consommateurs_performances.png", dpi=750)

fig3 = plt.figure()
plt.plot(X, to_milli(Mean_read_write), "o", color="blue", markersize=5, linestyle="solid", linewidth=0.5)
plt.errorbar(X, to_milli(Mean_read_write), yerr=to_milli(StDev_read_write), linestyle="None", ecolor="red", elinewidth=1.5, markersize=0, capsize=10, fmt="o")
plt.legend(['Moyenne', 'Écart-type'], loc='best')
plt.xlim(1, 6)
plt.xticks(np.arange(7), [1, 2, 4, 8, 16, 32, 64])
plt.xlabel('Nombre de threads')
plt.ylim(0, round(max(to_milli(Mean_read_write)) + 0.1, 1))
plt.ylabel('Temps d\'exécution (sec)')
plt.title('Moyenne et écart-type du temps d\'exécution\nde lecteurs_ecrivains.c en fonction du nombre de threads')
plt.grid(True)
plt.savefig("lecteurs_ecrivains_performances.png", dpi=750)
plt.show()
