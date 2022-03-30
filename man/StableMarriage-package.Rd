\name{StableMarriage}
\alias{StableMarriage}
\alias{StableMarriage}
\docType{package}
\title{
  The Stable Marriage Algorithm
}
\description{
  Implementation of the Stable Marriage Algorithm in C++ for Usage in R. The algorithm takes n men and n women, with corresponding tables of preferences then allocates matches between the men and women.
}
\details{
  The package contains a single function which takes a DataFrame of respective preference tables, each column contains the name of the person as a column header, then column items of the oredered preference of that person. The output of the algorithm is a List of matchings between the men and women. Preference table size is limited only by computation time one is willing to wait. The algorithm runs at O(n squared) complexity.
}
\author{
Ben Lowery.

Maintainer: Ben Lowery <b.lowery@lancaster.ac.uk>
}
\references{
 Donald, E. Knuth. Stable Marriage and Its Relation to Other Combinatorial Problems : An Introduction to the Mathematical Analysis of Algorithms, American Mathematical Society, 1996. 

}
\keyword{ package }
\examples{
  # n=4 stable marriage example
  male<-data.frame("A" = c("c", "b", "d", "a"), "B" = c("b", "a", "c", "d"), "C" = c("b", "d", "a", "c"), "D" = c("c", "a", "d", "b"), stringsAsFactors = FALSE)
  female<-data.frame("a" = c("A", "B", "D", "C"), "b" = c("C", "A", "D", "B"), "c" = c("C", "B", "D", "A"), "d" = c("B", "A", "C", "D"), stringsAsFactors = FALSE)
  
  res<-StableMarriageAlgo(male, female)
  
  # Print results
  for (val in res) {
    cat(val[1] ,"marries", val[2], "\n")
  }
}
