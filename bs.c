#include <stdio.h>
#include <math.h>
#include <string.h>

// Generates PI
double PI()
{
    return M_PI;
}

// Normal Distribution Function
double F(double x)
{
    double factor = 1.0/sqrt(2.0*PI());
    double e = exp(-pow(x, 2)/2.0);
    return factor*e;
}

// Cumulative normal distribution function
double N(double x)
{
    double x0 = -15.0;
    double n = 101;
    double dx = (x - x0)/(n - 1);
    double total = 0;

    for(int i = 0; i < n; i++){
        if(i == 0 || i == n - 1){
            total += 1.0*F(x0 + i*dx);
        } else if (i % 2 == 0){
            total += 2.0*F(x0 + i*dx);
        } else {
            total += 4.0*F(x0 + i*dx);
        }
    }

    total *= dx/3.0;
    return total;
}

// Calculate D2
double D2(double d1, double v, double t)
{
    return d1 - v*sqrt(t);
}

// Calculate D1
double D1(double S, double K, double r, double q, double v, double t)
{
    double top = log(S/K) + (r - q + 0.5*pow(v, 2))*t;
    double bottom = v*sqrt(t);
    return top / bottom;
}

// Computes the option price with the black scholes formula
double BlackScholes(double S, double K, double r, double q, double v, double t, int optype)
{
    double d1 = D1(S, K, r, q, v, t);
    double d2 = D2(d1, v, t);
    double option_price = 0;
    if(optype == 0){
        option_price = S*exp(-q*t)*N(d1) - K*exp(-r*t)*N(d2);
    } else {
        option_price = K*exp(-r*t)*N(-d2) - S*exp(-q*t)*N(-d1);
    }
    return option_price;
}

// Computes the vega of the black scholes formula
double Vega(double S, double K, double r, double q, double v, double t)
{
    double d1 = D1(S, K, r, q, v, t);
    return (1.0/100.0)*S*N(d1)*sqrt(t);
}

int main()
{
    double S = 100.0;
    double K = 105.0;
    double r = 0.0783;
    double q = 0.0147;
    double v = 0.11;
    double t = 30.0/365.0;
    int optype = 0; // 0 = Call, 1 = Put
    
    // Compute Implied Volatility
    double fixed_option_price = 1.77;

    // Define initial rates of volatility
    double volatility_0 = 0.11, volatility_1 = 0.99;

    // Loop

    double learning_rate = 0.01;

    while(optype != 3){
        volatility_1 = volatility_0 - learning_rate*(BlackScholes(S, K, r, q, volatility_0, t, optype) - fixed_option_price)/Vega(S, K, r, q, volatility_0, t);
        if(fabs(volatility_1 - volatility_0) < 0.00001){
            break;
        }
        volatility_0 = volatility_1;
    }

    printf("Implied Volatility: %f\n", volatility_1);
    printf("Check Price Of Option: %f\n", BlackScholes(S, K, r, q, volatility_1, t, optype));

    return 0;
}