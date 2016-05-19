#include "f_base.h"


void inicia_fila(fila **pista)
{
    *pista = NULL;
}

fila *cria_nodo()
{
    return (fila*)(malloc(sizeof(fila)));
}

void push(fila **pista, aviao *dado)
{
    fila *nodo,*temp = *pista;
    nodo = cria_nodo();
    nodo->dado = dado;
    nodo->prox = NULL;
    if (*pista == NULL)
    {
        *pista = nodo;
    }
    else
    {
        while(temp->prox != NULL)
        {
            temp = temp->prox;
        }
        temp->prox = nodo;
    }

}

aviao *pop(fila **pista)
{
    aviao *dado;

    fila *nodo = *pista;
    if ((*pista) != NULL)
    {

        dado = nodo->dado;
        nodo = nodo->prox;
        *pista = nodo;
        free(&nodo);
    }
    else
    {

        return NULL;
    }
    return(dado);
}



aviao *cria_nodo_aviao()
{
    return (aviao*)(malloc(sizeof(aviao)));
}

int contar_elem_fila(fila *fil){
    int count = 0;
    fila *aux = fil;

    while(aux != NULL){
        aux = aux->prox;
        count++;
    }
    return count;

}

