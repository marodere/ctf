#!/usr/bin/env python2.7

class TGraph:
    def __init__(self):
        pass

    def read(self, fd):
        self.edges = [ ]
        self.nodeEdges = [ ]
        (self.N, self.M) = [int(x) for x in fd.readline().split(' ')]
        self.edges = [[int(x) for x in fd.readline().split(' ')] for i in range(self.M)]
        for i in range(self.N+1):
            self.nodeEdges.append([ ])
        for e in self.edges:
            self.nodeEdges[e[0]].append([e[1], e[2]])
            self.nodeEdges[e[1]].append([e[0], e[2]])

    def reset_marks(self):
        self.nodeMarked = [False for x in range(self.N+1)]

    def mark_subgraph(self, nodeStart, threshold):
        queue = [nodeStart]
        queueTotalLen = 1
        self.nodeMarked[nodeStart] = True
        while len(queue) > 0:
            n = queue.pop(0)
            for e in self.nodeEdges[n]:
                if e[1] > threshold and not self.nodeMarked[e[0]]:
                    self.nodeMarked[e[0]] = True
                    queueTotalLen += 1
                    queue.append(e[0])
        return queueTotalLen

    def get_subgraphs_sizes(self, threshold):
        self.reset_marks()
        for n in range(1, self.N+1):
            if not self.nodeMarked[n]:
                yield self.mark_subgraph(n, threshold)

    def get_edges_weights(self):
        return [x[2] for x in self.edges]

def fit_subset(fP, weightNeed):
    sumAvailable = [x == 0 for x in range(weightNeed+1)]
    for f in fP:
        newSumAvailable = [x for x in sumAvailable]
        for w in range(1, weightNeed + 1):
            if w - f >= 0:
                newSumAvailable[w] = sumAvailable[w] or sumAvailable[w - f]
        sumAvailable = newSumAvailable
    return sumAvailable[weightNeed]

def solve(fd):
        g = TGraph()
        g.read(fd)
        edgesWeights = [0] + g.get_edges_weights()
        edgesWeights.sort(reverse=True)
        l = 0
        r = len(edgesWeights)
        while r - l > 1:
            m = l + (r - l) // 2
            ew = edgesWeights[m]
            if fit_subset(g.get_subgraphs_sizes(ew), g.N//2):
                l = m
            else:
                r = m
        return edgesWeights[l]

def main():
    ans = [solve(open(network, 'r')) for network in ['network1.txt', 'network2.txt', 'network3.txt', 'network4.txt']]
    print 'ssctf{%s}' % ';'.join((str(a) for a in ans))

if __name__== "__main__":
    main()
