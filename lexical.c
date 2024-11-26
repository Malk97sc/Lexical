#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int statenumb(const char *st, int mtx[][4]); //AFD Numbers
int var(char ch);
int stateVar(const char *st, int mtx[][3]); //AFD variables
int isdelimiter(char ch);
const char* delimiterName(const char *token);
int isoperator(char ch);
const char *operatorName(const char *token);
const char *iskeyword(char *tok);
void analizer(const char *source);
void readFile(const char *file, char **source);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Send a file\n");
        return EXIT_FAILURE;
    }
    const char *st = argv[1];
    char *file;

    readFile(st, &file);
    if(file){
        analizer(file);
        free(file);
    }  
    
    return EXIT_SUCCESS;
}

void readFile(const char *file, char **source){
    FILE *fl;
    fl = fopen(file, "r");
    if(!fl){perror("FAIL TO OPEN FILE\n");} 
    char tok;
    int indx = 0;
    long size;

    fseek(fl, 0, SEEK_END); //send the pointer to the end the file
    size = ftell(fl); //get the size of file by the end position
    fseek(fl, 0, SEEK_SET); //send the pointer to start

    (*source) = (char*) malloc((size + 1) * sizeof(char)); // +1 saving the end file '\0'
    if(!(*source)){
        perror("FAIL TO SAVE MEMORY\n");
        fclose(fl);
    }

    while((tok = fgetc(fl)) != EOF){ //read file
        (*source)[indx++] = tok; 
    }
    (*source)[indx] = '\0';

    printf("The file is: \n\n");

    for(int i=0; i < indx; i++){
        printf("%c", (*source)[i]);  //show the file
    }

    fclose(fl);
}

void analizer(const char *source){
    int constNumb[9][4] = {// Digit, spot, sign, exp 
        {2, 7, 1, 8}, // state 0
        {2, 7, 8, 8}, // state 1
        {2, 3, 8, 4}, // state 2 (A)
        {3, 8, 8, 4}, // state 3 (A)
        {6, 8, 5, 8}, // state 4
        {6, 8, 8, 8}, // state 5
        {6, 8, 8, 8}, // state 6 (A)
        {3, 8, 8, 8}, // state 7
        {8, 8, 8, 8}  // state 8 (Error)
    };
    int var[3][3] = {
        {1, 2, 1}, //state 0
        {1, 1, 1}, //state 1 (A)
        {2, 2, 2} //state 2
    };

    char tok[50]; //save temporaly tokens   
    int tokIndex, j;

    printf("\n\nThe tokens in the file: \n\n");

    for(int i=0; i < strlen(source); i++){
        if(!isspace(source[i])){ //skip spaces
            tokIndex = 0; //restart index to tokens
            
            if(source[i] == '#'){
                j = i;
                tok[tokIndex++] = source[j++];
                while(isspace(source[j])) j++; //skip spaces in # include

                while(!isspace(source[j]) && source[j] != '\0'){
                    tok[tokIndex++] = source[j++]; //save the include
                }
                tok[tokIndex] = '\0';

                if(!strcmp(tok, "#include")){  //if is #include in the token
                    printf("TOKEN (INCLUDE): %s\n", tok); 
                    
                    while(isspace(source[j])) j++; //skip spaces between #include and <header.h>
 
                    if(source[j] == '<'){ 
                        tokIndex = 0;
                        tok[tokIndex++] = source[j++];

                        while(source[j] != '>' && source[j] != '\0'){
                            if(isprint(source[j])){
                                tok[tokIndex++] = source[j++];
                            } else {
                                printf("Error: Invalid character in header\n");
                                break;
                            }
                        }

                        if(source[j] == '>'){
                            tok[tokIndex++] = source[j++];
                            tok[tokIndex] = '\0';  //end the token
                            printf("TOKEN (HEADER): %s\n", tok);
                        }else{
                            printf("Error: Unterminated header file\n");
                        }
                    }else{
                        printf("Error: Unknown preprocessor directive: %s\n", tok);  // manejar directivas desconocidas
                    }
                }
                i = j-1;

            }else if(source[i] == '"'){ //detect strings
                j = i + 1; //start after opening '"'
                while(source[j] != '"' && source[j] != '\0'){
                    tok[tokIndex++] = source[j++];
                }
                if(source[j] == '"'){
                    tok[tokIndex] = '\0';
                    printf("TOKEN (STRING): \"%s\" \n", tok);                    
                    i = j; // move to closing '"'
                }else{
                    printf("Error: Unterminated string literal\n");
                }
            }else if(isalpha(source[i]) || source[i] == '_'){ //variable ID and keyword
                j = i; //to move 
                while(!isdelimiter(source[j]) && !isspace(source[j])  && !isoperator(source[j]) &&  source[j] != '\0'){
                    tok[tokIndex++] = source[j++]; // Traverse the variable until the end and store it in the token
                }
                tok[tokIndex] = '\0'; //in the last position end the tok
                i = j-1; //update i to the last processed index

                const char *keywordName = iskeyword(tok);
                if(keywordName){ //if have something in the keywordName, printf
                    printf("TOKEN (%s): %s\n", keywordName, tok);
                }else if(stateVar(tok, var) == 1){
                    printf("TOKEN (ID): %s\n", tok);
                }else{
                    printf("Error: %s is not a valid variable\n", tok);
                }
            }else if(isdigit(source[i])){ //digit
                j = i;//to move
                while(!isdelimiter(source[j]) && !isspace(source[j]) && source[j] != '\0'){
                    tok[tokIndex++] = source[j++]; 
                }
                tok[tokIndex] = '\0';
                i = j-1; //update i to the last processed index

                if(statenumb(tok, constNumb) == 8){
                    printf("Error: %s is not a valid number\n", tok);
                }else{                    
                    printf("TOKEN (NUMBER): %s\n", tok);
                }
            }else if(isoperator(source[i])){ //operator
                j = i;
                while(isoperator(source[j]) && source[j] != '\0'){
                    tok[tokIndex++] = source[j++];
                }
                tok[tokIndex] = '\0';
                i = j-1;
                printf("TOKEN (%s): %s\n", operatorName(tok), tok);
            }else if(isdelimiter(source[i])){ //delimiter
                tok[0] = source[i];
                tok[1] = '\0'; //the second position has the end
                printf("TOKEN (%s): %c\n", delimiterName(tok), source[i]);
            }else{
                printf("Error: '%c'\n", source[i]);
            }
        }
    }
}

