#!/usr/bin/env python3
"""Ejecuta pruebas reales, guarda cada tiempo y resume por mediana."""
import argparse
import csv
import os
import platform
import statistics
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

def main():
    parser = argparse.ArgumentParser(description="Experimentos de rendimiento RGB OpenMP")
    parser.add_argument("--resoluciones", nargs="+", type=int, default=[256, 512, 1024, 2048])
    parser.add_argument("--hilos", nargs="+", type=int, default=[1, 2, 4])
    parser.add_argument("--filtros", nargs="+", type=int, default=[3, 7, 15])
    parser.add_argument("--repeticiones", type=int, default=5)
    parser.add_argument("--salida", type=Path, default=ROOT / "resultados_experimentos")
    args = parser.parse_args()
    if any(v<=0 for v in args.resoluciones+args.hilos) or any(v not in (3,7,15) for v in args.filtros) or args.repeticiones<1:
        parser.error("Resoluciones, hilos y repeticiones positivos; filtros 3, 7 o 15")
    out = args.salida.resolve(); out.mkdir(parents=True, exist_ok=True)
    raw=out/"mediciones.csv"
    raw.unlink(missing_ok=True)
    exe=ROOT/("benchmark.exe" if os.name=="nt" else "benchmark")
    if not exe.exists():
        subprocess.run(["make","benchmark"],cwd=ROOT,check=True)
    with (out/"entorno.txt").open("w",encoding="utf-8") as f:
        f.write(f"Sistema: {platform.platform()}\nCPU reportadas: {os.cpu_count()}\nPython: {platform.python_version()}\n")
        f.write("Compilacion: GCC -O2 -fopenmp\n")
        for command in (["gcc","--version"],["lscpu"]):
            try:
                result=subprocess.run(command,check=False,capture_output=True,text=True)
                f.write(f"\n$ {' '.join(command)}\n{result.stdout[:9000]}")
            except FileNotFoundError: pass
    # Un caso produce el mismo dato secuencial y paralelo para que sean comparables.
    env={**os.environ,"OMP_DYNAMIC":"FALSE"}
    for n in args.resoluciones:
        for k in args.filtros:
            for p in args.hilos:
                print(f"Midiendo {n}x{n} / {k}x{k} / {p} hilos",flush=True)
                subprocess.run([str(exe),str(n),str(n),str(k),str(p),str(args.repeticiones),str(raw)],cwd=ROOT,env=env,check=True)
    groups=defaultdict(list)
    with raw.open(newline="",encoding="utf-8") as f:
        for row in csv.DictReader(f):
            groups[(int(row["ancho"]),int(row["alto"]),int(row["filtro"]),int(row["hilos"]),row["modo"])].append(float(row["tiempo_s"]))
    summary=[]
    for n in args.resoluciones:
        for k in args.filtros:
            for p in args.hilos:
                ts=statistics.median(groups[n,n,k,p,"secuencial"])
                tp=statistics.median(groups[n,n,k,p,"paralelo"])
                speedup=ts/tp if tp else float("nan")
                efficiency=speedup/p
                # Sobrecosto estimado respecto al ideal; incluye desequilibrio y limitaciones de memoria.
                overhead=tp-ts/p
                summary.append(dict(ancho=n,alto=n,filtro=k,hilos=p,tiempo_secuencial_s=round(ts,9),tiempo_paralelo_s=round(tp,9),speedup=round(speedup,5),eficiencia=round(efficiency,5),sobrecosto_ideal_s=round(overhead,9)))
    dest=out/"resumen.csv"
    with dest.open("w",newline="",encoding="utf-8") as f:
        writer=csv.DictWriter(f,fieldnames=list(summary[0])); writer.writeheader();writer.writerows(summary)
    try:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
        for k in args.filtros:
            for n in args.resoluciones:
                subset=sorted((r for r in summary if r["filtro"]==k and r["ancho"]==n),key=lambda r:r["hilos"])
                for measure,ylabel in (("speedup","Speedup"),("eficiencia","Eficiencia"),("tiempo_paralelo_s","Tiempo (s)")):
                    fig,ax=plt.subplots()
                    ax.plot([r["hilos"] for r in subset],[r[measure] for r in subset],marker="o",label="Medido")
                    if measure=="speedup":ax.plot([r["hilos"] for r in subset],[r["hilos"] for r in subset],"--",label="Ideal")
                    if measure=="eficiencia":ax.axhline(1,linestyle="--",label="Ideal")
                    ax.set(xlabel="Numero de hilos",ylabel=ylabel,title=f"{measure} - {n}x{n}, filtro {k}x{k}")
                    ax.grid(True,alpha=.3);ax.legend();fig.tight_layout()
                    fig.savefig(out/f"{measure}_k{k}_{n}.png",dpi=150);plt.close(fig)
        print("Graficas PNG generadas")
    except ImportError:
        print("Matplotlib no instalado: CSV generados; para graficas: pip install matplotlib")
    print(f"Datos guardados en: {out}")

if __name__=="__main__":main()
