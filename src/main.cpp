#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono> // this is for calculating the time

#include "C:\Users\HP\db_engine\src\backend\BTree.h"

#define FAIL    "\e[0;31m"
#define SUCCESS "\e[0;32m"
#define DEFAULT "\e[0;37m"

/*

WORKING SYNTAX DOCUMENTATION:
CRUD

CREATE:
CREATE NEW DATABASE <DB_NAME>
CREATE NEW TABLE <TABLE_NAME>
******CREATE NEW TABLE T1 (INTEGER : ID, STRING DATA, BOOL IS_STUDENT)
USE DATABASE


INSERTION :
INSERT INTO <TABLE> VALUE (<ELEMENT1>, ... , <ELEMENTN>)

READING : 
SEARCH IN <TABLE> VALUE ()

DELETE:
DELETE FROM <TABLE> VALUE ()

UPDATE :
UPDATE <TABLE> WHERE (<CONDITION>) WITH 
== < >

token exit

*/
typedef enum
{
    LEXER_SUCCESS,
    LEXER_FAIL,
    LEXER_PASS,
}LEXER_STATUS;

typedef enum 
{
    PARSER_SUCCESS,
    PARSER_FAIL,
} PARSER_STATUS;

 typedef enum 
    {
    TOKEN_INSERT, 
    TOKEN_INTO,
    TOKEN_VALUE,
    TOKEN_DELETE,
    TOKEN_FROM,
    TOKEN_SEARCH,
    TOKEN_IN,
    TOKEN_CREATE,
    TOKEN_NEW,
    TOKEN_TABLE,
    TOKEN_DATABASE,
    TOKEN_USE,
    TOKEN_UPDATE,
    TOKEN_WHERE,
    TOKEN_WITH,
    TOKEN_EQUALS,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_STRING, // This would be for the name of the database.
    TOKEN_INTEGER,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_COMMA,
    TOKEN_ID,
    TOKEN_EXIT,
    TOKEN_END_OF_INPUT,
    } TOKEN_SET; 

typedef enum
{
    NODE_CREATE_DATABASE,
    NODE_CREATE_TABLE,
    NODE_USE,
    NODE_INSERT,
    NODE_SEARCH,
    NODE_DELETE,
    NODE_UPDATE,
    NODE_SUB_VALUES,
    NODE_STRING,
    NODE_INTEGER,
    NODE_CONDITION_EQUALS,
    NODE_CONDITION_GREATER_THAN,
    NODE_CONDITION_LESS_THAN,
    NODE_EXIT,
} NODE_SET;

struct AST_NODE
{
    NODE_SET NODE_TYPE;
    std::string * PAYLOAD;
    std::string * SUB_PAYLOAD;
    std::vector < AST_NODE*> CHILDREN;
    std::vector <AST_NODE*> UPDATE_CHILDREN;
    //somehow handle condition
};

std::string nodeTypeToString(NODE_SET REQUIRED_NODE)
{
    switch (REQUIRED_NODE)
    {
    case NODE_CREATE_DATABASE        : return "NODE_CREATE_DATABASE";
    case NODE_CREATE_TABLE           : return "NODE_CREATE_TABLE";
    case NODE_USE                    : return "NODE_USE";
    case NODE_INSERT                 : return "NODE_INSERT";
    case NODE_SEARCH                 : return "NODE_SEARCH";
    case NODE_DELETE                 : return "NODE_DELETE";
    case NODE_UPDATE                 : return "NODE_UPDATE";   
    case NODE_EXIT                   : return "NODE_EXIT";
    case NODE_INTEGER                : return "NODE_INTEGER";
    case NODE_STRING                 : return "NODE_STRING";
    case NODE_CONDITION_EQUALS       : return "NODE_CONDITION_EQUALS";   
    case NODE_CONDITION_GREATER_THAN : return "NODE_CONDITION_GREATER_THAN";
    case NODE_CONDITION_LESS_THAN    : return "NODE_CONDITION_LESS_THAN";
    case NODE_SUB_VALUES             : return "NODE_SUB_VALUES";

    }
    return "[!] UNIDENTIFIED NODE :" + REQUIRED_NODE;
}

