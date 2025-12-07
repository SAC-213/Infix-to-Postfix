#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pila_din.h"

/*
Programa: 03.c
Versión: 1.30 — 06 de diciembre del 2025
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

    Flag = VerificarParentesis(&Parentesis);
    Destroy(&Parentesis);
    (Flag == 1) ? printf("Parentesis bien colocados\n") : (printf("\nError, parentesis mal colocados\n"), exit(1));

    PostFijo(&Expresion);

    SustituirLiterales(&Expresion);

    R = EvaluarPost(&Expresion);

    printf("El resultado de la expresion es: %.2f", R);

    return 0;
}

void Lectura(pila *S, pila *P)
{
    printf("Ingrese expresion: ");
    char c;
    elemento e;
    while ((c = getchar()) != '\n')
    {
        if (c == '(' || c == ')')
        {
            e.caracter = c;
            Push(P, e);
            Push(S, e);
        }
        else
        {
            e.caracter = c;
            Push(S, e);
        }
    }
}

int VerificarParentesis(pila *P)
{
    pila Aux;
    Initialize(&Aux);
    char c;
    int Flag, i;
    Flag = FALSE;

    if (Size(P) % 2 == 0)
    {
        i = 0;
        Flag = TRUE;
        while (!Empty(P))
        {
            if (Top(P).caracter == ')')
            {
                Push(&Aux, Pop(P));
            }
            if (Top(P).caracter == '(')
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
        Flag = (Empty(&Aux)) ? TRUE : FALSE;
    }
    Destroy(&Aux);
    return Flag;
}

void PostFijo(pila *S)
{
    pila Aux;
    pila Copia;
    elemento e;
    Initialize(&Aux);
    Initialize(&Copia);
    CopiarPila(S, &Copia);
    VaciarPila(S);
    Flip(&Copia);
    while (!Empty(&Copia))
    {
        if ((Top(&Copia).caracter >= 'A' && Top(&Copia).caracter <= 'Z') || (Top(&Copia).caracter >= 'a' && Top(&Copia).caracter <= 'z'))
        {
            printf("%c", Top(&Copia).caracter);
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
                printf("%c", Top(&Aux).caracter);
                Push(S, Pop(&Aux));
            }
            Pop(&Aux);
            Pop(&Copia);
        }
        else
        {
            while (!Empty(&Aux) && (Precedencia(Top(&Aux).caracter) >= Precedencia(Top(&Copia).caracter)) && Top(&Aux).caracter != '(')
            {
                printf("%c", Top(&Aux).caracter);
                Push(S, Pop(&Aux));
            }
            Push(&Aux, Pop(&Copia));
        }
    }
    while (!Empty(&Aux))
    {
        printf("%c", Top(&Aux).caracter);
        Push(S, Pop(&Aux));
    }
    Destroy(&Aux);
}

void SustituirLiterales(pila *S)
{
    pila Aux;
    Initialize(&Aux);
    float a;
    elemento e;
    Flip(S);
    while (!Empty(S))
    {
        if ((Top(S).caracter >= 'A' && Top(S).caracter <= 'Z') || (Top(S).caracter >= 'a' && Top(S).caracter <= 'z'))
        {
            printf("\nIngrese un caracter para %c: ", Top(S).caracter);
            Pop(S);
            scanf("%f", &a);
            e.numero = a;
            e.tipo = TRUE;
            Push(&Aux, e);
        }
        else
        {
            e = Pop(S);
            e.tipo = FALSE;
            Push(&Aux, e);
        }
    }
    Flip(&Aux);
    CopiarPila(&Aux, S);
    VaciarPila(&Aux);
}

float EvaluarPost(pila *S)
{
    elemento e, u;
    float R;
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
            b = Pop(&Aux).numero;
            a = Pop(&Aux).numero;
            u.numero = Operacion(a, b, e.caracter);
            u.tipo = TRUE;
            Push(&Aux, u);
        }
    }
    R = Aux.tope->e.numero;
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
        R = pow((double)a, (double)b);
        break;
    default:
        0;
        break;
    }
    return R;
}

int Precedencia(char C)
{
    int R;
    switch (C)
    {
    case ')':
        R = 4;
        break;
    case '^':
        R = 3;
        break;
    case '*':
        R = 2;
        break;
    case '/':
        R = 2;
        break;
    case '+':
        R = 1;
        break;
    case '-':
        R = 1;
        break;
    case '(':
        R = 0;
        break;
    default:
        R = -1;
    }
    return R;
}

void CopiarPila(pila *Origen, pila *Destino)
{
    for (int i = 1; i <= Size(Origen); i++)
    {
        Push(Destino, Element(Origen, i));
    }
}

void VaciarPila(pila *S)
{
    while (!Empty(S))
    {
        Pop(S);
    }
}

void Debug(pila *S)
{
    pila Copia;
    CopiarPila(S, &Copia);
    while (!Empty(&Copia))
    {
        if (!(Top(&Copia).tipo))
        {
            printf("%c", Pop(&Copia).caracter);
        }
        else
        {
            printf("(%.2f)", Pop(&Copia).numero);
        }
    }

    Destroy(&Copia);
}