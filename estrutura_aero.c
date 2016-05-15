#include "f_base.h"




fila *pista_decolagem;
fila *pista_pouso;

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

void yay(){
    printf("**yay**");
}

aviao *pop(fila **pista)
{
    aviao *dado;
    fila *nodo = *pista;
    if ((*pista) =! NULL)
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

void main2()
{
    int c;

    inicia_fila(&pista_decolagem);
    inicia_fila(&pista_pouso);

    push(&pista_pouso,2);
    push(&pista_pouso,4);
    push(&pista_pouso,6);

    push(&pista_decolagem,1);
    push(&pista_decolagem,3);
    push(&pista_decolagem,5);

    c = pop(&pista_decolagem);
    printf("%d",c);


    c = pop(&pista_pouso);
    printf("%d",c);


    c = pop(&pista_decolagem);
    printf("%d",c);

}