struct TOKEN
{
    TOKEN_SET TOKEN_TYPE;
    std::string VALUE; 
};

std::string tokenTypeToString(TOKEN_SET REQUIRED_TOKEN){
    switch (REQUIRED_TOKEN)
    {
        case TOKEN_INSERT       : return "TOKEN_INSERT";
        case TOKEN_INTO         : return "TOKEN_INTO";
        case TOKEN_VALUE        : return "TOKEN_VALUE";
        case TOKEN_DELETE       : return "TOKEN_DELETE";
        case TOKEN_FROM         : return "TOKEN_FROM";
        case TOKEN_SEARCH       : return "TOKEN_SEARCH";
        case TOKEN_IN           : return "TOKEN_IN";
        case TOKEN_CREATE       : return "TOKEN_CREATE";
        case TOKEN_NEW          : return "TOKEN_NEW";
        case TOKEN_TABLE        : return "TOKEN_TABLE";
        case TOKEN_DATABASE     : return "TOKEN_DATABASE";
        case TOKEN_USE          : return "TOKEN_USE";
        case TOKEN_UPDATE       : return "TOKEN_UPDATE";
        case TOKEN_WHERE        : return "TOKEN_WHERE";
        case TOKEN_WITH         : return "TOKEN_WITH";
        case TOKEN_EQUALS       : return "TOKEN_EQUALS";
        case TOKEN_LESS_THAN    : return "TOKEN_LESS_THAN";
        case TOKEN_GREATER_THAN : return "TOKEN_GREATER_THAN";
        case TOKEN_STRING       : return "TOKEN_STRING";
        case TOKEN_INTEGER      : return "TOKEN_INT";
        case TOKEN_LEFT_PAREN   : return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN  : return "TOKEN_RIGHT_PAREN";
        case TOKEN_COMMA        : return "TOKEN_COMMA";
        case TOKEN_ID           : return "TOKEN_ID";
        case TOKEN_EXIT         : return "TOKEN_EXIT";
        case TOKEN_END_OF_INPUT : return "TOKEN_END_OF_INPUT";
        
    }
    return "[!] ERROR : UNIDENTIFIED TOKEN:" + REQUIRED_TOKEN;
}

std::unordered_map <std::string , TOKEN_SET> KEYWORD_MAP = {
    {"insert"  , TOKEN_INSERT},
    {"into"    , TOKEN_INTO},
    {"value"   , TOKEN_VALUE},
    {"delete"  , TOKEN_DELETE},
    {"from"    , TOKEN_FROM},
    {"search"  , TOKEN_SEARCH},
    {"in"      , TOKEN_IN},
    {"create"  , TOKEN_CREATE},
    {"new"     , TOKEN_NEW},
    {"table"   , TOKEN_TABLE},
    {"database", TOKEN_DATABASE},
    {"use"     , TOKEN_USE},
    {"update"  , TOKEN_UPDATE},
    {"where"   , TOKEN_WHERE},
    {"with"    , TOKEN_WITH},
    {"exit"    , TOKEN_EXIT},
    {"INSERT"  , TOKEN_INSERT},
    {"INTO"    , TOKEN_INTO},
    {"VALUE"   , TOKEN_VALUE},
    {"DELETE"  , TOKEN_DELETE},
    {"FROM"    , TOKEN_FROM},
    {"SEARCH"  , TOKEN_SEARCH},
    {"IN"      , TOKEN_IN},
    {"CREATE"  , TOKEN_CREATE},
    {"NEW"     , TOKEN_NEW},
    {"TABLE"   , TOKEN_TABLE},
    {"DATABASE", TOKEN_DATABASE},
    {"USE"     , TOKEN_USE},
    {"UPDATE"  , TOKEN_UPDATE},
    {"WHERE"   , TOKEN_WHERE},
    {"WITH"    , TOKEN_WITH},
    {"EXIT"    , TOKEN_EXIT},


};

std::unordered_map<std::string, BTree*> TABLE_MAP;


