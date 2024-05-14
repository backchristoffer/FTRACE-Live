## This tool uses ftrace for live tracing kernel events. Ensure that your system's kernel supports ftrace and that it's enabled. The program isolates ftrace to run on a specific CPU core using CPU affinity. Make sure to specify a valid CPU core number as an argument. Trace output will be printed to the console in real-time and in microseconds. You can redirect the output to a file if needed.

## Only tested with kernel 6.8.6-200.fc39.x86_64 so far (non rt) 2024-05-14

## How to run
```
sudo ./ftrace_live --affinity <cpu_number>
```

## Output
### The program will print the trace output to the console in real-time. ctrl+c to stop. 
```
CPU 0/KVM-1300312 [004] d.h3. 1907753.676095: update_min_vruntime <-update_curr
CPU 0/KVM-1300312 [004] d.h3. 1907753.676096: __update_load_avg_se <-update_load_avg
CPU 0/KVM-1300312 [004] d.h3. 1907753.676097: __accumulate_pelt_segments <-__update_load_avg_se
CPU 0/KVM-1300312 [004] d.h3. 1907753.676097: __update_load_avg_cfs_rq <-update_load_avg
CPU 0/KVM-1300312 [004] d.h3. 1907753.676097: __accumulate_pelt_segments <-__update_load_avg_cfs_rq
CPU 0/KVM-1300312 [004] d.h3. 1907753.676097: update_cfs_group <-task_tick_fair
CPU 0/KVM-1300312 [004] d.h3. 1907753.676097: reweight_entity <-task_tick_fair
CPU 0/KVM-1300312 [004] d.h3. 1907753.676097: update_curr <-reweight_entity
CPU 0/KVM-1300312 [004] d.h3. 1907753.676098: update_curr_se <-update_curr
CPU 0/KVM-1300312 [004] d.h3. 1907753.676098: avg_vruntime <-reweight_entity
CPU 0/KVM-1300312 [004] d.h3. 1907753.676098: update_min_vruntime <-task_tick_fair
CPU 0/KVM-1300312 [004] d.h3. 1907753.676098: hrtimer_active <-task_tick_fair
CPU 0/KVM-1300312 [004] d.h3. 1907753.676098: calc_global_load_tick <-scheduler_tick
CPU 0/KVM-1300312 [004] d.h3. 1907753.676099: task_tick_mm_cid <-scheduler_tick
```

