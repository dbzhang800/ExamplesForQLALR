
%parser CommandParser
%merged_output commandparser.cpp

-- The verbs

%token EAT "eat"
%token GO "go"
%token NORTH "north"
%token EAST "east"
%token SOUTH "south"
%token WEST "west"
%token PUT "put"

-- The preposition

%token IN "in"

-- Object names

%token THE "the"

%token OBJECTNAME "object"

%start Input

/:

#include <QtCore>
#include "command.h"

class CommandInterpreter : public $table
{

public:
    CommandInterpreter();
    ~CommandInterpreter();

    Command parse();

    int nextToken();

    void setInput(const QString &input);

    inline void reallocateStack();

    inline QString &sym(int index)
    { return sym_stack [tos + index - 1]; }

private:
    int tos;
    QStringList tokens;
    QVector<QString> sym_stack;
    QVector<int> state_stack;
    QString yylval; 
};

:/

/.
void CommandInterpreter::setInput(const QString &input)
{
    tokens = input.split(" ");
}

void CommandInterpreter::reallocateStack()
{
    int size = state_stack.size();
    if (size == 0)
        size = 128;
    else
        size <<= 1;

    sym_stack.resize(size);
    state_stack.resize(size);
}

CommandInterpreter::CommandInterpreter()
{
    tos = 0;
    reallocateStack();
}

CommandInterpreter::~CommandInterpreter()
{
}

Command CommandInterpreter::parse()
{
    Command command;

    const int INITIAL_STATE = 0;
    int yytoken = -1;

    tos = 0;
    state_stack[++tos] = INITIAL_STATE;

    while (true) {
        const int state = state_stack.at(tos);
        if (yytoken == -1 && - TERMINAL_COUNT != action_index [state] ) {
            yytoken = nextToken();
        }

        int act = t_action (state, yytoken);
        if (act == ACCEPT_STATE) {
            command.state = Command::Valid;
            return command;

        } else if (act > 0) {
            if (++tos == state_stack.size())
                reallocateStack();
            state_stack[tos] = act;
            yytoken = -1;
        } else if (act < 0) {
            int r = -act - 1;
            tos -= rhs[r];
            act = state_stack.at(tos++);

            switch(r) {
./

Input: Sentence ;

Sentence: GO SingleVerbSentence ;
Sentence: SingleVerbSentence ;
Sentence: OneNounSentence ;
Sentence: TwoNounSentence ;

ObjectName: OBJECTNAME ;
/.
    case $rule_number: {
        sym(1) = yylval;
    } break;
./

ObjectList: ObjectList ObjectName ;

Noun: THE ObjectList ;
Noun: ObjectList ;

ObjectList: ObjectName ;
/.
    case $rule_number: {
        command.nounNames.append(sym(1));
    } break;
./

SecondList: ObjectName ;
/.
    case $rule_number: {
        command.secondNames.append(sym(1));
    } break;
./

SecondList: SecondList ObjectName ;
/.
    case $rule_number: {
    } break;
./

Second: THE SecondList ;
Second: SecondList ;

SingleVerbSentence: NORTH ;
/.
    case $rule_number: {
        command.verb = Command::North;
    } break;
./
SingleVerbSentence: EAST ;
/.
    case $rule_number: {
        command.verb = Command::East;
    } break;
./
SingleVerbSentence: SOUTH ;
/.
    case $rule_number: {
        command.verb = Command::South;
    } break;
./
SingleVerbSentence: WEST ;
/.
    case $rule_number: {
        command.verb = Command::West;
    } break;
./

OneNounSentence: OneNounVerb Noun ;

OneNounVerb: EAT ;
/.
    case $rule_number: {
        command.verb = Command::Eat;
    } break;
./

TwoNounSentence: PUT Noun IN Second ;
/.
    case $rule_number: {
        command.verb = Command::Put;
        command.preposition = Command::In;
    } break;
./


/.
            } // of the switch

            state_stack [tos] = nt_action (act, lhs [r] - TERMINAL_COUNT);
        } else {
              QString m_errorMessage;
              int ers = state;
              int shifts = 0;
              int reduces = 0;
              int expected_tokens[3];
              for (int tk = 0; tk < TERMINAL_COUNT; ++tk) {
                int k = t_action(ers, tk);

                  if (! k)
                    continue;
                  else if (k < 0)
                    ++reduces;
                  else if (spell[tk]) {
                      if (shifts < 3)
                        expected_tokens[shifts] = tk;
                    ++shifts;
                  }
              }
            // Generic error message. It brings back bad memories from the past.
            command.errorMessage = "I didn't understand that sentence."; 
            command.state = Command::Invalid;
            return command;
        }
    } // of the loop
    return command; 
}

int CommandInterpreter::nextToken()
{
    if (tokens.isEmpty()) {
        return EOF_SYMBOL;
    }

    QString nextToken = tokens.takeFirst();

    nextToken = nextToken.toLower();

    if (nextToken == "eat") {
        return CommandParser::EAT;
    }

    if (nextToken == "go") {
        return CommandParser::GO;
    }

    if (nextToken == "north") {
        return CommandParser::NORTH;
    }
    if (nextToken == "south")  {
        return CommandParser::SOUTH;
    }

    if (nextToken == "west") {
        return CommandParser::WEST;
    }

    if (nextToken == "east") {
        return CommandParser::EAST;
    }

    if (nextToken == "in") {
        return CommandParser::IN;
    }

    if (nextToken == "put") {
        return CommandParser::PUT;
    }
    
    if (nextToken == "the") {
        return CommandParser::THE;
    }

    yylval = nextToken;
    return CommandParser::OBJECTNAME;
}

./

