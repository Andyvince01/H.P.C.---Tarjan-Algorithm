import numpy as np
import struct

# Crea una matrice di adiacenza di dimensione 10000x10000
adj_matrix = np.zeros((500,500), dtype=np.uint8)
for i in range(500):
    k = np.random.randint(0,500)
    j = np.random.randint(0,500)
    adj_matrix[k][j] = 1
#adj_matrix = np.random.choice([0, 1], size=(1000,1000), p=[0.9999, 0.0001])
# Salva la matrice di adiacenza su file
with open("graph.txt", "w") as fp:
    # Scrivi il numero di vertici nel file
    ##fp.write(len(adj_matrix))
    # Scrivi la matrice di adiacenza nel file
    for row in adj_matrix:
        for item in row:
            fp.write(str(item))
            fp.write(" ")
        fp.write("\n")
with open("graph.bin", "wb") as fp:
    # Scrivi il numero di vertici nel file
    fp.write(struct.pack("i", len(adj_matrix)))
    # Scrivi la matrice di adiacenza nel file
    for row in adj_matrix:
        fp.write(struct.pack("i" * len(row), *row))

'''for i in range(500):
    print("Riga",i)
    for j in range(500):
        print(adj_matrix[i][j])'''