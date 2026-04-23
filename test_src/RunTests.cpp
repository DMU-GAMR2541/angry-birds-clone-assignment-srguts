#include <gtest/gtest.h>
#include "Enemy.h"
#include "Slingshot.h"


/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Enemy> enemy;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    EnemyTest() {
        // You can do set-up work for each test here.
                    
    }

    ~EnemyTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
                    
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

//A single test, not a fixture. No setup is called.
TEST(Enemy, First_test) {                                      // Tests that health is initialized correctly.
    Enemy e(100);
    EXPECT_EQ(e.getHealth(), 100);  
}
// SUCCEED() << "Test test passed";
// FAIL() << "Test didn't pass";

TEST_F(EnemyTest, LethalDamagePopsPig) {                        // Tests that lethal damage pops the pig.
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, NonLethalDamageDoesntPopPig) {                // Tests that non-lethal damage doesn't pop the pig.
    enemy->takeDamage(30);
    EXPECT_FALSE(enemy->checkIfPopped());
}
    
TEST_F(EnemyTest, NegDamageDoesntPopPig) {                      // Tests that negative damage doesn't pop the pig.
	enemy->takeDamage(-10);
	EXPECT_FALSE(enemy->checkIfPopped());
}
TEST_F(EnemyTest, NonLethalDamageReducesHealth) {               // Tests that non-lethal damage reduces the pig's health.
	enemy->takeDamage(20);
	EXPECT_NE(enemy->getHealth(), 50);
}

TEST_F(EnemyTest, DamageReducesHealth) {                        // Tests that damage reduces the pig's health.
    enemy->takeDamage(20);
    EXPECT_EQ(enemy->getHealth(), 30);
}

TEST_F(EnemyTest, NonLethalDamageDoesntZeroHealth) {            // Tests that non-lethal damage doesn't reduce the pig's health to zero.
    enemy->takeDamage(20);
    EXPECT_GT(enemy->getHealth(), 0);
}

TEST_F(EnemyTest, Assert_LethalDamagePopsPig) {                 // Tests that lethal damage pops the pig, using ASSERT_TRUE.
    enemy->takeDamage(60);
    ASSERT_TRUE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, Assert_LethalDamageZerosHealth) {             // Tests that lethal damage reduces the pig's health to zero or lower, using ASSERT_LE.
    enemy->takeDamage(60);
    ASSERT_LE(enemy->getHealth(), 0);  // getHealth() <= 0
}

TEST_F(EnemyTest, StringsIgnoreCase) {                          // Tests that two strings are equal, ignoring case.
    std::string str1 = "Pig";
    std::string str2 = "pig";
	EXPECT_STRCASEEQ(str1.c_str(), str2.c_str());
}

TEST_F(EnemyTest, StringsNotEqual) {                            // Tests that two strings are not equal.
    std::string str1 = "Pig";
    std::string str2 = "Bird";
    EXPECT_STRNE(str1.c_str(), str2.c_str());
}

int main(int argc, char** argv) {                               // Initializes GoogleTest and runs all tests.
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