class Lexer
{
    private:

    int cursor;
    int length;
    char current;
    bool stringParsingError;
    std::string LocalInputBuffer;
    std::vector <TOKEN *> TOKEN_LIST; 

    char advance()
    {
        if (cursor == length-1) // This means that we are at the end of the input buffer
            {
                current = '\0'; 
                return current;
            }
        else
        {
            current = LocalInputBuffer[++cursor];
            return current; 
        }
    }

    void skipWhiteSpaces()
    {
        while (current == ' ' & current != '\0')
        advance();
    }

    TOKEN * tokenizeID()
    {
        TOKEN * newToken = new TOKEN; 
        std::string temporaryBuffer = "";
        temporaryBuffer.push_back(current);
        advance();
        while (isalnum(current) || current == '_')
        {
            temporaryBuffer.push_back(current);
            advance();
        }

        // create a unordered_map of keywords, search the newToken->VALUE 

        

        newToken -> TOKEN_TYPE = TOKEN_ID;
        newToken -> VALUE = temporaryBuffer; 

        if (KEYWORD_MAP.find(newToken -> VALUE) != KEYWORD_MAP.end())
            newToken -> TOKEN_TYPE = KEYWORD_MAP[newToken ->VALUE];

        return newToken; 
        
    }

    TOKEN * tokenizeSTRING()
    {
        advance(); // advancing the opening quotes
        TOKEN * newToken = new TOKEN;
        std::string temporaryBuffer = "";
        while (current != '"')
        {
            if (current == '\0')
            {
                stringParsingError = true;
                break;
            }
            temporaryBuffer.push_back(current);
            advance();
        }
        advance(); //advancing the closing quotes
        newToken->TOKEN_TYPE = TOKEN_STRING;
        newToken -> VALUE = temporaryBuffer;

        return newToken;

    }

    TOKEN * tokenizeINTEGER()
    {
        TOKEN* newToken = new TOKEN;
        std::string temporaryBuffer = "";
        
        while (isdigit(current))
        {
            temporaryBuffer.push_back(current);
            advance();
        }

        newToken ->TOKEN_TYPE = TOKEN_INTEGER;
        newToken -> VALUE = temporaryBuffer;

        return newToken;
        

    }

    TOKEN * tokenizeSPECIAL(TOKEN_SET NEW_TOKEN_TYPE)
    {
        TOKEN* newToken = new TOKEN;
        newToken -> TOKEN_TYPE = NEW_TOKEN_TYPE;
        if (newToken->TOKEN_TYPE == TOKEN_EQUALS)
        {
            newToken -> VALUE = ""=="";
        }
        else
        {
             newToken -> VALUE = current;
        }
        
        advance();
        return newToken;
    }

    void displayAllTokens()
    {
        int counter = 0;
        for (TOKEN * CURRENT_TOKEN : TOKEN_LIST)
        {
            std::cout << ++counter << ')' << CURRENT_TOKEN ->VALUE << " " ;
            std::cout << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << std::endl;

        }
        
    }

    LEXER_STATUS throwLexerError()
    {
        std::cout << FAIL << "[!] LEXER ERROR : UNIDENTIFIED CHARACTER AT INDEX " << cursor 
        << " : " << current<< std::endl;
        return LEXER_FAIL;
    }

    LEXER_STATUS throwStringParsingError()
    {
        std::cout << FAIL << "[!] LEXER ERROR : CLOSING QUOTES NOT FOUND IN THE GIVEN COMMAND " << cursor 
        << " : " << current<< std::endl;
        return LEXER_FAIL;
    }

    public: 
    Lexer()
    {
    }

    std::vector<TOKEN *> * getTokenStream()
    {
        return &TOKEN_LIST;
    }

    void initialize(std::string InputBuffer)
    {
        LocalInputBuffer = InputBuffer;
        cursor = 0;
        length = InputBuffer.size();
        current = LocalInputBuffer[cursor];
        TOKEN_LIST.clear();
        stringParsingError = false;


    }

