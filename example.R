# library(Rcpp)
# sourceCpp("stable_matching.cpp")
library("praxi")
source("./stable_matching.R")

proposer_matrix = list(c("c","b","d","a"), c("b","a","c","d"), c("b","d","a","c"), c("c","a","d","b") )
acceptor_matrix = list(c("A","B","D","C"), c("C","A","D","B"), c("C","B","D","A"), c("B","A","C","D") )
proposer_syms = c("A","B","C","D")
acceptor_syms = c("a","b","c","d")
out = stable_matching(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms)

print(out)

random_stable_matching <- function(n){
    proposer_syms = 1:n
    acceptor_syms = 1:n
    proposer_matrix = list()
    for (i in 1:n) proposer_matrix[[i]] = sample(n)
    acceptor_matrix = list()
    for (i in 1:n) acceptor_matrix[[i]] = sample(n)
    start = Sys.time()
    out = stable_matching_int(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms)
    Sys.time()-start
}

set.seed(1)
# random_stable_matching(10)
#  1  2  3  4  5  6  7  8  9 10 
#  4  1  5  2  9 10  3  8  6  7 
random_stable_matching(n=10)
random_stable_matching(n=100)
random_stable_matching(n=1000)

repetitions = 20
for(n in c(10,100,1000) ) print(mean( sapply( rep(n,repetitions), random_stable_matching ) )) 
# [1] 9.375811e-05
# [1] 0.008967042
# [1] 0.7558424
# [0.0005329424515366554, 0.01679993639700115, 1.5274148536846042]