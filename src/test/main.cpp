#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <test/catch.hpp>

#include "pelopia.h"

using namespace pelopia;

TEST_CASE("lets see how this works") {
	REQUIRE ( Search () );
}

