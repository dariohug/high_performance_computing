# Exercise Session 02 #
In this session, you will compile and run your first programs on Eiger. This directory contains two versions of the code that were parallelized using MPI or OpenMP.
*Solutions Execrise 2* 

1. Module list:     List loaded modules
Currently Loaded Modules:
  1) craype-x86-rome                                 6) PrgEnv-gnu/8.5.0     11) gcc-native/12.3
  2) libfabric/1.22.0                                7) cray-dsmml/0.2.2     12) perftools-base/23.12.0
  3) craype-network-ofi                              8) cray-libsci/23.12.5  13) cpe/23.12
  4) xpmem/2.9.6-1.1_20240510205610__g087dc11fc19d   9) cray-mpich/8.1.28    14) cpeGNU/23.12
  5) cray/23.12                                     10) craype/2.7.30

2. module avail:    List available modules
3. module spider:   List all possible modules


4. squeue -o "%.5i %.8u %.5Q %.5A %.8j %.6D %.2t %.8R %.8S %.10L"
JOBID     USER PRIOR JOBID     NAME  NODES ST NODELIST(REASON) START_TI  TIME_LEFT
57229 cmalosso 81164 57319 aims_arr      1 CG nid001759 10:24:58    5:25:25
57229 cmalosso 79278 57309 aims_arr      1 CG nid001529 08:34:01    3:34:54

5. Observations:
 OMP_NUM_Threads = 10 --> 57321     dhug 25149 57321 dario_ex      1 CG nid002236 11:01:58       4:52

Thread rank: 5
Thread rank: 0
Thread rank: 3
Thread rank: 7
Thread rank: 1
Thread rank: 4
Thread rank: 6
Thread rank: 2
Thread rank: 8
Thread rank: 9
pi is approximately 3.1415926542564856, Error is 0.0000000006666925
Computed in 1.248 seconds


 OMP_NUM_Threads = 1 --> 57321     dhug 25149 57321 dario_ex      1  R nid002236 11:05:25       4:58


Thread rank: 0
pi is approximately 3.1415926542566006, Error is 0.0000000006668075
Computed in 4.436 seconds

