#include <utils.h>

/*
THINGS TO ADD


add auto detect for input in order for a auto output

endiness (MSB LSB)

negative number fix and printing negative numbers from hex

*/


int main(int argc, char* argv[])
{
	FlagParser flags;

	SetUpFlags(argc, argv, flags);

	if (argc < 2 || flags.hasLongFlag(HELP_FLAG_LONG) || flags.hasShortFlag(HELP_FLAG_SHORT))
	{
		flags.printUsage("uni_converter");
		return 1;
	}
	

	std::string sInput = argv[1];
	size_t nInputSize = sInput.size();
	std::vector<uint64_t> bytes;

	bytes = ConvertInputToVector(sInput, nInputSize, flags);
	

	PrintOutput(bytes, flags);

	return 0;
}