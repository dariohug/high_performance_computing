import re
import glob

pattern = re.compile(r'[Tt]emperature:\s*([\d.]+).*time[: ]\s*([\d.]+)')
temps = []

with open("out.txt", "w") as out:
    for f in glob.glob("measured/measurements*.txt"):
        with open(f, errors='ignore') as infile:
            for line in infile:
                m = pattern.search(line)
                if m:
                    temp, time = m.groups()
                    out.write(f"{time} {temp}\n")
                    temps.append(float(temp))

avg = sum(temps) / len(temps) if temps else 0
print(f"Average temperature: {avg:.2f}")

