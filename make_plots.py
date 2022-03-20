import matplotlib.pyplot as plt
import seaborn as sns
import glob

sns.set_style('darkgrid')
sns.color_palette('deep')
plt.rc('axes', titlesize=18)
plt.rc('axes', labelsize=14)
plt.rc('xtick', labelsize=13)
plt.rc('ytick', labelsize=13)
plt.rc('legend', fontsize=13)
plt.rc('font', size=13)


def build_from_file(filename):
    plt.figure(figsize=(10, 6), tight_layout=True)
    plt.xlabel('Key length, bytes')
    plt.ylabel('Computation time, ms')
    plt.title("MurMur vs XX")
    pts = False
    curpts_x = []
    curpts_y = []
    curlabel = ""
    with open(filename, "r") as f:
        for line in f:
            if line == "\n":
                if pts:
                    plt.plot(curpts_x, curpts_y, label=curlabel)
                    curpts_x = []
                    curpts_y = []
                    curlabel = ""
                    pts = False
            elif not pts:
                pts = True
                curlabel = line.removesuffix("\n")
            else:
                x, y = map(float, line.split(':'))
                curpts_x.append(x)
                curpts_y.append(y)
    plt.legend(title="Functions")
    plt.savefig(filename.removesuffix(".txt") + "_plot.png")
    f.close()


for i in glob.glob("./artifacts/*.txt"):
    build_from_file(i)
