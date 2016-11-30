#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(void){

    float a,b,c,d,l,m,n,k,p,D,q,r,s,x,y,z;
    printf("PROGRAM TO SOLVE THREE VARIABLE LINEAR SIMULTANEOUS EQUATIONS\n");
    printf("The equations are of the form: ax+by+cz+d=0\n lx+my+nz+k=0\n px+qy+rz+s=0\n ");

    printf("Enter the coefficients in the order a,b,c,d,l,m,n,k,p,q,r,s\n");

    scanf("%f%f%f%f%f%f%f%f%f%f%f%f",&a,&b,&c,&d,&l,&m,&n,&k,&p,&q,&r,&s);

    printf("The equations you have input are: \n");
    
    printf(" %.2f*x + %.2f*y + %.2f*z + %.2f = 0 \n",a,b,c,d);
    printf("  %.2f*x + %.2f*y + %.2f*z + %.2f = 0 \n",l,m,n,k);
    printf("  %.2f*x + %.2f*y + %.2f*z + %.2f = 0 \n",p,q,r,s);

    D = (a*m*r+b*p*n+c*l*q)-(a*n*q+b*l*r+c*m*p);
    x = ((b*r*k+c*m*s+d*n*q)-(b*n*s+c*q*k+d*m*r))/D;
    y = ((a*n*s+c*p*k+d*l*r)-(a*r*k+c*l*s+d*n*p))/D;
    z = ((a*q*k+b*l*s+d*m*p)-(a*m*s+b*p*k+d*l*q))/D;

    printf("The solutions to the above three equations are :\n");
    printf("x = %f\ny = %f\nz = %f\n",x,y,z);

    return 0;
}