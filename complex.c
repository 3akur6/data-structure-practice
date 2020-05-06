#include <stdio.h>
typedef struct
{
    float realpart;
    float imagepart;
} Complex;
void create(Complex *, float, float);
void show_complex(Complex *);
Complex conjugate(Complex);
Complex add(Complex, Complex);
Complex sub(Complex, Complex);
Complex multiply(Complex, Complex);
Complex divide(Complex, Complex);
int main(void)
{
    float c1_real, c1_image, c2_real, c2_image;
    int operation;
    Complex c1, c2, result;
    printf("请按序输入两个复数的实部、虚部：\n");
    scanf("%f %f %f %f", &c1_real, &c1_image, &c2_real, &c2_image);
    create(&c1, c1_real, c1_image);
    create(&c2, c2_real, c2_image);
    printf(
        "四则运算：\n"
        "1) 加法\n"
        "2) 减法\n"
        "3) 乘法\n"
        "4) 除法\n"
        "请输入所需运算对应的序号："
        );
    scanf("%d", &operation);
    while (!(operation >= 1 && operation <= 4))
    {
        printf("请输入所需运算对应的序号：");
        scanf("%d", &operation);
    }
    switch (operation)
    {
        case 1:
        result = add(c1, c2);
        break;
        case 2:
        result = sub(c1, c2);
        break;
        case 3:
        result = multiply(c1, c2);
        break;
        case 4:
        result = divide(c1, c2);
        break;
    }
    show_complex(&result);
    return 0;
}
void create(Complex * c, float x, float y)
{
    c->realpart = x;
    c->imagepart = y;
}
void show_complex(Complex * c)
{
    if (c->imagepart == 0)
    {
        printf("%.1f\n", c->realpart);
    } else if (c->realpart == 0)
    {
        printf("%.fi\n", c->imagepart);
    } else if (c->imagepart > 0)
    {
        printf("%.1f+%.1fi\n", c->realpart, c->imagepart);
    } else if (c->imagepart < 0)
    {
        printf("%.1f%.1fi\n", c->realpart, c->imagepart);
    }
}
Complex conjugate(Complex c)
{
    Complex conjugate;
    conjugate.realpart = c.realpart;
    conjugate.imagepart = -c.imagepart;
    return conjugate;
}
Complex add(Complex c1, Complex c2)
{
    Complex sum;
    sum.realpart = c1.realpart + c2.realpart;
    sum.imagepart = c1.imagepart + c2.imagepart;
    return sum;
}
Complex sub(Complex c1, Complex c2)
{
    Complex difference;
    difference.realpart = c1.realpart - c2.realpart;
    difference.imagepart = c1.imagepart - c2.imagepart;
    return difference;
}
Complex multiply(Complex c1, Complex c2)
{
    Complex product;
    product.realpart = c1.realpart * c2.realpart - c1.imagepart * c2.imagepart;
    product.imagepart = c1.realpart * c2.imagepart + c1.imagepart * c2.realpart;
    return product;
}
Complex divide(Complex c1, Complex c2)
{
    Complex result;
    Complex conj = conjugate(c2);
    Complex denominator = multiply(c2, conj);
    Complex temp = multiply(c1, conj);
    result.realpart = temp.realpart * (1 / denominator.realpart);
    result.imagepart = temp.imagepart * (1 / denominator.realpart);
    return result;
}