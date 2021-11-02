#include <iostream>
#include <math.h>
#include <fstream>

#define PI 3.14159

int MUL = 2;
float vratio = 0.40;
int64_t dim[2] = {1920, 1080};
int64_t MAXT = 1000;
double oratio[2] = {0.65, 0.5};
int sel = 0;
bool print_verbose = 1;
char sla[5] = {'-', '/', '|', '\\', '-'};
const int64_t bound = INT64_MAX;

using namespace std;

class complex
{
    public:
        double re, im;
        complex(double R, double I)
        {
            re = R;
            im = I;
        }
        complex& operator= (complex &C){
            re = C.re;
            im = C.im;
            return *this;
        }
        complex operator! (){
            return complex(re, -im);
        }
        complex operator+ (complex &C){
            return complex(C.re + re, C.im + im);
        }
        complex operator- (complex &C){
            return complex(re - C.re, im - C.im);
        }
        complex operator* (complex &C){
            return complex((C.re)*re - (C.im)*im, (C.re)*im + (C.im)*re);
        }
        complex operator/ (complex &C){
            double magC = C.mag();
            complex A = complex(re, im);
            complex C1 = !C;
            return A*(C1)/magC;
        }
        complex operator/ (double d){
            return complex(re/d, im/d);
        }

        friend ostream& operator<< (ostream& out, const complex C);

        double mag(){
            return sqrt(re*re + im*im);
        }
        double arg(){
            return (-1*(im<0) + 1*(im>0))*(re<0)*PI + atan(im/re);
        }
};

ostream& operator<< (ostream& out, const complex C){
    out << "<" << C.re << "," << C.im << ">";
    return out;
}

complex gen0(complex &Z, complex &C, int sel);

int intvar(char* arg){
    int intv;
    sscanf (arg, "%i", &intv) == 1;
    return intv;
}
float rr(char* arg){
    float rr;
    sscanf (arg, "%f", &rr) == 1;
    return rr;
}

int main(int argc, char *argv[])
{
  
    for(int i = 1; i < argc; i++){
        char o = argv[i][0];
        if(o == '-'){
            char option = argv[i][1];
            switch(option){
                case 'h': {
                    dim[0] = intvar(argv[i+1]); i++;
                    break;
                }
                case 'd': {
                    dim[1] = intvar(argv[i+1]); i++;
                    break;
                }
                case 'm': {
                    MUL = intvar(argv[i+1]); i++;
                    break;
                }
                case 'M': {
                    MAXT = intvar(argv[i+1]); i++;
                    break;
                }
                case 'r': {
                    vratio = rr(argv[i+1]); i++;
                    break;
                }
                case 'f': {
                    sel = intvar(argv[i+1]); i++;
                    break;
                }
                case 's': {
                    print_verbose = 0;
                    break;
                }
            }
        }
    }

    const int64_t DIM[2] = {dim[0]*MUL,dim[1]*MUL};
    const double int_per_pix = 1080*vratio*MUL;
    const int64_t ORIGIN[2] = {(DIM[0]*oratio[0]), (DIM[1]*oratio[1])};

    if(print_verbose){
        system("setterm --cursor off");
        cout << "--------------------------" << endl;
        cout << "   Config: " << endl;
        cout << "--------------------------" << endl;
        cout << "   vratio  \t" << vratio << endl;
        cout << "   multiplier  \t" << MUL << endl;
        cout << "   height  \t" << dim[0] << endl;
        cout << "   width  \t" << dim[1] << endl;
        cout << "   max_iter \t" << MAXT << endl << endl;
        cout << "   Effective Resolution \n\t" << DIM[0] << "*" << DIM[1] << endl;
        cout << "--------------------------" << endl;
        cout << "   Progress:\n   ";
    }

    ofstream f;
    f.open("mendel_cpp.txt", ios::out);
    f << DIM[0] << "," << DIM[1] << endl;
    complex origin = complex(ORIGIN[0]/int_per_pix, ORIGIN[1]/int_per_pix);

    float per = 0;
    printf("%c %2.0f%%\b\b\b\b\b", sla[0], per);
    for(int j = DIM[1] - 1; j >= 0 ; j--){
        for(int i = 0; i < DIM[0]; i++){
            // Loop over complete pixel field
            complex C = complex(i/int_per_pix, j/int_per_pix) - origin;
            complex Z = complex(0, 0);

            // Run gen0 Condition Loop
            int k = 0;
            while(k <= MAXT && Z.re < bound && Z.im < bound ){
                complex X = gen0(Z, C, sel);
                Z = X;
                k++;
            }
            f << (int)(k > MAXT)<< " ";
        }
        float per = (((DIM[1]-j))*100)/DIM[1];
        printf("%c %2.0f%%\b\b\b\b\b", sla[j % 5], per);
        f << " \n";
    }

    if(print_verbose){
        cout << endl;
        cout << "--------------------------" << endl;
        system("setterm --cursor on");
    }

    return 0;
}

complex gen0(complex &Z, complex &C, int sel = 0){
    switch(sel){
        case 0: {
            return Z*Z + C;
            break;
        }
        default:{
            return Z*Z*Z + C ;
            break;
        }
    }
}
