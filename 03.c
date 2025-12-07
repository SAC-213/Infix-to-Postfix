#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pila_din.h"

/*
Programa: 03.c
Versión: 1.35 — 06 de diciembre del 2025
Autor(es): Ascencion Cruz Saul

Descripción: Este programa recibe una expresión algebraica en forma infija y la convierte a postfija. Posteriormente,
solicita se le asignen valores a cada una de las literales de la expresión para poder evaluarla.
*/

/*Recibe la expresión algebraica y la guarda en la pila "S".
Además copia los paréntesis a la pila "P".*/
void Lectura(pila *S, pila *P);

/*Recibe la pila "P" con todos los paréntesis encontrados
y realiza un análisis para determinar si están correctamente emparejados.
Retorna TRUE si están bien colocados, FALSE en caso contrario.*/
int VerificarParentesis(pila *P);

/*Convierte la expresión almacenada en la pila "Expresion" desde notación infija
a notación postfija utilizando una pila auxiliar para operadores.*/
void PostFijo(pila *S);

/*Sustituye cada literal por su valor numérico. Pide al usuario
un valor para cada literal y reescribe la pila "Expresion" colocando números y operadores.*/
void SustituirLiterales(pila *S);

/*Evalúa la expresión en postfijo contenida en la pila "Expresion".
Utiliza una pila auxiliar para ir resolviendo las operaciones.
Retorna el resultado final en formato float.*/
float EvaluarPost(pila *S);

/*Realiza la operación correspondiente entre los operandos "a" y "b" usando el
operador "Op". Soporta +, -, *, / y ^.*/
float Operacion(float a, float b, char Op);

/*Retorna la precedencia del operador dado.
Valores mayores indican mayor prioridad en la jerarquía.*/
int Precedencia(char C);

/*Copia el contenido de la pila "Origen" a la pila "Destino"
manteniendo el mismo orden.*/
void CopiarPila(pila *Origen, pila *Destino);

/*Vacía completamente la pila "S" removiendo todos sus elementos.*/
void VaciarPila(pila *S);

/*Imprime el contenido de la pila "S" para propósitos de depuración.
Muestra números o caracteres según el tipo almacenado.*/
void Debug(pila *S);

int main(int argc, char const *argv[])
{
    pila Expresion;
    pila Parentesis;
    int Flag;
    float R;
    Initialize(&Parentesis);
    Initialize(&Expresion);

    Lectura(&Expresion, &Parentesis);

    if (!Empty(&Parentesis))
    {
        Flag = VerificarParentesis(&Parentesis);
        Destroy(&Parentesis);
        if (Flag == 0)
        {
            printf("Error, parentesis mal colocados\n");
            exit(1);
        }
        printf("Parentesis bien colocados\n");
    }

    PostFijo(&Expresion);
    SustituirLiterales(&Expresion);
    R = EvaluarPost(&Expresion);

    printf("El resultado de la expresion es: %.2f\n", R);
    return 0;
}

void Lectura(pila *S, pila *P)
{
    printf("Ingrese expresion: ");
    char c;
    elemento e;

    while ((c = getchar()) != '\n')
    {
        e.caracter = c;
        if (c == '(' || c == ')')
            Push(P, e);

        Push(S, e);
    }
}

int VerificarParentesis(pila *P)
{
    pila Aux;
    Initialize(&Aux);
    int Flag = FALSE;

    if (Size(P) % 2 == 0)
    {
        Flag = TRUE;
        while (!Empty(P))
        {
            if (Top(P).caracter == ')')
            {
                Push(&Aux, Pop(P));
            }
            else if (Top(P).caracter == '(')
            {
                Pop(P);
                if (!Empty(&Aux))
                {
                    Pop(&Aux);
                }
                else
                {
                    Flag = FALSE;
                    break;
                }
            }
        }
        if (!Empty(&Aux)) Flag = FALSE;
    }

    Destroy(&Aux);
    return Flag;
}

