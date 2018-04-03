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

const unsigned char testCert[] = {"\n\
-----BEGIN CERTIFICATE-----\n\
MIIEyjCCA7KgAwIBAgIJALm8kO2tqgqMMA0GCSqGSIb3DQEBCwUAMIGeMQswCQYD\n\
VQQGEwJVUzEQMA4GA1UECAwHTW9udGFuYTEQMA4GA1UEBwwHQm96ZW1hbjEVMBMG\n\
A1UECgwMd29sZlNTTF8yMDQ4MRkwFwYDVQQLDBBQcm9ncmFtbWluZy0yMDQ4MRgw\n\
FgYDVQQDDA93d3cud29sZnNzbC5jb20xHzAdBgkqhkiG9w0BCQEWEGluZm9Ad29s\n\
ZnNzbC5jb20wHhcNMTYwODExMjAwNzM3WhcNMTkwNTA4MjAwNzM3WjCBnjELMAkG\n\
A1UEBhMCVVMxEDAOBgNVBAgMB01vbnRhbmExEDAOBgNVBAcMB0JvemVtYW4xFTAT\n\
BgNVBAoMDHdvbGZTU0xfMjA0ODEZMBcGA1UECwwQUHJvZ3JhbW1pbmctMjA0ODEY\n\
MBYGA1UEAwwPd3d3LndvbGZzc2wuY29tMR8wHQYJKoZIhvcNAQkBFhBpbmZvQHdv\n\
bGZzc2wuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwwPRK/45\n\
pDJFO1PIhCsqfHSavaoqUgdH1qY2sgcyjtC6aXvGw0Se1IFI/S1oootnu6F1yDYs\n\
StIb94u6zw357+zxgR57mwNHmr9lzH9lJGmm6BSJW+Q098WwFJP1Z3s6enjhAVZW\n\
kaYTQo3SPECcTO/Rht83URsMoTv18aNKNeThzpbfG36/TpfQEOioCDCBryALQxTF\n\
dGe0MoJvjYbCiECZNoO6HkByIhfXUmUkc7DO7xnNrv94bHvAEgPUTnINUG07ozuj\n\
mV6dyNkMhbPZitlUJttt+qy7/yVMxNF59HHThkAYE7BjtXJOMMSXhIYtVi/XFfd/\n\
wK71/Fvl+6G60wIDAQABo4IBBzCCAQMwHQYDVR0OBBYEFDPYRWbXaIcYflQNcCeR\n\
xybXhWXAMIHTBgNVHSMEgcswgciAFDPYRWbXaIcYflQNcCeRxybXhWXAoYGkpIGh\n\
MIGeMQswCQYDVQQGEwJVUzEQMA4GA1UECAwHTW9udGFuYTEQMA4GA1UEBwwHQm96\n\
ZW1hbjEVMBMGA1UECgwMd29sZlNTTF8yMDQ4MRkwFwYDVQQLDBBQcm9ncmFtbWlu\n\
Zy0yMDQ4MRgwFgYDVQQDDA93d3cud29sZnNzbC5jb20xHzAdBgkqhkiG9w0BCQEW\n\
EGluZm9Ad29sZnNzbC5jb22CCQC5vJDtraoKjDAMBgNVHRMEBTADAQH/MA0GCSqG\n\
SIb3DQEBCwUAA4IBAQAzhQi0WA6iAAN03nf70St2nJeQICGi6C4iUCYEdrpbR3nl\n\
UvfEDXn/Yj8FfMMIbOC3gdDOxslGuY5LX1Z5SxO20WtmS84ADeN2XvvLtV0SMQXx\n\
uzn2hpDKklakoHUhth1MlsNF61qRlDLTWbjJcx8DqYFj4EPAHshlvjunU8NE/7P7\n\
R4Sotp0A1Wuuh/i7NbJsZgsR7m/+Eu1ZefE+8tNhJ4uVfpl1jaSfNIXxJU1IHptr\n\
cPZmzFaxowJSinyqrwfal8YMpY/ty/XYBF2XCl1aK0n1vZPlI5uZtQz/DH44grJu\n\
q4rJp0Wr1teTNXAHfsg9pf4zj9mFwMdaAuR81jWe\n\
-----END CERTIFICATE-----\n\n"};