    LEXER_STATUS tokenize ()
    {

        while (current)
        {
            skipWhiteSpaces();
            if (isalpha(current) || current=='_')
            {
                TOKEN_LIST.push_back(tokenizeID());
            }
            else if (isdigit(current))
            {
                TOKEN_LIST.push_back(tokenizeINTEGER());
            }
            else
            {
            switch(current)
            {
                case '(':
                {
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_LEFT_PAREN));
                    break;
                }

                case ')':
                {
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PAREN));
                    break;
                }

                case ',':
                {
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_COMMA));
                    break;
                }
                case '<':
                {
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_LESS_THAN));
                    break;
                }
                case '>':
                {
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_GREATER_THAN));
                    break;

                }
                case '"':
                {
                    TOKEN_LIST.push_back(tokenizeSTRING());
                    if (stringParsingError)
                        return throwStringParsingError();
                    break;

                }
                case '=':
                {
                    advance();
                    if (current != '=')
                        return throwLexerError();
                    TOKEN_LIST.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                    break;

                }
                case '\0': break;
                default : return throwLexerError();
                
            }
            }
            

        }
        //displayAllTokens();
        TOKEN * END_TOKEN = new TOKEN;
        END_TOKEN->TOKEN_TYPE = TOKEN_END_OF_INPUT;
        TOKEN_LIST.push_back(END_TOKEN);
        return LEXER_SUCCESS;
        
    }

};

class Parser
{
    private:
    TOKEN * CURRENT_TOKEN;
    std::vector <TOKEN *> LOCAL_COPY_TOKEN_STREAM;
    int token_number;
    bool syntaxError;
    AST_NODE * EVALUATED_NODE;

    public:
    PARSER_STATUS throwSyntaxError()
    {
        std::cout << FAIL << "[!] SYNTAX ERROR : UNEXPECTED TOKEN : " << tokenTypeToString(CURRENT_TOKEN->TOKEN_TYPE) << DEFAULT << std::endl;
        exit(0); 
        return PARSER_FAIL;
    }

    AST_NODE* getEvaluatedNode() {
        return EVALUATED_NODE;
    }

    void check(TOKEN_SET REQUIRED_CHECK_TOKEN)
    {
        if (CURRENT_TOKEN -> TOKEN_TYPE != REQUIRED_CHECK_TOKEN)
            throwSyntaxError();
    }

    TOKEN * proceed(TOKEN_SET REQUIRED_TOKEN )
    {
        if (CURRENT_TOKEN->TOKEN_TYPE != REQUIRED_TOKEN)
        {
            throwSyntaxError();
            syntaxError = true;
            return CURRENT_TOKEN;
        }
        token_number++;
        CURRENT_TOKEN = LOCAL_COPY_TOKEN_STREAM[token_number];
        return CURRENT_TOKEN;
    }

    TOKEN * checkAndProceed(TOKEN_SET REQUIRED_TOKEN)
    {
        TOKEN * bufferPointer = CURRENT_TOKEN;
        proceed(REQUIRED_TOKEN);
        return bufferPointer;
    }

