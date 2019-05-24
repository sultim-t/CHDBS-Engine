#include "StdH.h"
#include <catch.hpp>
#include <Engine/DataStructures/HashTable.h>
#include <Engine/Base/String.h>

TEST_CASE("Hash table")
{
	HashTable<String, int> table;

	// init
	table.Init(2, 4);
	table.DeclareHashFunction(String::StringHash);

	// add
	table.Add("Qw", 10);
	table.Add("As", 20);
	table.Add("Zx", 30);

	REQUIRE(table.GetSize() == 3);

	int found;

	// find
	REQUIRE(table.Find("Qw", found));
	REQUIRE(found == 10);
	REQUIRE(table.Find("As", found));
	REQUIRE(found == 20);
	REQUIRE(table.Find("Zx", found));
	REQUIRE(found == 30);

	REQUIRE_FALSE(table.Find("Er", found));

	int removed;

	// remove
	table.Remove("Qw", removed);

	REQUIRE(removed == 10);
	REQUIRE_FALSE(table.Find("Qw", found));

	REQUIRE(table.GetSize() == 2);

	// add more than allocated
	table.Add("Dfdsaw", 40);
	table.Add("Cvdsax", 40);
	table.Add("Tycrcx", 40);
	table.Add("Ghcdgs", 40);
	table.Add("Bvsbtn", 40);
	table.Add("Uihzbt", 40);
	table.Add("Jbbdfk", 40);
	table.Add("Mbyjjo", 40);
	table.Add("Pgstul", 40);

	REQUIRE(table.GetSize() == 11);
}