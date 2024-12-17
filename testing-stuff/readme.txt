testcase1.txt
    Description:
        - Arrivals are clustered to arrive every 10 time units and have varying burst times that are long enough to still be executing when the next cluster arrives. Priorities vary to illustrate differences in priority and non-priority algorithms.

    Use Case:
        - Should help test how the algorithms can handle clustered arrivals and overlap of current completion. Should also show differences in priority assignment causes. 

testcase2.txt
    Description:
        - Processes arrive every 5 time units with varying, although reasonably long burst times. May simulate heavier computational workloads. 

    Use Case:
        - Should help illustrate how algorithms handle processes that arrive at regular intervals. Priority versions should help to illustrate the variance in execution that priority levels cause. 

testcase3.txt
    Description:
        - Continuous arrival with a wide variety of burst times and varying priorities.

    Use Case:
        - Should help emphasize how burst time vs priority influences algorithm execution. 

testcase4.txt
    Description:
        - Continuous arrival with varying but relatively short burst times.

    Use Case:
        - Should help check out burst time vs priority but under tighter constraints.

testcase5.txt
    Description:
        - Mix of clustered and continuous process arrivals with varying burst times and priority levels.

    Use Case:
        - Should test how different algorithms handle this while maintaining performance for individual arrivals. Priority versions should help to illustrate the variance in execution that priority levels cause. 