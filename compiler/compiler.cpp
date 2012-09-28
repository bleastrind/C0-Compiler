#include "compiler.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void Compiler::compile(char *filename,int options,bool changeline)
{
	std::ifstream ifile(filename);
	Lexer lexer(ifile);
	Parser parser(lexer);

	std::ofstream ofile("ans.asm");

	tuple::TupleManager::initHandlers(ofile,options,changeline);

	if (options & tuple::TupleManager::DAG)
		std::cout<<"Optimization: DAG opened!"<<std::endl;
	if (options & tuple::TupleManager::REGISTER)
		std::cout<<"Optimization: Global Register opened!"<<std::endl;

	std::cout<<"Start Compile.."<<std::endl;
	clock_t start = clock();
	parser.parse();
	clock_t end = clock();
	

	ifile.close();
	ofile.close();

	if ( err::Error::IsError() )
		std::cout<<"Fatel Error Happened,Can't compile!"<<std::endl;
	else
	{
		system("ml /c /coff ans.asm");
		system("link /subsystem:console ans.obj");
	}
	std::cout<<"Compile time:"<< end-start<<std::endl;
}
//#define RELAEASE
#ifdef RELAEASE
int main(int argc, char **args)
{
	if (argc <= 1)
	{
		std::cout<<"Usage: complie <filename> [<optimization mask(0|1|2|3)> <printf changeline>(yes|no)]"<<std::endl;
		return 0;
	}	
	int options = 0;
	bool changeline = true;
	if (argc >= 3)
		options = atoi(args[2]);
	if (argc >= 4)
		changeline = strcmp(args[3],"yes");
	char* filename = args[1];

	Compiler compiler;
	compiler.compile(filename,options,changeline);

	return 0;

}
#endif