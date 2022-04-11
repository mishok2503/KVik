from subprocess import PIPE, Popen
import os
import re


def get_compression_info(command, path, prefix, ext):
    compressed = f'{path}.{ext}'
    stream = Popen(f"./get_time.sh {command} '{path} {prefix} {compressed}'", shell=True, stdout=PIPE, stderr=PIPE)
    stdout, stderr = stream.communicate()
    time = float(re.search(r"[0-9]+\.[0-9]+\n", stderr.decode("utf-8")).group(0))
    coefficient = os.path.getsize(compressed) / os.path.getsize(path)
    os.remove(compressed)
    return int(time * 1000), coefficient


def lz4(path):
    return get_compression_info("lz4", path, "", "lz4")


def zstd_1(path):
    return get_compression_info("'zstd -1'", path, "-o", "zst")


def zstd_9(path):
    return get_compression_info("'zstd -9'", path, "-o", "zst")


def run_tests_in_dir(path, funs, times_filename, coefs_filename):
    times = open(times_filename, "w")
    coefs = open(coefs_filename, "w")
    for f in os.listdir(path):
        times.write(f)
        coefs.write(f)
        for fun in funs:
            time, coef = fun(os.path.join(path, f))
            times.write(f" {time}")
            coefs.write(f" {coef}")
        times.write("\n")
        coefs.write("\n")
    times.close()
    coefs.close()


run_tests_in_dir("../silesia", [lz4, zstd_1, zstd_9], "../times.txt", "../coefs.txt")
