%{

    #include <iostream>
    #include <string>
    #include <cstring>
    #include <stdio.h>
    #include <stdexcept>
    #include "json_st.hh"
    
    extern "C" 
    {
        void yyerror(const char *);
        int yylex();
    } 
        
    void load_string(const char *);
    void load_file(FILE*);
    JSON::Value* parsd = nullptr;
%}

%code requires { #include "json_st.hh" }

%union
{
    // "Pure" types
    long long int int_v;
    long double float_v;
    bool bool_v;
    bool null_p;
    char* string_v;
    
    // Pointers to more complex classes
    JSON::Object* object_p;
    JSON::Array* array_p;
    JSON::Value* value_p;
} 

/** Define types for union values */
%type<string_v> SINGLE_QUOTED_STRING DOUBLE_QUOTED_STRING WORD_STRING string property
%type<int_v> NUMBER_I
%type<float_v> NUMBER_F
%type<bool_v> BOOLEAN
    
/** Declare tokens */
%token COMMA COLON
%token SQUARE_BRACKET_L SQUARE_BRACKET_R
%token CURLY_BRACKET_L CURLY_BRACKET_R
%token DOUBLE_QUOTED_STRING SINGLE_QUOTED_STRING WORD_STRING
%token NUMBER_I NUMBER_F
%token BOOLEAN
%token NULL_T

%type <object_p> object assignment_list
%type <array_p> array list
%type <value_p> value

%start json

%%

// Entry point (every JSON file represents a value)
json: value { parsd = $1; } ;

// Object rule
object: CURLY_BRACKET_L assignment_list CURLY_BRACKET_R { $$ = $2; } ;

// Array rule
array : SQUARE_BRACKET_L list SQUARE_BRACKET_R { $$ = $2; } ;

// Values rule
value : NUMBER_I { $$ = new JSON::Value($1); }
    | NUMBER_F { $$ = new JSON::Value($1); }
    | BOOLEAN { $$ = new JSON::Value($1); }
    | NULL_T { $$ = new JSON::Value(); }
    | string { $$ = new JSON::Value((std::string($1))); free($1); }
    | object { $$ = new JSON::Value((*$1)); delete $1; }
    | array { $$ = new JSON::Value((*$1)); delete $1; }
    ;

// String rule
string : DOUBLE_QUOTED_STRING { $$ = $1; } 
    | SINGLE_QUOTED_STRING { $$ = $1; }
    ;

property: string { $$ = $1; }
    | WORD_STRING {
        $$ = $1;
    };

// Assignments rule
assignment_list: /* empty */ { $$ = new JSON::Object(); } 
    | property COLON value {
        $$ = new JSON::Object();
        $$->insert(std::make_pair(std::string($1), (*$3)));
        free($1);
        delete $3;
    } 
    | assignment_list COMMA property COLON value { 
        $$->insert(std::make_pair(std::string($3), (*$5)));
        free($3);
        delete $5;
    }
    ;
    
// List rule
list: /* empty */ { $$ = new JSON::Array(); }
    | value {
        $$ = new JSON::Array();
        $$->push_back((*$1));
        delete $1;
    }
    | list COMMA value { 
        $$->push_back((*$3)); 
        delete $3;
    }
    ;
    
%%

JSON::Value parse_file(const char* filename)
{    
    FILE* fh = fopen(filename, "r");
    JSON::Value v;
    
    if (fh)
    {
        load_file(fh);
        int status = yyparse();
        
        if (status)
            throw std::runtime_error("Error parsing file: JSON syntax.");
        else
            v = *parsd;
        
        delete parsd;
    } 
    else
        throw std::runtime_error("Impossible to open file.");

    return v;
}

JSON::Value parse_string(const std::string& s)
{
    load_string(s.c_str());
    
    int status = yyparse();
    
    if (status)
    {
        throw std::runtime_error("Error parsing file: JSON syntax.");
        delete parsd;
    }
    else
    {
        JSON::Value v = *parsd;
        delete parsd;
        return v;    
    }
}

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
}
