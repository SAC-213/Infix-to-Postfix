/*
Programa: pila_din.c
Versión: 1.1 — 07 de diciembre del 2025
Autor(es): Ascencion Cruz Saul

Descripción:
Archivo a convertir en .o con la lógica de la librería de pila dinámica.
*/

#include <stdlib.h>
#include <stdio.h>
#include "pila_din.h"

/*
Inicializar pila (Initialize): recibe<- pila (s);
Initialize(s);
Efecto: Recibe una pila y la inicializa para su trabajo normal.
*/
void Initialize(pila *s)
{
	s->size = 0;
	(*s).tope = NULL;
	return;
}

/*
Empilar (Push): recibe<- pila (s); recibe<- elemento (e)
Push(s,e);
Efecto: Recibe la pila y aumenta su tamaño, poniendo el elemento en la cima de la pila.
*/
void Push(pila *s, elemento e)
{
	nodo *aux;

	aux = malloc(sizeof(nodo));

	if (aux != NULL)
	{
		s->size++;
		aux->e = e;
		aux->abajo = s->tope;
		s->tope = aux;
	}
	else // Desbordamiento de pila
	{
		printf("\nERROR Push(s,e):\"Desbordamiento de pila\"");
		exit(1);
	}
	return;
}

/*
Desempilar (Pop): recibe<- pila (s); retorna -> elemento
e=Pop(s);
Efecto: Recibe la pila, remueve el elemento tope y lo retorna.
Excepción: si la pila esta vacía, produce error.
*/
elemento Pop(pila *s)
{
	elemento r;
	nodo *aux;
	if (s->tope != NULL)
	{
		r = s->tope->e;
		aux = s->tope;
		s->tope = s->tope->abajo; // s->tope=aux->abajo
		free(aux);
		s->size--;
	}
	else // Subdesbordamiento de la pila
	{
		printf("\nERROR e=Pop(s):\"Subdesbordamiento de pila\"");
		exit(1);
	}

	return r;
}

/*
Es vacía (Empty): recibe<- pila (s); retorna -> boolean
b=Empty(s);
Efecto: Recibe una pila y devuelve true si esta vacía y false en caso contrario.
*/
boolean Empty(pila *s)
{
	boolean r;
	if (s->tope != NULL)
		r = FALSE;
	else
		r = TRUE;

	return r;
}

/*
Tope pila (Top): recibe<- pila (s); retorna -> elemento
e=Top(s);
Efecto: Devuelve el elemento cima de la pila.
Excepción: si la pila esta vacía produce error
*/
elemento Top(pila *s)
{
	elemento r;
	if (s->tope != NULL)
	{
		r = s->tope->e;
	}
	else // Subdesbordamiento de la pila
	{
		printf("\nERROR e=Top(s):\"Subdesbordamiento de pila\"");
		exit(1);
	}

	return r;
}

/*
Tamaño pila (size): recibe<- pila (s); retorna -> tamaño de la pila (entero)
n=size(s);
Efecto: Devuelve el número de elementos que contiene la pila (Altura de la pila).
*/
int Size(pila *s)
{
	int r = s->size;
	/*int r=0;
	nodo *aux;
	aux=s->tope;
	while(aux!=NULL)
	{
		r++;
		aux=aux->abajo;
	}
*/
	return r;
}

/*
Eliminar pila (Destroy): recibe<- pila (s)
Destroy(s);
Efecto: Recibe una pila y la libera completamente
*/
void Destroy(pila *s)
{
	nodo *aux;
	if (s->tope != NULL)
	{
		do
		{
			aux = s->tope->abajo;
			free(s->tope);
			s->tope = aux;
		} while (aux != NULL);
	}
	s->size = 0;
	return;
}

/*
Busca elemento (Element): recibe<- pila (S); recibe<- numero(n); retorna-> elemento (elemento)
Element(s,n);
Efecto: Recibe una pila y busca en ella un elemento en una posición dada
*/
elemento Element(pila *S, int n)
{
	nodo *aux;
	elemento r;
	int d;
	d = (Size(S) - n);
	aux = S->tope;
	if (1 <= n && n <= Size(S))
	{
		for (int i = 0; i < d; i++)
		{
			aux = aux->abajo;
		}
		r = aux->e;
	}
	else
	{
		printf("\nERROR n=Element(s,n):\"Intenta acceder a indice fuera de la pila\"");
		exit(1);
	}
	return r;
}

/*
Voltea pila (Flip): recibe<- pila (S); retorna-> pila (S)
Flip(s);
Efecto: Recibe una pila e invierte el orden de la pila,
el primer elemento se convierte en el tope y el tope en el
primero
*/
void Flip(pila *s)
{
	pila aux;
	Initialize(&aux);

	while (!Empty(s))
	{
		Push(&aux, Pop(s));
	}

	// Regresar contenido
	while (!Empty(&aux))
	{
		Push(s, Pop(&aux));
	}
}