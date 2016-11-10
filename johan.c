#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int main() {
    double a=0,b=0,c=0,s1=0,s2=0;
    printf("ange värde på X2 X och konst:");
    scanf("%lf %lf %lf",&a,&b,&c);
    b = b/a;
    c=c/a;
    if(b>0) b=(b-(2*b)) /2; else b=(b+(2*b)) /2;
    if(c>0) c=(c-(2*c)); else c=(c+(2*c));
    printf(" %f %f ",b,c);
    double temp = 0;
    temp = sqrt(pow(b,2)+c);
    s1 = b + temp;
    s2 = b - temp;
    printf("svar: %f %f ", s1,s2);
    return 0;
}