void PostFijo(pila *S)
{
    pila Aux;
    pila Copia;
    Initialize(&Aux);
    Initialize(&Copia);

    CopiarPila(S, &Copia);
    VaciarPila(S);

    if (!Empty(&Copia))
        Flip(&Copia);
    else
        return;

    while (!Empty(&Copia))
    {
        if ((Top(&Copia).caracter >= 'A' && Top(&Copia).caracter <= 'Z') ||
            (Top(&Copia).caracter >= 'a' && Top(&Copia).caracter <= 'z'))
        {
            putchar(Top(&Copia).caracter);
            Push(S, Pop(&Copia));
        }
        else if (Top(&Copia).caracter == '(')
        {
            Push(&Aux, Pop(&Copia));
        }
        else if (Top(&Copia).caracter == ')')
        {
            while (!Empty(&Aux) && Top(&Aux).caracter != '(')
            {
                putchar(Top(&Aux).caracter);
                Push(S, Pop(&Aux));
            }
            Pop(&Aux);
            Pop(&Copia);
        }
        else
        {
            while (!Empty(&Aux) &&
                   (Precedencia(Top(&Aux).caracter) >= Precedencia(Top(&Copia).caracter)) &&
                   Top(&Aux).caracter != '(')
            {
                putchar(Top(&Aux).caracter);
                Push(S, Pop(&Aux));
            }
            Push(&Aux, Pop(&Copia));
        }
    }

    while (!Empty(&Aux))
    {
        putchar(Top(&Aux).caracter);
        Push(S, Pop(&Aux));
    }

    putchar('\n');
    Destroy(&Aux);
    Destroy(&Copia);
}

void SustituirLiterales(pila *S)
{
    if (Empty(S)) return;

    pila Temp;
    pila Result;
    Initialize(&Temp);
    Initialize(&Result);

    CopiarPila(S, &Temp);
    Flip(&Temp);

    while (!Empty(&Temp))
    {
        elemento cur = Pop(&Temp);

        if ((cur.caracter >= 'A' && cur.caracter <= 'Z') ||
            (cur.caracter >= 'a' && cur.caracter <= 'z'))
        {
            float val;
            printf("Ingrese un valor para %c: ", cur.caracter);
            scanf("%f", &val);

            elemento num;
            num.numero = val;
            num.caracter = cur.caracter;
            num.tipo = TRUE;
            Push(&Result, num);
        }
        else
        {
            elemento op;
            op.caracter = cur.caracter;
            op.tipo = FALSE;
            Push(&Result, op);
        }
    }

    VaciarPila(S);
    CopiarPila(&Result, S);

    Destroy(&Temp);
    Destroy(&Result);
}

float EvaluarPost(pila *S)
{
    elemento e, u;
    float a, b;
    pila Aux;
    Initialize(&Aux);

    while (!Empty(S))
    {
        e = Pop(S);

        if (e.tipo == TRUE)
        {
            Push(&Aux, e);
        }
        else
        {
            if (Size(&Aux) < 2)
            {
                printf("Error: operandos insuficientes\n");
                Destroy(&Aux);
                return 0;
            }

            b = Pop(&Aux).numero;
            a = Pop(&Aux).numero;

            u.numero = Operacion(a, b, e.caracter);
            u.tipo = TRUE;
            Push(&Aux, u);
        }
    }

    float R = 0.2f;
    if (!Empty(&Aux))
        R = Pop(&Aux).numero;

    Destroy(&Aux);
    return R;
}

float Operacion(float a, float b, char Op)
{
    float R;
    switch (Op)
    {
    case '+':
        R = a + b;
        break;
    case '-':
        R = a - b;
        break;
    case '*':
        R = a * b;
        break;
    case '/':
        R = a / b;
        break;
    case '^':
        R = pow(a, b);
        break;
    default:
        R = 0;
    }
    return R;
}

int Precedencia(char C)
{
    float Op;
    switch (C)
    {
    case ')': 
        Op = 4;
        break;
    case '^': 
        Op =  3;
        break;
    case '*':
    case '/': 
        Op =  2;
        break;
    case '+':
    case '-': 
        Op =  1;
        break;
    case '(': 
        Op =  0;
        break;
    default:  
        Op = -1;
    }
    return Op;
}

void CopiarPila(pila *Origen, pila *Destino)
{
    pila Aux;
    Initialize(&Aux);

    VaciarPila(Destino);
    Flip(Origen);

    while (!Empty(Origen))
    {
        elemento e = Pop(Origen);
        Push(&Aux, e);
        Push(Destino, e);
    }

    Flip(&Aux);
    while (!Empty(&Aux))
        Push(Origen, Pop(&Aux));

    Destroy(&Aux);
}

void VaciarPila(pila *S)
{
    while (!Empty(S))
        Pop(S);
}

void Debug(pila *S)
{
    pila Copia;
    Initialize(&Copia);
    CopiarPila(S, &Copia);

    while (!Empty(&Copia))
    {
        elemento e = Pop(&Copia);
        if (e.tipo == FALSE)
            printf("%c", e.caracter);
        else
            printf("(%.2f)", e.numero);
    }

    printf("\n");
    Destroy(&Copia);
}
