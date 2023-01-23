import sys
import numpy as np

rows = 42 # no of departments
cols = 42 # no of departments
sender_list = [] # sender dept id 
reciver_list  = []  # reciver dept id

for line in sys.stdin:
    
    line = line.strip("\n\r")
    edge1 , edge2  = line.split(" ")

    edge1 = int(edge1.strip())
    edge2 = int(edge2.strip())
    sender_list.append(edge1)  # making list for edge relation ship - edge 1
    reciver_list.append(edge2) # making list for edge relation ship - edge 2
 
matrix =  [[0 for x in range(rows)] for y in range(cols)]  # 42 X 42 matrix

# updating the adjacency matrix as per edge list
for i in range(len(sender_list)):                  
    matrix[sender_list[i]][reciver_list[i]] = 1
    matrix[reciver_list[i]][sender_list[i]] = 1

 # matrix multiplication using numpy
one_hop_mat = np.dot(matrix,matrix) 


# printing upper triangular matric for the comman nodes relation ship
# printing for all pair only once 
for i in range(len(one_hop_mat)):   
    for j in range(len(one_hop_mat[0])):
        if i < j and one_hop_mat[i][j] > 0:
            print(i, j, one_hop_mat[i][j])

