
#include "catch.hpp"
#include "NumberTree.h"

using namespace cis263;

// Note:  "Clean" tests are tests with clean input that should not raise any exceptions.
//        "Dirty" tests should raise an exception
//         Passing "clean" or "dirty" on the command line will run only those tests.

TEST_CASE("empty tree") {
  NumberTree nt;
  REQUIRE(0 == nt.size());
  REQUIRE(-1 == nt.height());
}

TEST_CASE("clean load: one named node") {
  NumberTree nt;
  nt.load("root;");
  REQUIRE(1 == nt.size());
  REQUIRE(0 == nt.height());
}

TEST_CASE("clean load: one numbered node") {
  NumberTree nt;
  nt.load("17;");
  REQUIRE(1 == nt.size());
  REQUIRE(0 == nt.height());
}

TEST_CASE("dirty load: empty string") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load(""), std::invalid_argument);
}

TEST_CASE("dirty load: invalid node name starts with number") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load("4ourth;"), std::invalid_argument);
}

TEST_CASE("dirty load: invalid node name starts with minus") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load("-1;"), std::invalid_argument);
}

TEST_CASE("dirty load: invalid node name starts with punctuation") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load(".hidden;"), std::invalid_argument);
}

TEST_CASE("clean load: two named nodes") {
  NumberTree nt;
  nt.load("a; root a;");
  REQUIRE(2 == nt.size());
  REQUIRE(1 == nt.height());
}

TEST_CASE("clean load: one named node; one numbered node") {
  NumberTree nt;
  nt.load("1; root 1;");
  REQUIRE(2 == nt.size());
  REQUIRE(1 == nt.height());
}

TEST_CASE("clean load: named leaf; numbered root") {
  NumberTree nt;
  nt.load("a; 1 a;");
  REQUIRE(2 == nt.size());
  REQUIRE(1 == nt.height());
}

TEST_CASE("dirty load: value too large") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load("2; root 2;"), std::invalid_argument);
}

TEST_CASE("dirty load: value too small (0)") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load("0; root 0;"), std::invalid_argument);
}

TEST_CASE("dirty load: value too small (-1)") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load("-1; root -1;"), std::invalid_argument);
}

TEST_CASE("dirty load: node with no parent") {
  NumberTree nt;
  REQUIRE_THROWS_AS(nt.load("a; b; 3 a;"), std::invalid_argument);
}

//
// Verify
//

TEST_CASE("clean verify true: one correctly assigned inner node") {
  NumberTree nt;
  nt.load("a; 1 a;");
  REQUIRE(nt.verify("a 1;"));
}

TEST_CASE("clean verify true: two correctly assigned inner nodes; fixed root") {
  NumberTree nt;
  nt.load("a; b; 3 a b;");
  REQUIRE(nt.verify("a 1; b 2;"));
}

// Notice that root value does not need to be between 1 and 9
TEST_CASE("clean verify true: three correctly assigned inner nodes; open root") {
  NumberTree nt;
  nt.load("a; b; c; root a b c;");
  REQUIRE(nt.verify("a 1; b 2; c 3; root 6"));
}

// Notice that root value does not need to be between 1 and 9
TEST_CASE("clean verify false: open root must have value specified") {
  NumberTree nt;
  nt.load("a; b; c; root a b c;");
  REQUIRE(!nt.verify("a 1; b 2; c 3;"));
}


TEST_CASE("dirty verify: one inner node value too large") {
  NumberTree nt;
  nt.load("a; 2 a");
  REQUIRE_THROWS_AS(!nt.verify("a 2;"), std::invalid_argument);
}

TEST_CASE("dirty verify: one inner node value too small (0)") {
  NumberTree nt;
  nt.load("a; 1 a");
  REQUIRE_THROWS_AS(!nt.verify("a 0;"), std::invalid_argument);
}

TEST_CASE("dirty verify: one inner node value too small (-1)") {
  NumberTree nt;
  nt.load("a; 1 a");
  REQUIRE_THROWS_AS(!nt.verify("a -1;"), std::invalid_argument);
}

TEST_CASE("dirty verify: one inner node -- unknown node") {
  NumberTree nt;
  nt.load("a; 2 a");
  REQUIRE_THROWS_AS(!nt.verify("b 1;"), std::invalid_argument);
}

TEST_CASE("dirty verify: one inner node --- not all nodes have values") {
  NumberTree nt;
  nt.load("a; 1 a");
  REQUIRE(!nt.verify(""));
}

TEST_CASE("dirty verify: one inner node -- unparsable value") {
  NumberTree nt;
  nt.load("a; 1 a");
  REQUIRE_THROWS_AS(nt.verify("a fred;"), std::invalid_argument);
}

TEST_CASE("clean verify true: height 2 returns true") {
  NumberTree nt;
  nt.load("a; b; c; d a b; e b c; 8 d e;");
  REQUIRE(nt.verify("a 2; b 1; c 4; d 3; e 5;"));
}

TEST_CASE("clean verify false: height 2 returns false") {
  NumberTree nt;
  nt.load("a; b; c; d a b; e b c; 8 d e;");
  REQUIRE(!nt.verify("a 1; b 2; c 4; d 3; e 5;"));
}

TEST_CASE("clean verify false: height 2 value used multiple times.") {
  NumberTree nt;
  nt.load("a; b; c; d a b; e b c; 6 d e;");
  REQUIRE(!nt.verify("a 1; b 1; c 3; d 2; e 4;"));
}

TEST_CASE("dirty verify: attempted reassignment of fixed node") {
  NumberTree nt;
  nt.load("a; b; c; d a b; 5 b c; 8 d 5;");
  REQUIRE_THROWS_AS(nt.verify("a 4; b 1; c 2; d 5; 5 3;"), std::invalid_argument);
}


//
// Solve
//

TEST_CASE("solve: named leaf; numbered root") {
  NumberTree nt;
  nt.load("a; 1 a;");
  map<string, int> answer = nt.solve();
  REQUIRE(1 == answer.size());
  REQUIRE(1 == answer["a"]);
}

TEST_CASE("solve: unsolvable puzzle returns empty map") {
  NumberTree nt;
  nt.load("a; 2 a;");
  REQUIRE(nt.solve().empty());
}

TEST_CASE("solve: height 2 (v1)") {
  NumberTree nt;
  nt.load("a; b; c; 5 a b; e b c; 8 5 e;");
  map<string, int> answer = nt.solve();nt.solve();
  REQUIRE(4 == answer.size());
  REQUIRE(4 == answer["a"]);
  REQUIRE(1 == answer["b"]);
  REQUIRE(2 == answer["c"]);
  REQUIRE(3 == answer["e"]);
}

TEST_CASE("solve: height 2 (v2)") {
  NumberTree nt;
  nt.load("a; b; c; 3 a b; e b c; 8 3 e;");
  map<string, int> answer = nt.solve();nt.solve();
  REQUIRE(4 == answer.size());
  REQUIRE(2 == answer["a"]);
  REQUIRE(1 == answer["b"]);
  REQUIRE(4 == answer["c"]);
  REQUIRE(5 == answer["e"]);
}

// Add more tests here 