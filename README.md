
#ex4

in this project we had to create a server which serving clients by solving their find-path in graph problems.

# design

 - the server is waiting for clients to connect and when one clients is trying to connect,
 one of the threads in the thread pool startrs to serve his request,
 by the acceptClient method which is changing by the accept-client object to open method got.
 - the only client handler now is the graph solver.
 - the client has to send "solve find-graph-path (algorithm)" and the client handler will serve him. than the client
 has to send the wanted graph matrix.
when one of the thread finishes his task, he close the socket and returns to the available thread queue.
if more than 20 clients are trying to connect at the same time then they get connection refused.
- there is a queue of free available tasks. every time a task goes in, one of the thread is notified and he 
starts handling the problem.
- the graph solver client handler owns a vector of the possible agorithm-solvers and it chooeses which one
of them it use by the algorithm the client sends. if no algorithm is given then it will solve the problen with A*.

# error statuses

0) successes - all worked, you sould get the solution of the problem you asked for
1) noPath - means that the matrix grpah you sent has no route from the given start point
to the given and one.
2) unsuportedTaskError - means that you send a task which isnt supported by the graph-solver client handler
3) wrongMatrixGraphInput - the matrix you entered is wrong
4) wrongGraph - the values is the matrix you send are wrong (supporting b or >1)
5) unknownErrorOccuerd - unpredictable error occured
6) unvalidInput - the input you entered isnt supported by the client-handler