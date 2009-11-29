#include "ScoreKeeper.h"

ScoreKeeper::ScoreKeeper(){
	for(int i=0; i<21; i++){
		throws[i] = -1; //set all scores to -1
	}
	for(int i=0; i<10; i++){
		frameScore[i] = -1; //set all scores to -1
	}
	totalScore = 0;
	frame = 1;
	throwNum = 1;
	gameOver = false;
}


ScoreKeeper::~ScoreKeeper(){
}

int
ScoreKeeper::getScoreThrow(int s_frame, int s_throwNum){
	if(s_throwNum == 1)
		return throws[(s_frame*s_throwNum)+1];
	else if(throwNum == 2)
		return throws[(s_frame*s_throwNum)-1];
	else 
		return -1;
}

int
ScoreKeeper::getScoreThrow(int s_throwNum){
	return throws[s_throwNum];
}

int 
ScoreKeeper::getFrameScore(int s_frame){
	return frameScore[s_frame-1];
}

int
ScoreKeeper::getTotalScore(){
	return totalScore;
}

void
ScoreKeeper::setScoreThrow(int score){
	if(frame>10){
		if((throws[(10*2)-2] == 10)||((throws[(10*2)-2] + throws[(10*2)-1])== 10)){
			throws[20] = score;
			gameOver = true;
		}
	}
	else if(throwNum == 1){
		throws[(frame*2)-2] = score;
		if(score == 10){
			frame++;
			throwNum = 1;
		}
		else
		{
			throwNum = 2;
		}
	}
	else if(throwNum == 2){
		throws[(frame*2)-1] = score-throws[(frame*2)-2];
		//if 2nd throw also calculate frame score
		calculateScore(frame);
		frame++;
		throwNum = 1;
	}

	if(frame == 11){
		if((throws[(10*2)-2] == 10)||((throws[(10*2)-2] + throws[(10*2)-1])== 10)){
			
		}
		else{
			gameOver = true;
		}
	}
}

void
ScoreKeeper::calculateScore(int currentFrame){
	for(int i=1; i<(currentFrame+1); i++){
		if(throws[(i*2)-2] == 10){
			if(i==1)
				frameScore[i-1] = calculateStrike(i,currentFrame);
			else
				frameScore[i-1] = calculateStrike(i,currentFrame)+frameScore[i-2];
		}
		else if((throws[(i*2)-2] + throws[(i*2)-1])== 10){
			if(i==1)
				frameScore[i-1] = calculateSpare(i,currentFrame);
			else
				frameScore[i-1] = calculateSpare(i,currentFrame)+frameScore[i-2];
		}
		else{
			if(i == 1)
				frameScore[i-1] = throws[(i*2)-2] + throws[(i*2)-1];
			else
				frameScore[i-1] = throws[(i*2)-2] + throws[(i*2)-1]+frameScore[i-2];
		}
	}
	calculateTotalScore();
}

void
ScoreKeeper::calculateTotalScore(){
	if(frame<11){
		totalScore = frameScore[frame-1];
		if(totalScore == -1)
			totalScore = 0;
	}
	else{
		if(throws[20] != -1)
			totalScore = frameScore[9]+throws[20];
	}
}
int 
ScoreKeeper::calculateSpare(int thisFrame, int currentFrame){
	if(thisFrame<currentFrame){
		int score = 10+throws[((thisFrame+1)*2)-2];
		return score;
	}
	else
		return -1;
}

int
ScoreKeeper::calculateStrike(int thisFrame, int currentFrame){
	if(thisFrame<currentFrame){
		int score = 10;
		int nextFrame = throws[((thisFrame+1)*2)-2];
		//case: next frame got strike
		if(nextFrame == 10){
			score += nextFrame;
			//can't calculate score until all the needed frames are played
			if(thisFrame+2 <currentFrame+1){
				score += throws[((thisFrame+2)*2)-2];
				return score;
			}
			else
				return -1;
		}
		else{
			//case: the next frame didn't get strike
			score = score +throws[((thisFrame+1)*2)-2]+throws[((thisFrame+1)*2)-1];
			return score;
		}
	}
	//case: we are in the last frame
	else if(thisFrame == 10){
		int score = 10;
		int nextThrow = throws[((thisFrame)*2)-1];
		if(nextThrow == 10){
			score += nextThrow;
			if(throws[20]!=-1)
				score += throws[20];
		}
		else
			score += nextThrow;

	}
	else
		return -1;
}

void ScoreKeeper::reset()
{
	for(int i=0; i<21; i++){
		throws[i] = -1; //set all scores to -1
	}
	for(int i=0; i<10; i++){
		frameScore[i] = -1; //set all scores to -1
	}
	totalScore = 0;
	frame = 1;
	throwNum = 1;
	gameOver = false;
}