#include "catch.hpp"
#include <entity/entity.hpp>
#include <entity/json.hpp>

using namespace std;
using namespace ent;


struct SimpleEntity : ent::entity
{
	string name		= "simple";
	bool flag		= true;
	int integer		= 42;
	long bignumber	= 20349758;
	double floating	= 3.142;

	mapping describe()
	{
		return { eref(name), eref(flag), eref(integer), eref(bignumber), eref(floating) };
	}
};


struct CollectionEntity : ent::entity
{
	vector<string> strings;
	vector<double> doubles;
	vector<byte> binary;
	map<string, string> dictionary;

	mapping describe()
	{
		return { eref(strings), eref(doubles), eref(binary), eref(dictionary) };
	}
};


struct ComplexEntity : ent::entity
{
	string name;
	vector<SimpleEntity> entities;
	CollectionEntity collection;
	SimpleEntity simple;

	mapping describe()
	{
		return { eref(name), eref(entities), eref(collection), eref(simple) };
	}
};


TEST_CASE("an entity can be serialised", "[entity]")
{
	SimpleEntity e;
	string data = u8R"json({"bignumber":20349758,"flag":true,"floating":3.142,"integer":42,"name":"simple"})json";

	REQUIRE(entity::encode<json>(e) == data);
}


TEST_CASE("an entity can be deserialised", "[entity]")
{
	auto e = entity::decode<json, ComplexEntity>(u8R"json({
		"name":	"parsed complex",
		"simple": {
			"name":		"parsed simple",
			"flag":		false,
			"integer":	1234
		},
		"collection": {
			"strings":		[ "a", "b", "c", "d" ],
			"doubles":		[ 9, 8, 7, 6 ],
			"binary":		"AAECiP8=",
			"dictionary":	{ "a": "1", "b": "2" }
		},
		"entities": [
			{ "name": "simple 1", "integer": 1 },
			{ "name": "simple 2", "integer": 2 },
		]
	})json");

	REQUIRE(e.name							== "parsed complex");
	REQUIRE(e.simple.name					== "parsed simple");
	REQUIRE(e.simple.integer				== 1234);
	REQUIRE(e.collection.strings[1]			== "b");
	REQUIRE(e.collection.doubles[2]			== 7);
	REQUIRE(e.collection.binary[3]			== 0x88);
	REQUIRE(e.collection.dictionary["b"]	== "2");
	REQUIRE(e.entities[0].name				== "simple 1");
	REQUIRE(e.entities[1].integer			== 2);
}
