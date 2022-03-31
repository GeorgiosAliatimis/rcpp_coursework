stable_matching <- function(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms){
  type <- typeof(proposer_matrix[[1]][1])
  if (type == "double"){
    return(stable_matching_int(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms))
  } else if (type == "character"){
    return(stable_matching_string(proposer_matrix,proposer_syms,acceptor_matrix,acceptor_syms))
  } else {
    print("Unsupported type")
  }
}