#include <gtest/gtest.h>
#include "Enemy.h"
#include "Slingshot.h"
#include "RedBird.h"
#include "StaticObject.h"

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

TEST(Enemy, Health_test) {                                       // Tests that health is initialized correctly.
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

// Health Reduction tests.
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

// Assert tests.
TEST_F(EnemyTest, Assert_LethalDamagePopsPig) {                 // Tests that lethal damage pops the pig, using ASSERT_TRUE.
    enemy->takeDamage(60);
    ASSERT_TRUE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, Assert_LethalDamageZerosHealth) {             // Tests that lethal damage reduces the pig's health to zero or lower, using ASSERT_LE.
    enemy->takeDamage(60);
    ASSERT_LE(enemy->getHealth(), 0);  
}
 
// String tests.
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

// Slingshot tests.
TEST(Slingshot, Instantiation) {
    Slingshot s(sf::Vector2f(0, 0));                            // Tests that the slingshot can be instantiated without crashing.
	EXPECT_TRUE(true); 
}

// tests that the slingshot can be dragged without crashing.
TEST(Slingshot, Dragging) {
    Slingshot s(sf::Vector2f(0, 0));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Window");
    sf::Event event;
    std::shared_ptr<Bird> activeBird = nullptr; // no bird is active
    s.update(window, event, activeBird);
    EXPECT_TRUE(true); 
}

// Tests that the red bird moves when given a velocity 
TEST(DynamicObjectTest, CharacterMovement) {
	// create a Box2D world with gravity
	b2Vec2 gravity(0.0f, 9.81f);                              
    b2World world(gravity);
	
	// create a RedBird at origin in the world
	RedBird redbird(b2Vec2(0.0f, 0.0f), world);                   
	
	// get the initial position of the RedBird
	b2Vec2 initialPosition = redbird.getBody()->GetPosition();    
	
	// apply a velocity to the RedBird's body to act as movement
    redbird.getBody()->SetLinearVelocity(b2Vec2(10.0f, 0.0f));
	
	// move the Box2D world forward to update the RedBird's position based on its velocity
    float timeStep = 1.0f / 60.0f;
    world.Step(timeStep, 8, 3);
	
	// update RedBird's state
    redbird.update();
	
	// get the final position of the RedBird 
    b2Vec2 finalPosition = redbird.getBody()->GetPosition();
    EXPECT_NE(finalPosition.x, initialPosition.x);
}

TEST(StaticObjectTest, SceneryPlacementWithinBounds) {
	// create a Box2D world with gravity
    b2Vec2 gravity(0.0f, 9.81f);
    b2World world(gravity);
	
	// create a StaticObject (plank) at position (20, 12) in the world with size (2, 0.5)
    StaticObject plank("../../../../assets/wall.png", b2Vec2(20.0f, 12.0f), world, sf::Vector2f(2.0f, 0.5f));
	
	// check that the plank's position is within bounds
    b2Vec2 pos = plank.getBody()->GetPosition();
    EXPECT_GE(pos.x, 0.0f);
    EXPECT_LE(pos.x, 40.0f);
    EXPECT_GE(pos.y, 0.0f);
    EXPECT_LE(pos.y, 24.0f);
}

TEST(SpatialTests, RelativePositions) {                                    // test to check that objects are placed in the correct positions relative to other objects
    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);
    sf::Vector2f size(1.0f, 1.0f);

    // create 3 objects in a straight line
    StaticObject obj1("../../../../assets/wall.png", b2Vec2(0.0f, 0.0f), world, size);
    StaticObject obj2("../../../../assets/wall.png", b2Vec2(10.0f, 0.0f), world, size);
    StaticObject obj3("../../../../assets/wall.png", b2Vec2(20.0f, 0.0f), world, size);

	// get their x positions
    float x1 = obj1.getBody()->GetPosition().x;
    float x2 = obj2.getBody()->GetPosition().x;
    float x3 = obj3.getBody()->GetPosition().x;

    // and the maths to check if they work
    EXPECT_EQ(x2, x1 + 10.0f);
    EXPECT_EQ(x3, x2 + 10.0f);
    EXPECT_EQ(x3, x1 + 20.0f);
}

TEST(SpriteTextureTest, SpriteTextureLoad) {                               // test to check that sprite textures load correctly 
    sf::Texture texture;
    
    // check for a file that does exist
    EXPECT_TRUE(texture.loadFromFile("../../../../assets/Ang_Birds/Pig.png"));
    
    // check for a file that doesnt exist
    EXPECT_FALSE(texture.loadFromFile("../../../../assets/Ang_Birds/thisfiledoesntexist.png"));
}

class ParamTest : public ::testing::TestWithParam<int> {                    // Tests that the value of the param is greater than 1.
protected:
    ParamTest() = default;
    ~ParamTest() = default;

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).

    }

    void TearDown() override {

    }
};

TEST_P(ParamTest, SimpleTest) {                                 
    int i_test = GetParam();
    std::cout << "Param value:: " << i_test << std::endl;
    EXPECT_GT(i_test, 1);
}

INSTANTIATE_TEST_SUITE_P(                                       // Instantiates the test suite with a list of values.
    Simple,
    ParamTest, ::testing::Values(2, 3, 4, 5)
);

int main(int argc, char** argv) {                               // Initializes GoogleTest and runs all tests.
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
