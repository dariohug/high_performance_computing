#!/bin/bash

OUTPUT_DIR="mapreduce_output"
mkdir -p "$OUTPUT_DIR"

echo "Output directory: $OUTPUT_DIR"
echo ""

cat trump_tweets_*.csv | \
    python mapper3.py | \
    sort -k1,1 | \
    python reducer3.py > "$OUTPUT_DIR/results.txt"

echo "MapReduce completed successfully!"

