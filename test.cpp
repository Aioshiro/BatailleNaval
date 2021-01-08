#include "pch.h"
#include "../BatailleNaval/batailleNavaleFinale.cpp"


TEST(getColorTouche_test, brw) {
  vector<int> color = getColortouche('r');
  EXPECT_EQ(1, color[0]);
  EXPECT_EQ(0, color[1]);
  EXPECT_EQ(0, color[2]);
  color = getColortouche('b');
  EXPECT_EQ(0, color[0]);
  EXPECT_EQ(1, color[1]);
  EXPECT_EQ(0, color[2]);
  color = getColortouche('w');
  EXPECT_EQ(1, color[0]);
  EXPECT_EQ(1, color[1]);
  EXPECT_EQ(1, color[2]);
}

TEST(getColorTouche_test, autrecarac) {
	vector<int> color = getColortouche('p');
	EXPECT_EQ(0, color[0]);
	EXPECT_EQ(0, color[1]);
	EXPECT_EQ(0, color[2]);

}

TEST(getColorBateau_test, bateauxnormaux) {
	vector<int> color = getColorbateau(0);
	EXPECT_EQ(1, color[0]);
	EXPECT_EQ(1, color[1]);
	EXPECT_EQ(1, color[2]);
	vector<int> color = getColorbateau(1);
	EXPECT_EQ(1, color[0]);
	EXPECT_EQ(0, color[1]);
	EXPECT_EQ(0, color[2]);
}

TEST(getColorBateau_test, othercases) {
	vector<int> color = getColorbateau(-1);
	EXPECT_EQ(0, color[0]);
	EXPECT_EQ(0, color[1]);
	EXPECT_EQ(0, color[2]);
	vector<int> color = getColorbateau(100);
	EXPECT_EQ(0, color[0]);
	EXPECT_EQ(0, color[1]);
	EXPECT_EQ(0, color[2]);
}

class BoardTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		playerboard1.couleurs[5][4] = 'r';
		playerboard1.grid[5][4] = 0;
		playerboard1.grid[1][2] = 3;
	}
	virtual void TearDown() {
		//
	}
	Board playerboard1;
	Board ennemiboard1;
	
};

TEST_F(BoardTest, loadDataFromFile_validfile) {
	playerboard1.loadDataFromFile("config1.txt");
	EXPECT_EQ(playerboard1.lengthOfShips[2],3); //modification venant de config1.txt
}

TEST_F(BoardTest, loadDataFromFile_unvalidfile) {
	EXPECT_NO_THROW(playerboard1.loadDataFromFile("blablabla.txt"));
}

TEST_F(BoardTest, impossibledeplacer_impossible) {
	EXPECT_TRUE(playerboard1.impossibleplacer(3, 'd', 1, 2)); //commencer sur une case impossible
	EXPECT_TRUE(playerboard1.impossibleplacer(3, 'd', 1, 1)); //la case impossible est au milieu du bateau
	EXPECT_TRUE(playerboard1.impossibleplacer(3, 'r', 9, 9));//le bateau depasse à droite
}
TEST_F(BoardTest, impossibledeplacer_possible) {
	EXPECT_FALSE(playerboard1.impossibleplacer(3, 'd', 5, 4));
}

TEST_F(BoardTest, checkForVictory) {
	EXPECT_EXIT(ennemiboard1.checkForVictory(), ::testing::ExitedWithCode(0),"");
	EXPECT_NO_THROW(playerboard1.checkForVictory()); // nothing happen when the board still has ships
}