const char *iskeyword(char *tok){
    const char *keyw[][2] = {
        {"if", "IF"}, {"else", "ELSE"}, {"while", "WHILE"}, 
        {"return", "RETURN"}, {"int", "INT"}, {"float", "FLOAT"},
        {"main", "MAIN"}, {"void", "VOID"}, {"break", "BREAK"}, 
        {"do", "DO"}, {"scanf", "SCANF"}, {"printf", "PRINTF"},
        {"for", "FOR"}, {"char", "CHAR"}, {"NULL", "NULL"}, {"struct", "STRUCT"}
    };
    int size = sizeof(keyw)/sizeof(keyw[0]);

    for(int i=0; i < size; i++){
        if(strcmp(tok, keyw[i][0]) == 0){
            return keyw[i][1]; //return the keyword name
        }
    }
    return NULL; //if not found, return NULL
}

const char *operatorName(const char *token){
    const char *name[][2]={
        {"<<", "SHL_OP"},
        {">>", "SHR_OP"}, {"==", "EQ"},
        {"!=", "NOTEQ"}, {"<=", "LTEQ"},
        {"*=", "MUL_ASSIG"}, {"+=", "SUM_ASSIG"},
        {">=", "GTEQ"}, {"&&", "ANDAND"},
        {"||", "OROR"}, {"++", "INCRE"},
        {"--", "DECR"}, {"+", "PLUS"},
        {"-", "MINUS"}, {"*", "MUL_OP"},
        {"/", "DIV_OP"}, {"&", "AND_OP"},
        {"|", "OR_OP"}, {"!", "NOT_OP"},
        {"=", "ASSIGN"}, {"<", "LT"},
        {">", "GT"}, {":", "TERNARY"},
        {"?", "TERNARY"}
    };

    int size = sizeof(name) / sizeof(name[0]);

    for(int i=0; i < size; i++){
        if(!strcmp(token, name[i][0])){  //equal to (strcmp(token, name[i][0]) == 0)
            return name[i][1];
        }
    }
    return "UNKNOWN"; //if the token is not found
}

int isoperator(char ch){
    const char op[] = {'+', '-', '*', '/', '=', ':', '?', '!', '&', '|', '^', '~', '>', '<'};
    int flag =0, size = sizeof(op)/sizeof(op[0]);

    for(int i=0; i < size; i++){ //comprare the character with operators
        if(op[i] == ch){
            flag = 1; //if match found, set flag to 1
            break;
        }
    }
    return flag; //return 1 if its a operator
}

const char *delimiterName(const char *token){
    const char *names[][2]={
        {"{", "LBRACE"}, {"}", "RBRACE"},
        {"[", "LSQUARE"}, {"]", "RSQUARE"},
        {"(", "LPAR"}, {")", "RPAR"},
        {";", "SEMI"}, {",", "COMMA"},
        {"\n", "NEWLINE"}, {"\0", "NULL_CHAR"},
        {"\r", "CARRIAGE_RETURN"}, {"\t", "TAB"},
        {"\\", "BACKSLASH"}, {"\v", "VERTICAL_TAB"},
        {"\f", "FORM_FEED"}, {"\a", "ALERT"},
        {"\"", "QUOTE"}
    };

    int size = sizeof(names) / sizeof(names[0]);
    for(int i=0; i < size; i++){
        if(!strcmp(token, names[i][0])){ //if the element in the matrix is equal to the token
            return names[i][1];
        } 
    }
    return "UNKNOWN"; //if the token is not found
}

int isdelimiter(char ch){
    const char del[] = { ',', '[', ']', '(', ')', '{', '}', ';', '\n', '\0', '\r', '\t', '\\', '\v', '\f', '\a'};
    int flag = 0, size = sizeof(del)/sizeof(del[0]); // Calculate size of the delimiter array

    for(int i = 0; i < size; i++){ //compare the character with delimiters
        if(del[i] == ch){ 
            flag = 1; //if match found, set flag to 1
            break;
        }
    }
    return flag; //return 1 if it's a delimiter
}

int stateVar(const char *st, int mtx[][3]){ //afd variables
    int state = 0, type; //initial state and type of variable
    char current;

    while((current = *st++) != '\0'){
        type = var(current);
        if(type == 3){ //
            state = 2; //state error
            break;
        }
        state = mtx[state][type];
    }

    return state;
}

int var(char ch){
    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) return 0; //letter
    if(ch >= '0' && ch <= '9') return 1; //digit
    if(ch == '_' || ch == '$') return 2; // _ o $
    return 3; //other
}

int statenumb(const char *st, int mtx[][4]){ //afd numbers
    int state=0, type;
    char current;

    while((current = *st++) != '\0'){
        if(isdigit(current)){
            type = 0; //digit 
        }else if(current == '.'){
            type = 1; //spot
        }else if(current  == '+' || current == '-'){
            type = 2; //sign
        }else if(current == 'e' || current == 'E'){
            type = 3; //exp
        }else{
            state = 8; //other case
            break;
        }
        state = mtx[state][type];
    }
    
    return state;
}
