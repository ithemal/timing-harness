Use it as follow, where <binary> is a file containing the binary of basic block you want to profile (note that we literally the binary, not an object file).
`./test <binary> <reps>`

For instance, you can profile the throughput of `addq %(rbx), %rax` like this.
```
# Get the binary
$ echo 'addq (%rbx), %rax' > t.s
$ as t.s -o t.o
$ objcopy -j .text t.o t.bin -O binary
$ ./test t.bin 100
Core_cyc	L1_read_misses	L1_write_misses	iCache_misses	Context_switches
875	21	-1	9	0
766	0	-1	0	0
776	0	-1	1	0
764	0	-1	0	0
768	0	-1	0	0
766	0	-1	0	0
767	0	-1	0	0
767	0	-1	0	0
766	0	-1	0	0
766	0	-1	0	0
767	0	-1	0	0
765	0	-1	0	0
766	0	-1	0	0
765	0	-1	0	0
764	0	-1	0	0
$ ./test t.bin 200
Core_cyc	L1_read_misses	L1_write_misses	iCache_misses	Context_switches
991	24	-1	5	0
897	1	-1	0	0
905	0	-1	0	0
865	0	-1	0	0
867	0	-1	0	0
868	0	-1	0	0
864	0	-1	0	0
865	0	-1	0	0
867	0	-1	0	0
867	0	-1	0	0
865	0	-1	0	0
868	0	-1	0	0
867	0	-1	0	0
867	0	-1	0	0
867	0	-1	0	0
# Core_cyc column reports latency (including measurement overhead) of executing the basic block 100 (200) iterations.
# We calculate the throughput as follow.
$ python <(echo 'print "Throughput:", (867.0 - 764.0)/100')
Throughput: 1.03
```


If you fancy using the harness as a library, just include `harness.h` and link against `harness.a`.