    AST_NODE * parseCHILDREN()
    {
        AST_NODE * NEW_CHILD_NODE = new AST_NODE;
        NEW_CHILD_NODE -> NODE_TYPE = (CURRENT_TOKEN -> TOKEN_TYPE == TOKEN_INTEGER) ? NODE_INTEGER :
        NODE_STRING;
        NEW_CHILD_NODE -> PAYLOAD = &CURRENT_TOKEN -> VALUE;
        if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_INTEGER)
            proceed(TOKEN_INTEGER);
        else
            proceed(TOKEN_STRING);
        return NEW_CHILD_NODE;
    }

    AST_NODE * parseCONDITION()
    {
        /*
         THIS IS THE SYNTAX OF PARSING CONDITION
         ( ID RELATIONAL_OPERATOR INT/STRING)
         PUT THE ID IN PAYLOAD TO MAKE IT EASIER
         PUT THE INT/STRING IN THE SUBPAYLOAD
         (NUMBER == 5) OR (NUMBER < 5)
         */
        proceed(TOKEN_LEFT_PAREN);
        AST_NODE * CONDITION_NODE = new AST_NODE;

        CONDITION_NODE -> PAYLOAD = &checkAndProceed(TOKEN_ID) -> VALUE;
        
        switch (CURRENT_TOKEN -> TOKEN_TYPE)
        {
        case TOKEN_LESS_THAN: 
        {
            CONDITION_NODE -> NODE_TYPE = NODE_CONDITION_LESS_THAN; 
            proceed(TOKEN_LESS_THAN);
            break;

        }
        case TOKEN_EQUALS: 
        {
            CONDITION_NODE -> NODE_TYPE = NODE_CONDITION_EQUALS;
            proceed(TOKEN_EQUALS);
            break;
        }
        case TOKEN_GREATER_THAN:
        {
            CONDITION_NODE -> NODE_TYPE = NODE_CONDITION_GREATER_THAN;
            proceed(TOKEN_GREATER_THAN);
            break;
        } 
        default: throwSyntaxError();
        }

        if (CURRENT_TOKEN -> TOKEN_TYPE == TOKEN_INTEGER)
           CONDITION_NODE -> SUB_PAYLOAD  = &checkAndProceed(TOKEN_INTEGER) -> VALUE;
        else if (CURRENT_TOKEN -> TOKEN_TYPE == TOKEN_STRING)
            CONDITION_NODE -> SUB_PAYLOAD = &checkAndProceed(TOKEN_STRING) -> VALUE;
        else
            throwSyntaxError(); 
        
        proceed(TOKEN_RIGHT_PAREN);
        
        return CONDITION_NODE;
    }

    PARSER_STATUS parseCREATE()
    {
        /*
        SYNTAX FOR CREATE :
        CREATE NEW DATABASE <NAME>
        CREATE NEW TABLE <TABLE_NAME>
        */
        EVALUATED_NODE = new AST_NODE;
        proceed(TOKEN_CREATE);
        proceed(TOKEN_NEW);

        if (CURRENT_TOKEN -> TOKEN_TYPE == TOKEN_DATABASE)
        {
            EVALUATED_NODE -> NODE_TYPE = NODE_CREATE_DATABASE;
            proceed(TOKEN_DATABASE);
        }
        else if (CURRENT_TOKEN -> TOKEN_TYPE == TOKEN_TABLE)
        {
            EVALUATED_NODE -> NODE_TYPE = NODE_CREATE_TABLE;
            proceed(TOKEN_TABLE);
        }
        else 
            throwSyntaxError();
        // ID : THIS_IS_AN_ID
        //STRING : "THIS_IS_A_STRING"
        //CREATE NEW TABLE T1   
        EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID) ->VALUE;
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;
    }
    PARSER_STATUS parseUSE()
    {
        /*
        SYNTAX FOR USE:
        USE <DB_NAME>
        */
        EVALUATED_NODE = new AST_NODE;
        EVALUATED_NODE -> NODE_TYPE = NODE_USE;
        proceed(TOKEN_USE);
        EVALUATED_NODE->PAYLOAD = &checkAndProceed(TOKEN_ID) ->VALUE;
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;

    }

    PARSER_STATUS parseINSERT()
    {
        /*
        SYNTAX FOR INSERT:
        INSERT INTO <TABLE_NAME> VALUE (...)
        */
        EVALUATED_NODE = new AST_NODE;
        EVALUATED_NODE -> NODE_TYPE = NODE_INSERT;
        proceed(TOKEN_INSERT);
        proceed(TOKEN_INTO);
        EVALUATED_NODE -> PAYLOAD = &checkAndProceed(TOKEN_ID) ->VALUE;
        proceed(TOKEN_VALUE);

        proceed(TOKEN_LEFT_PAREN);
        while (true)
        {
            // THIS IS CHECK FOR EMPTY INSERT
            if(CURRENT_TOKEN->TOKEN_TYPE == TOKEN_END_OF_INPUT)
                throwSyntaxError();
            if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_INTEGER && CURRENT_TOKEN->TOKEN_TYPE != TOKEN_STRING)
                throwSyntaxError();
            EVALUATED_NODE->CHILDREN.push_back(parseCHILDREN());
            
            if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_RIGHT_PAREN)
            {
                proceed(TOKEN_RIGHT_PAREN);
                break;
            }
            proceed(TOKEN_COMMA);
        }
        
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;
    }
    PARSER_STATUS parseSEARCH()
    {
        /*
        SYNTAX FOR SEARCH
        SEARCH IN <T_NAME>
        WHERE (key)
        */
       EVALUATED_NODE = new AST_NODE;
       EVALUATED_NODE->NODE_TYPE = NODE_SEARCH;
       proceed(TOKEN_SEARCH);
       proceed(TOKEN_IN);
       EVALUATED_NODE -> PAYLOAD = &checkAndProceed(TOKEN_ID) -> VALUE;
       proceed(TOKEN_WHERE);
       proceed(TOKEN_LEFT_PAREN);
        while (true)
        {
            // THIS IS CHECK FOR EMPTY INSERT
            if(CURRENT_TOKEN->TOKEN_TYPE == TOKEN_END_OF_INPUT)
                throwSyntaxError();
            if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_INTEGER && CURRENT_TOKEN->TOKEN_TYPE != TOKEN_STRING)
                throwSyntaxError();
            EVALUATED_NODE->CHILDREN.push_back(parseCHILDREN());
            
            if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_RIGHT_PAREN)
            {
                proceed(TOKEN_RIGHT_PAREN);
                break;
            }
        }
        
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;

    }

    PARSER_STATUS parseDELETE()
    {
        /*
        SYNTAX FOR DELETE:
        DELETE FROM <T_NAME)>  WHERE (CONDITION)
        T_NAME WOULD BE IN THE PAYLOAD
        */
       EVALUATED_NODE = new AST_NODE;
       EVALUATED_NODE->NODE_TYPE = NODE_DELETE;
       proceed(TOKEN_DELETE);
       proceed(TOKEN_FROM);
       EVALUATED_NODE -> PAYLOAD = &checkAndProceed(TOKEN_ID) -> VALUE;
       checkAndProceed(TOKEN_WHERE);
       EVALUATED_NODE -> CHILDREN.push_back(parseCONDITION());
       
       check(TOKEN_END_OF_INPUT);
       return PARSER_SUCCESS;



    }
    PARSER_STATUS parseUPDATE()
    {
        /*
        SYNTAX FOR UPDATE : 
        UPDATE <TNAME> WHERE (<CONDITION>) WITH (V1,V2,....)
        TO GET THE CONDITION NODE FROM THE NODE UPDATE, ACCESS
            CURRENT_NODE -> CHILDREN[0];
        <CONDITION> : (ID RELATIONAL_OPERATOR INT /STRING)
        PARSING CONDITION : 
        */
       EVALUATED_NODE = new AST_NODE;
       EVALUATED_NODE -> NODE_TYPE = NODE_UPDATE;
       proceed(TOKEN_UPDATE);
       EVALUATED_NODE -> PAYLOAD = &checkAndProceed(TOKEN_ID) -> VALUE;
       proceed(TOKEN_WHERE);
       EVALUATED_NODE->CHILDREN.push_back(parseCONDITION());
       proceed(TOKEN_WITH);
       proceed(TOKEN_LEFT_PAREN);
        while (true)
        {
            if(CURRENT_TOKEN->TOKEN_TYPE == TOKEN_END_OF_INPUT)
                throwSyntaxError();
            if (CURRENT_TOKEN->TOKEN_TYPE != TOKEN_INTEGER && CURRENT_TOKEN->TOKEN_TYPE != TOKEN_STRING)
                throwSyntaxError();
            EVALUATED_NODE->UPDATE_CHILDREN.push_back(parseCHILDREN());
            
            if (CURRENT_TOKEN->TOKEN_TYPE == TOKEN_RIGHT_PAREN)
            {
                proceed(TOKEN_RIGHT_PAREN);
                break;
            }
            proceed(TOKEN_COMMA);
        }
       check(TOKEN_END_OF_INPUT);
       return PARSER_SUCCESS;
    }
    
    PARSER_STATUS parseEXIT()
    {
        /*
        SYNTAX FOR EXIT:
        EXIT
        */
        EVALUATED_NODE = new AST_NODE;
        EVALUATED_NODE -> NODE_TYPE = NODE_EXIT;
        proceed(TOKEN_EXIT);
    
        check(TOKEN_END_OF_INPUT);
        return PARSER_SUCCESS;
    }

    public:
    Parser()
    {

    }
    void initialize(std ::vector<TOKEN *> *TOKEN_LIST_ADDRESS)
    {
        LOCAL_COPY_TOKEN_STREAM.clear();
        LOCAL_COPY_TOKEN_STREAM = *(TOKEN_LIST_ADDRESS);
        token_number = 0;
        CURRENT_TOKEN = LOCAL_COPY_TOKEN_STREAM[token_number];
        syntaxError = false;
    }

    PARSER_STATUS parse()
    {
            switch (CURRENT_TOKEN -> TOKEN_TYPE)
            {
                // FOR EVERY FUNCTION, IF THERE IS A SYNTAX ERROR
                // INSIDE THE FUNCTION, WE NEED TO FIRST CALL THE
                // THROW SYNTAX ERROR FROM INSIDE THE FUNCION
                // THEN WE NEED TO RETURN THE PARSE FAIL ENUM
                case  TOKEN_CREATE: return parseCREATE();
                case  TOKEN_USE   : return parseUSE();
                case  TOKEN_INSERT: return parseINSERT(); 
                case  TOKEN_SEARCH: return parseSEARCH();
                case  TOKEN_DELETE: return parseDELETE();
                case  TOKEN_UPDATE: return parseUPDATE();
                case  TOKEN_EXIT  : return parseEXIT();
                default           : return throwSyntaxError();

            }
        }

};

