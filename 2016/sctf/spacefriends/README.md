# Space Friends
We have a set of N friends, and M pairwise connections between friends. Each connection goes both ways, so if, for example, Aariss Weiron is friends with Bendelman, then Bendelman is also friends with Aariss Weiron. Each connection also has an associated weight, representing how close the two friends are, which is a positive integer value. No two pairs of friends with a connection have the same weight.

Our objective is to somehow divide our set of friends into two 'equal' halves. We also want to put friends who are pretty close to each other into the same set. We formally define the problem as follows:

We have a set of friends of even size, some with bi-directional pairwise connections. Each connection has a positive integer weight, and each connection has a distinct weight. Our objective is to divide our friends into two sets of equal size with this property: The largest connection-weight that crosses between the two sets should be as small as possible. Compute this connection-weight for each friend network given. A crossing connection is one such that the two friends it connects are in different sets. Each friend network is in the following format: The first line has two numbers:

N (guaranteed to be even) and M (number of connections). M lines follow this line. Each one represents a connection and has three tokens. The first two are the friends it connects, and the last one is the weight of the connection.

## Dividing graph
Pick some integer `ew > 0`. Assume that `ew` is the largest connection-weight that crosses between the two halfs. Notice that if threr's an edge greater than `ew` between two nodes, both should belong to the same half. Therefore, if there's a path between two nodes consisting edges greater than `ew`, both should belong to the same half.

Remove all edges but greater than `ew` from the graph. The graph got splitted into a few connected subgraphs, each containing one or more nodes. So with `ew = 0` no edge will be removed - it will be the original graph. With `ew` greater than the greatest edge, all edges will be removed, so all nodes will be disconnected and N subgraphs should be there.

Every such subgraph should entirely belong to the one half.  we can split the graph into two equal halves with the largest connection-weight `ew` if and only if we there's some set of the subgraphs that contains exactly N/2 nodes.

## Subset sum
How to find out is thre're such set? We'll find out an answer using dynamic programming. Let `freeParts[i]` be a count of nodes in the `i`'s subgraph and try to score `w` nodes.
First of all, suppose that we're able to score 0 nodes using no subgraphs.
Next,
- If we're able to score `w` nodes using subgraphs `1..i-1`, then we're able to score `w` nodes even with subgraph `i` - we just not required to include it.
- If we're able to score `w-freeParts[i]` nodes using subgraphs `1..i-1`, then we're able to score `w` nodes using subgraph `i` by adding it to the set.
By applying this formula for `w` from 0 to N/2 and for all subgraphs we arrive to the answer.

## So
Find out the answer binary searching by `ew`.

```
./friends.py 
ssctf{43226;9474;40749;750}

```
