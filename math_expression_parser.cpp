// math_expression_parser.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <stack>
using namespace std;
typedef char OperatorVal;
typedef int TValue;
struct Toperator
{
	OperatorVal val;
	int priority;
};
enum lexem_type
{
	oper,
	num,
	bracket,
	empty
};
union lexem_val
{
	TValue number;
	Toperator oper;
};
struct Lexem
{
	lexem_type field_type = lexem_type::empty;
	lexem_val val;
};
void split(string expression_str, vector<Lexem> &expression)
{
	string digits_buffer = "";
	Lexem* newLexem;
	for (int i = 0;i < expression_str.length(); i++)
	{
		switch (expression_str[i])
		{
		case '*':
		case '/':
		case '^':
		case '+':
		case '-':
			if (digits_buffer.length() > 0)
			{
				newLexem = new Lexem;
				newLexem->field_type = lexem_type::num;
				newLexem->val.number = atoi(digits_buffer.c_str());
				digits_buffer = "";
				expression.push_back(*newLexem);
			}
			newLexem = new Lexem;
			newLexem->field_type = oper;
			newLexem->val.oper.val = expression_str[i];
			if (expression_str[i] == '^')
			{
				newLexem->val.oper.priority = 6;
			}
			else
			{
				newLexem->val.oper.priority = ((expression_str[i] == '*') || (expression_str[i] == '/') ? 5 : 4);
			}
			expression.push_back(*newLexem);
			break;
		default:
			digits_buffer += expression_str[i];
			break;
		}
	}
	if (!digits_buffer.empty())
	{
		Lexem* newLexem = new Lexem;
		newLexem->field_type = lexem_type::num;
		newLexem->val.number = atoi(digits_buffer.c_str());
		digits_buffer = "";
		expression.push_back(*newLexem);
	}
}
void convert_to_postfix(vector<Lexem> &expression, vector<Lexem> &result)
{
	stack<Lexem> tmp;
	for (int i = 0; i < expression.size(); i++)
	{
		if (expression[i].field_type == lexem_type::num) // if token is number
		{
			result.push_back(expression[i]);
		}
		else if(expression[i].field_type == lexem_type::oper) // if token is operator
		{
			while ((!tmp.empty()) && (tmp.top().field_type == lexem_type::oper) && (tmp.top().val.oper.priority >= expression[i].val.oper.priority)) // if at the top of stack placed more priority operator
			{
				result.push_back(tmp.top());
				tmp.pop();
			}
			tmp.push(expression[i]);
		}
		else // if token is bracket
		{
			if (expression[i].val.oper.val == '(')
			{
				result.push_back(expression[i]);
			}
			else // if token is )
			{
				while ((!tmp.empty()) && (tmp.top().field_type != lexem_type::bracket) && (tmp.top().val.oper.val != '('))
				{
					result.push_back(tmp.top());
					tmp.pop();
				}
				tmp.pop();
			}
		}
	}
	while (!tmp.empty())
	{
		result.push_back(tmp.top());
		tmp.pop();
	}
}

void print_expression(vector<Lexem> &expression)
{
	for (int i = 0; i < expression.size(); i++)
	{
		if ((expression[i].field_type == lexem_type::oper) || (expression[i].field_type == lexem_type::bracket))
		{
			cout << expression[i].val.oper.val;
		}
		else
		{
			cout << expression[i].val.number;
		}
	}
	cout << endl;
}

int main()
{
	vector<Lexem> expression;
	string exp = "2+2*2^2";
	split(exp, expression);
	print_expression(expression);
	cout <<"Postfix form" << endl;
	vector<Lexem> postfix;
	convert_to_postfix(expression, postfix);
	print_expression(postfix);
	system("pause");
    return 0;
}

