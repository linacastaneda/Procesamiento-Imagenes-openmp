#define _POSIX_C_SOURCE 200809L
#include "filtros.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

static unsigned char *alloc_rgb(int w, int h) {
    if (w <= 0 || h <= 0 || (size_t)w > SIZE_MAX / (size_t)h / 3) return NULL;
    return malloc((size_t)w * (size_t)h * 3);
}
static void fill_rgb(Imagen *a) {
    uint32_t x=123456789u;
    size_t n=(size_t)a->ancho*a->alto*3;
    for(size_t i=0;i<n;i++){x ^= x<<13; x ^= x>>17; x ^= x<<5; a->pixeles[i]=(unsigned char)x;}
}
static void usage(const char *name) {
    fprintf(stderr,"Uso: %s ANCHO ALTO FILTRO HILOS REPETICIONES ARCHIVO_CSV\n",name);
    fprintf(stderr,"Ejemplo: %s 1024 1024 7 4 5 resultados/mediciones.csv\n",name);
}
static int parse_int(const char *s,int *out){
    char *end; long v=strtol(s,&end,10);
    if(!*s || *end || v<=0 || v>INT_MAX) return 0;
    *out=(int)v;return 1;
}
int main(int argc,char **argv){
    if(argc!=7){usage(argv[0]);return 2;}
    int w,h,k,p,reps;
    if(!parse_int(argv[1],&w)||!parse_int(argv[2],&h)||!parse_int(argv[3],&k)||
       !parse_int(argv[4],&p)||!parse_int(argv[5],&reps)|| (k!=3&&k!=7&&k!=15)||reps>10000){usage(argv[0]);return 2;}
    if((size_t)w > SIZE_MAX/(size_t)h/3){fputs("Tamano excesivo\n",stderr);return 2;}
    Imagen a={w,h,NULL,3},s={w,h,NULL,3},o={w,h,NULL,3};
    a.pixeles=alloc_rgb(w,h);s.pixeles=alloc_rgb(w,h);o.pixeles=alloc_rgb(w,h);
    if(!a.pixeles||!s.pixeles||!o.pixeles){fputs("Memoria insuficiente\n",stderr);free(a.pixeles);free(s.pixeles);free(o.pixeles);return 2;}
    fill_rgb(&a);
    // Calentamiento; comparar la salida byte a byte antes de medir.
    suavizar_secuencial(&a,&s,k);
    suavizar_paralelo(&a,&o,k,p);
    size_t bytes=(size_t)w*h*3;
    if(memcmp(s.pixeles,o.pixeles,bytes)!=0){fputs("ERROR: resultados distintos\n",stderr);free(a.pixeles);free(s.pixeles);free(o.pixeles);return 3;}
    FILE *f=fopen(argv[6],"a+");
    if(!f){perror(argv[6]);free(a.pixeles);free(s.pixeles);free(o.pixeles);return 2;}
    if(fseek(f,0,SEEK_END)!=0){perror("fseek");fclose(f);return 2;}
    long pos=ftell(f);
    if(pos==0) fputs("ancho,alto,filtro,hilos,repeticion,modo,tiempo_s,verificado\n",f);
    // Alternar el orden reduce sesgo termico y por cache.
    for(int r=1;r<=reps;r++){
        for(int z=0;z<2;z++){
            int parallel = (r%2==0) ? (z==0) : (z==1);
            double t0=omp_get_wtime();
            if(parallel) suavizar_paralelo(&a,&o,k,p);
            else suavizar_secuencial(&a,&s,k);
            double elapsed=omp_get_wtime()-t0;
            if(memcmp(s.pixeles,o.pixeles,bytes)!=0){fputs("ERROR: divergencia durante medicion\n",stderr);fclose(f);return 3;}
            fprintf(f,"%d,%d,%d,%d,%d,%s,%.9f,1\n",w,h,k,p,r,parallel?"paralelo":"secuencial",elapsed);
        }
    }
    if(fclose(f)!=0){perror("fclose");return 2;}
    printf("OK %dx%d filtro=%d hilos=%d repeticiones=%d\n",w,h,k,p,reps);
    free(a.pixeles);free(s.pixeles);free(o.pixeles);
    return 0;
}