class EvaluationWrapper
{
    private:
    Lexer * MAIN_LEXER;
    Parser * MAIN_PARSER;
    int commandCount;
    BTree* btree;

    public :
    EvaluationWrapper()
    {
        MAIN_LEXER = new Lexer();
        MAIN_PARSER = new Parser();
        commandCount = 0;
        btree = nullptr;
    }

    void handle(std::string InputBuffer)
    {
        auto startTimer = std::chrono::high_resolution_clock::now();
        // using the lexer to tokenize the input buffer
        MAIN_LEXER->initialize(InputBuffer);
        //track the history of command
        LEXER_STATUS CURRENT_LEXER_STATUS = MAIN_LEXER -> tokenize();
        PARSER_STATUS CURRENT_PARSER_STATUS;
        if (CURRENT_LEXER_STATUS == LEXER_SUCCESS)
        {
            MAIN_PARSER -> initialize(MAIN_LEXER ->getTokenStream());
            CURRENT_PARSER_STATUS = MAIN_PARSER -> parse();

            if (CURRENT_PARSER_STATUS == PARSER_SUCCESS)
            {
                executeParsedCommand();
            }
            
        }
        
        auto endTimer = std::chrono::high_resolution_clock::now();
        auto timeconsumed = std::chrono::duration_cast<std::chrono::milliseconds> (endTimer - startTimer);

        commandCount++;

        //Using the parser to parse the token_stream
        if (CURRENT_LEXER_STATUS == LEXER_FAIL || CURRENT_PARSER_STATUS == PARSER_FAIL)
            std::cout << FAIL << "$ Command ID : " << commandCount << " failed in " << timeconsumed.count() << " ms\n\n" << DEFAULT;   

        
        else
            std::cout << SUCCESS << "$ Command ID : " << commandCount << " executed in " << timeconsumed.count() <<" ms\n\n" << DEFAULT ;
        
    }

