// #define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_RUNNER
#include <test/catch.hpp>

using namespace std;

#include <pelopia/Pelopia.h>

#define LIBPOSTAL 0

#if LIBPOSTAL
#include <../../libpostal/src/libpostal.h>
#endif

using namespace Mapzen :: Pelopia;
using namespace std;

#if LIBPOSTAL
TEST_CASE("basic call to libpostal") 
{
	char en[] = "en";
    char *languages [ 1 ] = { en };
	
    normalize_options_t options = LIBPOSTAL_DEFAULT_OPTIONS;
    options.languages = languages;

    uint64_t num_expansions;

	SECTION ( "full US address, default options" )
	{
		char input[] = "101 Curved Iron Court, Herndon Virginia 20171";
		char **strings = expand_address(input, options, &num_expansions);

		REQUIRE ( 1 == num_expansions );
		REQUIRE ( string ( strings [ 0 ] ) == "101 curved iron court, herndon virginia 20171" );
		
		free ( strings [ 0 ] );
		free(strings);
	}
	SECTION ( "full US address, ADDRESS_HOUSE_NUMBER" )
	{
		char input[] = "101 Curved Iron Court, Herndon Virginia 20171";
		options . address_components = ADDRESS_UNIT;
		char **strings = expand_address(input, options, &num_expansions);

		REQUIRE ( 1 == num_expansions );
		REQUIRE ( string ( strings [ 0 ] ) == "101 curved iron court, herndon virginia 20171" );
		
		free ( strings [ 0 ] );
		free(strings);
	}
}
#endif


int main( int argc, char* const argv[] )
{
#if LIBPOSTAL
	// global setup...
    if (!libpostal_setup()) {
        exit(EXIT_FAILURE);
    }
#endif

	int result = Catch::Session().run( argc, argv );

#if LIBPOSTAL
	// global clean-up...
    libpostal_teardown();
#endif

	return result;
}