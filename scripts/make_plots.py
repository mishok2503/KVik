import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import re

sns.color_palette('deep')


def build_from_file(filename):
    labels = ['lz4', 'zstd 1', 'zstd 7']
    values = [[], [], []]
    files = []
    with open(filename, "r") as f:
        for line in f:
            a = re.split(r'[ \n]', line)
            files.append(a.pop(0))
            a.pop(-1)
            values1 = list(map(float, a))
            for i in range(len(values1)):
                values[i].append(values1[i])
    x = np.arange(len(files))
    width = 0.25
    fig, ax = plt.subplots()
    fig.set_size_inches(12, 6)
    for i in range(len(labels)):
        ax.bar(x - width * len(labels) / 2 + width / 2 + width * i, values[i], width, label=labels[i])
    ax.set_ylabel('Parrots')
    ax.set_title('Performance on different files')
    ax.set_xticks(x, files)
    ax.legend()
    fig.tight_layout()
    plt.show()
    fig.savefig(filename.replace(".txt", "") + "_plot.png")


build_from_file("../times.txt")
build_from_file("../coefs.txt")