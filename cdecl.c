#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKENS 100
#define MAXTOKENLEN 64

enum type_tag{ IDENTIFIER, QUALIFIER, TYPE };

struct token {
		char type;
		char string[MAXTOKENLEN];
};

int top = -1;
struct token stack[MAXTOKENS];
struct token this;

#define pop stack[top--]
#define push(s) stack[++top] = s
#define SRTCMP(s, r, x) (strcmp(s, x) r 0)

enum type_tag classify_string(void)
{
		char *s = this.string;
		if(SRTCMP(s, ==, "const"))
		{
				strcpy(s, "read-only");
				return QUALIFIER;
		}
		if(SRTCMP(s, ==, "volatile")) return QUALIFIER;
		if(SRTCMP(s, ==, "void")) return TYPE;
		if(SRTCMP(s, ==, "char")) return TYPE;
		if(SRTCMP(s, ==, "signed")) return TYPE;
		if(SRTCMP(s, ==, "unsigned")) return TYPE;
		if(SRTCMP(s, ==, "short")) return TYPE;
		if(SRTCMP(s, ==, "int")) return TYPE;
		if(SRTCMP(s, ==, "long")) return TYPE;
		if(SRTCMP(s, ==, "float")) return TYPE;
		if(SRTCMP(s, ==, "double")) return TYPE;
		if(SRTCMP(s, ==, "struct")) return TYPE;
		if(SRTCMP(s, ==, "union")) return TYPE;
		if(SRTCMP(s, ==, "enum")) return TYPE;
		return IDENTIFIER;
}

void gettoken(void)
{
		char *p = this.string;

		while((*p = getchar()) == ' ');

		if(isalnum(*p))
		{
				while(isalnum(*++p = getchar()));
				ungetc(*p, stdin);
				*p = '\0';
				this.type = classify_string();
				return;
		}

		if(*p == '*')
		{
				strcpy(this.string, "pointer to");
				this.type = '*';
				return;
		}
		this.string[1] = '\0';
		this.type = *p;
		return;
}

void read_to_first_identifer()
{
		gettoken();
		while(this.type != IDENTIFIER)
		{
				push(this);
				gettoken();
		}
		printf("%s is ", this.string);
		gettoken();
}

void deal_with_array()
{
		while(this.type == '[')
		{
				printf("array ");
				gettoken();
				if(isdigit(this.string[0]))
				{
						printf("0..%d ", atoi(this.string) - 1);
						gettoken();
				}
				gettoken();
				printf("of ");
		}
}

void deal_with_function_args()
{
		while(this.type != ')')
		{
				gettoken();
		}
		gettoken();
		printf("function returning ");
}

void deal_with_pointers()
{
		while(stack[top].type == '*')
		{
				printf("%s ", pop.string);
		}
}

void deal_with_declarator()
{
		switch(this.type)
		{
				case '[': deal_with_array(); break;
				case '(': deal_with_function_args(); break;
		}
		deal_with_pointers();

		while(top >= 0)
		{
				if(stack[top].type == '(')
				{
						pop;
						gettoken();
						deal_with_declarator();
				}
				else
				{
						printf("%s ", pop.string);
				}
		}
}

int main(int argc, char** argv)
{
		read_to_first_identifer();
		deal_with_declarator();
		printf("\n");
		return 0;
}
