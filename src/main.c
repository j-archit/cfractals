#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Define constants
#define PI 3.14159

// These control output image and are globally set.
int dimension_y = 1080;
int dimension_x = 1920;
double oratio[2] = {0.65, 0.5};
float y_max = 2.222;
int MAXT = 100;

// These control printing to the console
int print_verbose = 1;
char sla[5] = {'-', '/', '|', '\\', '-'};

double bound = pow(2, 128);

int fractal = 0;

// Complex Numbers Structure
struct complex{
    double re;
    double im;
};
struct complex _complex (double re, double im){
    struct complex C;
    C.re = re;
    C.im = im;
    return C;
}
struct complex _add (struct complex A, struct complex B){
    struct complex C;
    C.re = A.re + B.re;
    C.im = A.im + B.im;
    return C;
}
struct complex _sub (struct complex A, struct complex B){
    struct complex C;
    C.re = A.re - B.re;
    C.im = A.im - B.im;
    return C;
}
struct complex _mul (struct complex A, struct complex B){
    struct complex C;
    C.re = A.re * B.re - A.im * B.im;
    C.im = A.re * B.im + A.im * B.re;
    return C;
}
struct complex _muld (struct complex A, double B){
    struct complex C;
    C.re = A.re * B;
    C.im = A.re * B;
    return C;
}
struct complex _conj (struct complex A){
    struct complex C;
    C.re = A.re;
    C.im = -A.im;
    return C;
}
double _mag(struct complex A){
    return sqrt((A.re * A.re + A.im * A.im));
}
struct complex _div (struct complex A, struct complex B){
    struct complex C;
    C = _mul(A, _conj(B));
    C = _muld(C, 1/_mag(B));
    return C;
}
struct complex _divd (struct complex A, double b){
    struct complex C;
    C.re = A.re / b;
    C.im = A.im / b;
    return C;
}

void progress(int p, int total){
    printf("%c %2.0f%%\b\b\b\b\b", sla[p % 5], (double)((p*100)/total));
}

int check(struct complex Z){
    if(Z.re < bound && Z.im < bound)
        //if(Z.re > -bound && Z.im > -bound)
            return 1;
    return 0;
}

// All generation functions work with the Complex Coordinate System(CCS) and not with the Array Coordinate System(ACS)
void mandelbrot(int dimx, int dimy, double inv_res, struct complex S, struct complex origin, FILE* f){

    struct complex Z;
    progress(0, dimy);
    // Loop over ACS Coordinates and convert to CCS on the fly.
    int i, j, m;
    for(j = dimy-1; j >= 0 ; j--){
        for(m = 0; m < dimx; m++){
            // Conversion to CCS
            struct complex C = _sub(_complex(m/inv_res, j/inv_res), origin);
            int k = 0;
            Z = S;
            while(k <= MAXT && check(Z)){
                struct complex X = _add(_mul(Z, Z), C);
                Z.re = X.re;
                Z.im = X.im;
                k++;
            }
            fprintf(f, "%d,", (uint8_t)(k > MAXT));
        }
        progress(dimy - j, dimy);
        fprintf(f, " \n");
    }
}

void newton(){

}

void main(int argc, char *argv[])
{
    int i;
    for(i = 1; i < argc; i++){
        char* option = argv[i];
        if(option[0] == '-'){
            int intv;
            float floatv;
            sscanf (argv[i+1], "%i", &intv) == 1;
            sscanf (argv[i+1], "%f", &floatv) == 1;
            switch(option[1]){
                case 'h': {
                    dimension_y = intv; i++;
                    break;
                }
                case 'w': {
                    dimension_x = intv; i++;
                    break;
                }
                case 'm': {
                    MAXT = intv; i++;
                    break;
                }
                case 'y': {
                    y_max = floatv; i++;
                    break;
                }
                case 'f': {
                    fractal = intv;
                    if(fractal > 1){
                        printf("--------------------------\n");
                        printf("Unknown Fractal:\n");
                        printf("Available:\n");
                        printf("   -> mandelbrot (0)\n");
                        printf("   -> newton (1)\n");
                        exit(1);
                    }
                    else
                        fractal = 0;
                    i++;
                    break;
                }
                case 's': {
                    print_verbose = 0;
                    break;
                }
                case 'M':{
                    dimension_x *= intv;
                    dimension_y *= intv;
                    i++; break;
                }
            }
            if(option){}
        }
    }

    const double inv_res = dimension_y/y_max;

    if(print_verbose){
        system("setterm --cursor off");
        printf("--------------------------\n");
        printf("   Config: \n");
        printf("--------------------------\n");
        printf("   height  \t%d\n", dimension_y);
        printf("   width  \t%d\n", dimension_x);
        printf("   y_max  \t%.2f\n", y_max);
        printf("   inv_res  \t%.2f\n", inv_res);
        printf("   max_iter \t%d\n\n", MAXT);
        printf("   Effective Resolution \n\t%d*%d\n", dimension_x, dimension_y);
        printf("--------------------------\n");
        printf("   Progress:\n   ");
    }

    // Common to all Fractals
    struct complex origin = _complex(dimension_x*oratio[0]/inv_res, dimension_y*oratio[1]/inv_res);
    FILE *f;
    f = fopen("default_c.txt", "w");
    fprintf(f, "%d,%d\n", dimension_x, dimension_y);
    
    // Switch control based on fractal input
    switch(fractal){
        case 0:{
            mandelbrot(dimension_x, dimension_y, inv_res, _complex(0, 0), origin, f);
            break;
        }
        case 1:{
            newton();
            break;
        }
    }
    fclose(f);

    if(print_verbose){
        printf("\n--------------------------\n");
        system("setterm --cursor on");
    }
}