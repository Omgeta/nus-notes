#!/usr/bin/env bash

ARRAY_SIZES=(5000 10000 15000 20000 25000)
MAX_VALUES=(50000 100000 150000 200000 250000)
ITERATIONS=3
OUT="out.csv"
RECORDED="cycles,instructions"
PARTITION="i7-7700"
PROGRAMS=("asdf" "asdf_merge")

perf_stat() {
  srun --partition "$PARTITION" perf stat -e "$RECORDED" "$1" 2>&1 1>/dev/null # perf spits out results in stderr
}

get_cycles() {
  awk '/cycles/ { gsub(/,/,"",$1); print $1; exit }'
}

get_instructions() {
  awk '/instructions/ { gsub(/,/,"",$1); print $1; exit }'
}

get_elapsed() {
  awk '/seconds time elapsed/ { gsub(/,/,"",$1); print $1; exit }'
}

get_user() {
  awk '/seconds user/ { gsub(/,/,"",$1); print $1; exit }'
}

# Measure across array size
echo "program,ARRAY_SIZE,elapsed,user,instructions,cycles" > "$OUT"
for P in "${PROGRAMS[@]}"; do
  for N in "${ARRAY_SIZES[@]}"; do
    g++ -DARRAY_SIZE=$N -o "$P" "$P".cpp

    for I in $(seq 1 "$ITERATIONS"); do 
      out="$(perf_stat ./"$P")"
      elapsed="$(printf '%s\n' "$out" | get_elapsed)"
      user="$(printf '%s\n' "$out" | get_user)"
      instructions="$(printf '%s\n' "$out" | get_instructions)"
      cycles="$(printf '%s\n' "$out" | get_cycles)"
      echo "$P,$N,$elapsed,$user,$cycles,$instructions" >> "$OUT"
    done
  done
done

echo "" >> "$OUT"
echo "program,MAX_VALUE,elapsed,user,instructions,cycles" >> "$OUT"
for P in "${PROGRAMS[@]}"; do
  for N in ${MAX_VALUES[@]}; do
    g++ -DMAX_VALUE=$N -o "$P" "$P".cpp

    for I in $(seq 1 "$ITERATIONS"); do 
      out="$(perf_stat ./"$P")"
      elapsed="$(printf '%s\n' "$out" | get_elapsed)"
      user="$(printf '%s\n' "$out" | get_user)"
      instructions="$(printf '%s\n' "$out" | get_instructions)"
      cycles="$(printf '%s\n' "$out" | get_cycles)"
      echo "$P,$N,$elapsed,$user,$cycles,$instructions" >> "$OUT"
    done
  done
done
