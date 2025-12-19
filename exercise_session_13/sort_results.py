import sys

word_counts = []
with open('mapreduce_output/results.txt', 'r') as f:
    for line in f:
        line = line.strip()
        if line:
            try:
                word, count = line.split('\t')
                word_counts.append((word, int(count)))
            except:
                continue

word_counts.sort(key=lambda x: (-x[1], x[0]))

with open('mapreduce_output/results_sorted.txt', 'w') as f:
    for word, count in word_counts:
        f.write('%s\t%d\n' % (word, count))

print("Sorted results saved to: mapreduce_output/results_sorted.txt")
print("\nTop 20 words:")
for word, count in word_counts[:20]:
    print('%s\t%d' % (word, count))