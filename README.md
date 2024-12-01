# cs375project
Some notes:
- Starts by taking input from an input file in the form "[node] [node] [capacity]"
- For simplicity I have it assume that the first node in the file is the source and the last node in the file is the sink
- While loop that calls "dfs" and then updates maxflow accordingly
- "dfs" finds each augmented path. Should work, but I haven't been able to fully test it since the input isn't working right
- I've been using this to help understand: https://www.w3schools.com/dsa/dsa_algo_graphs_fordfulkerson.php