    void executeParsedCommand()
{
    AST_NODE* parsedNode = MAIN_PARSER -> getEvaluatedNode();

    if (!parsedNode)
    {
        std::cout << FAIL << "No Valid command to execute!!" << DEFAULT << std::endl;
        return;
    } 

    switch (parsedNode -> NODE_TYPE)
    {
        case NODE_CREATE_TABLE: 
        {
            std::string tableName = *(parsedNode -> PAYLOAD);

            if(TABLE_MAP.find(tableName) != TABLE_MAP.end())
            {
                std::cout << FAIL << "Error: Table '" << tableName << "' already exists.\n" << DEFAULT;
                return;
            }
            std::cout << "Creating a BTree...\n";
            int degree = 3;
            btree = new BTree(degree);

            TABLE_MAP[tableName] = btree;
            break;

        }
        case NODE_INSERT:
        {
            std::string tableName = (*parsedNode -> PAYLOAD);

            //check if table exists
            if (TABLE_MAP.find(tableName) == TABLE_MAP.end())
            {
                std::cout << FAIL << "Error: Table '" << tableName << "' does not exist.\n" << DEFAULT;
            }

            const std::vector<std::string> columnNames = {"ID", "Name", "PantherID"};

             if (parsedNode->CHILDREN.size() != columnNames.size()) 
             {
            std::cout << "Error: Mismatched number of values. Expected " << columnNames.size() << " values.\n";
            }       

            Row newRow;
            // Check for primary key uniqueness
            const std::string& primaryKey = *(parsedNode -> CHILDREN[0] -> PAYLOAD);

            if (TABLE_MAP[tableName] -> search(std::stoi(primaryKey)) != nullptr)
            {
                std::cout << "ID must be unique," << primaryKey << " has been used before"; 
                break;
            }
            for (int i =0; i < columnNames.size(); i++)
            {
                const std::string& value = *(parsedNode -> CHILDREN[i] -> PAYLOAD);

                try
                {
                    if (i==0 || i == 2)
                        newRow.setColumn(columnNames[i], std::stoi(value));
                    else
                        newRow.setColumn(columnNames[i], value);
                }
                catch(const std::exception& e)
                {
                    std::cout << "Error: Failed to parse value for column '" << columnNames[i] << "': " << value << "\n";
                }
            }    
            TABLE_MAP[tableName]->insertRow(newRow);
            std::cout << "Successfully inserted row into table" << tableName; 
            break;
        }
        case NODE_SEARCH:
        {

            std::string tableName = (*parsedNode -> PAYLOAD);
            int primarykey = std::stoi(*(parsedNode->CHILDREN[0]->PAYLOAD));

            if (TABLE_MAP.find(tableName) == TABLE_MAP.end())
            {
                std::cout << FAIL << "Error: Table '" << tableName << "' does not exist.\n" << DEFAULT;
                return;
            }
            
            BTreeNode* result = TABLE_MAP[tableName] -> search(primarykey);
            if (result != nullptr)
            {
                Row searchRow = *(*result -> row);
                try {
                    // Use std::visit to print each column value
                    std::visit([](auto&& value) {
                        std::cout << "ID: " << value << std::endl;
                    }, searchRow.getColumn("ID"));

                    std::visit([](auto&& value) {
                        std::cout << "Name: " << value << std::endl;
                    }, searchRow.getColumn("Name"));

                    std::visit([](auto&& value) {
                        std::cout << "PantherId: " << value << std::endl;
                    }, searchRow.getColumn("PantherID"));

                } catch (const std::exception& e) {
                    std::cout << "Error accessing column: " << e.what() << "\n";
                }
    
            }
            else
            {
                std::cout << "Row with primary key " << primarykey << " not found.\n";
            }
            break;

        }
        case NODE_DELETE:
        {

        }
        case NODE_UPDATE:
        {

        }
        

        default: 
        std::cout << FAIL << "Unsupported command type: " << nodeTypeToString(parsedNode->NODE_TYPE) << DEFAULT << std::endl;
    }
}
};



int main()
{
    system("cls"); 
    std::string InputBuffer; //maybe change this to a class of it's own
    EvaluationWrapper* main_io = new EvaluationWrapper();
    Lexer mainLexer;
    while (true)
    {
        std::cout << DEFAULT << "db_engine: "; 
        std::getline(std::cin, InputBuffer);
        main_io->handle(InputBuffer);
    }
    return 0;
}