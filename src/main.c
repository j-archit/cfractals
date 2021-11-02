#include <stdio.h>
#include <string.h>
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
float inv_res = 1080/2.222;
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
struct complex _pow(struct complex A, int p){
    struct complex C = _complex(1, 0);
    for(p; p > 0; p--)
        C = _mul(C, A);
    return C;
}
struct complex _powf (struct complex A, double p){

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

// Loops start at the bottom left in terms of their iterating variables(i, j) and at the top right via (i, dimension_y - j). 
// This is mostly for compatibility to write out pixel file.

// Generating Function for all complex recursive function defined using bound-ablity, eg Mandelbrot, Tricorn and Multi variations of these. 
void complexRecursiveBounded(struct complex (*func)(struct complex, struct complex, double), double inv_res, struct complex S, struct complex origin, double degree, FILE* f){
    
    struct complex Z;
    progress(0, dimension_y);
    // Loop over ACS Coordinates and convert to CCS on the fly.
    int i, j, m;
    for(j = dimension_y-1; j >= 0 ; j--){
        for(m = 0; m < dimension_x; m++){
            // Conversion to CCS
            struct complex C = _sub(_complex(m/inv_res, j/inv_res), origin);
            int k = 0;
            Z = S;
            while(k <= MAXT && check(Z)){
                struct complex X = func(Z, C, degree); //_add(_mul(Z, Z), C);
                Z.re = X.re;
                Z.im = X.im;
                k++;
            }
            fprintf(f, "%d ", (uint8_t)(k > MAXT));
        }
        progress(dimension_y - j, dimension_y);
        fprintf(f, " \n");
    }
}

struct complex int_multibrot(struct complex Z, struct complex C, double d){
    return _add(_pow(Z, d), C);
}
struct complex double_multibrot(struct complex Z, struct complex C, double d){
    return _add(_pow(Z, d), C);
}

struct complex int_multicorn(struct complex Z, struct complex C, double d){
    return _add(_conj(_pow(Z, d)), C);
}
struct complex double_multicorn(struct complex Z, struct complex C, double d){
    return _add(_conj(_pow(Z, d)), C);
}

// void mandelbrot(double inv_res, struct complex S, struct complex origin, FILE* f){
//     struct complex Z;
//     progress(0, dimension_y);
//     // Loop over ACS Coordinates and convert to CCS on the fly.
//     int i, j, m;
//     for(j = dimension_y-1; j >= 0 ; j--){
//         for(m = 0; m < dimension_x; m++){
//             // Conversion to CCS
//             struct complex C = _sub(_complex(m/inv_res, j/inv_res), origin);
//             int k = 0;
//             Z = S;
//             while(k <= MAXT && check(Z)){
//                 struct complex X = _add(_mul(Z, Z), C);
//                 Z.re = X.re;
//                 Z.im = X.im;
//                 k++;
//             }
//             fprintf(f, "%d ", (uint8_t)(k > MAXT));
//         }
//         progress(dimension_y - j, dimension_y);
//         fprintf(f, " \n");
//     }
// }

// void multibrot(double inv_res, struct complex S, struct complex origin, double deg, FILE* f){
//     struct complex Z;
//     progress(0, dimension_y);
//     // Loop over ACS Coordinates and convert to CCS on the fly.
//     int i, j, m;
//     for(j = dimension_y-1; j >= 0 ; j--){
//         for(m = 0; m < dimension_x; m++){
//             // Conversion to CCS
//             struct complex C = _sub(_complex(m/inv_res, j/inv_res), origin);
//             int k = 0;
//             Z = S;
//             while(k <= MAXT && check(Z)){
//                 struct complex X = _add(_powf(Z, deg), C);
//                 Z.re = X.re;
//                 Z.im = X.im;
//                 k++;
//             }
//             fprintf(f, "%d ", (uint8_t)(k > MAXT));
//         }
//         progress(dimension_y - j, dimension_y);
//         fprintf(f, " \n");
//     }
// }

// void multicorn(double inv_res, struct complex S, struct complex origin, double deg, FILE* f){
//     struct complex Z;
//     progress(0, dimension_y);
//     // Loop over ACS Coordinates and convert to CCS on the fly.
//     int i, j, m;
//     for(j = dimension_y-1; j >= 0 ; j--){
//         for(m = 0; m < dimension_x; m++){
//             // Conversion to CCS
//             struct complex C = _sub(_complex(m/inv_res, j/inv_res), origin);
//             int k = 0;
//             Z = S;
//             while(k <= MAXT && check(Z)){
//                 struct complex X = _add(_conj(_pow(Z, deg)), C);
//                 Z.re = X.re;
//                 Z.im = X.im;
//                 k++;
//             }
//             fprintf(f, "%d ", (uint8_t)(k > MAXT));
//         }
//         progress(dimension_y - j, dimension_y);
//         fprintf(f, " \n");
//     }
// }

void newton(){

}

void main(int argc, char *argv[])
{
    int i;
    float degree = 2;
    for(i = 1; i < argc; i++){
        char* option = argv[i];
        if(option[0] == '-'){
            int intv;
            float floatv;
            sscanf (argv[i+1], "%i", &intv) == 1;
            sscanf (argv[i+1], "%f", &floatv) == 1;
            switch(option[1]){
                case 'h': {
                    dimension_y = intv;
                    inv_res = dimension_y/y_max;
                    i++; break;
                }
                case 'w': {
                    dimension_x = intv; i++; break;
                }
                case 'm': {
                    MAXT = intv; i++; break;
                }
                case 'f': {
                    fractal = intv;
                    if(fractal > 3){
                        printf("--------------------------\n");
                        printf("Unknown Fractal:\n");
                        printf("Available:\n");
                        printf("   -> mandelbrot (0)\n");
                        printf("   -> multibrot (1) : Requires degree argument\n");
                        printf("   -> tricorn (2)\n");
                        printf("   -> multicorn (3) : Requires degree argument\n");
                        printf("   -> newton (4)\n");
                        exit(1);
                    }
                    i++; break;
                }
                case 'M': {
                    dimension_x *= intv;
                    dimension_y *= intv;
                    inv_res = dimension_y/y_max;
                    i++; break;
                }
                case 's': {
                    print_verbose = 0; break;
                }
                case '-': {
                    if(!strcmp(option, "--ox")){
                        oratio[0] = floatv;
                    }
                    if(!strcmp(option, "--oy")){
                        oratio[1] = floatv;
                    }
                    if(!strcmp(option, "--ymax")){
                        y_max = floatv;
                        inv_res = dimension_y/y_max;
                    }
                    if(!strcmp(option, "--degree")){
                        degree = floatv;
                    }
                }    
            }
        }
    }
    
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
    f = fopen("output/default_c.txt", "w");
    fprintf(f, "%d,%d\n", dimension_x, dimension_y);
    
    // Switch control based on fractal input
    switch(fractal){
        case 0:{
            // Mandelbrot Set
            complexRecursiveBounded(int_multibrot, inv_res, _complex(0, 0), origin, 2, f);
            break;
        }
        case 1:{
            // Multibrot Set
            if(floor(degree) == degree)
                complexRecursiveBounded(int_multibrot, inv_res, _complex(0, 0), origin, degree, f);
            else
                complexRecursiveBounded(double_multibrot, inv_res, _complex(0, 0), origin, degree, f);
            break;
        }
        case 2:{
            // Tricorn Set
            complexRecursiveBounded(int_multicorn, inv_res, _complex(0, 0), origin, 3, f);
            break;
        }
        case 3:{
            // Multicorn Set
            if(floor(degree) == degree)
                complexRecursiveBounded(int_multicorn, inv_res, _complex(0, 0), origin, degree, f);
            else
                complexRecursiveBounded(double_multicorn, inv_res, _complex(0, 0), origin, degree, f);
            break;
        }
    }
    fclose(f);

    if(print_verbose){
        printf("\n--------------------------\n");
        system("setterm --cursor on");
    }
}