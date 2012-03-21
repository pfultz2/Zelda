#ifndef ZELDA_TOKENS_H
#define ZELDA_TOKENS_H

$for x in ["and", "abstract", "array", "as", "auto", "base", "bind", "bool", "box", "break", "case", "catch", "char", "class", "const", "const_cast", "continue", "decltype", "default", "delegate", "delete", "deprecated", "do", "double", "dyanmic_cast", "each", "else", "enum", "event", "except", "exclude", "explicit", "export", "extern", "false", "final", "finally", "float", "for", "friend", "from", "generic", "get", "global", "goto", "if", "implicit", "import", "in", "include", "inline", "int", "interface", "is", "leave", "literal", "long", "mutable", "naked", "namespace", "new", "not", "null", "nullptr", "object", "operator", "or", "out", "override", "private", "property", "protected", "public", "raise", "ref", "register", "reinterpret_cast", "return", "sealed", "set", "short", "signed", "sizeof", "static", "static_cast", "string", "struct", "super", "switch", "template", "this", "thread", "throw", "true", "try", "type", "typedef", "typeid", "typeof", "typename", "union", "unsafe", "unsigned", "using", "value", "var", "virtual", "void", "volatile", "while", "with", "yield"]  
{
#ifndef PP_PREFIX_$(x)
#define PP_PREFIX_$(x) $(x),
#endif
#ifndef $(x)_PP_SUFFIX
#define $(x)_PP_SUFFIX ,$(x)
#endif
#ifndef PP_TOKEN_$(x)
#define PP_TOKEN_$(x)(x) x
#endif   
}

$for x in range(65536)
{
#ifndef PP_PREFIX_$(x)
#define PP_PREFIX_$(x) $(x),
#endif
#ifndef PP_TOKEN_$(x)
#define PP_TOKEN_$(x)(x) x
#endif   
}

#endif