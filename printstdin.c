#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FACTOR 2
#define DEFLINE 10
#define DEFCHAR 5

typedef struct _line {
    char *content;
    int n_char;
} line_t;
typedef line_t *line_pt;


typedef struct _context {
    line_pt *linearrays;
    int n_line;
} context_t;
typedef context_t *context_pt;



line_pt new_line_with_c(int n_char)
{
    line_t *line = (line_t *) malloc(sizeof(line_t));
    line->content = (char *) malloc(n_char + 1);
    line->n_char = n_char;
    return line;
}

context_pt init_context(int n_line)
{
    context_pt cont = (context_pt) malloc(sizeof(context_t));
    cont->linearrays = (line_pt *) malloc(sizeof(line_pt) * n_line);
    cont->n_line = n_line;
    memset(cont->linearrays, 0, n_line);
    return cont;
}

int large_line(line_pt line, int factor)
{
    int olen = line->n_char;
    int nlen = olen * factor;
    char *ncontent = (char *) malloc(nlen);
//    strlcpy((char*)ncontent,(char*)(line->content),olen+1);
    memcpy((void *) ncontent, (void *) (line->content), olen + 1);
    free(line->content);

    line->content = ncontent;
    line->n_char = nlen;

    return nlen;
}

void del_line(line_pt pline)
{
    if (NULL != pline) {
	if (NULL != pline->content) {
	    free(pline->content);
	}
	free(pline);
    }
}

void fini_context(context_pt cont)
{
    if (NULL != cont) {

	if ((cont->n_line) > 0) {
	    int n_lines=cont->n_line;
	    line_pt *larr = cont->linearrays;
	    for (int i = 0; larr[i] != 0&&i<n_lines; i++) {
		del_line(larr[i]);

	    }
	    free(cont->linearrays);
	    free(cont);
	}

    }
}

int large_context(context_pt cont, int factor)
{
    int olen = cont->n_line;
    line_pt *olinearrays = cont->linearrays;
    int nlen = factor * olen * sizeof(int *);
    line_pt *nlinearrays = (line_pt*) malloc(nlen);
    memcpy(nlinearrays, olinearrays, olen*sizeof(int));
    free(olinearrays);
    //printf("free.....\n");
    memset(nlinearrays + olen*sizeof(int*), 0, (nlen-olen)*sizeof(int*));
    cont->linearrays=nlinearrays;
    cont->n_line = factor * olen;
    return factor * olen;

}

/*
 *dose not work well & and make me confused... @.@
 *
 
int read_into_context_o(context_t * cont)
{
    int c;
    int i;
    int ia = 0;
    line_pt line = cont->linearrays[ia];
    if (line == 0) {
	line = new_line_with_c(DEFCHAR);
	cont->linearrays[ia] = line;    
    }

    int caplen = line->n_char;
    int cn_lines = cont->n_line;

    for (i = 0; EOF != (c = getchar()); i++) {
	line->content[i] = c;
	if (caplen - 1 == i) {
	    if (c != '\n') {
		caplen = large_line(line, FACTOR);
	    } else {
		line->content[i] = '\0';
		ia++;
		i=0;
		if (ia >= cn_lines) {
		    cn_lines = large_context(cont, FACTOR);
		}
		line = cont->linearrays[ia];
		if (line == 0) {
		    cont->linearrays[ia] = new_line_with_c(DEFCHAR);
		    line = cont->linearrays[ia];
		    caplen=DEFCHAR;
		}
	    }
	} else {
	    if (c == '\n') {
		line->content[i] = '\0';
		ia++;
		i=0;
		if (ia >= cn_lines) {
		    cn_lines = large_context(cont, FACTOR);
		}
		line = cont->linearrays[ia];
		if (line == 0) {
		    cont->linearrays[ia] = new_line_with_c(DEFCHAR);
		    line = cont->linearrays[ia];		    
		    caplen=DEFCHAR;
		}
	    }
	}
    }
    return cn_lines;
}
*/

int read_into_line(line_pt line)
{

    int caplen=line->n_char;
    int i;
    int c;
    for(i=0;EOF!=(c=getchar());i++){

	if(c=='\n'){
	    line->content[i]='\0';
	    return 0;
	}else{
	    if(i==caplen-1){
		caplen=large_line(line,FACTOR);
	    }
	    line->content[i]=c;
	}	
    }
    return 1;
}


void read_into_context(context_pt cont)
{
    int i=0;
    int n_lines=cont->n_line;
    line_pt cline=cont->linearrays[i];
    int rt=0;
    while(1){
	cline=new_line_with_c(DEFCHAR);
	rt=read_into_line(cline);
//	printf("=%s=\n",cline->content);
	if(rt){
	    break;
	}
	cont->linearrays[i]=cline;
	
	i++;
	if(i==n_lines){
	    n_lines=large_context(cont,FACTOR);
	}
	cline=cont->linearrays[i];	
    }
}



void print_context(context_pt cont)
{
    int i = 0;
    int total_char=0;
    while (cont->linearrays[i]) {
	total_char+=cont->linearrays[i]->n_char;
	printf(":%3d:%3d:%s:\n", i,cont->linearrays[i]->n_char,cont->linearrays[i]->content);
	i++;
    }
    printf("Total chars: %d\n",total_char);

}


int main(int argc, char **argv)
{

    context_pt context = init_context(DEFLINE);

    read_into_context(context);

    print_context(context);
    fini_context(context);
    
    return 0;

}
