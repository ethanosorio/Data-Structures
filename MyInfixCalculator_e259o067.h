#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "MyStack.h"
#include "MyVector.h"

class MyInfixCalculator{

  public:
    
    MyInfixCalculator()
    {
        
    }

    ~MyInfixCalculator()
    {
     
    }

    double calculate(const std::string& s)
    {
        MyVector<std::string> infix_tokens;
        MyVector<std::string> postfix_tokens;
        tokenize(s, infix_tokens);
        infixToPostfix(infix_tokens, postfix_tokens);
        return calPostfix(postfix_tokens);
    }

  private:

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {
        switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    // checks if a character corresponds to a valid parenthesis
    bool isValidParenthesis(const char c) const
    {
        switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    // checks if a character corresponds to a valid digit
    bool isDigit(const char c) const
    {
        if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {
        double o1 = std::stod(ornd1);
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {
            case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cout << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }

    // tokenizes an infix string s into a set of tokens (operands or operators)
    void tokenize(const std::string& s, MyVector<std::string>& tokens)
    {
        std::string token = "";
        for (size_t i=0; i<s.size(); ++i) {
            if(s[i] == '+' || s[i] == '-'|| s[i] == '*'|| s[i] == '/'|| s[i] == '('|| s[i] == ')'){
                if(!token.empty()){
                    tokens.push_back(token);
                    token = "";
                }
                if(s[i] == '-' && (tokens.empty() || tokens.back() == "+" || tokens.back() == "-"|| tokens.back() == "*"|| tokens.back() == "/" || tokens.back() == "("))
                    token.push_back(s[i]);
                else
                    tokens.push_back(std::string(1, s[i]));
            }
            else{
                token.push_back(s[i]);
                if(i == s.size()-1)
                    tokens.push_back(token);
            }
        }
    }

    // converts a set of infix tokens to a set of postfix tokens
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens)
    {
        MyStack<char> stack;
        for (size_t i=0; i<infix_tokens.size(); ++i) {
            char c = infix_tokens[i].back();
            if (isDigit(c))
                postfix_tokens.push_back(infix_tokens[i]);
            else if (c == '(')
                stack.push('(');
            else if (c == ')') {
                while (stack.top() != '(')
                {
                    postfix_tokens.push_back(std::string(1, stack.top()));
                    stack.pop();
                }
                stack.pop();
            }
            else {
                while (!stack.empty()) {
                    if(operatorPrec(c) >= operatorPrec(stack.top()) && !isValidParenthesis(stack.top())){
                        postfix_tokens.push_back(std::string(1, stack.top()));
                        stack.pop();
                    }
                    else break;
                }
                stack.push(c);
            }
        }
        while (!stack.empty()) {
            postfix_tokens.push_back(std::string(1, stack.top()));
            stack.pop();
        }
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {
        MyStack<std::string> stack;
        for(size_t i=0; i<postfix_tokens.size(); ++i){
            char c = postfix_tokens[i].back();
            if (isDigit(c))
                stack.push(postfix_tokens[i]);
            else{
                std::string ornd2 = stack.top();
                stack.pop();
                std::string ornd1 = stack.top();
                stack.pop();
                stack.push(std::to_string(computeBinaryOperation(ornd1,ornd2,postfix_tokens[i])));
            }
        }
        return std::stod(stack.top());
    }
};

#endif // __MYINFIXCALCULATOR_H__