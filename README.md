Use it as follow, where <hex> the hex encoding of the basic block that you want to profile.
`./test <hex> <reps>`

For instance, you can profile the throughput of `cmpl %ebx, %eax` like this.

$ ./test 39d8 100 # hex code of `cmpl %ebx, %eax` is `39d8`
Core_cyc	L1_read_misses	L1_write_misses	iCache_misses	Context_switches
884	26	-1	1	0
779	0	-1	0	0
729	0	-1	0	0
730	0	-1	0	0
731	0	-1	0	0
730	0	-1	0	0
729	0	-1	0	0
730	0	-1	0	0
729	0	-1	0	0
730	0	-1	0	0
727	0	-1	0	0
730	0	-1	0	0
729	0	-1	0	0
732	0	-1	0	0
729	0	-1	0	0

# Get latency for 200 iterations 
$ ./test t.bin 200
Core_cyc	L1_read_misses	L1_write_misses	iCache_misses	Context_switches
1096	43	-1	1	0
775	1	-1	0	0
761	0	-1	0	0
765	0	-1	0	0
763	0	-1	0	0
765	0	-1	0	0
767	0	-1	0	0
764	0	-1	0	0
762	0	-1	0	0
766	0	-1	0	0
764	0	-1	0	0
763	0	-1	0	0
763	0	-1	0	0
765	0	-1	0	0
767	0	-1	0	0

# Core_cyc column reports latency (including measurement overhead) of executing the basic block 100 (200) iterations.
# We calculate the throughput as follow.
$ python <(echo 'print "Throughput:", (762.0 - 729.0)/100')
Throughput: 0.33
```


If you fancy using the harness as a library, just include `harness.h` and link against `harness.a`.
