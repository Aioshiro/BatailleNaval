#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/// <summary>
/// \class Board
///		\brief A class which take care of the board
///		Contains methods to place ships, displaying the grid in text form, check for hit
/// </summary>

class Board
{
protected:
	/// <summary>
	///		\brief Describes the players fleet
	///		Contains the ships, described by their size
	/// </summary>
	int lengthOfShips[5] = { 5,4,3,3,2 }; 

public:
    /// <summary>
	///		Represents the terrain
    /// </summary>
    int** grid;
    /// <summary>
	///		Memorizes which cases are hit
    /// </summary>
    vector<vector<char>> couleurs;
	/// <summary>
	///  \brief Text shown at the end of the game
	/// </summary>
	static string endGame;

	Board(string fileName)
	{
		createGrid();
        couleurs.resize(10);
		for (int i = 0; i < 10; i++) {
			couleurs[i].resize(10);
			for (int j = 0; j < 10; j++) {
				couleurs[i][j] = 'b';
			}
		}
		loadDataFromFile(fileName);
	}

	~Board()
	{
		delete(grid);
	}

	/// <summary>
	/// \brief Initialize the grid
	/// </summary>
	void createGrid()
	{
		grid = new int* [10];
		for (int i = 0; i < 10; i++)
		{
			grid[i] = new int[10];
			for (int j = 0; j < 10; j++)
			{
				grid[i][j] = 0; //intialized to 0 (no ships)
			}
		}
	}
	/// <summary>
	/// \brief Load the file to read the fleet, if issue reading file, fleet is { 5,4,3,3,2 } by default.
	/// </summary>
	/// <param name="fileName"> Name of the file </param>
	void loadDataFromFile(string fileName)
	{
		int i = 0;
		int ship = 0;
		string line;
		ifstream myfile(fileName);
		if (myfile.is_open())
		{
			{
				while (getline(myfile, line))
				{
					i = line[0]-'0';
					lengthOfShips[i] = line[2]-'0';
					cout<< lengthOfShips[i]<<endl;


				}
				myfile.close();
			}

		}

		else cout << "Unable to open file" << endl;
	}


	/// <summary>
	/// \Brief Check is the placement of a ship is impossible
	/// </summary>
	/// <param name="taille"> Size of the grid</param>
	/// <param name="dir"> Direction of the ship (d for down, r for right)</param>
	/// <param name="colonne"> Number of the row </param>
	/// <param name="ligne"> Number of the row </param>
	/// <returns> Boolean true if it's not possible to place the ship</returns>
	bool impossibleplacer(int taille, char dir, int colonne, int ligne)
	{
		if (dir == 'r')
		{
			if (taille + colonne > 10) // ship will be out of the board on the right
			{
				return true;
			}
			else
			{
				for (int i = colonne; i < colonne + taille; i++)
				{
					if (grid[ligne][i] != 0) //ship is colliding with an already placed ship
					{
						return true;
					}
				}
			}
		}
		else if (dir == 'd')
		{
			if (taille + ligne > 10) //ship will be out of the board on the down side
			{
				return true;
			}
			else
			{
				for (int i = ligne; i < ligne + taille; i++) //ship is colliding with an already place ship
				{
					if (grid[i][colonne] != 0)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	/// <summary>
	/// \Brief Prompts the player to place his ships
	/// </summary>
	void placerplayer()
	{

		// joueur
		for (int i = 0; i < 5; i++)
		{

			bool siplace = false;
			while (!siplace)
			{
				int col;
				char L;
				int lig;
				char dir;
				cout << "Placement du bateau : taille" << " (" << lengthOfShips[i] << ") " << endl;
				cout << "X (1-10) : ";
				cin >> col;
				col--;
				cout << "Y (A-J) : ";
				cin >> L;
				lig = L - 'A';
				cout << "Direction (d/r) : ";
				cin >> dir;
				/* col=0;
				lig=i;
				dir='r'; */
				int taille = lengthOfShips[i];
				if (!impossibleplacer(taille, dir, col, lig))
				{
					for (int j = 0; j < taille; j++)
					{
						if (dir == 'd')
						{
							grid[j + lig][col] = 1;
						}
						else if (dir == 'r')
						{
							grid[lig][j + col] = 1;
						}
					}
					siplace = true;
				}
				else {
					cout << "Impossible to place ship, try again" << endl;
				}
			}
		}


	}
	/// <summary>
	/// \Randomly places the computer ships
	/// </summary>
	void placerordi()
	{
		// ordi
		for (int i = 0; i < 5; i++)
		{
			bool siplace = false;
			while (!siplace)
			{
				int col = rand() % 10;
				int lig = rand() % 10;
				char dir = rand() % 2 == 0 ? 'd' : 'r';
				int taille = lengthOfShips[i];
				if (!impossibleplacer(taille, dir, col, lig))
				{
					for (int j = 0; j < taille; j++)
					{
						if (dir == 'd')
						{
							grid[j + lig][col] = i + 1;
						}
						else if (dir == 'r')
						{
							grid[lig][j + col] = i + 1;
						}
					}
					siplace = true;
				}
			}
		}
	}
	/// <summary>
	/// \Brief Displays the grid in text form
	/// </summary>
	void displayGrid()
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cout << grid[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	/// <summary>
	/// /Brief Checks if a ship is hit by a shot
	/// </summary>
	/// <param name="x"> X coordinate of the shot</param>
	/// <param name="y"> Y coordinate of the shot</param>
	/// <returns> Return "Rate" if not hit, touché if hit, "Victory" if it's the end of the game throw and exit on an unvalid shot</returns>
	string hit(int x, int y)
	{
		if (x < 0 || y < 0 || x > 9 || y > 9)
		{
			cerr << "Error : trying to fire outside grid bounds." << endl;
			exit(-1);
		}
		bool victory = false;
		int ship = grid[x][y];

		bool lostGame = true;

		if (ship == 0)
		{
		    couleurs[x][y]='w';
			return("Raté");
		}
		else {
			couleurs[x][y] = 'r';
			grid[x][y] = 0;

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (grid[i][j] != 0)
					{
						victory = false;
					}
				}
			}

			if (victory)
			{
				cout << endGame << endl;
				exit(0);
			}
			else {
				return ("touché");
			}
		}
	}
};
