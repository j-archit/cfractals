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
double origin_offset[2] = {0.65, 0.5};
float y_max = 2.222;
float ppu = 1080/2.222;
int max_iter = 100;
int gen_grad = 0;

// These control printing to the console
int print_verbose = 1;
char sla[5] = {'-', '/', '|', '\\', '-'};

// Fractal options
double bound = pow(2, 128);
int fractal = 0;

// Complex Numbers Structure
struct complex {
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
struct complex _pow (struct complex A, int p){
    struct complex C = _complex(1, 0);
    for(p; p > 0; p--)
        C = _mul(C, A);
    return C;
}
struct complex _powf (struct complex A, double p){

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
double _magsq(struct complex A){
    return A.re * A.re + A.im * A.im;
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

// Loops start at the bottom left in terms of their iterating variables(i, j) and at the top right via (i, dimension_y - j). 
// This is mostly for compatibility to write out pixel file.
// Generating Function for all complex recursive function defined using bound-ablity, eg Mandelbrot, Tricorn and Multi variations of these. 
void complexRecursiveBounded(
    struct complex (*nextz)(struct complex, struct complex, double),
    int (*check)(struct complex),
    double ppu, struct complex S, struct complex origin, double degree, FILE* f){
    
    struct complex Z;
    progress(0, dimension_y);
    // Loop over ACS Coordinates and convert to CCS on the fly.
    int i, j, col, fl;
    int last_color = 0;  // Base color = 0
    long pos;
    for(j = dimension_y - 1; j >= 0 ; j--){
        fl = 0;
        pos = ftell(f);
        last_color = 0;
        // Print Line number
        fprintf(f, "%d ", dimension_y - j - 1);
        for(i = 0; i < dimension_x; i++){
            // Conversion to CCS
            struct complex C = _sub(_complex(i/ppu, j/ppu), origin);
            int k = 0;
            Z = S;
            while(k <= max_iter && check(Z)){
                Z = nextz(Z, C, degree);
                k++;
            }
            col =   (gen_grad)*(
                    0*(k == 1) +
                    1*(k > 1) + 
                    1*(k >= 2) + 
                    1*(k >= 4) + 
                    1*(k >= 6) + 
                    1*(k >= 16) +
                    1*(k >= 64) + 
                    1*(k >= 128)
                    ) + 1*(k > max_iter);

            if(last_color ^ col){
                fprintf(f, "%d:%d ", col, i);
                fl = 1;
            }
            last_color = col;
        }
        // Go back to start of line if no color changes (other than base) on the line, otherwise add newline
        !fl? fseek(f, pos, SEEK_SET): fprintf(f, " \n");
        progress(dimension_y - j, dimension_y);
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

int check_mandelbrot(struct complex Z){
    return _magsq(Z) < 4;
}
int check_general(struct complex Z){
    return Z.re < bound && Z.im < bound && Z.re > -bound && Z.im > -bound;
}

void newton(){

}

void print_help(int code){
    switch(code){
        case(1):{
            printf("--------------------------\n");
            printf("Unknown Fractal:\n");
            printf("Available:\n");
            printf("   -> mandelbrot (0)\n");
            printf("   -> multibrot (1) : Requires degree argument\n");
            printf("   -> tricorn (2)\n");
            printf("   -> multicorn (3) : Requires degree argument\n");
            printf("   -> newton (4)\n");
            break;    
        }
    }
}

int main(int argc, char *argv[]){
    int i;
    char fname[] = "output/default_c.txt";
    float degree = 2;
    for(i = 1; i < argc; i++){
        char* option = argv[i];
        if(option[0] == '-'){
            int intv;
            float floatv;
            if(i != argc - 1){
                sscanf (argv[i+1], "%i", &intv) == 1;
                sscanf (argv[i+1], "%f", &floatv) == 1;
            }
            switch(option[1]){
                // Handle short (-) options
                case 'h': {
                    dimension_y = intv;
                    ppu = dimension_y/y_max;
                    i++; break;
                }
                case 'w': {
                    dimension_x = intv; i++; break;
                }
                case 'm': {
                    max_iter = intv; i++; break;
                }
                case 'f': {
                    fractal = intv;
                    if(fractal > 3){
                        print_help(1);
                        exit(1);
                    }
                    i++; break;
                }
                case 'M': {
                    dimension_x *= intv;
                    dimension_y *= intv;
                    ppu = dimension_y/y_max;
                    i++; break;
                }
                case 's': {
                    print_verbose = 0; break;
                }
                // Handle long (--) options
                case '-': {
                    if(!strcmp(option, "--grad")){
                        gen_grad = 1;
                    }
                    if(!strcmp(option, "--ox")){
                        origin_offset[0] = floatv;
                    }
                    if(!strcmp(option, "--oy")){
                        origin_offset[1] = floatv;
                    }
                    if(!strcmp(option, "--ymax")){
                        y_max = floatv;
                        ppu = dimension_y/y_max;
                    }
                    if(!strcmp(option, "--ppu")){
                        ppu = floatv;
                        y_max = dimension_y/ppu;
                    }
                    if(!strcmp(option, "--degree")){
                        degree = floatv;
                    }
                    if(!strcmp(option, "--outfile")){
                        strcpy(fname, argv[i+1]);
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
        printf("   ppu  \t%.2f\n", ppu);
        printf("   max_iter \t%d\n\n", max_iter);
        printf("   Effective Resolution \n\t%d*%d\n", dimension_x, dimension_y);
        printf("--------------------------\n");
        printf("   Progress:\n   ");
    }

    // Common to all Fractals
    struct complex origin = _complex(dimension_x*origin_offset[0]/ppu, dimension_y*origin_offset[1]/ppu);
    FILE *f;
    f = fopen(fname, "w");
    fprintf(f, "%d,%d\n", dimension_x, dimension_y);
    
    // Switch control based on fractal input
    switch(fractal){
        case 0: { // Mandelbrot Set
            complexRecursiveBounded(int_multibrot, check_mandelbrot, ppu, _complex(0, 0), origin, 2, f);
            break;
        }
        case 1: { // Multibrot Set
            if(floor(degree) == degree)
                complexRecursiveBounded(int_multibrot, check_general, ppu, _complex(0, 0), origin, degree, f);
            else
                complexRecursiveBounded(double_multibrot, check_general, ppu, _complex(0, 0), origin, degree, f);
            break;
        }
        case 2: { // Tricorn Set
            complexRecursiveBounded(int_multicorn, check_general, ppu, _complex(0, 0), origin, 3, f);
            break;
        }
        case 3: { // Multicorn Set
            if(floor(degree) == degree)
                complexRecursiveBounded(int_multicorn, check_general, ppu, _complex(0, 0), origin, degree, f);
            else
                complexRecursiveBounded(double_multicorn, check_general, ppu, _complex(0, 0), origin, degree, f);
            break;
        }
    }
    fclose(f);

    if(print_verbose){
        printf("\n--------------------------\n");
        system("setterm --cursor on");
    }

    return 0;
}
