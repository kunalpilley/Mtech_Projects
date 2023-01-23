import sys
import numpy as np

# implementing dijakstra's algo 
def dijkstra(matrix, n, source):
    dist = [float('inf') for i in range(n)]
    dist[source] = 0
    visited_node = [False for i in range(n)]
    visited_node[source] = True
    for i in range(n):
        for j in range(n):
            if matrix[i][j] != 0 and dist[i] > dist[j] + matrix[j][i]:
                dist[i] = dist[j] + matrix[j][i]
    return dist


rows = 42 # no of departments
cols = 42 # no of departments
sender_list = [] # sender dept id 
reciver_list  = []  # reciver dept id

weight_matrix =  [[0 for x in range(rows)] for y in range(cols)]  # 42 X 42 matrix

for line in sys.stdin:
    # print(repr(line))
    line = line.strip("\n\r")
    edge1 , edge2  = line.split(" ")

    edge1 = int(edge1.strip())
    edge2 = int(edge2.strip())

    # populating matrics with the ege weight as per the no of correspondece 
    weight_matrix[edge1][edge2] += 1
    weight_matrix[edge2][edge1] += 1


# finding the maximum and minimum degree
minimum_sum = float('inf')
min_dept = -1
max_sum = -1
max_dept = -1
list_with_degree = []

for i in range(len(weight_matrix)):
    sum = 0
    for col_val in weight_matrix[i]:
        if col_val > 0:
            sum += 1

    list_with_degree.append(sum)

    # get the minimum degree 
    if minimum_sum > sum:
        minimum_sum = sum

    # get the maximum degree   
    if max_sum < sum:
        max_sum = sum
        max_dept = i
        

# to get all the nodes with minimum degree to be picked up as destination 
destination_nodes = []
for i in range(len(list_with_degree)):
    if list_with_degree[i] == minimum_sum:
        destination_nodes.append(i)

# calculating single source shortest path and the finding  
# the path weight/ cost with max and min degree nodes 

for destination in destination_nodes:
    dist = dijkstra(weight_matrix, 42, int(destination))
    print(max_dept, destination, dist[int(max_dept)])


