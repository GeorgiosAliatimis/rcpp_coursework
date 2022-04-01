# rcpp_coursework

To install the praxi package, run the following lines in R
```
install.packages("devtools") 
install_github("GeorgiosAliatimis/rcpp_coursework") 
library("praxi")
```
Below is a testcase for the ```stable_matching``` function, which returns a matching. 
The test case and the fundamental algorithm are taken from the second chapter of [Stable Marriage and Its Relation to Other Combinatorial Problems](https://ebookcentral.proquest.com/lib/lancaster/detail.action?docID=4908424).
```
proposer_matrix = list(c("c","b","d","a"), c("b","a","c","d"), c("b","d","a","c"), c("c","a","d","b") )
acceptor_matrix = list(c("A","B","D","C"), c("C","A","D","B"), c("C","B","D","A"), c("B","A","C","D") )
proposer_syms = c("A","B","C","D")
acceptor_syms = c("a","b","c","d")
matching = stable_matching(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms)
print(matching)
```
The solution matches the one from the book referenced above. 

|Proposer| A | B | C | D |
| -------| - | - | - | - |
|Acceptor| d | a | b | c |

We now write a function that randomly generates the preference lists of n proposers and n acceptors and outputs the time it takes to run. 
```
random_stable_matching <- function(n){
    proposer_syms = 1:n
    acceptor_syms = 1:n
    proposer_matrix = list()
    for (i in 1:n) proposer_matrix[[i]] = sample(n)
    acceptor_matrix = list()
    for (i in 1:n) acceptor_matrix[[i]] = sample(n)
    stable_matching(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms)
}
```
We try it with n=10 

```
set.seed(1)
random_stable_matching(n=10)
```

|Proposer| 1 | 2 | 3 | 4 | 5 |  6| 7 | 8 | 9 |10 |
| -------| - | - | - | - | - | - | - | - | - | - |
|Acceptor| 4 | 1 | 5 | 2 | 9 | 10| 3 | 8 | 6 | 7 |

## Time performance

To find the time performance of ```stable_matching``` we run 

```
for (N in c(10,100,1000)){
    start = Sys.time()
    out = stable_matching(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms)
    print(Sys.time()-start)
}
```

|                |  N=10   |  N = 100 | N = 1000 |
| -------------- |  -----  | -------- | -------- |
| Rcpp           |  9e-5 s | 9e-3   s | 0.8  s   |
| Python         |  5e-4 s | 0.02  s  | 1.5   s  |

Clearly Rcpp is faster than the Python implementation; across those three test cases considered the former is twice as fast as the latter.

## 5 Rs 

| Implementation | Re-run | Repeat | Reproduce | Reuse | Replicate |
| ---------------| ------ | -----  | ----------| ------| --------- |
|     Python     |    2   |    3   |      4    |    4  |     0     | 
|      Cpp       |    4   |    4   |      2    |    2  |     0     | 
|     Rcpp       |    3   |    3   |      3    |    2  |     0     | 

1. <b> Re-runnable</b>; cpp is easily re-runnable because only standard libraries have been used (which are typically stable and rarely change). Unlike python, Cpp is also backwards compatible so any version higher than cpp17 should also work. Rcpp is slightly less re-runnable because of potential updates in the Rcpp library (marshalling in particular). For python, some care has been taken in clearly stating all the packages that have been used through a requirements text file and a yaml environomnet file.
3. <b> Repeatable</b>; cpp is more repeatable, partially because we did not consider random test cases. In Rcpp and Python we set the seed for the random test cases.
4. <b> Reproducible</b>; a few assertions have been considered in all implementations, but more of them in the python projects and fewer on the cpp assignment.
6. <b> Reusable</b>; a lot of documentation was given on the python coursework and relatively little on the other two assignments.
7. <b> Replicable</b>; no consideration has been given to replicability in any of the three implementations. 
