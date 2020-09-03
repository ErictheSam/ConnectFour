<center><font size='6'>Connect Four</font></center>

<center>Yibo Shen CS76</center>

### Core Algorithm:

Generally, I used Monte Carlo method and UCT algorithm to complete this agent. By constructing UCT tree ,randomly simulate the process and calculating the values, the agent automatically finds out the best move.

For each move, the agent sets the root of instant UCT tree as the current status and set the children nodes as the every legal move from the current status.

For each simulation, the agent select a situation by expanding a new child node if possible, or select the best-performed child node if full, until the last node or a expandable one. Then it randomly generates the match from that very situation.

The scoring rule is:
$$
Score(v) = \frac{S(v)}{N(v)} + \sqrt{\frac{2ln(N(u))}{N(v)}}
$$
where u is the current node ,v is a child node, S(v) is the difference between the winning times and losing times. N(u) and N(v) are the selected times of u and v.

At last , select the best performed child node as the new move.

### Experiment Result

I got a 90% percent winning rate with the best ranking strategy under a i5-8500 3GHz  CPU.