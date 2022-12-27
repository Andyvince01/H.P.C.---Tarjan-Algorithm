# H.P.C. - Tarjan Algorithm in MPI

<b>Goal -</b> Provide a parallel version of the Tarjan's algorithm to find Strongly Connected Components in a Graph. The implementation MUST use a message passing paradigm, and has to be implemented by using MPI. Students MUST store and load the input graph FROM FILES. The whole graph MUST be distributed on files on each node (i.e.: the whole graph cannot be stored on a single (even replicated) file). Good Graph dimensions are greater than 4GB of data. Students have to choose the proper data structure to represent the graph in memory
