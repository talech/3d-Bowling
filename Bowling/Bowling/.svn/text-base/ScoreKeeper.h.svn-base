#ifndef ScoreKeeper_H
#define ScoreKeeper_H

class ScoreKeeper{
public:
	ScoreKeeper();
	~ScoreKeeper();

	int getScoreThrow(int s_frame, int s_throwNum);
	int getScoreThrow(int s_throwNum);
	int getFrameScore(int s_frame);
	int getTotalScore();

	int getFrame(){ return frame;}
	int getThrow(){ return throwNum;}

	void setScoreThrow(int score);
	bool isGameOver(){return gameOver;}

	void reset();
	

private:
	int throws[21];
	int frameScore[10];
	int totalScore;

	bool gameOver;
	int frame;
	int throwNum;

	void calculateScore(int currentFrame);
	void calculateTotalScore();
	int calculateSpare(int thisFrame, int currentFrame);
	int calculateStrike(int thisFrame, int currentFrame);

};

#endif