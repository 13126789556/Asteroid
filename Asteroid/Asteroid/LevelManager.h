#pragma once
class LevelManager
{
public:
	int levelRank, score;

	LevelManager();
	void NewLevel();
	void CollisionDetection();
};

