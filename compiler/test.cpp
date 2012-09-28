#define DEBUG
#ifdef DEBUG
#include "parser/parser.h"
#include "astnodes.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "tuple/tuplemanager.h"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <ctime>
using namespace std;



int main(int argc,char ** args){

	std::ofstream os0("testo0.asm");
	std::ofstream os1("testo1.asm");
	std::ofstream os2("testo2.asm");
	std::ofstream os3("testo3.asm");
	char* filename = "testcase2.txt";
	{
		tuple::TupleManager::initHandlers(os0,0,true);


		std::ifstream ifile2(filename);
		Lexer lexer2(ifile2);
		Parser parser2(lexer2);
		clock_t start1 = clock();
	
		pCompileUnit pcu2 = parser2.parse();
		clock_t end1 = clock();
		system("ml /c /coff testo0.asm");
		system("link /subsystem:console testo0.obj");
		clock_t start = clock();
		system("testo0");
		clock_t end = clock();
		std::cout<<"No optimization"<<std::endl;
		std::cout<<"compile time:"<< end1-start1<<std::endl;
		std::cout<<"excution time:"<<end-start <<std::endl;
		
	}
	{
		tuple::TupleManager::initHandlers(os1,tuple::TupleManager::DAG,true);

		std::ifstream ifile2(filename);
		Lexer lexer2(ifile2);
		Parser parser2(lexer2);
		clock_t start1 = clock();
	
		pCompileUnit pcu2 = parser2.parse();
		clock_t end1 = clock();
		system("ml /c /coff testo1.asm");
		system("link /subsystem:console testo1.obj");
		clock_t start = clock();
		system("testo1");
		clock_t end = clock();
		std::cout<<"DAG optimization"<<std::endl;
		std::cout<<"compile time:"<< end1-start1<<std::endl;
		std::cout<<"excution time:"<<end-start <<std::endl;
	}

	{
		tuple::TupleManager::initHandlers(os2,tuple::TupleManager::DAGANDREGISTER,true);

		std::ifstream ifile2(filename);
		Lexer lexer2(ifile2);
		Parser parser2(lexer2);
		clock_t start1 = clock();
	
		pCompileUnit pcu2 = parser2.parse();
		clock_t end1 = clock();
		system("ml /c /coff testo2.asm");
		system("link /subsystem:console testo2.obj");
		clock_t start = clock();
		system("testo2");
		clock_t end = clock();
		std::cout<<"DAGREGISTER optimization"<<std::endl;
		std::cout<<"compile time:"<< end1-start1<<std::endl;
		std::cout<<"excution time:"<<end-start <<std::endl;
	}
	{
		tuple::TupleManager::initHandlers(os3,tuple::TupleManager::ALL,true);

		std::ifstream ifile2(filename);
		Lexer lexer2(ifile2);
		Parser parser2(lexer2);
		clock_t start1 = clock();
	
		pCompileUnit pcu2 = parser2.parse();
		clock_t end1 = clock();
		system("ml /c /coff testo3.asm");
		system("link /subsystem:console testo3.obj");
		clock_t start = clock();
		system("testo3");
		clock_t end = clock();
		std::cout<<"ALL optimization"<<std::endl;
		std::cout<<"compile time:"<< end1-start1<<std::endl;
		std::cout<<"excution time:"<<end-start <<std::endl;
	}
	int s = sizeof(Token);
	return 0;
}
#endif