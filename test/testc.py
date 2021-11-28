import os
import subprocess
import re

def do(new_exec, base_exec, maxiter, mag, grad, t_iters, log):
    
    test_command_base = f"/usr/bin/time -f 'Time=%U' {base_exec} -m {maxiter} -M {mag}{' --grad' if grad else ''}"
    test_command_new = f"/usr/bin/time -f 'Time=%U' {new_exec} -m {maxiter} -M {mag}{' --grad' if grad else ''}"
    
    f = open(log, "a")
    f.write("\n######################\n")
    f.write("Results for:\n")
    f.write(f"Max_Iter: {maxiter}\nMultiplier: {mag}\nGrad: {grad}\n")
    f.write("\n----------------------\n")

    # Run Tests
    print("Running Tests")
    avgs = []
    rune = [('Base', test_command_base), ('New', test_command_new)]
    rune.reverse()
    for na, comm in rune:
        avg = 0
        for i in range(t_iters):
            print(f"\r{na} Test {i+1}", end=' ')    
            val = subprocess.run(comm.split(" "), stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True)
            val = float(re.findall("Time=([0-9.]*)", val.stderr)[0])
            avg += val
            print(f"\t| L: {val:>.03f}s ", end='')
            f.write(f"{na} Test {i+1}:\t {val:>.03f}s\n")
        f.write("\n")

        a = avg/t_iters
        print(f" .. Done\nA: {a:>.03f}s\n")
        avgs.append(f"{na} Avg:\t {a:>0.3f}s\n")
        
    f.writelines(avgs)
    f.close()

if __name__ == "__main__":
    sdir = os.path.dirname(__file__)
    sdir = os.path.abspath(os.path.join(sdir, ".."))
    # sdir should contain root directory
    os.chdir(sdir)

    base = os.path.join(sdir, "src", "mainb.c")
    base_e = os.path.join(sdir, "test", "mainb")

    new = os.path.join(sdir, "src", "main.c")
    new_e = os.path.join(sdir, "test", "main")

    # Commands
    compiler_base = f"gcc -w {base} -o {base_e} -lm -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant"
    compiler_new = f"gcc -w {new} -o {new_e} -lm -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant"
    # Compile
    subprocess.run(compiler_base.split(" "))
    subprocess.run(compiler_new.split(" "))
    
    # Options
    maxiter = 200
    mag = 1
    grad = True
    t_iters = 50
    log_file = os.path.join(sdir, "test", "test_result.log")

    for mags in range(1, 10):
        do(new_exec=new_e, base_exec=base_e, maxiter=maxiter, mag=mags, grad=grad, t_iters=t_iters, log=log_file)