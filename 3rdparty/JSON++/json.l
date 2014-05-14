%option noyywrap yylineno nodefault
%option stack
%{
    // Provided externally
    extern "C" 
    {
        int yylex();
    }
    
    #include <cstdio>
    #include <string>
    #include <codecvt>
    // Needed for the tokens
    #include "json.tab.hh"
%}

%x DQSTR
%x SQSTR
%%
	std::string string_buf;

\"                                           { BEGIN(DQSTR); string_buf.clear(); }
\'                                           { BEGIN(SQSTR); string_buf.clear(); }
<DQSTR>[^\"\\]+                              { string_buf.append(yytext); }
<SQSTR>[^\'\\]+                              { string_buf.append(yytext); }
<DQSTR,SQSTR>\\\"                            { string_buf.append("\""); }
<DQSTR,SQSTR>\\\'                            { string_buf.append("\'"); }
<DQSTR,SQSTR>\\\\                            { string_buf.append("\\"); }
<DQSTR,SQSTR>\\\/                            { string_buf.append("/"); }
<DQSTR,SQSTR>\\b                             { string_buf.append("\b"); }
<DQSTR,SQSTR>\\f                             { string_buf.append("\f"); }
<DQSTR,SQSTR>\\n                             { string_buf.append("\n"); }
<DQSTR,SQSTR>\\r                             { string_buf.append("\r"); }
<DQSTR,SQSTR>\\t                             { string_buf.append("\t"); }
<DQSTR,SQSTR>\\u[0-9a-fA-F]{4}               { wchar_t ch; sscanf(yytext,"\\u%x",&ch); string_buf.append(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(ch)); }
<DQSTR>\"                                    { BEGIN(INITIAL); yylval.string_v = strdup(string_buf.c_str()); return DOUBLE_QUOTED_STRING; }
<SQSTR>\'                                    { BEGIN(INITIAL); yylval.string_v = strdup(string_buf.c_str()); return SINGLE_QUOTED_STRING; }
<DQSTR,SQSTR>.                               { string_buf.append(yytext); }

"["                                     { return SQUARE_BRACKET_L; }
"]"                                     { return SQUARE_BRACKET_R; }
"{"                                     { return CURLY_BRACKET_L; }
"}"                                     { return CURLY_BRACKET_R; }
","                                     { return COMMA; }
":"                                     { return COLON; }
\n                                      { }
[ \t]                                   { }
[-+]?[0-9]+                             { yylval.int_v = atoi(yytext); return NUMBER_I; }
[-+]?[0-9]*\.?[0-9]*([eE][-+]?[0-9]+)?  { yylval.float_v = atof(yytext); return NUMBER_F; }
true|false                              { yylval.bool_v = ( strcmp(yytext, "true") == 0 ? true : false); return BOOLEAN; }
null                                    { return NULL_T; }
[a-zA-Z0-9_]+                           { yylval.string_v = strdup(yytext); return WORD_STRING; }
.                                       {  }

%%

/** Sets the input string to the lexer */
void load_string(const char* str)
{
    yy_switch_to_buffer(yy_scan_string(str));
}

/** Sets the input file to the lexer */
void load_file(FILE* file)
{
    yyrestart(file);
}