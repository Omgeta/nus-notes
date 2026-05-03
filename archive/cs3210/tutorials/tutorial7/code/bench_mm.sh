#!/bin/bash
set -euo pipefail
 
RUNS=5
SIZE_EX10=2048
SIZE_EX11=256
SRC=mm-mpi.cpp
EXEC=./mm-mpi
OUT_CSV="measurements_mm.csv"
exec > >(tee "${OUT_CSV}")   # all stdout goes to the CSV from here on
 
echo "Compiling ${SRC}..." >&2
mpic++ "${SRC}" -O2 -o "${EXEC}"
echo "Compilation done." >&2
 
# Extract master total time (seconds)
get_total() {
    grep -oP '(?<=total time=)\s*[0-9.]+' | tr -d ' '
}
 
# Average comm_time across all worker lines
get_avg_comm() {
    grep -oP '(?<=communication_time=)\s*[0-9.]+' \
    | tr -d ' ' \
    | awk '{s+=$1; c++} END {if(c>0) printf "%.4f", s/c; else print "NA"}'
}
 
# Average comp_time across all worker lines
get_avg_comp() {
    grep -oP '(?<=computation_time=)\s*[0-9.]+' \
    | tr -d ' ' \
    | awk '{s+=$1; c++} END {if(c>0) printf "%.4f", s/c; else print "NA"}'
}
 
# run_config <label> <size> <srun_args...>
run_config() {
    local label="$1"
    local size="$2"
    shift 2                      # remaining args go straight to srun
 
    for ((r=1; r<=RUNS; r++)); do
        # mm-mpi writes everything to stderr; redirect both streams so we can parse
        out=$(srun "$@" "${EXEC}" "${size}" 2>&1)
 
        total=$(printf '%s\n' "${out}" | get_total   || echo "NA")
        acomm=$(printf '%s\n' "${out}" | get_avg_comm)
        acomp=$(printf '%s\n' "${out}" | get_avg_comp)
 
        echo "${label},${size},${r},${total},${acomm},${acomp}"
    done
}
 
echo "config,matrix_size,run,total_time_s,avg_worker_comm_s,avg_worker_comp_s"
 
 
# Config A: one i7-7700 node, 5 processes
run_config "ex10_i7-7700_1node" "${SIZE_EX10}" \
    -p i7-7700 -N1 -n5 --cpu-bind=thread
 
# Config B: one xs-4114 node, 5 processes
run_config "ex10_xs-4114_1node" "${SIZE_EX10}" \
    -p xs-4114 -N1 -n5 --cpu-bind=thread
 
run_config "ex10_mixed_2nodes_cyclic" "${SIZE_EX10}" \
    -N2 -n5 \
    --distribution=cyclic \
    --cpu-bind=thread \
    --constraint="[i7-7700*1&xs-4114*1]"
 
# Baseline: 5 processes (4 workers + 1 master), no overcommit
run_config "ex11_5proc_normal" "${SIZE_EX11}" \
    -p i7-7700 -N1 -n5
 
# Overcommitted: 9 processes (8 workers + 1 master) on 1 node
run_config "ex11_9proc_overcommit" "${SIZE_EX11}" \
    -p i7-7700 -N1 -n9 --overcommit
