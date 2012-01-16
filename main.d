// This will be the main driving program for the behavioral simulator

module SimulationDriver;

private import std.stdio;

int main(string[] args)
{
	writeln("Test");
	for ( auto i = 0; i < args.length; i++ )
	{
		writeln("Argument " , i , " was: " , args[i]);
	}
	return 0;
}